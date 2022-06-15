#include "precomp.h"
#include "bvh.h"
#include "gpgpu.h"

// THIS SOURCE FILE:
// Code for the article "How to Build a BVH", part 9: GPGPU.
// This version shows how to render a scene using ray tracing on the
// GPU - without hardware ray tracing. The scene (and accstruc) is
// fully maintained on the CPU.
// Feel free to copy this code to your own framework. Absolutely no
// rights are reserved. No responsibility is accepted either.
// For updates, follow me on twitter: @j_bikker.

TheApp* CreateApp() { return new GPGPUApp(); }

// GPGPUApp implementation

void GPGPUApp::Init()
{
	mesh = new Mesh( "assets/teapot.obj", "assets/bricks.png" );
	for (int i = 0; i < 16; i++)
		bvhInstance[i] = BVHInstance( mesh->bvh, i );
	tlas = TLAS( bvhInstance, 16 );
	// load HDR sky
	skyPixels = stbi_loadf( "assets/sky_19.hdr", &skyWidth, &skyHeight, &skyBpp, 0 );
	for (int i = 0; i < skyWidth * skyHeight * 3; i++) skyPixels[i] = sqrtf( skyPixels[i] );
	// prepare OpenCL
	tracer = new Kernel( "cl/kernels.cl", "render" );
	target = new Buffer( SCRWIDTH * SCRHEIGHT * 4 ); // intermediate screen buffer / render target
	skyData = new Buffer( skyWidth * skyHeight * 3 * sizeof( float ), skyPixels );
	skyData->CopyToDevice();
	triData = new Buffer( 1024 * sizeof( Tri ), mesh->tri );
	triExData = new Buffer( 1024 * sizeof( TriEx ), mesh->triEx );
	Surface* tex = mesh->texture;
	texData = new Buffer( tex->width * tex->height * sizeof( uint ), tex->pixels );
	instData = new Buffer( 256 * sizeof( BVHInstance ), bvhInstance );
	tlasData = new Buffer( tlas.nodesUsed * sizeof( TLASNode ), tlas.tlasNode );
	bvhData = new Buffer( mesh->bvh->nodesUsed * sizeof( BVHNode ), mesh->bvh->bvhNode );
	idxData = new Buffer( 1024 * sizeof( uint ), mesh->bvh->triIdx );
	triData->CopyToDevice();
	triExData->CopyToDevice();
	texData->CopyToDevice();
	instData->CopyToDevice();
	bvhData->CopyToDevice();
	idxData->CopyToDevice();
}

void GPGPUApp::AnimateScene()
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
  
void GPGPUApp::Tick( float deltaTime )
{
	// update the TLAS
	AnimateScene();
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