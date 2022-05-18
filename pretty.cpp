#include "precomp.h"
#include "bvh.h"
#include "pretty.h"

// THIS CODE IS UNDER CONSTRUCTION - WILL BE USED FOR ARTICLE #7.

// THIS SOURCE FILE:
// Code for the article "How to Build a BVH", part 7: consolidation.
// This version shows how to ray trace a textured mesh, with basic
// shading using interpolated normals and a Lambertian material.
// Feel free to copy this code to your own framework. Absolutely no
// rights are reserved. No responsibility is accepted either.
// For updates, follow me on twitter: @j_bikker.

TheApp* CreateApp() { return new PrettyApp(); }

// TopLevelApp implementation

void PrettyApp::Init()
{
	Mesh* mesh = new Mesh( "assets/teapot.obj", "assets/bricks.png" );
	for (int i = 0; i < 256; i++)
		bvhInstance[i] = BVHInstance( mesh->bvh );
	tlas = TLAS( bvhInstance, 256 );
	// set up spacy armadillo army
	position = new float3[256];
	direction = new float3[256];
	orientation = new float3[256];
	for( int i = 0; i < 256; i++ )
	{
		position[i] = 4 * float3( RandomFloat(), RandomFloat(), RandomFloat() ) - 2;
		direction[i] = normalize( position[i] ) * 0.05f;
		orientation[i] = float3( RandomFloat(), RandomFloat(), RandomFloat() ) * 2.5f;
	}
}

void PrettyApp::Tick( float deltaTime )
{
	// animate the scene
	for( int i = 0; i < 256; i++ )
	{
		mat4 R = mat4::RotateX( orientation[i].x ) * 
				 mat4::RotateY( orientation[i].y ) *
				 mat4::RotateZ( orientation[i].z ) * mat4::Scale( 0.2f );
		bvhInstance[i].SetTransform( mat4::Translate( position[i] ) * R );
		position[i] += direction[i], orientation[i] += direction[i];
		if (position[i].x < -3 || position[i].x > 3) direction[i].x *= -1;
		if (position[i].y < -3 || position[i].y > 3) direction[i].y *= -1;
		if (position[i].z < -3 || position[i].z > 3) direction[i].z *= -1;
	}
	// update the TLAS
	Timer t;
	tlas.Build();
	float tlasTime = t.elapsed() * 1000;
	// draw the scene
	float3 p0( -1, 1, 2 ), p1( 1, 1, 2 ), p2( -1, -1, 2 );
#pragma omp parallel for schedule(dynamic)
	for (int tile = 0; tile < 6400; tile++)
	{
		int x = tile % 80, y = tile / 80;
		Ray ray;
		ray.O = float3( 0, 0, -6.5f );
		for (int v = 0; v < 8; v++) for (int u = 0; u < 8; u++)
		{
			float3 pixelPos = ray.O + p0 +
				(p1 - p0) * ((x * 8 + u) / 640.0f) +
				(p2 - p0) * ((y * 8 + v) / 640.0f);
			ray.D = normalize( pixelPos - ray.O ), ray.t = 1e30f;
			tlas.Intersect( ray );
			uint c = ray.t < 1e30f ? (int)(255 / (1 + max( 0.f, ray.t - 4 ))) : 0;
			screen->Plot( x * 8 + u, y * 8 + v, c * 0x10101 );
		}
	}
	// report
	float elapsed = t.elapsed() * 1000;
	printf( "tlas build: %.2fms, tracing time: %.2fms (%5.2fK rays/s)\n", tlasTime, elapsed, sqr( 630 ) / elapsed );
}

// EOF