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

void BeyondApp::HandleKeys( float dt )
{
	float3 V = normalize( camTarget - camPos );
	float3 R = normalize( cross( float3( 0, 1, 0 ), V ) );
	float3 up = normalize( cross( V, R ) );
	float s = 0.005f * dt;
	if (GetAsyncKeyState( 'W' )) camPos += s * V;
	if (GetAsyncKeyState( 'S' )) camPos -= s * V;
	if (GetAsyncKeyState( 'A' )) camPos += s * R;
	if (GetAsyncKeyState( 'D' )) camPos -= s * R;
	if (GetAsyncKeyState( 'R' )) camPos += s * up;
	if (GetAsyncKeyState( 'F' )) camPos -= s * up;
	camTarget = camPos + V;
	if (GetAsyncKeyState( VK_LEFT )) camTarget += s * R;
	if (GetAsyncKeyState( VK_RIGHT )) camTarget -= s * R;
	if (GetAsyncKeyState( VK_UP )) camTarget -= s * 2 * up;
	if (GetAsyncKeyState( VK_DOWN )) camTarget += s * 2 * up;
}

void BeyondApp::Init()
{
	mesh = new Mesh( "assets/dragon.obj", "assets/bricks.png" );
	// load HDR sky
	skyPixels = stbi_loadf( "assets/sky_19.hdr", &skyWidth, &skyHeight, &skyBpp, 0 );
	for (int i = 0; i < skyWidth * skyHeight * 3; i++) skyPixels[i] = sqrtf( skyPixels[i] );
	// dragons in the shape of a dragon
#if 0
	bvhInstance = new BVHInstance[16];
	for (int i = 0; i < 16; i++)
		bvhInstance[i] = BVHInstance( mesh->bvh, i );
	tlas = TLAS( bvhInstance, 16 );
	// animate the scene
	static float h[16] = { 5, 4, 3, 2, 1, 5, 4, 3 }, s[16] = { 0 };
	for (int i = 0, x = 0; x < 4; x++) for (int y = 0; y < 4; y++, i++)
	{
		mat4 R, T = mat4::Translate( (x - 1.5f) * 2.5f, 0, (y - 1.5f) * 2.5f );
		if ((x + y) & 1) R = mat4::RotateY( i * 0.2f );
		else R = mat4::Translate( 0, h[i / 2], 0 );
		bvhInstance[i].SetTransform( T * R * mat4::Scale( 0.015f ) );
	}
#else
	uint instances = mesh->triCount;
	bvhInstance = new BVHInstance[instances];
	for( uint i = 0; i < instances; i++ )
	{
		uint f = i;
		float3 P = (mesh->tri[f].vertex0 + mesh->tri[f].vertex1 + mesh->tri[f].vertex2) / 3;
		bvhInstance[i] = BVHInstance( mesh->bvh, i );
		bvhInstance[i].SetTransform( 
			mat4::Translate( P * 0.2f ) * 
			mat4::Scale( 0.0025f  ) *
			mat4::Rotate( mesh->N[i & 1023], 0 ) *
			mat4::RotateX( PI / 2 )
		);
	}
	tlas = TLAS( bvhInstance, instances );
#endif
	tlas.BuildQuick();
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
	instData = new Buffer( tlas.blasCount * sizeof( BVHInstance ), bvhInstance );
	tlasData = new Buffer( (tlas.blasCount * 2 + 64) * sizeof( TLASNode ), tlas.tlasNode );
	bvhData = new Buffer( mesh->bvh->nodesUsed * sizeof( BVHNode ), mesh->bvh->bvhNode );
	idxData = new Buffer( mesh->triCount * sizeof( uint ), mesh->bvh->triIdx );
	triData->CopyToDevice();
	triExData->CopyToDevice();
	texData->CopyToDevice();
	instData->CopyToDevice();
	bvhData->CopyToDevice();
	idxData->CopyToDevice();
	tlasData->CopyToDevice();
	// fetch camera
	FILE* f = fopen( "camera.bin", "rb" );
	if (f)
	{
		fread( &camPos, 1, sizeof( camPos ), f );
		fread( &camTarget, 1, sizeof( camTarget ), f );
		fclose( f );
	}
}

void CL_CALLBACK process( cl_event ev, cl_int status, void* data )
{
	cl_ulong start, end;
	clGetEventProfilingInfo( ev, CL_PROFILING_COMMAND_START, sizeof( cl_ulong ), &start, NULL );
	clGetEventProfilingInfo( ev, CL_PROFILING_COMMAND_END, sizeof( cl_ulong ), &end, NULL );
	float sec = ((float)(end - start)) / 1000000000.0f;
	static float avg = 0;
	static int frames = 0;
	if (frames++ < 10) avg = sec; else avg = 0.95f * avg + 0.05f * sec;
	printf( "kernel time: %.2fms (%.2fMrays/s)\n", sec * 1000, ((float)(512 * 1024 * 16) / 1000000) / avg );
}

void BeyondApp::Tick( float deltaTime )
{
#if 1
	// rebuild the TLAS
	static int frameCount = 500;
	if (frameCount-- > 0)
	{
		Timer t;
		tlas.BuildQuick();
		printf( "TLAS update: %.2fms\n", t.elapsed() * 1000 );
		tlasData->CopyToDevice();
	}
#endif
	// construct camera matrix
	HandleKeys( deltaTime );
	mat4 M = mat4::LookAt( camPos, camTarget );
	static float ar = (float)SCRWIDTH / SCRHEIGHT;
	p0 = TransformPosition( float3( -1 * ar, 1, 1.5f ), M );
	p1 = TransformPosition( float3( 1 * ar, 1, 1.5f ), M );
	p2 = TransformPosition( float3( -1 * ar, -1, 1.5f ), M );
	// render the scene using the GPU & gather profling information
	tracer->SetArguments( 
		target, skyData, 
		triData, triExData, texData, tlasData, instData, bvhData, idxData, 
		camPos, p0, p1, p2 
	);
	static bool inited = false;
	static cl_event ev;
	if (!inited) ev = clCreateUserEvent( tracer->GetContext(), 0 ), inited = true;
	// clSetEventCallback( ev, CL_COMPLETE, &process, NULL );
	tracer->Run( SCRWIDTH * SCRHEIGHT, 0, 0, &ev );
}

void BeyondApp::Shutdown()
{
	FILE* f = fopen( "camera.bin", "wb" );
	fwrite( &camPos, 1, sizeof( camPos ), f );
	fwrite( &camTarget, 1, sizeof( camTarget ), f );
	fclose( f );
}

// EOF