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
	struct Intersection hit;	// total ray size: 64 bytes
};

float3 Trace( struct Ray ray, float* skyPixels )
{
	// sample sky
	float phi = atan2( ray.D.z, ray.D.x );
	uint u = (uint)(3200 * (phi > 0 ? phi : (phi + 2 * PI)) * INV2PI - 0.5f);
	uint v = (uint)(1600 * acos( ray.D.y ) * INVPI - 0.5f);
	uint skyIdx = (u + v * 3200) % (3200 * 1600);
	return 0.65f * (float3)(skyPixels[skyIdx * 3], skyPixels[skyIdx * 3 + 1], skyPixels[skyIdx * 3 + 2]);
}

__kernel void render( __global uint* target, __global float* skyData, float3 camPos, float3 p0, float3 p1, float3 p2 )
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
	float3 color = Trace( ray, skyData );
	// plot the result
	target[x + y * SCRWIDTH] = RGB32FtoRGB8( color );
}

// EOF