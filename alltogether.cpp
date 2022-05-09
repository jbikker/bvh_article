#include "precomp.h"
#include "alltogether.h"

// UNDER CONSTRUCTION - ARTICLE IS BEING WRITTEN

// THIS SOURCE FILE:
// Code for the article "How to Build a BVH", part 5: top-level.
// This version shows how to build and maintain a BVH using
// a TLAS (top level acceleration structure) over a collection of
// BLAS'es (bottom level accstructs).
// Feel free to copy this code to your own framework. Absolutely no
// rights are reserved. No responsibility is accepted either.
// For updates, follow me on twitter: @j_bikker.

TheApp* CreateApp() { return new AllTogetherApp(); }

// functions

void IntersectTri( Ray& ray, const Tri& tri )
{
	const float3 edge1 = tri.vertex1 - tri.vertex0;
	const float3 edge2 = tri.vertex2 - tri.vertex0;
	const float3 h = cross( ray.D, edge2 );
	const float a = dot( edge1, h );
	if (a > -0.00001f && a < 0.00001f) return; // ray parallel to triangle
	const float f = 1 / a;
	const float3 s = ray.O - tri.vertex0;
	const float u = f * dot( s, h );
	if (u < 0 || u > 1) return;
	const float3 q = cross( s, edge1 );
	const float v = f * dot( ray.D, q );
	if (v < 0 || u + v > 1) return;
	const float t = f * dot( edge2, q );
	if (t > 0.0001f) ray.t = min( ray.t, t );
}

inline float IntersectAABB( const Ray& ray, const float3 bmin, const float3 bmax )
{
	float tx1 = (bmin.x - ray.O.x) * ray.rD.x, tx2 = (bmax.x - ray.O.x) * ray.rD.x;
	float tmin = min( tx1, tx2 ), tmax = max( tx1, tx2 );
	float ty1 = (bmin.y - ray.O.y) * ray.rD.y, ty2 = (bmax.y - ray.O.y) * ray.rD.y;
	tmin = max( tmin, min( ty1, ty2 ) ), tmax = min( tmax, max( ty1, ty2 ) );
	float tz1 = (bmin.z - ray.O.z) * ray.rD.z, tz2 = (bmax.z - ray.O.z) * ray.rD.z;
	tmin = max( tmin, min( tz1, tz2 ) ), tmax = min( tmax, max( tz1, tz2 ) );
	if (tmax >= tmin && tmin < ray.t && tmax > 0) return tmin; else return 1e30f;
}

float IntersectAABB_SSE( const Ray& ray, const __m128& bmin4, const __m128& bmax4 )
{
	static __m128 mask4 = _mm_cmpeq_ps( _mm_setzero_ps(), _mm_set_ps( 1, 0, 0, 0 ) );
	__m128 t1 = _mm_mul_ps( _mm_sub_ps( _mm_and_ps( bmin4, mask4 ), ray.O4 ), ray.rD4 );
	__m128 t2 = _mm_mul_ps( _mm_sub_ps( _mm_and_ps( bmax4, mask4 ), ray.O4 ), ray.rD4 );
	__m128 vmax4 = _mm_max_ps( t1, t2 ), vmin4 = _mm_min_ps( t1, t2 );
	float tmax = min( vmax4.m128_f32[0], min( vmax4.m128_f32[1], vmax4.m128_f32[2] ) );
	float tmin = max( vmin4.m128_f32[0], max( vmin4.m128_f32[1], vmin4.m128_f32[2] ) );
	if (tmax >= tmin && tmin < ray.t && tmax > 0) return tmin; else return 1e30f;
}

// BVH class implementation

BVH::BVH( char* triFile, int N )
{
	FILE* file = fopen( triFile, "r" );
	triCount = N;
	tri = new Tri[N];
	for (int t = 0; t < N; t++) fscanf( file, "%f %f %f %f %f %f %f %f %f\n",
		&tri[t].vertex0.x, &tri[t].vertex0.y, &tri[t].vertex0.z,
		&tri[t].vertex1.x, &tri[t].vertex1.y, &tri[t].vertex1.z,
		&tri[t].vertex2.x, &tri[t].vertex2.y, &tri[t].vertex2.z );
	bvhNode = (BVHNode*)_aligned_malloc( sizeof( BVHNode ) * N * 2, 64 );
	triIdx = new uint[N];
	Build();
	SetTransform( mat4::Identity() );
}

