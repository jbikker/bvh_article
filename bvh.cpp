#include "precomp.h"
#include "bvh.h"

/*
Performance: 1858ms without kD-tree
with kDtree:
- 15s without culling
- 8.1s with culling
- 3.4s without removeLeaf refitting
- 858ms with recursive refitting
- 836ms with cache alignment
*/

// functions

void IntersectTri( Ray& ray, const Tri& tri, const uint instPrim )
{
	// Moeller-Trumbore ray/triangle intersection algorithm, see:
	// en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	const float3 edge1 = tri.vertex1 - tri.vertex0;
	const float3 edge2 = tri.vertex2 - tri.vertex0;
	const float3 h = cross( ray.D, edge2 );
	const float a = dot( edge1, h );
	if (fabs( a ) < 0.00001f) return; // ray parallel to triangle
	const float f = 1 / a;
	const float3 s = ray.O - tri.vertex0;
	const float u = f * dot( s, h );
	if (u < 0 || u > 1) return;
	const float3 q = cross( s, edge1 );
	const float v = f * dot( ray.D, q );
	if (v < 0 || u + v > 1) return;
	const float t = f * dot( edge2, q );
	if (t > 0.0001f && t < ray.hit.t)
		ray.hit.t = t, ray.hit.u = u,
		ray.hit.v = v, ray.hit.instPrim = instPrim;
}

inline float IntersectAABB( const Ray& ray, const float3 bmin, const float3 bmax )
{
	// "slab test" ray/AABB intersection
	float tx1 = (bmin.x - ray.O.x) * ray.rD.x, tx2 = (bmax.x - ray.O.x) * ray.rD.x;
	float tmin = min( tx1, tx2 ), tmax = max( tx1, tx2 );
	float ty1 = (bmin.y - ray.O.y) * ray.rD.y, ty2 = (bmax.y - ray.O.y) * ray.rD.y;
	tmin = max( tmin, min( ty1, ty2 ) ), tmax = min( tmax, max( ty1, ty2 ) );
	float tz1 = (bmin.z - ray.O.z) * ray.rD.z, tz2 = (bmax.z - ray.O.z) * ray.rD.z;
	tmin = max( tmin, min( tz1, tz2 ) ), tmax = min( tmax, max( tz1, tz2 ) );
	if (tmax >= tmin && tmin < ray.hit.t && tmax > 0) return tmin; else return 1e30f;
}

float IntersectAABB_SSE( const Ray& ray, const __m128& bmin4, const __m128& bmax4 )
{
	// "slab test" ray/AABB intersection, using SIMD instructions
	static __m128 mask4 = _mm_cmpeq_ps( _mm_setzero_ps(), _mm_set_ps( 1, 0, 0, 0 ) );
	__m128 t1 = _mm_mul_ps( _mm_sub_ps( _mm_and_ps( bmin4, mask4 ), ray.O4 ), ray.rD4 );
	__m128 t2 = _mm_mul_ps( _mm_sub_ps( _mm_and_ps( bmax4, mask4 ), ray.O4 ), ray.rD4 );
	__m128 vmax4 = _mm_max_ps( t1, t2 ), vmin4 = _mm_min_ps( t1, t2 );
	float tmax = min( vmax4.m128_f32[0], min( vmax4.m128_f32[1], vmax4.m128_f32[2] ) );
	float tmin = max( vmin4.m128_f32[0], max( vmin4.m128_f32[1], vmin4.m128_f32[2] ) );
	if (tmax >= tmin && tmin < ray.hit.t && tmax > 0) return tmin; else return 1e30f;
}

// Mesh class implementation

Mesh::Mesh( const uint primCount )
{
	// basic constructor, for top-down TLAS construction
	tri = (Tri*)_aligned_malloc( primCount * sizeof( Tri ), 64 );
	memset( tri, 0, primCount * sizeof( Tri ) );
	triEx = (TriEx*)_aligned_malloc( primCount * sizeof( TriEx ), 64 );
	memset( triEx, 0, primCount * sizeof( TriEx ) );
	triCount = primCount;
}

