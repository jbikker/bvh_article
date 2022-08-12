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
#if 1
	// default renderer
	int rayDepth = 0;
	float3 R;
	// bounce until we hit the sky or a diffuse surface
	while (rayDepth < 4)
	{
		TLASIntersect( ray, triData, instData, tlasData, bvhNodeData, idxData );
		struct Intersection i = ray->hit;
		if (i.t == 1e30f)
		{
			// sample sky
			return SampleSky( &ray->D, skyPixels );
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
		float3 I = ray->O + (ray->D * i.t);
		// shading
		bool mirror = (instIdx * 17) & 1;
		if (mirror)
		{
			// calculate the specular reflection in the intersection point
			float3 R = ray->D - (2 * N * dot( N, ray->D ));
			if (rayDepth == 1) return SampleSky( &R, skyPixels );
			ray->D = R;
			ray->O = I + ray->D * 0.005f;
			ray->hit.t = 1e30f;
		}
		else
		{
			// calculate the diffuse reflection in the intersection point
			float3 L = lightPos - I;
			float dist = length( L );
			L *= 1.0f / dist;
			return albedo * (ambient + max( 0.0f, dot( N, L ) ) * lightColor * (1.0f / (dist * dist)));
		}
		rayDepth++;
	}
	return (float3)( 1, 1, 1 );
#else
	// minimal depth renderer for performance experiments
	TLASIntersect( ray, triData, instData, tlasData, bvhNodeData, idxData );
	struct Intersection i = ray->hit;
	if (i.t == 1e30f) return (float3)( 0, 0, 0 );
	float d = 4.0f / i.t;
	return (float3)( d, d, d );
#endif
}

__kernel void render( 
	write_only image2d_t target,
	__global float* skyPixels,
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
	// intialize RNG
	uint seed = WangHash( threadIdx * 17 + 1 );
	// create a primary ray for the pixel
	struct Ray ray;
	float3 color = (float3)( 0, 0, 0 );
	for( int i = 0; i < 2; i++ )
	{
		float3 pixelPos = p0 +
			(p1 - p0) * (((float)x + RandomFloat( &seed )) / SCRWIDTH) +
			(p2 - p0) * (((float)y + RandomFloat( &seed )) / SCRHEIGHT);
		ray.O = camPos;
		ray.D = normalize( pixelPos - ray.O );
		ray.hit.t = 1e30f; // 1e30f denotes 'no hit'
		// trace the primary ray
		color += Trace( &ray, skyPixels, instData, tlasData, texData, triData, triExData, bvhNodeData, idxData );
	}
	write_imagef( target, (int2)(x, y), (float4)( color * (1.0f / 2.0f), 1 ) );
}

// EOF