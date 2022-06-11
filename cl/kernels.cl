#include "template/common.h" 

inline uint RGB32FtoRGB8( float3 c )
{
	int r = (int)(min( c.x, 1.f ) * 255);
	int g = (int)(min( c.y, 1.f ) * 255);
	int b = (int)(min( c.z, 1.f ) * 255);
	return (r << 16) + (g << 8) + b;
}

struct Intersection
{
	float t;			// intersection distance along ray
	float u, v;			// barycentric coordinates of the intersection
	uint instPrim;		// instance index (12 bit) and primitive index (20 bit)
};

struct Ray
{
	float3 O, D, rD;	// in OpenCL, each of these will be padded to 16 bytes
	struct Intersection hit;
};

struct Tri 
{ 
	float v0x, v0y, v0z;
	float v1x, v1y, v1z;
	float v2x, v2y, v2z;
	float cx, cy, cz;
};

struct TriEx 
{ 
	float2 uv0, uv1, uv2; 
	float N0x, N0y, N0z;
	float N1x, N1y, N1z;
	float N2x, N2y, N2z;
	float dummy;
};

struct BVHNode
{
	float minx, miny, minz;
	int leftFirst;
	float maxx, maxy, maxz;
	int triCount;
};

struct TLASNode
{
	float minx, miny, minz;
	uint leftRight; // 2x16 bits
	float maxx, maxy, maxz;
	uint BLAS;
};

struct BVHInstance
{
	uint dummy1, dummy2;
	uint idx;
	float16 transform;
	float16 invTransform; // inverse transform
	uint dummy[6];
};

void IntersectTri( struct Ray* ray, struct Tri* tri, const uint instPrim )
{
	float3 v0 = (float3)(tri->v0x, tri->v0y, tri->v0z);
	float3 v1 = (float3)(tri->v1x, tri->v1y, tri->v1z);
	float3 v2 = (float3)(tri->v2x, tri->v2y, tri->v2z);
	float3 edge1 = v1 - v0, edge2 = v2 - v0;
	float3 h = cross( ray->D, edge2 );
	float a = dot( edge1, h );
	if (a > -0.00001f && a < 0.00001f) return; // ray parallel to triangle
	float f = 1 / a;
	float3 s = ray->O - v0;
	float u = f * dot( s, h );
	if (u < 0 || u > 1) return;
	const float3 q = cross( s, edge1 );
	const float v = f * dot( ray->D, q );
	if (v < 0 || u + v > 1) return;
	const float t = f * dot( edge2, q );
	if (t > 0.0001f && t < ray->hit.t)
		ray->hit.t = t, ray->hit.u = u,
		ray->hit.v = v, ray->hit.instPrim = instPrim;
}

float IntersectAABB( struct Ray* ray, struct BVHNode* node )
{
	float tx1 = (node->minx - ray->O.x) * ray->rD.x, tx2 = (node->maxx - ray->O.x) * ray->rD.x;
	float tmin = min( tx1, tx2 ), tmax = max( tx1, tx2 );
	float ty1 = (node->miny - ray->O.y) * ray->rD.y, ty2 = (node->maxy - ray->O.y) * ray->rD.y;
	tmin = max( tmin, min( ty1, ty2 ) ), tmax = min( tmax, max( ty1, ty2 ) );
	float tz1 = (node->minz - ray->O.z) * ray->rD.z, tz2 = (node->maxz - ray->O.z) * ray->rD.z;
	tmin = max( tmin, min( tz1, tz2 ) ), tmax = min( tmax, max( tz1, tz2 ) );
	if (tmax >= tmin && tmin < ray->hit.t && tmax > 0) return tmin; else return 1e30f;
}

void BVHIntersect( struct Ray* ray, uint instanceIdx, 
	struct Tri* tri, struct BVHNode* bvhNode, uint* triIdx )
{
	ray->rD = (float3)( 1 / ray->D.x, 1 / ray->D.y, 1 / ray->D.z );
	struct BVHNode* node = &bvhNode[0], *stack[32];
	uint stackPtr = 0;
	while (1)
	{
		if (node->triCount > 0) // isLeaf()
		{
			for (uint i = 0; i < node->triCount; i++)
			{
				uint instPrim = (instanceIdx << 20) + triIdx[node->leftFirst + i];
				IntersectTri( ray, &tri[instPrim & 0xfffff /* 20 bits */], instPrim );
			}
			if (stackPtr == 0) break; else node = stack[--stackPtr];
			continue;
		}
		struct BVHNode* child1 = &bvhNode[node->leftFirst];
		struct BVHNode* child2 = &bvhNode[node->leftFirst + 1];
		float dist1 = IntersectAABB( ray, child1 );
		float dist2 = IntersectAABB( ray, child2 );
		if (dist1 > dist2) 
		{ 
			float d = dist1; dist1 = dist2; dist2 = d;
			struct BVHNode* c = child1; child1 = child2; child2 = c; 
		}
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

float3 Trace( struct Ray* ray, float* skyPixels, struct Tri* triData, struct BVHNode* bvhNodeData, uint* idxData )
{
	// see if we hit a teapot
	BVHIntersect( ray, 0, triData, bvhNodeData, idxData );
	if (ray->hit.t < 1e30f) return (float3)(1,1,1);
	// sample sky
	float phi = atan2( ray->D.z, ray->D.x );
	uint u = (uint)(3200 * (phi > 0 ? phi : (phi + 2 * PI)) * INV2PI - 0.5f);
	uint v = (uint)(1600 * acos( ray->D.y ) * INVPI - 0.5f);
	uint skyIdx = (u + v * 3200) % (3200 * 1600);
	return 0.65f * (float3)(skyPixels[skyIdx * 3], skyPixels[skyIdx * 3 + 1], skyPixels[skyIdx * 3 + 2]);
}

__kernel void render( __global uint* target, __global float* skyPixels,
	__global struct Tri* triData, __global struct TriEx* triExData,
	__global uint* texData, __global struct TLASNode* tlasData,
	__global struct BVHInstance* instData,
	__global struct BVHNode* bvhNodeData, __global uint* idxData,
	float3 camPos, float3 p0, float3 p1, float3 p2 
)
{
	// plot a pixel into the target array in GPU memory
	int threadIdx = get_global_id( 0 );
	if (threadIdx >= SCRWIDTH * SCRHEIGHT) return;
	int x = threadIdx % SCRWIDTH;
	int y = threadIdx / SCRWIDTH;
	// create a primary ray for the pixel
	struct Ray ray;
	ray.O = camPos;
	float3 pixelPos = ray.O + p0 +
		(p1 - p0) * ((float)x / SCRWIDTH) +
		(p2 - p0) * ((float)y / SCRHEIGHT);
	ray.D = normalize( pixelPos - ray.O );
	ray.hit.t = 1e30f; // 1e30f denotes 'no hit'
	// trace the primary ray
	float3 color = Trace( &ray, skyPixels, triData, bvhNodeData, idxData );
	// plot the result
	target[x + y * SCRWIDTH] = RGB32FtoRGB8( color );
}

// EOF