Mesh::Mesh( const char* objFile, const char* texFile )
{
	// bare-bones obj file loader; only supports very basic meshes
	tri = new Tri[25000];
	triEx = new TriEx[25000];
	float2* UV = new float2[11042]; // enough for dragon.obj
	N = new float3[11042], P = new float3[11042];
	int UVs = 0, Ns = 0, Ps = 0, a, b, c, d, e, f, g, h, i;
	FILE* file = fopen( objFile, "r" );
	if (!file) return; // file doesn't exist
	while (!feof( file ))
	{
		char line[512] = { 0 };
		fgets( line, 511, file );
		if (line == strstr( line, "vt " ))
			sscanf( line + 3, "%f %f", &UV[UVs].x, &UV[UVs].y ), UVs++;
		else if (line == strstr( line, "vn " ))
			sscanf( line + 3, "%f %f %f", &N[Ns].x, &N[Ns].y, &N[Ns].z ), Ns++;
		else if (line[0] == 'v')
			sscanf( line + 2, "%f %f %f", &P[Ps].x, &P[Ps].y, &P[Ps].z ), Ps++;
		if (line[0] != 'f') continue; else
			sscanf( line + 2, "%i/%i/%i %i/%i/%i %i/%i/%i",
				&a, &b, &c, &d, &e, &f, &g, &h, &i );
		tri[triCount].vertex0 = P[a - 1], triEx[triCount].N0 = N[c - 1];
		tri[triCount].vertex1 = P[d - 1], triEx[triCount].N1 = N[f - 1];
		tri[triCount].vertex2 = P[g - 1], triEx[triCount].N2 = N[i - 1];
		triEx[triCount].uv0 = UV[b - 1], triEx[triCount].uv1 = UV[e - 1];
		triEx[triCount++].uv2 = UV[h - 1];
	}
	fclose( file );
	bvh = new BVH( this );
	texture = new Surface( texFile );
}

// BVH class implementation

BVH::BVH( Mesh* triMesh )
{
	mesh = triMesh;
	bvhNode = (BVHNode*)_aligned_malloc( sizeof( BVHNode ) * mesh->triCount * 2 + 64, 64 );
	triIdx = new uint[mesh->triCount];
	Build();
}

void BVH::Intersect( Ray& ray, uint instanceIdx )
{
	BVHNode* node = &bvhNode[0], * stack[64];
	uint stackPtr = 0;
	while (1)
	{
		if (node->isLeaf())
		{
			for (uint i = 0; i < node->triCount; i++)
			{
				uint instPrim = (instanceIdx << 20) + triIdx[node->leftFirst + i];
				IntersectTri( ray, mesh->tri[instPrim & 0xfffff /* 20 bits */], instPrim );
			}
			if (stackPtr == 0) break; else node = stack[--stackPtr];
			continue;
		}
		BVHNode* child1 = &bvhNode[node->leftFirst];
		BVHNode* child2 = &bvhNode[node->leftFirst + 1];
	#ifdef USE_SSE
		float dist1 = IntersectAABB_SSE( ray, child1->aabbMin4, child1->aabbMax4 );
		float dist2 = IntersectAABB_SSE( ray, child2->aabbMin4, child2->aabbMax4 );
	#else
		float dist1 = IntersectAABB( ray, child1->aabbMin, child1->aabbMax );
		float dist2 = IntersectAABB( ray, child2->aabbMin, child2->aabbMax );
	#endif
		if (dist1 > dist2) { swap( dist1, dist2 ); swap( child1, child2 ); }
		if (dist1 == 1e30f)
		{
			if (stackPtr == 0) break; else node = stack[--stackPtr];
		}
		else
		{
			node = child1;
			if (dist2 != 1e30f) stack[stackPtr++] = child2;
		}
	}
}

