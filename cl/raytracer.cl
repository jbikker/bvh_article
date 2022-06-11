#include "template/common.h"
#include "cl/tools.cl"

__constant float3 lightPos = (float3)(3, 10, 2);
__constant float3 lightColor = (float3)(150, 150, 120);
__constant float3 ambient = (float3)(0.2f, 0.2f, 0.4f);

float3 Trace( struct Ray* ray, float* skyPixels, 
	struct BVHInstance* instData, struct TLASNode* tlasData,
	uint* texData, struct Tri* triData, struct TriEx* triExData,
	struct BVHNode* bvhNodeData, uint* idxData 
)
{
	int rayDepth = 0;
	// bounce until we hit the sky or a diffuse surface
	while (1)
	{
		TLASIntersect( ray, triData, instData, tlasData, bvhNodeData, idxData );
		struct Intersection i = ray->hit;
		if (i.t == 1e30f)
		{
			// sample sky
			float phi = atan2( ray->D.z, ray->D.x );
			uint u = (uint)(3200 * (phi > 0 ? phi : (phi + 2 * PI)) * INV2PI - 0.5f);
			uint v = (uint)(1600 * acos( ray->D.y ) * INVPI - 0.5f);
			uint skyIdx = (u + v * 3200) % (3200 * 1600);
			return 0.65f * (float3)(skyPixels[skyIdx * 3], skyPixels[skyIdx * 3 + 1], skyPixels[skyIdx * 3 + 2]);
		}
		// calculate texture uv based on barycentrics
		uint triIdx = i.instPrim & 0xfffff;
		uint instIdx = i.instPrim >> 20;
		struct TriEx* tri = triExData + triIdx;
		float2 uv = i.u * tri->uv1 + i.v * tri->uv2 + (1 - (i.u + i.v)) * tri->uv0;
		int iu = (int)(uv.x * 1024) & 1023;
		int iv = (int)(uv.y * 1024) & 1023;
		uint texel = texData[iu + (iv << 10)];
		float3 albedo = RGB8toRGB32F( texel );
		// calculate the normal for the intersection
		float3 N0 = (float3)( tri->N0x, tri->N0y, tri->N0z );
		float3 N1 = (float3)( tri->N1x, tri->N1y, tri->N1z );
		float3 N2 = (float3)( tri->N2x, tri->N2y, tri->N2z );
		float3 N = i.u * N1 + i.v * N2 + (1 - (i.u + i.v)) * N0;
		N = normalize( TransformVector( &N, &instData[instIdx].transform ) );
		float3 I = ray->O + i.t * ray->D;
		// shading
		bool mirror = (instIdx * 17) & 1;
		if (mirror)
		{
			// calculate the specular reflection in the intersection point
			if (rayDepth >= 2) return (float3)( 1, 1, 1 );
			ray->D = ray->D - 2 * N * dot( N, ray->D );
			ray->O = I + ray->D * 0.001f;
			ray->hit.t = 1e30f;
			rayDepth++;
		}
		else
		{
			// calculate the diffuse reflection in the intersection point
			float3 L = lightPos - I;
			float dist = length( L );
			L *= 1.0f / dist;
			return albedo * (ambient + max( 0.0f, dot( N, L ) ) * lightColor * (1.0f / (dist * dist)));
		}
	}
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
	float3 color = Trace( &ray, skyPixels, instData, tlasData, texData, triData, triExData, bvhNodeData, idxData );
	// plot the result
	target[x + y * SCRWIDTH] = RGB32FtoRGB8( color );
}

// EOF