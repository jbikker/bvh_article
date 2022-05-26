#include "precomp.h"
#include "bvh.h"
#include "whitted.h"

// THIS CODE IS UNDER CONSTRUCTION - WILL BE USED FOR ARTICLE #8.

// THIS SOURCE FILE:
// Code for the article "How to Build a BVH", part 8: Whitted.
// This version shows how to build a simple Whitted-style ray tracer
// as a test case for the BVH code of the previous articles. This is
// also the final preparation for the GPGPU code in article 9.
// Feel free to copy this code to your own framework. Absolutely no
// rights are reserved. No responsibility is accepted either.
// For updates, follow me on twitter: @j_bikker.

TheApp* CreateApp() { return new WhittedApp(); }

inline float3 RGB8toRGB32F( uint c )
{
	float s = 1 / 256.0f;
	int r = (c >> 16) & 255;
	int g = (c >> 8) & 255;
	int b = c & 255;
	return float3( r * s, g * s, b * s );
}

// WhittedApp implementation

void WhittedApp::Init()
{
	mesh = new Mesh( "assets/teapot.obj", "assets/bricks.png" );
	for (int i = 0; i < 16; i++)
		bvhInstance[i] = BVHInstance( mesh->bvh, i );
	tlas = TLAS( bvhInstance, 16 );
	// setup screen plane in world space
	p0 = TransformPosition( float3( -1, 1, 2 ), mat4::RotateX( 0.5f ) );
	p1 = TransformPosition( float3( 1, 1, 2 ), mat4::RotateX( 0.5f ) );
	p2 = TransformPosition( float3( -1, -1, 2 ), mat4::RotateX( 0.5f ) );
	// create a floating point accumulator for the screen
	accumulator = new float3[640 * 640];
	// load HDR sky
	int bpp = 0;
	skyPixels = stbi_loadf( "assets/sky_19.hdr", &skyWidth, &skyHeight, &skyBpp, 0 );
}

void WhittedApp::AnimateScene()
{
	// animate the scene
	static float a[16] = { 0 }, h[16] = { 5, 4, 3, 2, 1, 5, 4, 3 }, s[16] = { 0 };
	for (int i = 0, x = 0; x < 4; x++) for (int y = 0; y < 4; y++, i++)
	{
		mat4 R, T = mat4::Translate( (x - 1.5f) * 2.5f, 0, (y - 1.5f) * 2.5f );
		if ((x + y) & 1) R = mat4::RotateY( a[i] );
		else R = mat4::Translate( 0, h[i / 2], 0 );
		if ((a[i] += (((i * 13) & 7) + 2) * 0.005f) > 2 * PI) a[i] -= 2 * PI;
		if ((s[i] -= 0.01f, h[i] += s[i]) < 0) s[i] = 0.2f;
		bvhInstance[i].SetTransform( T * R * mat4::Scale( 1.5f ) );
	}
	// update the TLAS
	tlas.Build();
}

float3 WhittedApp::Trace( Ray& ray )
{
	tlas.Intersect( ray );
	Intersection i = ray.hit;
	if (i.t == 1e30f) 
	{
		// sample sky
		float stheta = acosf( clamp( ray.D.z, -1.f, 1.f ) );
		uint u = skyWidth * atan2f( ray.D.y, ray.D.x ) * INV2PI - 0.5f;
		uint v = skyHeight * stheta * INVPI - 0.5f;
		uint skyIdx = u + v * skyWidth;
		return 0.35f * float3( skyPixels[skyIdx * 3], skyPixels[skyIdx * 3 + 1], skyPixels[skyIdx * 3 + 2] );
	}
	// calculate texture uv based on barycentrics
	uint triIdx = i.instPrim & 0xfffff;
	uint instIdx = i.instPrim >> 20;
	TriEx& tri = mesh->triEx[triIdx];
	Surface* tex = mesh->texture;
	float2 uv = i.u * tri.uv1 + i.v * tri.uv2 + (1 - (i.u + i.v)) * tri.uv0;
	int iu = (int)(uv.x * tex->width) % tex->width;
	int iv = (int)(uv.y * tex->height) % tex->height;
	uint texel = tex->pixels[iu + iv * tex->width];
	float3 albedo = RGB8toRGB32F( texel );
	// calculate the normal for the intersection
	float3 N = i.u * tri.N1 + i.v * tri.N2 + (1 - (i.u + i.v)) * tri.N0;
	N = normalize( TransformVector( N, bvhInstance[instIdx].GetTransform() ) );
	// illuminate the intersection point
	float3 lightPos( 3, 10, 2 );
	float3 lightColor( 150, 150, 120 );
	float3 ambient( 0.2f, 0.2f, 0.4f );
	float3 I = ray.O + i.t * ray.D;
	float3 L = lightPos - I;
	float dist = length( L );
	L *= 1.0f / dist;
	return albedo * (ambient + max( 0.0f, dot( N, L ) ) * lightColor * (1.0f / (dist * dist)));
}

void WhittedApp::Tick( float deltaTime )
{
	// update the TLAS
	AnimateScene();
	// render the scene: multithreaded tiles
#pragma omp parallel for schedule(dynamic)
	for (int tile = 0; tile < 6400; tile++)
	{
		// render an 8x8 tile
		int x = tile % 80, y = tile / 80;
		Ray ray;
		ray.O = float3( 0, 3, -6.5f );
		for (int v = 0; v < 8; v++) for (int u = 0; u < 8; u++)
		{
			// setup a primary ray
			float3 pixelPos = ray.O + p0 +
				(p1 - p0) * ((x * 8 + u) / 640.0f) +
				(p2 - p0) * ((y * 8 + v) / 640.0f);
			ray.D = normalize( pixelPos - ray.O );
			ray.hit.t = 1e30f; // 1e30f denotes 'no hit'
			uint pixelAddress = x * 8 + u + (y * 8 + v) * 640;
			accumulator[pixelAddress] = Trace( ray );
		}
	}
	// convert the floating point accumulator into pixels
	for( int i = 0; i < 640 * 640; i++ )
	{
		int r = min( 255, (int)(255 * accumulator[i].x) );
		int g = min( 255, (int)(255 * accumulator[i].y) );
		int b = min( 255, (int)(255 * accumulator[i].z) );
		screen->pixels[i] = (r << 16) + (g << 8) + b;
	}
}

// EOF