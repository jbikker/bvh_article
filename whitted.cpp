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

// WhittedApp implementation

void WhittedApp::Init()
{
	Mesh* mesh = new Mesh( "assets/teapot.obj", "assets/bricks.png" );
	for (int i = 0; i < 16; i++)
		bvhInstance[i] = BVHInstance( mesh->bvh, i );
	tlas = TLAS( bvhInstance, 16 );
	// setup screen plane in world space
	p0 = TransformPosition( float3( -1, 1, 2 ), mat4::RotateX( 0.5f ) );
	p1 = TransformPosition( float3( 1, 1, 2 ), mat4::RotateX( 0.5f ) );
	p2 = TransformPosition( float3( -1, -1, 2 ), mat4::RotateX( 0.5f ) );
	// create a floating point accumulator for the screen
	accumulator = new float3[640 * 640];
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
	// TODO: skydome
	if (i.t == 1e30f) return float3( 0 );
	// TODO: handle primary intersection result
	return float3( i.u, i.v, 1 - (i.u + i.v) );
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