#pragma once

namespace Tmpl8
{

// application class
class BeyondApp : public TheApp
{
public:
	// game flow methods
	void Init();
	void HandleKeys( float dt );
	void Tick( float deltaTime );
	void Shutdown();
	// input handling
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { mousePos.x = x, mousePos.y = y; }
	void MouseWheel( float y ) { /* implement if you want to handle the mouse wheel */ }
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown( int key ) { /* implement if you want to handle keys */ }
	// data members
	int2 mousePos;
	Mesh* mesh;
	BVHInstance* bvhInstance;
	TLAS tlas;
	float3 p0, p1, p2;	// virtual screen plane corners
	float* skyPixels;
	int skyWidth, skyHeight, skyBpp;
	Kernel* tracer;		// the ray tracing kernel
	Buffer* target;		// buffer encapsulating texture that holds the rendered image
	Buffer* skyData;	// buffer for the skydome texture
	Buffer* triData;	// buffer for the mesh Tri data (vertices for intersection)
	Buffer* triExData;	// buffer for the mesh TriEx data (vertices for shading)
	Buffer* texData;	// buffer for the brick texture
	Buffer* tlasData;	// buffer to store the TLAS
	Buffer* instData;	// buffer for BVHInstance data
	Buffer* bvhData;	// buffer for BVH node data
	Buffer* idxData;	// buffer for triangle index data for BVH
	// boids data
	float3* boidPos = 0;
	float3* boidDir = 0;
	int boidCount = 0;
};

} // namespace Tmpl8