void BVH::SetTransform( mat4& transform )
{
	invTransform = transform.Inverted();
	// calculate world-space bounds using the new matrix
	float3 bmin = bvhNode[0].aabbMin, bmax = bvhNode[0].aabbMax;
	bounds = aabb();
	for( int i = 0; i < 8; i++ ) 
		bounds.grow( TransformPosition( float3( i & 1 ? bmax.x : bmin.x,
			i & 2 ? bmax.y : bmin.y, i & 4 ? bmax.z : bmin.z ), transform ) );
}

void BVH::Intersect( Ray& ray )
{
	// backup ray and transform original
	Ray backupRay = ray;
	ray.O = TransformPosition( ray.O, invTransform );
	ray.D = TransformVector( ray.D, invTransform );
	ray.rD = float3( 1 / ray.D.x, 1 / ray.D.y, 1 / ray.D.z );
	// trace transformed ray
	BVHNode* node = &bvhNode[0], * stack[64];
	uint stackPtr = 0;
	while (1)
	{
		if (node->isLeaf())
		{
			for (uint i = 0; i < node->triCount; i++)
				IntersectTri( ray, tri[triIdx[node->leftFirst + i]] );
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
	// restore ray origin and direction
	backupRay.t = ray.t;
	ray = backupRay;
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
			UpdateNodeBounds( i );
			continue;
		}
		// interior node: adjust bounds to child node bounds
		BVHNode& leftChild = bvhNode[node.leftFirst];
		BVHNode& rightChild = bvhNode[node.leftFirst + 1];
		node.aabbMin = fminf( leftChild.aabbMin, rightChild.aabbMin );
		node.aabbMax = fmaxf( leftChild.aabbMax, rightChild.aabbMax );
	}
	printf( "BVH refitted in %.2fms  ", t.elapsed() * 1000 );
}

void BVH::Build()
{
	// reset node pool
	nodesUsed = 2;
	// populate triangle index array
	for (uint i = 0; i < triCount; i++) triIdx[i] = i;
	// calculate triangle centroids for partitioning
	for (uint i = 0; i < triCount; i++)
		tri[i].centroid = (tri[i].vertex0 + tri[i].vertex1 + tri[i].vertex2) * 0.3333f;
	// assign all triangles to root node
	BVHNode& root = bvhNode[0];
	root.leftFirst = 0, root.triCount = triCount;
	UpdateNodeBounds( 0 );
	// subdivide recursively
	Timer t;
	Subdivide( 0 );
	printf( "BVH constructed in %.2fms  ", t.elapsed() * 1000 );
}

void BVH::UpdateNodeBounds( uint nodeIdx )
{
	BVHNode& node = bvhNode[nodeIdx];
	node.aabbMin = float3( 1e30f );
	node.aabbMax = float3( -1e30f );
	for (uint first = node.leftFirst, i = 0; i < node.triCount; i++)
	{
		uint leafTriIdx = triIdx[first + i];
		Tri& leafTri = tri[leafTriIdx];
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex0 );
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex1 );
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex2 );
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex0 );
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex1 );
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex2 );
	}
}

