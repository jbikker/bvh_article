#include "precomp.h"
#include "myapp.h"

TheApp* CreateApp() { return new MyApp(); }

// triangle count
#define N	(1024*1024)

// forward declarations
void Subdivide( uint nodeIdx );
void UpdateNodeBounds( uint nodeIdx );

// minimal structs
struct Tri { float3 vertex0, vertex1, vertex2; float3 centroid; };
struct BVHNode
{
	float3 aabbMin, aabbMax;
	uint leftNode, firstTriIdx, triCount;
	bool isLeaf() { return triCount > 0; }
};
struct Ray { float3 O, D; float t = 1e30f; };

// application data
Tri tri[N];
uint triIdx[N];
BVHNode bvhNode[N * 2];
uint rootNodeIdx = 0, nodesUsed = 1;

// functions

void IntersectTri( Ray& ray, const Tri& tri )
{
	const float3 edge1 = tri.vertex1 - tri.vertex0;
	const float3 edge2 = tri.vertex2 - tri.vertex0;
	const float3 h = cross( ray.D, edge2 );
	const float a = dot( edge1, h );
	if (a > -0.0001f && a < 0.0001f) return; // ray parallel to triangle
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

bool IntersectAABB( const Ray& ray, const float3 bmin, const float3 bmax )
{
	float tx1 = (bmin.x - ray.O.x) / ray.D.x, tx2 = (bmax.x - ray.O.x) / ray.D.x;
	float tmin = min( tx1, tx2 ), tmax = max( tx1, tx2 );
	float ty1 = (bmin.y - ray.O.y) / ray.D.y, ty2 = (bmax.y - ray.O.y) / ray.D.y;
	tmin = max( tmin, min( ty1, ty2 ) ), tmax = min( tmax, max( ty1, ty2 ) );
	float tz1 = (bmin.z - ray.O.z) / ray.D.z, tz2 = (bmax.z - ray.O.z) / ray.D.z;
	tmin = max( tmin, min( tz1, tz2 ) ), tmax = min( tmax, max( tz1, tz2 ) );
	return tmax >= tmin && tmin < ray.t && tmax > 0;
}

void IntersectBVH( Ray& ray, const uint nodeIdx )
{
	BVHNode& node = bvhNode[nodeIdx];
	if (!IntersectAABB( ray, node.aabbMin, node.aabbMax )) return;
	if (node.isLeaf())
	{
		for (uint i = 0; i < node.triCount; i++ )
			IntersectTri( ray, tri[triIdx[node.firstTriIdx + i]] );
	}
	else
	{
		IntersectBVH( ray, node.leftNode );
		IntersectBVH( ray, node.leftNode + 1 );
	}
}

void BuildBVH()
{
	// populate triangle index array
	for (int i = 0; i < N; i++) triIdx[i] = i;
	// calculate triangle centroids for partitioning
	for (int i = 0; i < N; i++)
		tri[i].centroid = (tri[i].vertex0 + tri[i].vertex1 + tri[i].vertex2) * 0.3333f;
	// assign all triangles to root node
	BVHNode& root = bvhNode[rootNodeIdx];
	root.firstTriIdx = 0, root.triCount = N;
	UpdateNodeBounds( rootNodeIdx );
	// subdivide recursively
	Subdivide( rootNodeIdx );
}

void UpdateNodeBounds( uint nodeIdx )
{
	BVHNode& node = bvhNode[nodeIdx];
	node.aabbMin = float3( 1e30f );
	node.aabbMax = float3( -1e30f );
	for (uint first = node.firstTriIdx, i = 0; i < node.triCount; i++)
	{
		uint leafTriIdx = triIdx[first + i];
		Tri& leafTri = tri[leafTriIdx];
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex0 ),
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex1 ),
		node.aabbMin = fminf( node.aabbMin, leafTri.vertex2 ),
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex0 ),
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex1 ),
		node.aabbMax = fmaxf( node.aabbMax, leafTri.vertex2 );
	}
}

void Subdivide( uint nodeIdx )
{
	// terminate recursion
	BVHNode& node = bvhNode[nodeIdx];
	if (node.triCount <= 2) return;
	// determine split axis and position
	float3 extent = node.aabbMax - node.aabbMin;
	int axis = 0;
	if (extent.y > extent.x) axis = 1;
	if (extent.z > extent[axis]) axis = 2;
	float splitPos = node.aabbMin[axis] + extent[axis] * 0.5f;
	// in-place partition
	int i = node.firstTriIdx;
	int j = i + node.triCount - 1;
	while (i <= j)
	{
		if (tri[triIdx[i]].centroid[axis] < splitPos)
			i++;
		else
			swap( triIdx[i], triIdx[j--] );
	}
	// abort split if one of the sides is empty
	int leftCount = i - node.firstTriIdx;
	if (leftCount == 0 || leftCount == node.triCount) return;
	// create child nodes
	int leftChildIdx = nodesUsed++;
	int rightChildIdx = nodesUsed++;
	node.leftNode = leftChildIdx;
	bvhNode[leftChildIdx].firstTriIdx = node.firstTriIdx;
	bvhNode[leftChildIdx].triCount = leftCount;
	bvhNode[rightChildIdx].firstTriIdx = i;
	bvhNode[rightChildIdx].triCount = node.triCount - leftCount;
	node.triCount = 0;
	UpdateNodeBounds( leftChildIdx );
	UpdateNodeBounds( rightChildIdx );
	// recurse
	Subdivide( leftChildIdx );
	Subdivide( rightChildIdx );
}

void MyApp::Init()
{
	// intialize a scene with N random triangles
	for (int i = 0; i < N; i++)
	{
		float3 r0 = float3( RandomFloat(), RandomFloat(), RandomFloat() );
		float3 r1 = float3( RandomFloat(), RandomFloat(), RandomFloat() );
		float3 r2 = float3( RandomFloat(), RandomFloat(), RandomFloat() );
		tri[i].vertex0 = r0 * 9 - float3( 5 );
		tri[i].vertex1 = tri[i].vertex0 + r1, tri[i].vertex2 = tri[i].vertex0 + r2;
	}
	// construct the BVH
	BuildBVH();
}

void MyApp::Tick( float deltaTime )
{
	// draw the scene
	screen->Clear( 0 );
	float3 p0( -1, 1, -15 ), p1( 1, 1, -15 ), p2( -1, -1, -15 );
	Ray ray;
	Timer t;
	for (int y = 0; y < 640; y++) for (int x = 0; x < 640; x++)
	{
		float3 pixelPos = p0 + (p1 - p0) * (x / 640.0f) + (p2 - p0) * (y / 640.0f);
		ray.O = float3( 0, 0, -18 );
		ray.D = normalize( pixelPos - ray.O );
		ray.t = 1e30f;
	#if 0
		for( int i = 0; i < N; i++ ) IntersectTri( ray, tri[i] );
	#else
		IntersectBVH( ray, rootNodeIdx );
	#endif
		if (ray.t < 1e30f) screen->Plot( x, y, 0xffffff );
	}
	float elapsed = t.elapsed() * 1000;
	printf( "tracing time: %.2fms (%5.2fK rays/s)\n", elapsed, sqr( 630 ) / elapsed );
}