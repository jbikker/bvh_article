#include "precomp.h"
#include "bvh.h"
#include "beyond.h"

// CODE IS UNDER CONSTRUCTION

// THIS SOURCE FILE:
// Improvements made after the final article in the "How to Build a 
// BVH" series. In this code: 
// Faster TLAS construction, enabling a moving flock of dragons;
// Faster BVH construction, enabling exploding dragons.
// Feel free to copy this code to your own framework. Absolutely no
// rights are reserved. No responsibility is accepted either.
// For updates, follow me on twitter: @j_bikker.

TheApp* CreateApp() { return new BeyondApp(); }

struct Flock
{
	const static int GRIDDIM = 32;		// dimension of the 3D acceleration grid
	const static int BINSIZE = 128;	// capacity of each grid cell
	const static int GROUPS = 64;		// job count for threaded flock update
	struct Boid // adapted from https://processing.org/examples/flocking.html
	{
	public:
		Boid() = default;
		Boid( float3 pos ) : position( pos )
		{
			velocity = float3( RandomFloat() - 0.5f, RandomFloat() - 0.5f, RandomFloat() - 0.5f );
			maxspeed = 1, maxforce = 0.03f;
		}
		void Tick( int group )
		{
			Flock::FindNearbyBoids( group, position, 80 );
			velocity += separate( group ) * 2.5f;
			velocity += align( group ) * 1.0f;
			velocity += cohesion( group ) * 0.3f;
			velocity += aim( group ) * 0.05f;
			if (sqrLength( velocity ) > maxspeed * maxspeed) velocity = normalize( velocity ) * maxspeed;
			newpos = position + velocity;
			for (int a = 0; a < 3; a++)
			{
				if (newpos[a] < -249.9f) newpos[a] = 249.9f;
				if (newpos[a] > 249.9f) newpos[a] = -249.9f;
			}
		}
		float3 separate( int group )
		{
			float3 steer( 0 );
			int count = Flock::nearBoids[group], actual = 0;
			for (int i = 0; i < count; i++)
			{
				Boid& other = Flock::boid[nearBoid[group][i]];
				float d2 = sqrLength( position - other.position );
				if (d2 == 0 || d2 > ( 35 * 35 )) continue;
				steer += normalize( position - other.position ) * (1.0f / sqrtf( d2 )), actual++;
			}
			if (sqrLength( steer ) > 0)
			{
				steer = normalize( steer * (1.0f / actual) ) * maxspeed - velocity;
				if (sqrLength( steer ) > maxforce * maxforce) steer = normalize( steer ) * maxforce;
			}
			return steer;
		}
		float3 align( int group )
		{
			float3 sum( 0 );
			int count = Flock::nearBoids[group];
			if (count == 0) return float3( 0 );
			for (int i = 0; i < count; i++)
			{
				Boid& other = Flock::boid[nearBoid[group][i]];
				sum += other.velocity;
			}
			sum = normalize( sum * (1.0f / count) ) * maxspeed;
			float3 steer = sum - velocity;
			if (sqrLength( steer ) > maxforce * maxforce) steer = normalize( steer ) * maxforce;
			return steer;
		}
		float3 cohesion( int group )
		{
			float3 sum( 0 );
			int count = Flock::nearBoids[group];
			if (count == 0) return float3( 0 );
			for (int i = 0; i < count; i++) sum += Flock::boid[nearBoid[group][i]].position;
			float3 steer = normalize( sum * (1.0f / count) - position ) * maxspeed - velocity;
			if (sqrLength( steer ) > maxforce * maxforce) steer = normalize( steer ) * maxforce;
			return steer;
		}
		float3 aim( int group )
		{
			float3 steer = normalize( Flock::food - position ) * maxspeed - velocity;
			if (sqrLength( steer ) > maxforce * maxforce) steer = normalize( steer ) * maxforce;
			return steer;
		}
		// data members
		float3 position, newpos, velocity;
		float maxforce; // maximum steering force
		float maxspeed; // maximum speed
	};
	Flock()
	{
		for (int i = 0; i < GROUPS; i++) nearBoid[i] = new int[BINSIZE * 5 * 5 * 5];
		boid = new Boid[21000], boids = 0;
		grid = new int[GRIDDIM * GRIDDIM * GRIDDIM * BINSIZE];
	}
	void Tick()
	{
		// rebuild the grid
		memset( grid, 0, GRIDDIM * GRIDDIM * GRIDDIM * BINSIZE * 4 ); // bin holds counter + BINSIZE indices
		for (int i = 0; i < boids; i++)
		{
			int ix = (int)((boid[i].position.x + 256) * 0.0625f); // yields 0..31
			int iy = (int)((boid[i].position.y + 256) * 0.0625f); // yields 0..31
			int iz = (int)((boid[i].position.z + 256) * 0.0625f); // yields 0..31
			int addr = (ix + iy * GRIDDIM + iz * GRIDDIM * GRIDDIM) * BINSIZE;
			int& count = grid[addr];
			if (count < (BINSIZE - 1)) grid[++count] = i; // otherwise, skip the boid, np
		}
		// update the boids in groups using OpenMP
	#pragma omp parallel for schedule(dynamic)
		for (int g = 0; g < GROUPS; g++)
		{
			int first = (boids * g) / GROUPS;
			int last = (boids * (g + 1)) / GROUPS - 1;
			if (g == (GROUPS - 1)) last = boids - 1;
			for (int i = first; i <= last; i++) boid[i].Tick( g );
		}
		// record the updated positions
		for (int i = 0; i < boids; i++) boid[i].position = boid[i].newpos;
	}
	static void FindNearbyBoids( int group, float3 position, float radius )
	{
		int count = 0;
		int ix = (int)((position.x + 256) * 0.0625f);
		int iy = (int)((position.y + 256) * 0.0625f);
		int iz = (int)((position.z + 256) * 0.0625f);
		int gx1 = max( 0, ix - 2 ), gx2 = min( GRIDDIM - 1, ix + 2 );
		int gy1 = max( 0, iy - 2 ), gy2 = min( GRIDDIM - 1, iy + 2 );
		int gz1 = max( 0, iz - 2 ), gz2 = min( GRIDDIM - 1, iz + 2 );
		float r2 = radius * radius;
		for (int z = gz1; z <= gz2; z++) for (int y = gy1; y <= gy2; y++) for (int x = gx1; x <= gx2; x++)
		{
			int addr = (x + y * GRIDDIM + z * GRIDDIM * GRIDDIM) * BINSIZE;
			for (int c = 0; c < grid[addr]; c++)
			{
				int j = grid[addr + c + 1];
				float dist2 = sqrLength( position - boid[j].position );
				if (dist2 < r2 && dist2 > 0) nearBoid[group][count++] = j;
			}
		}
		nearBoids[group] = count;
	}
	// data members
	static inline int* nearBoid[GROUPS];
	static inline int nearBoids[GROUPS], boids = 0;
	static inline Boid* boid = 0;
	static inline int* grid = 0;
	static inline float3 food = float3( 0 );
};
Flock flock;