float BVH::FindBestSplitPlane( BVHNode& node, int& axis, float& splitPos )
{
	float bestCost = 1e30f;
	for (int a = 0; a < 3; a++)
	{
		float boundsMin = 1e30f, boundsMax = -1e30f;
		for (uint i = 0; i < node.triCount; i++)
		{
			Tri& triangle = tri[triIdx[node.leftFirst + i]];
			boundsMin = min( boundsMin, triangle.centroid[a] );
			boundsMax = max( boundsMax, triangle.centroid[a] );
		}
		if (boundsMin == boundsMax) continue;
		// populate the bins
		struct Bin { aabb bounds; int triCount = 0; } bin[BINS];
		float scale = BINS / (boundsMax - boundsMin);
		for (uint i = 0; i < node.triCount; i++)
		{
			Tri& triangle = tri[triIdx[node.leftFirst + i]];
			int binIdx = min( BINS - 1, (int)((triangle.centroid[a] - boundsMin) * scale) );
			bin[binIdx].triCount++;
			bin[binIdx].bounds.grow( triangle.vertex0 );
			bin[binIdx].bounds.grow( triangle.vertex1 );
			bin[binIdx].bounds.grow( triangle.vertex2 );
		}
		// gather data for the 7 planes between the 8 bins
		float leftArea[BINS - 1], rightArea[BINS - 1];
		int leftCount[BINS - 1], rightCount[BINS - 1];
		aabb leftBox, rightBox;
		int leftSum = 0, rightSum = 0;
		for (int i = 0; i < BINS - 1; i++)
		{
			leftSum += bin[i].triCount;
			leftCount[i] = leftSum;
			leftBox.grow( bin[i].bounds );
			leftArea[i] = leftBox.area();
			rightSum += bin[BINS - 1 - i].triCount;
			rightCount[BINS - 2 - i] = rightSum;
			rightBox.grow( bin[BINS - 1 - i].bounds );
			rightArea[BINS - 2 - i] = rightBox.area();
		}
		// calculate SAH cost for the 7 planes
		scale = (boundsMax - boundsMin) / BINS;
		for (int i = 0; i < BINS - 1; i++)
		{
			float planeCost = leftCount[i] * leftArea[i] + rightCount[i] * rightArea[i];
			if (planeCost < bestCost)
				axis = a, splitPos = boundsMin + scale * (i + 1), bestCost = planeCost;
		}
	}
	return bestCost;
}

void BVH::Subdivide( uint nodeIdx )
{
	// terminate recursion
	BVHNode& node = bvhNode[nodeIdx];
	// determine split axis using SAH
	int axis;
	float splitPos;
	float splitCost = FindBestSplitPlane( node, axis, splitPos );
	float nosplitCost = node.CalculateNodeCost();
	if (splitCost >= nosplitCost) return;
	// in-place partition
	int i = node.leftFirst;
	int j = i + node.triCount - 1;
	while (i <= j)
	{
		if (tri[triIdx[i]].centroid[axis] < splitPos)
			i++;
		else
			swap( triIdx[i], triIdx[j--] );
	}
	// abort split if one of the sides is empty
	int leftCount = i - node.leftFirst;
	if (leftCount == 0 || leftCount == node.triCount) return;
	// create child nodes
	int leftChildIdx = nodesUsed++;
	int rightChildIdx = nodesUsed++;
	bvhNode[leftChildIdx].leftFirst = node.leftFirst;
	bvhNode[leftChildIdx].triCount = leftCount;
	bvhNode[rightChildIdx].leftFirst = i;
	bvhNode[rightChildIdx].triCount = node.triCount - leftCount;
	node.leftFirst = leftChildIdx;
	node.triCount = 0;
	UpdateNodeBounds( leftChildIdx );
	UpdateNodeBounds( rightChildIdx );
	// recurse
	Subdivide( leftChildIdx );
	Subdivide( rightChildIdx );
}

// TLAS implementation

TLAS::TLAS( BVH* bvhList, int N )
{
	// copy a pointer to the array of bottom level accstructs
	blas = bvhList;
	blasCount = N;
	// allocate TLAS nodes
	tlasNode = (TLASNode*)_aligned_malloc( sizeof( TLASNode ) * 2 * N, 64 );
	nodesUsed = 2;
}

int TLAS::FindBestMatch( vector<int>& list, int A )
{
	// find BLAS B that, when joined with A, forms the smallest AABB
	float smallest = 1e30f;
	int bestB = -1;
	for( int s = (int)list.size(), B = 0; B < s; B++ ) if (B != A)
	{
		float3 bmin = fminf( tlasNode[A].aabbMin, tlasNode[B].aabbMin );
		float3 bmax = fmaxf( tlasNode[A].aabbMax, tlasNode[B].aabbMax );
		float3 e = bmax - bmin;
		float surfaceArea = e.x * e.y + e.y * e.z + e.z * e.x;
		if (surfaceArea < smallest) smallest = surfaceArea, bestB = B;
	}
	return bestB;
}