void BVH::Refit()
{
	Timer t;
	for (int i = nodesUsed - 1; i >= 0; i--) if (i != 1)
	{
		BVHNode& node = bvhNode[i];
		if (node.isLeaf())
		{
			// leaf node: adjust bounds to contained triangles
			float3 dummy1, dummy2; // we don't need centroid bounds here
			UpdateNodeBounds( i, dummy1, dummy2 );
			continue;
		}
		// interior node: adjust bounds to child node bounds
		BVHNode& leftChild = bvhNode[node.leftFirst];
		BVHNode& rightChild = bvhNode[node.leftFirst + 1];
		node.aabbMin = fminf( leftChild.aabbMin, rightChild.aabbMin );
		node.aabbMax = fmaxf( leftChild.aabbMax, rightChild.aabbMax );
	}
	printf( "BVH refitted in %.2fms\n", t.elapsed() * 1000 );
}

void BVH::Build()
{
	// reset node pool
	nodesUsed = 2;
	memset( bvhNode, 0, mesh->triCount * 2 * sizeof( BVHNode ) );
	// populate triangle index array
	for (int i = 0; i < mesh->triCount; i++) triIdx[i] = i;
	// calculate triangle centroids for partitioning
	Tri* tri = mesh->tri;
	for (int i = 0; i < mesh->triCount; i++)
		mesh->tri[i].centroid = (tri[i].vertex0 + tri[i].vertex1 + tri[i].vertex2) * 0.3333f;
	// assign all triangles to root node
	BVHNode& root = bvhNode[0];
	root.leftFirst = 0, root.triCount = mesh->triCount;
	float3 centroidMin, centroidMax;
	UpdateNodeBounds( 0, centroidMin, centroidMax );
	// subdivide recursively
	buildStackPtr = 0;
	Subdivide( 0, 0, nodesUsed, centroidMin, centroidMax );
}

void BVH::Subdivide( uint nodeIdx, uint depth, uint& nodePtr, float3& centroidMin, float3& centroidMax )
{
	BVHNode& node = bvhNode[nodeIdx];
	// determine split axis using SAH
	int axis, splitPos;
	float splitCost = FindBestSplitPlane( node, axis, splitPos, centroidMin, centroidMax );
	// terminate recursion
	if (subdivToOnePrim)
	{
		if (node.triCount == 1) return;
	}
	else
	{
		float nosplitCost = node.CalculateNodeCost();
		if (splitCost >= nosplitCost) return;
	}
	// in-place partition
	int i = node.leftFirst;
	int j = i + node.triCount - 1;
	float scale = BINS / (centroidMax[axis] - centroidMin[axis]);
	while (i <= j)
	{
		// use the exact calculation we used for binning to prevent rare inaccuracies
		int binIdx = min( BINS - 1, (int)((mesh->tri[triIdx[i]].centroid[axis] - centroidMin[axis]) * scale) );
		if (binIdx < splitPos) i++; else swap( triIdx[i], triIdx[j--] );
	}
	// abort split if one of the sides is empty
	int leftCount = i - node.leftFirst;
	if (leftCount == 0 || leftCount == node.triCount) return; // never happens for dragon mesh, nice
	// create child nodes
	int leftChildIdx = nodePtr++;
	int rightChildIdx = nodePtr++;
	bvhNode[leftChildIdx].leftFirst = node.leftFirst;
	bvhNode[leftChildIdx].triCount = leftCount;
	bvhNode[rightChildIdx].leftFirst = i;
	bvhNode[rightChildIdx].triCount = node.triCount - leftCount;
	node.leftFirst = leftChildIdx;
	node.triCount = 0;
	// recurse
	UpdateNodeBounds( leftChildIdx, centroidMin, centroidMax );
	Subdivide( leftChildIdx, depth + 1, nodePtr, centroidMin, centroidMax );
	UpdateNodeBounds( rightChildIdx, centroidMin, centroidMax );
	Subdivide( rightChildIdx, depth + 1, nodePtr, centroidMin, centroidMax );
}