// BeyondApp implementation

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
	// initial flock configuration: dragons in the shape of a dragon
	boidCount = mesh->triCount;
	bvhInstance = new BVHInstance[boidCount];
	flock.boids = boidCount;
	for (int i = 0; i < boidCount; i++)
		Flock::boid[i] = Flock::Boid( float3( RandomFloat() * 480 - 240, RandomFloat() * 480 - 240, RandomFloat() * 480 - 240 ) ),
		bvhInstance[i] = BVHInstance( mesh->bvh, i );
	tlas = TLAS( bvhInstance, boidCount );
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
	instData = new Buffer( boidCount * sizeof( BVHInstance ), bvhInstance );
	tlasData = new Buffer( (boidCount * 2 + 64) * sizeof( TLASNode ), tlas.tlasNode );
	bvhData = new Buffer( mesh->bvh->nodesUsed * sizeof( BVHNode ), mesh->bvh->bvhNode );
	idxData = new Buffer( mesh->triCount * sizeof( uint ), mesh->bvh->triIdx );
	triData->CopyToDevice();
	triExData->CopyToDevice();
	texData->CopyToDevice();
	bvhData->CopyToDevice();
	idxData->CopyToDevice();
	// fetch camera
	FILE* f = fopen( "camera.bin", "rb" );
	if (!f) return;
	fread( &camPos, 1, sizeof( camPos ), f );
	fread( &camTarget, 1, sizeof( camTarget ), f );
	fclose( f );
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
	// move the boids
	Timer t;
	flock.Tick();
	static int foodCounter = 300;
	if (--foodCounter < 0)
	{
		foodCounter = (RandomUInt() & 255) + 64;
		Flock::food = float3( RandomFloat() * 300 - 150, RandomFloat() * 300 - 150, RandomFloat() * 300 - 150 );
	}
	printf( "flock update: %.2fms, ", t.elapsed() * 1000 );
	t.reset();
	for (int i = 0; i < boidCount; i++)
	{
		float3 boidPos = Flock::boid[i].position * 0.1f;
		float3 boidDir = normalize( Flock::boid[i].velocity );
		mat4 orientation = mat4::LookAt( boidPos, boidPos + boidDir, float3( 0, 1, 0 ) );
		bvhInstance[i].SetTransform( mat4::Translate( boidPos ) * orientation * mat4::Scale( 0.0025f ) );
	}
	// rebuild the TLAS
	tlas.BuildQuick();
	printf( "TLAS update: %.2fms\n", t.elapsed() * 1000 );
	instData->CopyToDevice();
	tlasData->CopyToDevice();
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