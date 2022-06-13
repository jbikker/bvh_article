#include "precomp.h"
#include "bvh.h"
#include "massive.h"

// THIS FILE IS UNDER CONSTRUCTION - WILL BE USED WITH ARTICLE #10

// THIS SOURCE FILE:
// Code for the article "How to Build a BVH", part 10: Massive.
// This version shows how to render a scene with massive instancing
// on the GPU - without hardware ray tracing. A dragon of dragons is
// constructed by replacing each vertex by a dragon.
// Feel free to copy this code to your own framework. Absolutely no
// rights are reserved. No responsibility is accepted either.
// For updates, follow me on twitter: @j_bikker.

TheApp* CreateApp() { return new MassiveApp(); }

// MassiveApp implementation

void MassiveApp::Init()
{
	mesh = new Mesh( "assets/dragon.obj", "assets/bricks.png" );
	// load HDR sky
	int bpp = 0;
	skyPixels = stbi_loadf( "assets/sky_19.hdr", &skyWidth, &skyHeight, &skyBpp, 0 );
	for (int i = 0; i < skyWidth * skyHeight * 3; i++) skyPixels[i] = sqrtf( skyPixels[i] );
	// dragons in the shape of a dragon
	bvhInstance = new BVHInstance[11042];
	for( int i = 0; i < 11042; i++ )
	{
		bvhInstance[i] = BVHInstance( mesh->bvh, i );
		bvhInstance[i].SetTransform( 
			mat4::Translate( mesh->vertices[i] * 0.2f ) * 
			mat4::Scale( 0.0025f  ) 
		);
	}
	tlas = TLAS( bvhInstance, 11042 );
	Timer t;
	tlas.Build();
	printf( "building TLAS took %.2fms.\n", t.elapsed() * 1000 );
	// prepare OpenCL
	tracer = new Kernel( "cl/raytracer.cl", "render" );
	target = new Buffer( SCRWIDTH * SCRHEIGHT * 4 ); // intermediate screen buffer / render target
	skyData = new Buffer( skyWidth * skyHeight * 3 * sizeof( float ), skyPixels );
	skyData->CopyToDevice();
	triData = new Buffer( mesh->triCount * sizeof( Tri ), mesh->tri );
	triExData = new Buffer( mesh->triCount * sizeof( TriEx ), mesh->triEx );
	Surface* tex = mesh->texture;
	texData = new Buffer( tex->width * tex->height * sizeof( uint ), tex->pixels );
	instData = new Buffer( 11042 * sizeof( BVHInstance ), bvhInstance );
	tlasData = new Buffer( 11042 * 2 * sizeof( TLASNode ), tlas.tlasNode );
	bvhData = new Buffer( mesh->bvh->nodesUsed * sizeof( BVHNode ), mesh->bvh->bvhNode );
	idxData = new Buffer( mesh->triCount * sizeof( uint ), mesh->bvh->triIdx );
	triData->CopyToDevice();
	triExData->CopyToDevice();
	texData->CopyToDevice();
	instData->CopyToDevice();
	bvhData->CopyToDevice();
	idxData->CopyToDevice();
}
 
void MassiveApp::Tick( float deltaTime )
{
	// update the TLAS
	instData->CopyToDevice();
	tlasData->CopyToDevice();
	// setup screen plane in world space
	static float angle = 0, ar = (float)SCRWIDTH / SCRHEIGHT; angle += 0.001f;
	mat4 M1 = mat4::RotateY( angle ), M2 = M1 * mat4::RotateX( -0.65f );
	p0 = TransformPosition( float3( -1 * ar, 1, 1.5f ), M2 );
	p1 = TransformPosition( float3( 1 * ar, 1, 1.5f ), M2 );
	p2 = TransformPosition( float3( -1 * ar, -1, 1.5f ), M2 );
	float3 camPos = TransformPosition( float3( 0, -2, -8.5f ), M1 );
	// render the scene using the GPU
	tracer->SetArguments( 
		target, skyData, 
		triData, triExData, texData, tlasData, instData, bvhData, idxData, 
		camPos, p0, p1, p2 
	);
	tracer->Run( SCRWIDTH * SCRHEIGHT );
	// obtain the rendered result
	target->CopyFromDevice();
	memcpy( screen->pixels, target->GetHostPtr(), target->size );
}

// EOF