float BVH::FindBestSplitPlane( BVHNode& node, int& axis, int& splitPos, float3& centroidMin, float3& centroidMax )
{
	float bestCost = 1e30f;
	for (int a = 0; a < 3; a++)
	{
		float boundsMin = centroidMin[a], boundsMax = centroidMax[a];
		if (boundsMin == boundsMax) continue;
		// populate the bins
		float scale = BINS / (boundsMax - boundsMin);
		float leftCountArea[BINS - 1], rightCountArea[BINS - 1];
		int leftSum = 0, rightSum = 0;
	#ifdef USE_SSE
		__m128 min4[BINS], max4[BINS];
		uint count[BINS];
		for (uint i = 0; i < BINS; i++)
			min4[i] = _mm_set_ps1( 1e30f ),
			max4[i] = _mm_set_ps1( -1e30f ),
			count[i] = 0;
		for (uint i = 0; i < node.triCount; i++)
		{
			Tri& triangle = mesh->tri[triIdx[node.leftFirst + i]];
			int binIdx = min( BINS - 1, (int)((triangle.centroid[a] - boundsMin) * scale) );
			count[binIdx]++;
			min4[binIdx] = _mm_min_ps( min4[binIdx], triangle.v0 );
			max4[binIdx] = _mm_max_ps( max4[binIdx], triangle.v0 );
			min4[binIdx] = _mm_min_ps( min4[binIdx], triangle.v1 );
			max4[binIdx] = _mm_max_ps( max4[binIdx], triangle.v1 );
			min4[binIdx] = _mm_min_ps( min4[binIdx], triangle.v2 );
			max4[binIdx] = _mm_max_ps( max4[binIdx], triangle.v2 );
		}
		// gather data for the 7 planes between the 8 bins
		__m128 leftMin4 = _mm_set_ps1( 1e30f ), rightMin4 = leftMin4;
		__m128 leftMax4 = _mm_set_ps1( -1e30f ), rightMax4 = leftMax4;
		for (int i = 0; i < BINS - 1; i++)
		{
			leftSum += count[i];
			rightSum += count[BINS - 1 - i];
			leftMin4 = _mm_min_ps( leftMin4, min4[i] );
			rightMin4 = _mm_min_ps( rightMin4, min4[BINS - 2 - i] );
			leftMax4 = _mm_max_ps( leftMax4, max4[i] );
			rightMax4 = _mm_max_ps( rightMax4, max4[BINS - 2 - i] );
			const __m128 le = _mm_sub_ps( leftMax4, leftMin4 );
			const __m128 re = _mm_sub_ps( rightMax4, rightMin4 );
			leftCountArea[i] = leftSum * (le.m128_f32[0] * le.m128_f32[1] + le.m128_f32[1] * le.m128_f32[2] + le.m128_f32[2] * le.m128_f32[0]);
			rightCountArea[BINS - 2 - i] = rightSum * (re.m128_f32[0] * re.m128_f32[1] + re.m128_f32[1] * re.m128_f32[2] + re.m128_f32[2] * re.m128_f32[0]);
		}
	#else
		struct Bin { aabb bounds; int triCount = 0; } bin[BINS];
		for (uint i = 0; i < node.triCount; i++)
		{
			Tri& triangle = mesh->tri[triIdx[node.leftFirst + i]];
			int binIdx = min( BINS - 1, (int)((triangle.centroid[a] - boundsMin) * scale) );
			bin[binIdx].triCount++;
			bin[binIdx].bounds.grow( triangle.vertex0 );
			bin[binIdx].bounds.grow( triangle.vertex1 );
			bin[binIdx].bounds.grow( triangle.vertex2 );
		}
		// gather data for the 7 planes between the 8 bins
		aabb leftBox, rightBox;
		for (int i = 0; i < BINS - 1; i++)
		{
			leftSum += bin[i].triCount;
			leftBox.grow( bin[i].bounds );
			leftCountArea[i] = leftSum * leftBox.area();
			rightSum += bin[BINS - 1 - i].triCount;
			rightBox.grow( bin[BINS - 1 - i].bounds );
			rightCountArea[BINS - 2 - i] = rightSum * rightBox.area();
		}
	#endif
		// calculate SAH cost for the 7 planes
		scale = (boundsMax - boundsMin) / BINS;
		for (int i = 0; i < BINS - 1; i++)
		{
			const float planeCost = leftCountArea[i] + rightCountArea[i];
			if (planeCost < bestCost)
				axis = a, splitPos = i + 1, bestCost = planeCost;
		}
	}
	return bestCost;
}

