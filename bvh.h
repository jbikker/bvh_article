#pragma once

// enable the use of SSE in the AABB intersection function
#define USE_SSE

// bin count for binned BVH building
#define BINS 8

namespace Tmpl8
{

// minimalist triangle struct
struct Tri { float3 vertex0, vertex1, vertex2; float3 centroid; };

// additional triangle data, for texturing and shading
struct TriEx { float2 uv0, uv1, uv2; float3 N0, N1, N2; float dummy; };

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
	bool isLeaf() { return triCount > 0; } // empty BVH leafs do not exist
	float CalculateNodeCost()
	{
		float3 e = aabbMax - aabbMin; // extent of the node
		return (e.x * e.y + e.y * e.z + e.z * e.x) * triCount;
	}
};

// bounding volume hierarchy, to be used as BLAS
class BVH
{
public:
	BVH() = default;
	BVH( class Mesh* mesh );
	void Build();
	void Refit();
	void Intersect( Ray& ray, uint instanceIdx );
private:
	void Subdivide( uint nodeIdx );
	void UpdateNodeBounds( uint nodeIdx );
	float FindBestSplitPlane( BVHNode& node, int& axis, float& splitPos );
	class Mesh* mesh = 0;
	uint* triIdx = 0;
	uint nodesUsed;
public:
	BVHNode* bvhNode = 0;
};

// minimalist mesh class
class Mesh
{
public:
	Mesh() = default;
	Mesh( const char* objFile, const char* texFile );
	BVH* bvh;
	Tri tri[1024];			// triangle data for intersection
	TriEx triEx[1024];		// triangle data for shading
	int triCount = 0;
	Surface* texture;
};

// instance of a BVH, with transform and world bounds
class BVHInstance
{
public:
	BVHInstance() = default;
	BVHInstance( BVH* blas, uint index ) : bvh( blas ), idx( index ) { SetTransform( mat4() ); }
	void SetTransform( mat4& transform );
	void Intersect( Ray& ray );
private:
	BVH* bvh = 0;
	uint idx;
	mat4 invTransform; // inverse transform
public:
	aabb bounds; // in world space
};

// top-level BVH node
struct TLASNode
{
	float3 aabbMin;
	uint leftRight; // 2x16 bits
	float3 aabbMax;
	uint BLAS;
	bool isLeaf() { return leftRight == 0; }
};

// top-level BVH class
class TLAS
{
public:
	TLAS() = default;
	TLAS( BVHInstance* bvhList, int N );
	void Build();
	void Intersect( Ray& ray );
private:
	int FindBestMatch( int* list, int N, int A );
	TLASNode* tlasNode = 0;
	BVHInstance* blas = 0;
	uint nodesUsed, blasCount;
};

} // namespace Tmpl8

// EOF