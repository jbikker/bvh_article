#include "precomp.h"
#include "bvh.h"
#include "beyond.h"

// CODE IS UNDER CONSTRUCTION

// THIS SOURCE FILE:
// Improvements made after the final article in the "How to Build a 
// BVH" series. In this code: 
// Faster agglomerative clustering, enabling a moving flock of dragons;
// Faster BVH construction, enabling exploding dragons.
// Feel free to copy this code to your own framework. Absolutely no
// rights are reserved. No responsibility is accepted either.
// For updates, follow me on twitter: @j_bikker.

TheApp* CreateApp() { return new BeyondApp(); }

// MassiveApp implementation

float3 camPos( 0, 0, 0 ), camTarget( 0, 0, 1 );

void BeyondApp::HandleKeys()
{
	float3 V = normalize( camTarget - camPos );
	float3 R = normalize( cross( float3( 0, 1, 0 ), V ) );
	float3 up = normalize( cross( V, R ) );
	if (GetAsyncKeyState( 'W' )) camPos += 0.05f * V;
	if (GetAsyncKeyState( 'S' )) camPos -= 0.05f * V;
	if (GetAsyncKeyState( 'A' )) camPos += 0.05f * R;
	if (GetAsyncKeyState( 'D' )) camPos -= 0.05f * R;
	if (GetAsyncKeyState( 'R' )) camPos += 0.05f * up;
	if (GetAsyncKeyState( 'F' )) camPos -= 0.05f * up;
	camTarget = camPos + V;
	if (GetAsyncKeyState( VK_LEFT )) camTarget += 0.05f * R;
	if (GetAsyncKeyState( VK_RIGHT )) camTarget -= 0.05f * R;
	if (GetAsyncKeyState( VK_UP )) camTarget -= 0.02f * up;
	if (GetAsyncKeyState( VK_DOWN )) camTarget += 0.02f * up;
}

void BeyondApp::Init()
{
	mesh = new Mesh( "assets/dragon.obj", "assets/bricks.png" );
	// load HDR sky
	skyPixels = stbi_loadf( "assets/sky_19.hdr", &skyWidth, &skyHeight, &skyBpp, 0 );
	for (int i = 0; i < skyWidth * skyHeight * 3; i++) skyPixels[i] = sqrtf( skyPixels[i] );
	// dragons in the shape of a dragon
	bvhInstance = new BVHInstance[11042];
	for( int i = 0; i < 11042; i++ )
	{
		bvhInstance[i] = BVHInstance( mesh->bvh, i );
		bvhInstance[i].SetTransform( 
			mat4::Translate( mesh->P[i] * 0.2f ) * 
			mat4::Scale( 0.0025f  ) *
			mat4::Rotate( mesh->N[i], 0 ) *
			mat4::RotateX( PI / 2 )
		);
	}
	tlas = TLAS( bvhInstance, 11042 );
	Timer t;
	tlas.Build();
	printf( "building TLAS took %.2fms.\n", t.elapsed() * 1000 );
	// prepare OpenCL
	tracer = new Kernel( "cl/raytracer.cl", "render" );
	target = new Buffer( GetRenderTarget()->ID, 0, Buffer::TARGET );
	screen = 0;
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
	tlasData->CopyToDevice();
}
 
void BeyondApp::Tick( float deltaTime )
{
	// construct camera matrix
	HandleKeys();
	mat4 M = mat4::LookAt( camPos, camTarget );
	static float ar = (float)SCRWIDTH / SCRHEIGHT;
	p0 = TransformPosition( float3( -1 * ar, 1, 1.5f ), M );
	p1 = TransformPosition( float3( 1 * ar, 1, 1.5f ), M );
	p2 = TransformPosition( float3( -1 * ar, -1, 1.5f ), M );
	// render the scene using the GPU
	tracer->SetArguments( 
		target, skyData, 
		triData, triExData, texData, tlasData, instData, bvhData, idxData, 
		camPos, p0, p1, p2 
	);
	tracer->Run( SCRWIDTH * SCRHEIGHT );
}

// EOF