void BVH::UpdateNodeBounds( uint nodeIdx, float3& centroidMin, float3& centroidMax )
{
	BVHNode& node = bvhNode[nodeIdx];
#ifdef USE_SSE
	__m128 min4 = _mm_set_ps1( 1e30f ), max4 = _mm_set_ps1( -1e30f );
	__m128 cmin4 = _mm_set_ps1( 1e30f ), cmax4 = _mm_set_ps1( -1e30f );
	for (uint first = node.leftFirst, i = 0; i < node.triCount; i++)
	{
		Tri& leafTri = mesh->tri[triIdx[first + i]];
		min4 = _mm_min_ps( min4, leafTri.v0 ), max4 = _mm_max_ps( max4, leafTri.v0 );
		min4 = _mm_min_ps( min4, leafTri.v1 ), max4 = _mm_max_ps( max4, leafTri.v1 );
		min4 = _mm_min_ps( min4, leafTri.v2 ), max4 = _mm_max_ps( max4, leafTri.v2 );
		cmin4 = _mm_min_ps( cmin4, leafTri.centroid4 );
		cmax4 = _mm_max_ps( cmax4, leafTri.centroid4 );
	}
	__m128 mask4 = _mm_cmpeq_ps( _mm_setzero_ps(), _mm_set_ps( 1, 0, 0, 0 ) );
	node.aabbMin4 = _mm_blendv_ps( node.aabbMin4, min4, mask4 );
	node.aabbMax4 = _mm_blendv_ps( node.aabbMax4, max4, mask4 );
	centroidMin = *(float3*)&cmin4;
	centroidMax = *(float3*)&cmax4;
#else
	node.aabbMin = float3( 1e30f );
	node.aabbMax = float3( -1e30f );
	centroidMin = float3( 1e30f );
	centroidMax = float3( -1e30f );
	for (uint first = node.leftFirst, i = 0; i < node.triCount; i++)
	{
		uint leafTriIdx = triIdx[first + i];
		Tri& leafTri = mesh->tri[leafTriIdx];
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex0 );
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex1 );
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex2 );
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex0 );
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex1 );
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex2 );
		centroidMin = fminf( centroidMin, leafTri.centroid );
		centroidMax = fmaxf( centroidMax, leafTri.centroid );
	}
#endif
}

// BVHInstance implementation

void BVHInstance::SetTransform( mat4& T )
{
	transform = T;
	transform = T;
	invTransform = transform.Inverted();
	// calculate world-space bounds using the new matrix
	float3 bmin = bvh->bvhNode[0].aabbMin, bmax = bvh->bvhNode[0].aabbMax;
	bounds = aabb();
	for (int i = 0; i < 8; i++)
		bounds.grow( TransformPosition( float3( i & 1 ? bmax.x : bmin.x,
			i & 2 ? bmax.y : bmin.y, i & 4 ? bmax.z : bmin.z ), transform ) );
}

void BVHInstance::Intersect( Ray& ray )
{
	// backup ray and transform original
	Ray backupRay = ray;
	ray.O = TransformPosition( ray.O, invTransform );
	ray.D = TransformVector( ray.D, invTransform );
	ray.rD = float3( 1 / ray.D.x, 1 / ray.D.y, 1 / ray.D.z );
	// trace ray through BVH
	bvh->Intersect( ray, idx );
	// restore ray origin and direction
	backupRay.hit = ray.hit;
	ray = backupRay;
}

// TLAS implementation

