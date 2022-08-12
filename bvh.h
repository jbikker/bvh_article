#pragma once

// enable the use of SSE in the AABB intersection function
#define USE_SSE

// bin count for binned BVH building
#define BINS 8

namespace Tmpl8
{

// minimalist triangle struct
__declspec(align(64)) struct Tri
{
	// union each float3 with a 16-byte __m128 for faster BVH construction
	union { float3 vertex0; __m128 v0; };
	union { float3 vertex1; __m128 v1; };
	union { float3 vertex2; __m128 v2; };
	union { float3 centroid; __m128 centroid4; }; // total size: 64 bytes
};

// additional triangle data, for texturing and shading
struct TriEx { float2 uv0, uv1, uv2; float3 N0, N1, N2; };

// minimalist AABB struct with grow functionality
struct aabb
{
	float3 bmin = 1e30f, bmax = -1e30f;
	void grow( float3 p ) { bmin = fminf( bmin, p ); bmax = fmaxf( bmax, p ); }
	void grow( aabb& b ) { if (b.bmin.x != 1e30f) { grow( b.bmin ); grow( b.bmax ); } }
	float area()
	{
		float3 e = bmax - bmin; // box extent
		return e.x * e.y + e.y * e.z + e.z * e.x;
	}
};

// intersection record, carefully tuned to be 16 bytes in size
struct Intersection
{
	float t;		// intersection distance along ray
	float u, v;		// barycentric coordinates of the intersection
	uint instPrim;	// instance index (12 bit) and primitive index (20 bit)
};

// ray struct, prepared for SIMD AABB intersection
__declspec(align(64)) struct Ray
{
	Ray() { O4 = D4 = rD4 = _mm_set1_ps( 1 ); }
	union { struct { float3 O; float dummy1; }; __m128 O4; };
	union { struct { float3 D; float dummy2; }; __m128 D4; };
	union { struct { float3 rD; float dummy3; }; __m128 rD4; };
	Intersection hit; // total ray size: 64 bytes
};

// 32-byte BVH node struct
struct BVHNode
{
	union { struct { float3 aabbMin; uint leftFirst; }; __m128 aabbMin4; };
	union { struct { float3 aabbMax; uint triCount; }; __m128 aabbMax4; };
	bool isLeaf() const { return triCount > 0; } // empty BVH leaves do not exist
	float CalculateNodeCost()
	{
		float3 e = aabbMax - aabbMin; // extent of the node
		return (e.x * e.y + e.y * e.z + e.z * e.x) * triCount;
	}
};

// bounding volume hierarchy, to be used as BLAS
__declspec(align(64)) class BVH
{
	struct BuildJob
	{
		uint nodeIdx;
		float3 centroidMin, centroidMax;
	};
public:
	BVH() = default;
	BVH( class Mesh* mesh );
	void Build();
	void Refit();
	void Intersect( Ray& ray, uint instanceIdx );
private:
	void Subdivide( uint nodeIdx, uint depth, uint& nodePtr, float3& centroidMin, float3& centroidMax );
	void UpdateNodeBounds( uint nodeIdx, float3& centroidMin, float3& centroidMax );
	float FindBestSplitPlane( BVHNode& node, int& axis, int& splitPos, float3& centroidMin, float3& centroidMax );
	class Mesh* mesh = 0;
public:
	uint* triIdx = 0;
	uint nodesUsed;
	BVHNode* bvhNode = 0;
	bool subdivToOnePrim = false; // for TLAS experiment
	BuildJob buildStack[64];
	int buildStackPtr;
};

// minimalist mesh class
class Mesh
{
public:
	Mesh() = default;
	Mesh( uint primCount );
	Mesh( const char* objFile, const char* texFile );
	Tri* tri = 0;			// triangle data for intersection
	TriEx* triEx = 0;		// triangle data for shading
	int triCount = 0;
	BVH* bvh = 0;
	Surface* texture = 0;
	float3* P = 0, * N = 0;
};

// instance of a BVH, with transform and world bounds
class BVHInstance
{
public:
	BVHInstance() = default;
	BVHInstance( BVH* blas, uint index ) : bvh( blas ), idx( index ) { SetTransform( mat4() ); }
	void SetTransform( mat4& transform );
	mat4& GetTransform() { return transform; }
	void Intersect( Ray& ray );
private:
	mat4 transform;
	mat4 invTransform; // inverse transform
public:
	aabb bounds; // in world space
private:
	BVH* bvh = 0;
	uint idx;
	int dummy[7];
};

// top-level BVH node
struct TLASNode
{
	union { struct { float dummy1[3]; uint leftRight; }; struct { float dummy3[3]; unsigned short left, right; }; float3 aabbMin; __m128 aabbMin4; };
	union { struct { float dummy2[3]; uint BLAS; }; float3 aabbMax; __m128 aabbMax4; };
	bool isLeaf() { return leftRight == 0; }
};

// include kD-tree logic for fast agglomerative clustering
#include "kdtree.h"

// top-level BVH class
__declspec(align(64)) class TLAS
{
public:
	TLAS() = default;
	TLAS( BVHInstance* bvhList, int N );
	void Build();
	void Intersect( Ray& ray );
private:
	int FindBestMatch( int N, int A );
public:
	TLASNode* tlasNode = 0;
	BVHInstance* blas = 0;
	uint nodesUsed, blasCount;
	uint* nodeIdx = 0;
	// fast agglomerative clustering functionality
	struct SortItem { float pos; uint blasIdx; };
	void BuildQuick();
	void SortAndSplit( uint first, uint last, uint level );
	void CreateParent( uint idx, uint left, uint right );
	static void Swap( SortItem& a, SortItem& b ) { SortItem t = a; a = b; b = t; }
	void QuickSort( SortItem a[], int first, int last );
	// data for fast agglomerative clustering
	KDTree* tree[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint treeSize[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	SortItem* item = 0;
	uint treeIdx = 0;
};

} // namespace Tmpl8

// EOF