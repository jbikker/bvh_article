#pragma once

// enable the use of SSE in the AABB intersection function
#define USE_SSE

// bin count
#define BINS 8

namespace Tmpl8
{

// minimalist triangle struct
struct Tri { float3 vertex0, vertex1, vertex2; float3 centroid; };

// ray struct, prepared for SIMD AABB intersection
__declspec(align(64)) struct Ray
{
	Ray() { O4 = D4 = rD4 = _mm_set1_ps( 1 ); }
	union { struct { float3 O; float dummy1; }; __m128 O4; };
	union { struct { float3 D; float dummy2; }; __m128 D4; };
	union { struct { float3 rD; float dummy3; }; __m128 rD4; };
	float t = 1e30f;
};

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

// 32-byte BVH node struct
struct BVHNode
{
	union { struct { float3 aabbMin; uint leftFirst; }; __m128 aabbMin4; };
	union { struct { float3 aabbMax; uint triCount; }; __m128 aabbMax4; };
	bool isLeaf() { return triCount > 0; }
	float CalculateNodeCost()
	{
		float3 e = aabbMax - aabbMin; // extent of the node
		return (e.x * e.y + e.y * e.z + e.z * e.x) * triCount;
	}
};

// bvh 
class BVH
{
public:
	BVH() = default;
	BVH( char* triFile, int N );
	void Build();
	void Refit();
	void SetTransform( mat4& transform );
	void Intersect( Ray& ray );
private:
	void Subdivide( uint nodeIdx );
	void UpdateNodeBounds( uint nodeIdx );
	float FindBestSplitPlane( BVHNode& node, int& axis, float& splitPos );
	BVHNode* bvhNode = 0;
	Tri* tri = 0;
	uint* triIdx = 0;
	uint nodesUsed, triCount;
	mat4 invTransform; // inverse transform
	aabb bounds; // in world space
};

// top-level node
struct TLASNode
{
	float3 aabbMin;
	uint leftBLAS;
	float3 aabbMax;
	uint isLeaf;
};

// top-level BVH
class TLAS
{
public:
	TLAS() = default;
	TLAS( BVH* bvhList, int N );
	void Build();
	void Intersect( Ray& ray );
private:
	TLASNode* tlasNode = 0;
	BVH* blas = 0;
	uint nodesUsed, blasCount;
};

// game class
class TopLevelApp : public TheApp
{
public:
	// game flow methods
	void Init();
	void Tick( float deltaTime );
	void Shutdown() { /* implement if you want to do something on exit */ }
	// input handling
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { mousePos.x = x, mousePos.y = y; }
	void MouseWheel( float y ) { /* implement if you want to handle the mouse wheel */ }
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown( int key ) { /* implement if you want to handle keys */ }
	// data members
	int2 mousePos;
	BVH bvh[64];
	TLAS tlas;
};

} // namespace Tmpl8