TLAS::TLAS( BVHInstance* bvhList, int N )
{
	// copy a pointer to the array of bottom level accstruc instances
	blas = bvhList;
	blasCount = N;
	// allocate TLAS nodes
	tlasNode = (TLASNode*)_aligned_malloc( sizeof( TLASNode ) * 2 * (N + 64), 64 );
	nodeIdx = new uint[N];
	nodesUsed = 2;
}

int TLAS::FindBestMatch( int N, int A )
{
	// find BLAS B that, when joined with A, forms the smallest AABB
	float smallest = 1e30f;
	int bestB = -1;
	for (int B = 0; B < N; B++) if (B != A)
	{
		float3 bmax = fmaxf( tlasNode[nodeIdx[A]].aabbMax, tlasNode[nodeIdx[B]].aabbMax );
		float3 bmin = fminf( tlasNode[nodeIdx[A]].aabbMin, tlasNode[nodeIdx[B]].aabbMin );
		float3 e = bmax - bmin;
		float surfaceArea = e.x * e.y + e.y * e.z + e.z * e.x;
		if (surfaceArea < smallest) smallest = surfaceArea, bestB = B;
	}
	return bestB;
}

void TLAS::Build()
{
	// assign a TLASleaf node to each BLAS
	nodesUsed = 1;
	for (uint i = 0; i < blasCount; i++)
	{
		nodeIdx[i] = nodesUsed;
		tlasNode[nodesUsed].aabbMin = blas[i].bounds.bmin;
		tlasNode[nodesUsed].aabbMax = blas[i].bounds.bmax;
		tlasNode[nodesUsed].BLAS = i;
		tlasNode[nodesUsed++].leftRight = 0; // makes it a leaf
	}
	// use agglomerative clustering to build the TLAS
	int nodeIndices = blasCount;
	int A = 0, B = FindBestMatch( nodeIndices, A );
	// copy last remaining node to the root node
	tlasNode[0] = tlasNode[nodeIdx[A]];
}

void TLAS::SortAndSplit( uint first, uint last, uint level )
{
	if (!item) item = new SortItem[blasCount];
	uint axis = level % 3; // TODO: use dominant axis at each level?
	if (level == 0)
	{
		for (uint i = 0; i < blasCount; i++) item[i].blasIdx = i;
		treeIdx = 0;
	}
	for (uint idx, i = first; i <= last; i++)
		idx = item[i].blasIdx,
		item[i].pos = (blas[idx].bounds.bmin[axis] + blas[idx].bounds.bmin[axis]) * 0.5f;
	QuickSort( item, first, last );
	uint half = (first + last) >> 1;
	if (level < 3)
	{
		SortAndSplit( first, half, level + 1 );
		SortAndSplit( half + 1, last, level + 1 );
		return;
	}
	// create chunks
	for (uint i = first; i <= half; i++)
	{
		BVHInstance& b = blas[item[i].blasIdx];
		tlasNode[nodesUsed].aabbMin = b.bounds.bmin;
		tlasNode[nodesUsed].aabbMax = b.bounds.bmax;
		tlasNode[nodesUsed].BLAS = item[i].blasIdx;
		tlasNode[nodesUsed++].leftRight = 0; // makes it a leaf
	}
	if (!tree[treeIdx]) tree[treeIdx] = new KDTree( tlasNode + first + 32, half - first + 1, first + 32 );
	treeSize[treeIdx++] = half - first + 1;
	for (uint i = half + 1; i <= last; i++)
	{
		BVHInstance& b = blas[item[i].blasIdx];
		tlasNode[nodesUsed].aabbMin = b.bounds.bmin;
		tlasNode[nodesUsed].aabbMax = b.bounds.bmax;
		tlasNode[nodesUsed].BLAS = item[i].blasIdx;
		tlasNode[nodesUsed++].leftRight = 0; // makes it a leaf
	}
	if (!tree[treeIdx]) tree[treeIdx] = new KDTree( tlasNode + half + 33, last - half, half + 33 );
	treeSize[treeIdx++] = last - half;
}

