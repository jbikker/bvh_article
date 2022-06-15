#include "precomp.h"
#include "bvh.h"
#include "massive.h"

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

float3 spline[] = {
	float3( 1.32, 11.35, 0.31 ), float3( 0.35, 11.56, 0.18 ),
	float3( -0.92, 11.35, 2.92 ), float3( -1.58, 11.55, 2.19 ),
	float3( -5.51, 13.43, 4.96 ), float3( -5.82, 13.63, 4.03 ),
	float3( -10.90, 13.48, 4.53 ), float3( -10.39, 13.68, 3.69 ),
	float3( -13.15, 15.33, 2.21 ), float3( -12.20, 15.52, 1.96 ),
	float3( -12.44, 15.33, -1.92 ), float3( -11.92, 15.51, -1.08 ),
	float3( -8.17, 15.48, -4.71 ), float3( -7.32, 15.38, -4.19 ),
	float3( -0.19, 13.22, -6.09 ), float3( 0.31, 12.72, -5.38 ),
	float3( 8.44, 8.37, -5.22 ), float3( 8.01, 7.81, -4.52 ),
	float3( 14.31, 4.71, -1.89 ), float3( 13.40, 4.46, -1.56 ),
	float3( 11.81, 4.97, 6.36 ), float3( 11.19, 4.65, 5.64 ),
	float3( 10.57, 4.58, 5.19 ), float3( 9.85, 4.39, 4.52 ),
	float3( 7.33, 4.66, 2.14 ), float3( 6.61, 4.59, 1.45 ),
	float3( 6.01, 4.77, 0.68 ), float3( 5.28, 4.70, 0 ),
	float3( 3.00, 4.48, -2.16 ), float3( 2.27, 4.42, -2.85 ),
	float3( -1.53, 4.27, -5.49 ), float3( -1.89, 4.21, -4.56 ),
	float3( -3.63, 4.12, -3.77 ), float3( -3.80, 4.06, -2.78 ),
	float3( -4.97, 3.97, 2.62 ), float3( -5.38, 3.98, 3.53 ),
	float3( -9.38, 3.97, 7.47 ), float3( -8.92, 3.98, 6.59 ),
	float3( -16.17, 5.36, 7.89 ), float3( -15.37, 5.37, 7.29 ),
	float3( -27.30, 10.08, -1.143 ), float3( -26.33, 10.00, -0.89 ),
	float3( -30.81, 10.88, -13.78 ), float3( -29.94, 10.80, -13.30 ),
};

float3 camPos( 0, 0, 0 ), camTarget( 0, 0, 1 );

float3 CatmullRom( float t, float3& p0, float3& p1, float3& p2, float3& p3 )
{
	float3 c = 2 * p0 - 5 * p1 + 4 * p2 - p3, d = 3 * (p1 - p2) + p3 - p0;
	return 0.5f * (2 * p1 + ((p2 - p0) * t) + (c * t * t) + (d * t * t * t));
}

void SplineCam( int seg, float t )
{
	float3 c0 = spline[seg * 2 - 2], c1 = spline[seg * 2], c2 = spline[seg * 2 + 2], c3 = spline[seg * 2 + 4];
	float3 t0 = spline[seg * 2 - 1], t1 = spline[seg * 2 + 1], t2 = spline[seg * 2 + 3], t3 = spline[seg * 2 + 5];
	camPos = CatmullRom( t, c0, c1, c2, c3 ), camTarget = CatmullRom( t, t0, t1, t2, t3 );
}

void MassiveApp::HandleKeys()
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
	// spline path
	static float Pdown = false;
	if (!GetAsyncKeyState( 'P' )) Pdown = false; else
	{
		if (!Pdown) return;
		static FILE* f = fopen( "spline.txt", "w" );
		fprintf( f, "float3( %.2f, %2f, %2f ), float3( %.2f, %2f, %2f ), \n",
			camPos.x, camPos.y, camPos.z, camTarget.x, camTarget.y, camTarget.z );
		Pdown = true;
	}
}

void MassiveApp::Init()
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
	// target = new Buffer( SCRWIDTH * SCRHEIGHT * 4 ); // intermediate screen buffer / render target
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
 
void MassiveApp::Tick( float deltaTime )
{
	// construct camera matrix
#if 1
	// playback spline path
	static int seg = 1;
	static float t = 0;
	t += deltaTime * 0.0005f;
	if (t > 1) { t -= 1.0f; if (++seg == 20) seg = 1; }
	SplineCam( seg, t );
#else
	// press 'P' to record spline path vertices.
	HandleKeys();
#endif
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
	// obtain the rendered result
	// target->CopyFromDevice();
	// memcpy( screen->pixels, target->GetHostPtr(), target->size );
}

// EOF