void TLAS::Build()
{
	// assign a TLASleaf node to each BLAS
	vector<int> nodeIdxList;
	nodesUsed = 2;
	for( uint i = 0; i < blasCount; i++ )
	{
		nodeIdxList.push_back( nodesUsed );
		tlasNode[nodesUsed].aabbMin = blas[i].bounds.bmin,
		tlasNode[nodesUsed].aabbMax = blas[i].bounds.bmax,
		tlasNode[nodesUsed].BLAS = i;
		tlasNode[nodesUsed++].leftRight = 0; // makes it a leaf
	}
	// use agglomerative clustering to build the TLAS
	int A = 0, B = FindBestMatch( nodeIdxList, A );
	while (nodeIdxList.size() > 1)
	{
		int C = FindBestMatch( nodeIdxList, B );
		if (A == C)
		{
			int nodeIdxA = nodeIdxList[A], nodeIdxB = nodeIdxList[B];
			TLASNode& nodeA = tlasNode[nodeIdxA];
			TLASNode& nodeB = tlasNode[nodeIdxB];
			TLASNode& newNode = tlasNode[nodesUsed];
			newNode.leftRight = nodeIdxA + (nodeIdxB << 16);
			newNode.aabbMin = fminf( nodeA.aabbMin, nodeB.aabbMin );
			newNode.aabbMax = fmaxf( nodeA.aabbMax, nodeB.aabbMax );
			nodeIdxList[A] = nodesUsed++;
			nodeIdxList.erase( nodeIdxList.begin() + B );
			B = FindBestMatch( nodeIdxList, A );
		}
		else A = B, B = C;
	}
	tlasNode[0] = tlasNode[nodeIdxList[A]];
}

void TLAS::Intersect( Ray& ray )
{
	ray.rD = float3( 1 / ray.D.x, 1 / ray.D.y, 1 / ray.D.z );
	TLASNode* node = &tlasNode[0], *stack[64];
	uint stackPtr = 0;
	while (1)
	{
		if (node->isLeaf())
		{
			blas[node->BLAS].Intersect( ray );
			if (stackPtr == 0) break; else node = stack[--stackPtr];
			continue;
		}
		TLASNode* child1 = &tlasNode[node->leftRight & 0xffff];
		TLASNode* child2 = &tlasNode[node->leftRight >> 16];
		float dist1 = IntersectAABB( ray, child1->aabbMin, child1->aabbMax );
		float dist2 = IntersectAABB( ray, child2->aabbMin, child2->aabbMax );
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

// TopLevelApp implementation

void AllTogetherApp::Init()
{
	bvh[0] = BVH( "assets/armadillo.tri", 30000 );
	bvh[1] = BVH( "assets/armadillo.tri", 30000 );
	tlas = TLAS( bvh, 2 );
}

void AllTogetherApp::Tick( float deltaTime )
{
	// draw the scene
	float3 p0( -1, 1, 2 ), p1( 1, 1, 2 ), p2( -1, -1, 2 );
	Timer t;
	static float angle = 0;
	angle += 0.01f;
	if (angle > 2 * PI) angle -= 2 * PI;
	bvh[0].SetTransform( mat4::Translate( float3( -1.3f, 0, 0 ) ) );
	bvh[1].SetTransform( mat4::Translate( float3( 1.3f, 0, 0 ) ) * mat4::RotateY( angle ) );
	tlas.Build();
#pragma omp parallel for schedule(dynamic)
	for (int tile = 0; tile < 6400; tile++)
	{
		int x = tile % 80, y = tile / 80;
		Ray ray;
		ray.O = float3( 0, 0.5f, -4.5f );
		for (int v = 0; v < 8; v++) for (int u = 0; u < 8; u++)
		{
			float3 pixelPos = ray.O + p0 +
				(p1 - p0) * ((x * 8 + u) / 640.0f) +
				(p2 - p0) * ((y * 8 + v) / 640.0f);
			ray.D = normalize( pixelPos - ray.O ), ray.t = 1e30f;
			tlas.Intersect( ray );
			uint c = ray.t < 1e30f ? (255 - (int)((ray.t - 3) * 80)) : 0;
			screen->Plot( x * 8 + u, y * 8 + v, c * 0x10101 );
		}
	}
	float elapsed = t.elapsed() * 1000;
	printf( "tracing time: %.2fms (%5.2fK rays/s)\n", elapsed, sqr( 630 ) / elapsed );
}

// EOF