void TLAS::CreateParent( uint idx, uint left, uint right )
{
	tlasNode[idx].left = left, tlasNode[idx].right = right;
	tlasNode[idx].aabbMin = fminf( tlasNode[left].aabbMin, tlasNode[right].aabbMin );
	tlasNode[idx].aabbMax = fmaxf( tlasNode[left].aabbMax, tlasNode[right].aabbMax );
}

void TLAS::QuickSort( SortItem a[], int first, int last )
{
	struct Task { uint first, last; };
	_declspec (align(64)) Task stack[64];
	uint& stackPtr = stack[0].first; // so it sits in the same cacheline
	stackPtr = 1;
	while (1)
	{
		while (1)
		{
			if (first >= last) break;
			int p = first;
			SortItem e = a[first];
			for (int i = first + 1; i <= last; i++) if (a[i].pos <= e.pos) Swap( a[i], a[++p] );
			Swap( a[p], a[first] );
			stack[stackPtr].first = p + 1, stack[stackPtr++].last = last, last = p - 1;
		}
		if (stackPtr == 1) break;
		first = stack[--stackPtr].first, last = stack[stackPtr].last;
	}
}

void TLAS::BuildQuick()
{
	// building the TLAS top-down, fastest option for the Boids demo
	static Mesh m;
	if (!m.tri) m = Mesh( blasCount );
	for (uint i = 0; i < blasCount; i++)
	{
		m.tri[i].vertex0 = blas[i].bounds.bmin;
		m.tri[i].vertex1 = blas[i].bounds.bmax;
		m.tri[i].vertex2 = (blas[i].bounds.bmin + blas[i].bounds.bmax) * 0.5f; // degenerate but with the correct aabb
	}
	if (!m.bvh)
	{
		m.bvh = new BVH( &m );
		m.bvh->subdivToOnePrim = true;
	}
	m.bvh->Build();
	// copy the BVH to a TLAS
	memcpy( tlasNode, m.bvh->bvhNode, m.bvh->nodesUsed * sizeof( BVHNode ) );
	for (uint i = 0; i < m.bvh->nodesUsed; i++) if (i != 1)
	{
		const BVHNode& n = m.bvh->bvhNode[i];
		if (n.isLeaf())
			tlasNode[i].BLAS = m.bvh->triIdx[n.leftFirst],
			tlasNode[i].leftRight = 0; // mark as leaf
		else
			tlasNode[i].leftRight = n.leftFirst + ((n.leftFirst + 1) << 16);
	}
}

void TLAS::Intersect( Ray& ray )
{
	// calculate reciprocal ray directions for faster AABB intersection
	ray.rD = float3( 1 / ray.D.x, 1 / ray.D.y, 1 / ray.D.z );
	// use a local stack instead of a recursive function
	TLASNode* node = &tlasNode[0], * stack[64];
	uint stackPtr = 0;
	// traversl loop; terminates when the stack is empty
	while (1)
	{
		if (node->isLeaf())
		{
			// current node is a leaf: intersect BLAS
			blas[node->BLAS].Intersect( ray );
			// pop a node from the stack; terminate if none left
			if (stackPtr == 0) break; else node = stack[--stackPtr];
			continue;
		}
		// current node is an interior node: visit child nodes, ordered
		TLASNode* child1 = &tlasNode[node->leftRight & 0xffff];
		TLASNode* child2 = &tlasNode[node->leftRight >> 16];
		float dist1 = IntersectAABB( ray, child1->aabbMin, child1->aabbMax );
		float dist2 = IntersectAABB( ray, child2->aabbMin, child2->aabbMax );
		if (dist1 > dist2) { swap( dist1, dist2 ); swap( child1, child2 ); }
		if (dist1 == 1e30f)
		{
			// missed both child nodes; pop a node from the stack
			if (stackPtr == 0) break; else node = stack[--stackPtr];
		}
		else
		{
			// visit near node; push the far node if the ray intersects it
			node = child1;
			if (dist2 != 1e30f) stack[stackPtr++] = child2;
		}
	}
}

// EOF