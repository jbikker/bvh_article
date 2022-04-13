// Template, IGAD version 2
// IGAD/NHTV/UU - Jacco Bikker - 2006-2021

// add your includes to this file instead of to individual .cpp files
// to enjoy the benefits of precompiled headers:
// - fast compilation
// - solve issues with the order of header files once (here)
// do not include headers in header files (ever).

// C++ headers
#include <chrono>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <thread>
#include <math.h>
#include <algorithm>
#include <assert.h>
#include <io.h>

#include "lib/stb_image.h"

// header for AVX, and every technology before it.
// if your CPU does not support this (unlikely), include the appropriate header instead.
// see: https://stackoverflow.com/a/11228864/2844473
#include <immintrin.h>

// clang-format off

// "leak" common namespaces to all compilation units. This is not standard
// C++ practice but a simplification for template projects.
using namespace std;

// windows
#define NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#endif
#include <windows.h>

// OpenCL headers
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS // safe; see https://stackoverflow.com/a/28500846
#include "cl/cl.h"
#include <cl/cl_gl_ext.h>

// GLFW
#define GLFW_USE_CHDIR 0
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// zlib
#include "zlib.h"

// basic types
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;
#ifdef _MSC_VER
typedef unsigned char BYTE;		// for freeimage.h
typedef unsigned short WORD;	// for freeimage.h
typedef unsigned long DWORD;	// for freeimage.h
typedef int BOOL;				// for freeimage.h
#endif

namespace Tmpl8
{

// 32-bit surface container
class Surface
{
	enum { OWNER = 1 };
public:
	// constructor / destructor
	Surface() = default;
	Surface( int w, int h, uint* buffer );
	Surface( int w, int h );
	Surface( const char* file );
	~Surface();
	// operations
	void InitCharset();
	void SetChar( int c, const char* c1, const char* c2, const char* c3, const char* c4, const char* c5 );
	void Print( const char* t, int x1, int y1, uint c );
	void Clear( uint c );
	void Line( float x1, float y1, float x2, float y2, uint c );
	void Plot( int x, int y, uint c );
	void LoadImage( const char* file );
	void CopyTo( Surface* dst, int x, int y );
	void Box( int x1, int y1, int x2, int y2, uint color );
	void Bar( int x1, int y1, int x2, int y2, uint color );
	// attributes
	uint* pixels = 0;
	int width = 0, height = 0;
	bool ownBuffer = false;
};

};

// namespaces
using namespace Tmpl8;

// aligned memory allocations
#ifdef _MSC_VER
#define ALIGN( x ) __declspec( align( x ) )
#define MALLOC64( x ) ( ( x ) == 0 ? 0 : _aligned_malloc( ( x ), 64 ) )
#define FREE64( x ) _aligned_free( x )
#else
#define ALIGN( x ) __attribute__( ( aligned( x ) ) )
#define MALLOC64( x ) ( ( x ) == 0 ? 0 : aligned_alloc( 64, ( x ) ) )
#define FREE64( x ) free( x )
#endif
#if defined(__GNUC__) && (__GNUC__ >= 4)
#define CHECK_RESULT __attribute__ ((warn_unused_result))
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
#define CHECK_RESULT _Check_return_
#else
#define CHECK_RESULT
#endif

// vector type placeholders, carefully matching OpenCL's layout and alignment
struct ALIGN( 8 ) int2 
{ 
	int2() = default; 
	int2( const int a, const int b ) : x( a ), y( b ) {} 
	int2( const int a ) : x( a ), y( a ) {}
	union { struct { int x, y; }; int cell[2]; };
	int operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 8 ) uint2 
{ 
	uint2() = default; 
	uint2( const int a, const int b ) : x( a ), y( b ) {} 
	uint2( const uint a ) : x( a ), y( a ) {}
	union { struct { uint x, y; }; uint cell[2]; };
	uint operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 8 ) float2 
{ 
	float2() = default; 
	float2( const float a, const float b ) : x( a ), y( b ) {} 
	float2( const float a ) : x(a ), y(a ) {}
	union { struct { float x, y; }; float cell[2]; };
	float operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 16 ) int3 
{ 
	int3() = default; 
	int3( const int a, const int b, const int c ) : x( a ), y( b ), z( c ) {} 
	int3( const int a ) : x( a ), y( a ), z( a ) {}
	union { struct { int x, y, z; int dummy; }; int cell[4]; };
	int operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 16 ) uint3 
{ 
	uint3() = default; 
	uint3( const uint a, const uint b, const uint c ) : x( a ), y( b ), z( c ) {} 
	uint3( const uint a ) : x( a ), y( a ), z( a ) {}
	union { struct { uint x, y, z; uint dummy; }; uint cell[4]; };
	uint operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 16 ) float3
{
	float3() = default;
	float3( const float a, const float b, const float c ) : x( a ), y( b ), z( c ) {}
	float3( const float a ) : x( a ), y( a ), z( a ) {}
	union { struct { float x, y, z; float dummy; }; float cell[4]; };
	float operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 16 ) int4
{
	int4() = default;
	int4( const int a, const int b, const int c, const int d ) : x( a ), y( b ), z( c ), w( d ) {}
	int4( const int a ) : x( a ), y( a ), z( a ), w( a ) {}
	union { struct { int x, y, z, w; }; int cell[4]; };
	int operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 16 ) uint4 
{ 
	uint4() = default;
	uint4( const uint a, const uint b, const uint c, const uint d ) : x( a ), y( b ), z( c ), w( d ) {}
	uint4( const uint a ) : x( a ), y( a ), z( a ), w( a ) {}
	union { struct { uint x, y, z, w; }; uint cell[4]; };
	uint operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 16 ) float4 
{ 
	float4() = default;
	float4( const float a, const float b, const float c, const float d ) : x( a ), y( b ), z( c ), w( d) {}
	float4( const float a ) : x( a ), y( a ), z( a ), w( a ) {}
	union { struct { float x, y, z, w; }; float cell[4]; };
	float operator [] ( const int n ) const { return cell[n]; }
};
struct ALIGN( 4 ) uchar4 
{ 
	uchar4() = default;
	uchar4( const uchar a, const uchar b, const uchar c, const uchar d ) : x( a ), y( b ), z( c ), w( d ) {}
	uchar4( const uchar a ) : x( a ), y( a ), z( a ), w( a ) {}
	union { struct { uchar x, y, z, w; }; uchar cell[4]; };
	uchar operator [] ( const int n ) const { return cell[n]; }
};

// fatal error reporting (with a pretty window)
#define FATALERROR( fmt, ... ) FatalError( "Error on line %d of %s: " fmt "\n", __LINE__, __FILE__, ##__VA_ARGS__ )
#define FATALERROR_IF( condition, fmt, ... ) do { if ( ( condition ) ) FATALERROR( fmt, ##__VA_ARGS__ ); } while ( 0 )
#define FATALERROR_IN( prefix, errstr, fmt, ... ) FatalError( prefix " returned error '%s' at %s:%d" fmt "\n", errstr, __FILE__, __LINE__, ##__VA_ARGS__ );
#define FATALERROR_IN_CALL( stmt, error_parser, fmt, ... ) do { auto ret = ( stmt ); if ( ret ) FATALERROR_IN( #stmt, error_parser( ret ), fmt, ##__VA_ARGS__ ) } while ( 0 )

// OpenGL texture wrapper
class GLTexture
{
public:
	enum { DEFAULT = 0, FLOAT = 1, INTTARGET = 2 };
	// constructor / destructor
	GLTexture( uint width, uint height, uint type = DEFAULT );
	~GLTexture();
	// methods
	void Bind( const uint slot = 0 );
	void CopyFrom( Tmpl8::Surface* src );
	void CopyTo( Tmpl8::Surface* dst );
public:
	// public data members
	GLuint ID = 0;
	uint width = 0, height = 0;
};

// shader wrapper
class mat4;
class Shader
{
public:
	// constructor / destructor
	Shader( const char* vfile, const char* pfile, bool fromString );
	~Shader();
	// methods
	void Init( const char* vfile, const char* pfile );
	void Compile( const char* vtext, const char* ftext );
	void Bind();
	void SetInputTexture( uint slot, const char* name, GLTexture* texture );
	void SetInputMatrix( const char* name, const mat4& matrix );
	void SetFloat( const char* name, const float v );
	void SetInt( const char* name, const int v );
	void SetUInt( const char* name, const uint v );
	void Unbind();
private:
	// data members
	uint vertex = 0;	// vertex shader identifier
	uint pixel = 0;		// fragment shader identifier
	uint ID = 0;		// shader program identifier
};

// generic error checking for OpenGL code
#define CheckGL() { _CheckGL( __FILE__, __LINE__ ); }

// forward declarations of helper functions
void _CheckGL( const char* f, int l );
GLuint CreateVBO( const GLfloat* data, const uint size );
void BindVBO( const uint idx, const uint N, const GLuint id );
void CheckShader( GLuint shader, const char* vshader, const char* fshader );
void CheckProgram( GLuint id, const char* vshader, const char* fshader );
void DrawQuad();

// timer
struct Timer
{
	Timer() { reset(); }
	float elapsed() const
	{
		chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
		chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - start);
		return (float)time_span.count();
	}
	void reset() { start = chrono::high_resolution_clock::now(); }
	chrono::high_resolution_clock::time_point start;
};

// swap
template <class T> void Swap( T& x, T& y ) { T t; t = x, x = y, y = t; }

// Nils's jobmanager
class Job
{
public:
	virtual void Main() = 0;
protected:
	friend class JobThread;
	void RunCodeWrapper();
};
class JobThread
{
public:
	void CreateAndStartThread( unsigned int threadId );
	void Go();
	void BackgroundTask();
	HANDLE m_GoSignal, m_ThreadHandle;
	int m_ThreadID;
};
class JobManager	// singleton class!
{
protected:
	JobManager( unsigned int numThreads );
public:
	~JobManager();
	static void CreateJobManager( unsigned int numThreads );
	static JobManager* GetJobManager();
	static void GetProcessorCount( uint& cores, uint& logical );
	void AddJob2( Job* a_Job );
	unsigned int GetNumThreads() { return m_NumThreads; }
	void RunJobs();
	void ThreadDone( unsigned int n );
	int MaxConcurrent() { return m_NumThreads; }
protected:
	friend class JobThread;
	Job* GetNextJob();
	static JobManager* m_JobManager;
	Job* m_JobList[256];
	CRITICAL_SECTION m_CS;
	HANDLE m_ThreadDone[64];
	unsigned int m_NumThreads, m_JobCount;
	JobThread* m_JobThreadList;
};

// pixel operations
inline uint ScaleColor( const uint c, const uint scale )
{
	const uint rb = (((c & 0xff00ff) * scale) >> 8) & 0x00ff00ff;
	const uint ag = (((c & 0xff00ff00) >> 8) * scale) & 0xff00ff00;
	return rb + ag;
}
inline uint AddBlend( const uint c1, const uint c2 )
{
	const uint r1 = (c1 >> 16) & 255, r2 = (c2 >> 16) & 255;
	const uint g1 = (c1 >> 8) & 255, g2 = (c2 >> 8) & 255;
	const uint b1 = c1 & 255, b2 = c2 & 255;
	const uint r = min( 255u, r1 + r2 );
	const uint g = min( 255u, g1 + g2 );
	const uint b = min( 255u, b1 + b2 );
	return (r << 16) + (g << 8) + b;
}

// random numbers
uint RandomUInt();
uint RandomUInt( uint& seed );
float RandomFloat();
float RandomFloat( uint& seed );
float Rand( float range );

// Perlin noise
float noise2D( const float x, const float y );

// forward declaration of helper functions
void FatalError( const char* fmt, ... );
bool FileIsNewer( const char* file1, const char* file2 );
bool FileExists( const char* f );
bool RemoveFile( const char* f );
string TextFileRead( const char* _File );
int LineCount( const string s );
void TextFileWrite( const string& text, const char* _File );

// math
inline float fminf( float a, float b ) { return a < b ? a : b; }
inline float fmaxf( float a, float b ) { return a > b ? a : b; }
inline float rsqrtf( float x ) { return 1.0f / sqrtf( x ); }
inline float sqrf( float x ) { return x * x; }
inline int sqr( int x ) { return x * x; }

inline float2 make_float2( const float a, float b ) { float2 f2; f2.x = a, f2.y = b; return f2; }
inline float2 make_float2( const float s ) { return make_float2( s, s ); }
inline float2 make_float2( const float3& a ) { return make_float2( a.x, a.y ); }
inline float2 make_float2( const int2& a ) { return make_float2( float( a.x ), float( a.y ) ); } // explicit casts prevent gcc warnings
inline float2 make_float2( const uint2& a ) { return make_float2( float( a.x ), float( a.y ) ); }
inline int2 make_int2( const int a, const int b ) { int2 i2; i2.x = a, i2.y = b; return i2; }
inline int2 make_int2( const int s ) { return make_int2( s, s ); }
inline int2 make_int2( const int3& a ) { return make_int2( a.x, a.y ); }
inline int2 make_int2( const uint2& a ) { return make_int2( int( a.x ), int( a.y ) ); }
inline int2 make_int2( const float2& a ) { return make_int2( int( a.x ), int( a.y ) ); }
inline uint2 make_uint2( const uint a, const uint b ) { uint2 u2; u2.x = a, u2.y = b; return u2; }
inline uint2 make_uint2( const uint s ) { return make_uint2( s, s ); }
inline uint2 make_uint2( const uint3& a ) { return make_uint2( a.x, a.y ); }
inline uint2 make_uint2( const int2& a ) { return make_uint2( uint( a.x ), uint( a.y ) ); }
inline float3 make_float3( const float& a, const float& b, const float& c ) { float3 f3; f3.x = a, f3.y = b, f3.z = c; return f3; }
inline float3 make_float3( const float& s ) { return make_float3( s, s, s ); }
inline float3 make_float3( const float2& a ) { return make_float3( a.x, a.y, 0.0f ); }
inline float3 make_float3( const float2& a, const float& s ) { return make_float3( a.x, a.y, s ); }
inline float3 make_float3( const float4& a ) { return make_float3( a.x, a.y, a.z ); }
inline float3 make_float3( const int3& a ) { return make_float3( float( a.x ), float( a.y ), float( a.z ) ); }
inline float3 make_float3( const uint3& a ) { return make_float3( float( a.x ), float( a.y ), float( a.z ) ); }
inline int3 make_int3( const int& a, const int& b, const int& c ) { int3 i3; i3.x = a, i3.y = b, i3.z = c; return i3; }
inline int3 make_int3( const int& s ) { return make_int3( s, s, s ); }
inline int3 make_int3( const int2& a ) { return make_int3( a.x, a.y, 0 ); }
inline int3 make_int3( const int2& a, const int& s ) { return make_int3( a.x, a.y, s ); }
inline int3 make_int3( const uint3& a ) { return make_int3( int( a.x ), int( a.y ), int( a.z ) ); }
inline int3 make_int3( const float3& a ) { return make_int3( int( a.x ), int( a.y ), int( a.z ) ); }
inline int3 make_int3( const float4& a ) { return make_int3( int( a.x ), int( a.y ), int( a.z ) ); }
inline uint3 make_uint3( const uint a, uint b, uint c ) { uint3 u3; u3.x = a, u3.y = b, u3.z = c; return u3; }
inline uint3 make_uint3( const uint s ) { return make_uint3( s, s, s ); }
inline uint3 make_uint3( const uint2& a ) { return make_uint3( a.x, a.y, 0 ); }
inline uint3 make_uint3( const uint2& a, const uint s ) { return make_uint3( a.x, a.y, s ); }
inline uint3 make_uint3( const uint4& a ) { return make_uint3( a.x, a.y, a.z ); }
inline uint3 make_uint3( const int3& a ) { return make_uint3( uint( a.x ), uint( a.y ), uint( a.z ) ); }
inline float4 make_float4( const float a, const float b, const float c, const float d ) { float4 f4; f4.x = a, f4.y = b, f4.z = c, f4.w = d; return f4; }
inline float4 make_float4( const float s ) { return make_float4( s, s, s, s ); }
inline float4 make_float4( const float3& a ) { return make_float4( a.x, a.y, a.z, 0.0f ); }
inline float4 make_float4( const float3& a, const float w ) { return make_float4( a.x, a.y, a.z, w ); }
inline float4 make_float4( const int3& a, const float w ) { return make_float4( (float)a.x, (float)a.y, (float)a.z, w ); }
inline float4 make_float4( const int4& a ) { return make_float4( float( a.x ), float( a.y ), float( a.z ), float( a.w ) ); }
inline float4 make_float4( const uint4& a ) { return make_float4( float( a.x ), float( a.y ), float( a.z ), float( a.w ) ); }
inline int4 make_int4( const int a, const int b, const int c, const int d ) { int4 i4; i4.x = a, i4.y = b, i4.z = c, i4.w = d; return i4; }
inline int4 make_int4( const int s ) { return make_int4( s, s, s, s ); }
inline int4 make_int4( const int3& a ) { return make_int4( a.x, a.y, a.z, 0 ); }
inline int4 make_int4( const int3& a, const int w ) { return make_int4( a.x, a.y, a.z, w ); }
inline int4 make_int4( const uint4& a ) { return make_int4( int( a.x ), int( a.y ), int( a.z ), int( a.w ) ); }
inline int4 make_int4( const float4& a ) { return make_int4( int( a.x ), int( a.y ), int( a.z ), int( a.w ) ); }
inline uint4 make_uint4( const uint a, const uint b, const uint c, const uint d ) { uint4 u4; u4.x = a, u4.y = b, u4.z = c, u4.w = d; return u4; }
inline uint4 make_uint4( const uint s ) { return make_uint4( s, s, s, s ); }
inline uint4 make_uint4( const uint3& a ) { return make_uint4( a.x, a.y, a.z, 0 ); }
inline uint4 make_uint4( const uint3& a, const uint w ) { return make_uint4( a.x, a.y, a.z, w ); }
inline uint4 make_uint4( const int4& a ) { return make_uint4( uint( a.x ), uint( a.y ), uint( a.z ), uint( a.w ) ); }
inline uchar4 make_uchar4( const uchar a, const uchar b, const uchar c, const uchar d ) { uchar4 c4; c4.x = a, c4.y = b, c4.z = c, c4.w = d; return c4; }

inline float2 operator-( const float2& a ) { return make_float2( -a.x, -a.y ); }
inline int2 operator-( const int2& a ) { return make_int2( -a.x, -a.y ); }
inline float3 operator-( const float3& a ) { return make_float3( -a.x, -a.y, -a.z ); }
inline int3 operator-( const int3& a ) { return make_int3( -a.x, -a.y, -a.z ); }
inline float4 operator-( const float4& a ) { return make_float4( -a.x, -a.y, -a.z, -a.w ); }
inline int4 operator-( const int4& a ) { return make_int4( -a.x, -a.y, -a.z, -a.w ); }
inline int2 operator << ( const int2& a, int b ) { return make_int2( a.x << b, a.y << b ); }
inline int2 operator >> ( const int2& a, int b ) { return make_int2( a.x >> b, a.y >> b ); }
inline int3 operator << ( const int3& a, int b ) { return make_int3( a.x << b, a.y << b, a.z << b ); }
inline int3 operator >> ( const int3& a, int b ) { return make_int3( a.x >> b, a.y >> b, a.z >> b ); }
inline int4 operator << ( const int4& a, int b ) { return make_int4( a.x << b, a.y << b, a.z << b, a.w << b ); }
inline int4 operator >> ( const int4& a, int b ) { return make_int4( a.x >> b, a.y >> b, a.z >> b, a.w >> b ); }

inline float2 operator+( const float2& a, const float2& b ) { return make_float2( a.x + b.x, a.y + b.y ); }
inline float2 operator+( const float2& a, const int2& b ) { return make_float2( a.x + (float)b.x, a.y + (float)b.y ); }
inline float2 operator+( const float2& a, const uint2& b ) { return make_float2( a.x + (float)b.x, a.y + (float)b.y ); }
inline float2 operator+( const int2& a, const float2& b ) { return make_float2( (float)a.x + b.x, (float)a.y + b.y ); }
inline float2 operator+( const uint2& a, const float2& b ) { return make_float2( (float)a.x + b.x, (float)a.y + b.y ); }
inline void operator+=( float2& a, const float2& b ) { a.x += b.x;	a.y += b.y; }
inline void operator+=( float2& a, const int2& b ) { a.x += (float)b.x; a.y += (float)b.y; }
inline void operator+=( float2& a, const uint2& b ) { a.x += (float)b.x; a.y += (float)b.y; }
inline float2 operator+( const float2& a, float b ) { return make_float2( a.x + b, a.y + b ); }
inline float2 operator+( const float2& a, int b ) { return make_float2( a.x + (float)b, a.y + (float)b ); }
inline float2 operator+( const float2& a, uint b ) { return make_float2( a.x + (float)b, a.y + (float)b ); }
inline float2 operator+( float b, const float2& a ) { return make_float2( a.x + b, a.y + b ); }
inline void operator+=( float2& a, float b ) { a.x += b; a.y += b; }
inline void operator+=( float2& a, int b ) { a.x += (float)b; a.y += (float)b; }
inline void operator+=( float2& a, uint b ) { a.x += (float)b;	a.y += (float)b; }
inline int2 operator+( const int2& a, const int2& b ) { return make_int2( a.x + b.x, a.y + b.y ); }
inline void operator+=( int2& a, const int2& b ) { a.x += b.x;	a.y += b.y; }
inline int2 operator+( const int2& a, int b ) { return make_int2( a.x + b, a.y + b ); }
inline int2 operator+( int b, const int2& a ) { return make_int2( a.x + b, a.y + b ); }
inline void operator+=( int2& a, int b ) { a.x += b;	a.y += b; }
inline uint2 operator+( const uint2& a, const uint2& b ) { return make_uint2( a.x + b.x, a.y + b.y ); }
inline void operator+=( uint2& a, const uint2& b ) { a.x += b.x;	a.y += b.y; }
inline uint2 operator+( const uint2& a, uint b ) { return make_uint2( a.x + b, a.y + b ); }
inline uint2 operator+( uint b, const uint2& a ) { return make_uint2( a.x + b, a.y + b ); }
inline void operator+=( uint2& a, uint b ) { a.x += b;	a.y += b; }
inline float3 operator+( const float3& a, const float3& b ) { return make_float3( a.x + b.x, a.y + b.y, a.z + b.z ); }
inline float3 operator+( const float3& a, const int3& b ) { return make_float3( a.x + (float)b.x, a.y + (float)b.y, a.z + (float)b.z ); }
inline float3 operator+( const float3& a, const uint3& b ) { return make_float3( a.x + (float)b.x, a.y + (float)b.y, a.z + (float)b.z ); }
inline float3 operator+( const int3& a, const float3& b ) { return make_float3( (float)a.x + b.x, (float)a.y + b.y, (float)a.z + b.z ); }
inline float3 operator+( const uint3& a, const float3& b ) { return make_float3( (float)a.x + b.x, (float)a.y + b.y, (float)a.z + b.z ); }
inline void operator+=( float3& a, const float3& b ) { a.x += b.x;	a.y += b.y;	a.z += b.z; }
inline void operator+=( float3& a, const int3& b ) { a.x += (float)b.x; a.y += (float)b.y; a.z += (float)b.z; }
inline void operator+=( float3& a, const uint3& b ) { a.x += (float)b.x; a.y += (float)b.y; a.z += (float)b.z; }
inline float3 operator+( const float3& a, float b ) { return make_float3( a.x + b, a.y + b, a.z + b ); }
inline float3 operator+( const float3& a, int b ) { return make_float3( a.x + (float)b, a.y + (float)b, a.z + (float)b ); }
inline float3 operator+( const float3& a, uint b ) { return make_float3( a.x + (float)b, a.y + (float)b, a.z + (float)b ); }
inline void operator+=( float3& a, float b ) { a.x += b; a.y += b;	a.z += b; }
inline void operator+=( float3& a, int b ) { a.x += (float)b; a.y += (float)b; a.z += (float)b; }
inline void operator+=( float3& a, uint b ) { a.x += (float)b; a.y += (float)b; a.z += (float)b; }
inline int3 operator+( const int3& a, const int3& b ) { return make_int3( a.x + b.x, a.y + b.y, a.z + b.z ); }
inline void operator+=( int3& a, const int3& b ) { a.x += b.x;	a.y += b.y;	a.z += b.z; }
inline int3 operator+( const int3& a, int b ) { return make_int3( a.x + b, a.y + b, a.z + b ); }
inline void operator+=( int3& a, int b ) { a.x += b;	a.y += b;	a.z += b; }
inline uint3 operator+( const uint3& a, const uint3& b ) { return make_uint3( a.x + b.x, a.y + b.y, a.z + b.z ); }
inline void operator+=( uint3& a, const uint3& b ) { a.x += b.x;	a.y += b.y;	a.z += b.z; }
inline uint3 operator+( const uint3& a, uint b ) { return make_uint3( a.x + b, a.y + b, a.z + b ); }
inline void operator+=( uint3& a, uint b ) { a.x += b;	a.y += b;	a.z += b; }
inline int3 operator+( int b, const int3& a ) { return make_int3( a.x + b, a.y + b, a.z + b ); }
inline uint3 operator+( uint b, const uint3& a ) { return make_uint3( a.x + b, a.y + b, a.z + b ); }
inline float3 operator+( float b, const float3& a ) { return make_float3( a.x + b, a.y + b, a.z + b ); }
inline float4 operator+( const float4& a, const float4& b ) { return make_float4( a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w ); }
inline float4 operator+( const float4& a, const int4& b ) { return make_float4( a.x + (float)b.x, a.y + (float)b.y, a.z + (float)b.z, a.w + (float)b.w ); }
inline float4 operator+( const float4& a, const uint4& b ) { return make_float4( a.x + (float)b.x, a.y + (float)b.y, a.z + (float)b.z, a.w + (float)b.w ); }
inline float4 operator+( const int4& a, const float4& b ) { return make_float4( (float)a.x + b.x, (float)a.y + b.y, (float)a.z + b.z, (float)a.w + b.w ); }
inline float4 operator+( const uint4& a, const float4& b ) { return make_float4( (float)a.x + b.x, (float)a.y + b.y, (float)a.z + b.z, (float)a.w + b.w ); }
inline void operator+=( float4& a, const float4& b ) { a.x += b.x;	a.y += b.y;	a.z += b.z;	a.w += b.w; }
inline void operator+=( float4& a, const int4& b ) { a.x += (float)b.x; a.y += (float)b.y; a.z += (float)b.z; a.w += (float)b.w; }
inline void operator+=( float4& a, const uint4& b ) { a.x += (float)b.x; a.y += (float)b.y; a.z += (float)b.z; a.w += (float)b.w; }
inline float4 operator+( const float4& a, float b ) { return make_float4( a.x + b, a.y + b, a.z + b, a.w + b ); }
inline float4 operator+( const float4& a, int b ) { return make_float4( a.x + (float)b, a.y + (float)b, a.z + (float)b, a.w + (float)b ); }
inline float4 operator+( const float4& a, uint b ) { return make_float4( a.x + (float)b, a.y + (float)b, a.z + (float)b, a.w + (float)b ); }
inline float4 operator+( float b, const float4& a ) { return make_float4( a.x + b, a.y + b, a.z + b, a.w + b ); }
inline void operator+=( float4& a, float b ) { a.x += b;	a.y += b;	a.z += b;	a.w += b; }
inline void operator+=( float4& a, int b ) { a.x += (float)b; a.y += (float)b; a.z += (float)b; a.w += (float)b; }
inline void operator+=( float4& a, uint b ) { a.x += (float)b; a.y += (float)b; a.z += (float)b; a.w += (float)b; }
inline int4 operator+( const int4& a, const int4& b ) { return make_int4( a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w ); }
inline void operator+=( int4& a, const int4& b ) { a.x += b.x;	a.y += b.y;	a.z += b.z;	a.w += b.w; }
inline int4 operator+( const int4& a, int b ) { return make_int4( a.x + b, a.y + b, a.z + b, a.w + b ); }
inline int4 operator+( int b, const int4& a ) { return make_int4( a.x + b, a.y + b, a.z + b, a.w + b ); }
inline void operator+=( int4& a, int b ) { a.x += b;	a.y += b;	a.z += b;	a.w += b; }
inline uint4 operator+( const uint4& a, const uint4& b ) { return make_uint4( a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w ); }
inline void operator+=( uint4& a, const uint4& b ) { a.x += b.x;	a.y += b.y;	a.z += b.z;	a.w += b.w; }
inline uint4 operator+( const uint4& a, uint b ) { return make_uint4( a.x + b, a.y + b, a.z + b, a.w + b ); }
inline uint4 operator+( uint b, const uint4& a ) { return make_uint4( a.x + b, a.y + b, a.z + b, a.w + b ); }
inline void operator+=( uint4& a, uint b ) { a.x += b;	a.y += b;	a.z += b;	a.w += b; }

inline float2 operator-( const float2& a, const float2& b ) { return make_float2( a.x - b.x, a.y - b.y ); }
inline float2 operator-( const float2& a, const int2& b ) { return make_float2( a.x - (float)b.x, a.y - (float)b.y ); }
inline float2 operator-( const float2& a, const uint2& b ) { return make_float2( a.x - (float)b.x, a.y - (float)b.y ); }
inline float2 operator-( const int2& a, const float2& b ) { return make_float2( (float)a.x - b.x, (float)a.y - b.y ); }
inline float2 operator-( const uint2& a, const float2& b ) { return make_float2( (float)a.x - b.x, (float)a.y - b.y ); }
inline void operator-=( float2& a, const float2& b ) { a.x -= b.x;	a.y -= b.y; }
inline void operator-=( float2& a, const int2& b ) { a.x -= (float)b.x; a.y -= (float)b.y; }
inline void operator-=( float2& a, const uint2& b ) { a.x -= (float)b.x; a.y -= (float)b.y; }
inline float2 operator-( const float2& a, float b ) { return make_float2( a.x - b, a.y - b ); }
inline float2 operator-( const float2& a, int b ) { return make_float2( a.x - (float)b, a.y - (float)b ); }
inline float2 operator-( const float2& a, uint b ) { return make_float2( a.x - (float)b, a.y - (float)b ); }
inline float2 operator-( float b, const float2& a ) { return make_float2( b - a.x, b - a.y ); }
inline void operator-=( float2& a, float b ) { a.x -= b; a.y -= b; }
inline void operator-=( float2& a, int b ) { a.x -= (float)b; a.y -= (float)b; }
inline void operator-=( float2& a, uint b ) { a.x -= (float)b; a.y -= (float)b; }
inline int2 operator-( const int2& a, const int2& b ) { return make_int2( a.x - b.x, a.y - b.y ); }
inline void operator-=( int2& a, const int2& b ) { a.x -= b.x;	a.y -= b.y; }
inline int2 operator-( const int2& a, int b ) { return make_int2( a.x - b, a.y - b ); }
inline int2 operator-( int b, const int2& a ) { return make_int2( b - a.x, b - a.y ); }
inline void operator-=( int2& a, int b ) { a.x -= b;	a.y -= b; }
inline uint2 operator-( const uint2& a, const uint2& b ) { return make_uint2( a.x - b.x, a.y - b.y ); }
inline void operator-=( uint2& a, const uint2& b ) { a.x -= b.x;	a.y -= b.y; }
inline uint2 operator-( const uint2& a, uint b ) { return make_uint2( a.x - b, a.y - b ); }
inline uint2 operator-( uint b, const uint2& a ) { return make_uint2( b - a.x, b - a.y ); }
inline void operator-=( uint2& a, uint b ) { a.x -= b;	a.y -= b; }
inline float3 operator-( const float3& a, const float3& b ) { return make_float3( a.x - b.x, a.y - b.y, a.z - b.z ); }
inline float3 operator-( const float3& a, const int3& b ) { return make_float3( a.x - (float)b.x, a.y - (float)b.y, a.z - (float)b.z ); }
inline float3 operator-( const float3& a, const uint3& b ) { return make_float3( a.x - (float)b.x, a.y - (float)b.y, a.z - (float)b.z ); }
inline float3 operator-( const int3& a, const float3& b ) { return make_float3( (float)a.x - b.x, (float)a.y - b.y, (float)a.z - b.z ); }
inline float3 operator-( const uint3& a, const float3& b ) { return make_float3( (float)a.x - b.x, (float)a.y - b.y, (float)a.z - b.z ); }
inline void operator-=( float3& a, const float3& b ) { a.x -= b.x;	a.y -= b.y;	a.z -= b.z; }
inline void operator-=( float3& a, const int3& b ) { a.x -= (float)b.x; a.y -= (float)b.y; a.z -= (float)b.z; }
inline void operator-=( float3& a, const uint3& b ) { a.x -= (float)b.x; a.y -= (float)b.y; a.z -= (float)b.z; }
inline float3 operator-( const float3& a, float b ) { return make_float3( a.x - b, a.y - b, a.z - b ); }
inline float3 operator-( const float3& a, int b ) { return make_float3( a.x - (float)b, a.y - (float)b, a.z - (float)b ); }
inline float3 operator-( const float3& a, uint b ) { return make_float3( a.x - (float)b, a.y - (float)b, a.z - (float)b ); }
inline float3 operator-( float b, const float3& a ) { return make_float3( b - a.x, b - a.y, b - a.z ); }
inline void operator-=( float3& a, float b ) { a.x -= b; a.y -= b; a.z -= b; }
inline void operator-=( float3& a, int b ) { a.x -= (float)b; a.y -= (float)b; a.z -= (float)b; }
inline void operator-=( float3& a, uint b ) { a.x -= (float)b;	a.y -= (float)b; a.z -= (float)b; }
inline int3 operator-( const int3& a, const int3& b ) { return make_int3( a.x - b.x, a.y - b.y, a.z - b.z ); }
inline void operator-=( int3& a, const int3& b ) { a.x -= b.x;	a.y -= b.y;	a.z -= b.z; }
inline int3 operator-( const int3& a, int b ) { return make_int3( a.x - b, a.y - b, a.z - b ); }
inline int3 operator-( int b, const int3& a ) { return make_int3( b - a.x, b - a.y, b - a.z ); }
inline void operator-=( int3& a, int b ) { a.x -= b;	a.y -= b;	a.z -= b; }
inline uint3 operator-( const uint3& a, const uint3& b ) { return make_uint3( a.x - b.x, a.y - b.y, a.z - b.z ); }
inline void operator-=( uint3& a, const uint3& b ) { a.x -= b.x;	a.y -= b.y;	a.z -= b.z; }
inline uint3 operator-( const uint3& a, uint b ) { return make_uint3( a.x - b, a.y - b, a.z - b ); }
inline uint3 operator-( uint b, const uint3& a ) { return make_uint3( b - a.x, b - a.y, b - a.z ); }
inline void operator-=( uint3& a, uint b ) { a.x -= b;	a.y -= b;	a.z -= b; }
inline float4 operator-( const float4& a, const float4& b ) { return make_float4( a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w ); }
inline float4 operator-( const float4& a, const int4& b ) { return make_float4( a.x - (float)b.x, a.y - (float)b.y, a.z - (float)b.z, a.w - (float)b.w ); }
inline float4 operator-( const float4& a, const uint4& b ) { return make_float4( a.x - (float)b.x, a.y - (float)b.y, a.z - (float)b.z, a.w - (float)b.w ); }
inline float4 operator-( const int4& a, const float4& b ) { return make_float4( (float)a.x - b.x, (float)a.y - b.y, (float)a.z - b.z, (float)a.w - b.w ); }
inline float4 operator-( const uint4& a, const float4& b ) { return make_float4( (float)a.x - b.x, (float)a.y - b.y, (float)a.z - b.z, (float)a.w - b.w ); }
inline void operator-=( float4& a, const float4& b ) { a.x -= b.x;	a.y -= b.y;	a.z -= b.z;	a.w -= b.w; }
inline void operator-=( float4& a, const int4& b ) { a.x -= (float)b.x; a.y -= (float)b.y; a.z -= (float)b.z; a.w -= (float)b.w; }
inline void operator-=( float4& a, const uint4& b ) { a.x -= (float)b.x; a.y -= (float)b.y; a.z -= (float)b.z; a.w -= (float)b.w; }
inline float4 operator-( const float4& a, float b ) { return make_float4( a.x - b, a.y - b, a.z - b, a.w - b ); }
inline float4 operator-( const float4& a, int b ) { return make_float4( a.x - (float)b, a.y - (float)b, a.z - (float)b, a.w - (float)b ); }
inline float4 operator-( const float4& a, uint b ) { return make_float4( a.x - (float)b, a.y - (float)b, a.z - (float)b, a.w - (float)b ); }
inline void operator-=( float4& a, float b ) { a.x -= b; a.y -= b; a.z -= b; a.w -= b; }
inline void operator-=( float4& a, int b ) { a.x -= (float)b; a.y -= (float)b; a.z -= (float)b; a.w -= (float)b; }
inline void operator-=( float4& a, uint b ) { a.x -= (float)b; a.y -= (float)b; a.z -= (float)b; a.w -= (float)b; }
inline int4 operator-( const int4& a, const int4& b ) { return make_int4( a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w ); }
inline void operator-=( int4& a, const int4& b ) { a.x -= b.x;	a.y -= b.y;	a.z -= b.z;	a.w -= b.w; }
inline int4 operator-( const int4& a, int b ) { return make_int4( a.x - b, a.y - b, a.z - b, a.w - b ); }
inline int4 operator-( int b, const int4& a ) { return make_int4( b - a.x, b - a.y, b - a.z, b - a.w ); }
inline void operator-=( int4& a, int b ) { a.x -= b;	a.y -= b;	a.z -= b;	a.w -= b; }
inline uint4 operator-( const uint4& a, const uint4& b ) { return make_uint4( a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w ); }
inline void operator-=( uint4& a, const uint4& b ) { a.x -= b.x;	a.y -= b.y;	a.z -= b.z;	a.w -= b.w; }
inline uint4 operator-( const uint4& a, uint b ) { return make_uint4( a.x - b, a.y - b, a.z - b, a.w - b ); }
inline uint4 operator-( uint b, const uint4& a ) { return make_uint4( b - a.x, b - a.y, b - a.z, b - a.w ); }
inline void operator-=( uint4& a, uint b ) { a.x -= b;	a.y -= b;	a.z -= b;	a.w -= b; }

inline float2 operator*( const float2& a, const float2& b ) { return make_float2( a.x * b.x, a.y * b.y ); }
inline void operator*=( float2& a, const float2& b ) { a.x *= b.x;	a.y *= b.y; }
inline float2 operator*( const float2& a, float b ) { return make_float2( a.x * b, a.y * b ); }
inline float2 operator*( float b, const float2& a ) { return make_float2( b * a.x, b * a.y ); }
inline void operator*=( float2& a, float b ) { a.x *= b;	a.y *= b; }
inline int2 operator*( const int2& a, const int2& b ) { return make_int2( a.x * b.x, a.y * b.y ); }
inline void operator*=( int2& a, const int2& b ) { a.x *= b.x;	a.y *= b.y; }
inline int2 operator*( const int2& a, int b ) { return make_int2( a.x * b, a.y * b ); }
inline int2 operator*( int b, const int2& a ) { return make_int2( b * a.x, b * a.y ); }
inline void operator*=( int2& a, int b ) { a.x *= b;	a.y *= b; }
inline uint2 operator*( const uint2& a, const uint2& b ) { return make_uint2( a.x * b.x, a.y * b.y ); }
inline void operator*=( uint2& a, const uint2& b ) { a.x *= b.x;	a.y *= b.y; }
inline uint2 operator*( const uint2& a, uint b ) { return make_uint2( a.x * b, a.y * b ); }
inline uint2 operator*( uint b, const uint2& a ) { return make_uint2( b * a.x, b * a.y ); }
inline void operator*=( uint2& a, uint b ) { a.x *= b;	a.y *= b; }
inline float3 operator*( const float3& a, const float3& b ) { return make_float3( a.x * b.x, a.y * b.y, a.z * b.z ); }
inline void operator*=( float3& a, const float3& b ) { a.x *= b.x;	a.y *= b.y;	a.z *= b.z; }
inline float3 operator*( const float3& a, float b ) { return make_float3( a.x * b, a.y * b, a.z * b ); }
inline float3 operator*( float b, const float3& a ) { return make_float3( b * a.x, b * a.y, b * a.z ); }
inline void operator*=( float3& a, float b ) { a.x *= b;	a.y *= b;	a.z *= b; }
inline int3 operator*( const int3& a, const int3& b ) { return make_int3( a.x * b.x, a.y * b.y, a.z * b.z ); }
inline void operator*=( int3& a, const int3& b ) { a.x *= b.x;	a.y *= b.y;	a.z *= b.z; }
inline int3 operator*( const int3& a, int b ) { return make_int3( a.x * b, a.y * b, a.z * b ); }
inline int3 operator*( int b, const int3& a ) { return make_int3( b * a.x, b * a.y, b * a.z ); }
inline void operator*=( int3& a, int b ) { a.x *= b;	a.y *= b;	a.z *= b; }
inline uint3 operator*( const uint3& a, const uint3& b ) { return make_uint3( a.x * b.x, a.y * b.y, a.z * b.z ); }
inline void operator*=( uint3& a, const uint3& b ) { a.x *= b.x;	a.y *= b.y;	a.z *= b.z; }
inline uint3 operator*( const uint3& a, uint b ) { return make_uint3( a.x * b, a.y * b, a.z * b ); }
inline uint3 operator*( uint b, const uint3& a ) { return make_uint3( b * a.x, b * a.y, b * a.z ); }
inline void operator*=( uint3& a, uint b ) { a.x *= b;	a.y *= b;	a.z *= b; }
inline float4 operator*( const float4& a, const float4& b ) { return make_float4( a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w ); }
inline void operator*=( float4& a, const float4& b ) { a.x *= b.x;	a.y *= b.y;	a.z *= b.z;	a.w *= b.w; }
inline float4 operator*( const float4& a, float b ) { return make_float4( a.x * b, a.y * b, a.z * b, a.w * b ); }
inline float4 operator*( float b, const float4& a ) { return make_float4( b * a.x, b * a.y, b * a.z, b * a.w ); }
inline void operator*=( float4& a, float b ) { a.x *= b;	a.y *= b;	a.z *= b;	a.w *= b; }
inline int4 operator*( const int4& a, const int4& b ) { return make_int4( a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w ); }
inline void operator*=( int4& a, const int4& b ) { a.x *= b.x;	a.y *= b.y;	a.z *= b.z;	a.w *= b.w; }
inline int4 operator*( const int4& a, int b ) { return make_int4( a.x * b, a.y * b, a.z * b, a.w * b ); }
inline int4 operator*( int b, const int4& a ) { return make_int4( b * a.x, b * a.y, b * a.z, b * a.w ); }
inline void operator*=( int4& a, int b ) { a.x *= b;	a.y *= b;	a.z *= b;	a.w *= b; }
inline uint4 operator*( const uint4& a, const uint4& b ) { return make_uint4( a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w ); }
inline void operator*=( uint4& a, const uint4& b ) { a.x *= b.x;	a.y *= b.y;	a.z *= b.z;	a.w *= b.w; }
inline uint4 operator*( const uint4& a, uint b ) { return make_uint4( a.x * b, a.y * b, a.z * b, a.w * b ); }
inline uint4 operator*( uint b, const uint4& a ) { return make_uint4( b * a.x, b * a.y, b * a.z, b * a.w ); }
inline void operator*=( uint4& a, uint b ) { a.x *= b;	a.y *= b;	a.z *= b;	a.w *= b; }

inline float2 operator/( const float2& a, const float2& b ) { return make_float2( a.x / b.x, a.y / b.y ); }
inline void operator/=( float2& a, const float2& b ) { a.x /= b.x;	a.y /= b.y; }
inline float2 operator/( const float2& a, float b ) { return make_float2( a.x / b, a.y / b ); }
inline void operator/=( float2& a, float b ) { a.x /= b;	a.y /= b; }
inline float2 operator/( float b, const float2& a ) { return make_float2( b / a.x, b / a.y ); }
inline float3 operator/( const float3& a, const float3& b ) { return make_float3( a.x / b.x, a.y / b.y, a.z / b.z ); }
inline void operator/=( float3& a, const float3& b ) { a.x /= b.x;	a.y /= b.y;	a.z /= b.z; }
inline float3 operator/( const float3& a, float b ) { return make_float3( a.x / b, a.y / b, a.z / b ); }
inline void operator/=( float3& a, float b ) { a.x /= b;	a.y /= b;	a.z /= b; }
inline float3 operator/( float b, const float3& a ) { return make_float3( b / a.x, b / a.y, b / a.z ); }
inline float4 operator/( const float4& a, const float4& b ) { return make_float4( a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w ); }
inline void operator/=( float4& a, const float4& b ) { a.x /= b.x;	a.y /= b.y;	a.z /= b.z;	a.w /= b.w; }
inline float4 operator/( const float4& a, float b ) { return make_float4( a.x / b, a.y / b, a.z / b, a.w / b ); }
inline void operator/=( float4& a, float b ) { a.x /= b;	a.y /= b;	a.z /= b;	a.w /= b; }
inline float4 operator/( float b, const float4& a ) { return make_float4( b / a.x, b / a.y, b / a.z, b / a.w ); }

inline float2 fminf( const float2& a, const float2& b ) { return make_float2( fminf( a.x, b.x ), fminf( a.y, b.y ) ); }
inline float3 fminf( const float3& a, const float3& b ) { return make_float3( fminf( a.x, b.x ), fminf( a.y, b.y ), fminf( a.z, b.z ) ); }
inline float4 fminf( const float4& a, const float4& b ) { return make_float4( fminf( a.x, b.x ), fminf( a.y, b.y ), fminf( a.z, b.z ), fminf( a.w, b.w ) ); }
inline int2 min( const int2& a, const int2& b ) { return make_int2( min( a.x, b.x ), min( a.y, b.y ) ); }
inline int3 min( const int3& a, const int3& b ) { return make_int3( min( a.x, b.x ), min( a.y, b.y ), min( a.z, b.z ) ); }
inline int4 min( const int4& a, const int4& b ) { return make_int4( min( a.x, b.x ), min( a.y, b.y ), min( a.z, b.z ), min( a.w, b.w ) ); }
inline uint2 min( const uint2& a, const uint2& b ) { return make_uint2( min( a.x, b.x ), min( a.y, b.y ) ); }
inline uint3 min( const uint3& a, const uint3& b ) { return make_uint3( min( a.x, b.x ), min( a.y, b.y ), min( a.z, b.z ) ); }
inline uint4 min( const uint4& a, const uint4& b ) { return make_uint4( min( a.x, b.x ), min( a.y, b.y ), min( a.z, b.z ), min( a.w, b.w ) ); }

inline float2 fmaxf( const float2& a, const float2& b ) { return make_float2( fmaxf( a.x, b.x ), fmaxf( a.y, b.y ) ); }
inline float3 fmaxf( const float3& a, const float3& b ) { return make_float3( fmaxf( a.x, b.x ), fmaxf( a.y, b.y ), fmaxf( a.z, b.z ) ); }
inline float4 fmaxf( const float4& a, const float4& b ) { return make_float4( fmaxf( a.x, b.x ), fmaxf( a.y, b.y ), fmaxf( a.z, b.z ), fmaxf( a.w, b.w ) ); }
inline int2 max( const int2& a, const int2& b ) { return make_int2( max( a.x, b.x ), max( a.y, b.y ) ); }
inline int3 max( const int3& a, const int3& b ) { return make_int3( max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z ) ); }
inline int4 max( const int4& a, const int4& b ) { return make_int4( max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z ), max( a.w, b.w ) ); }
inline uint2 max( const uint2& a, const uint2& b ) { return make_uint2( max( a.x, b.x ), max( a.y, b.y ) ); }
inline uint3 max( const uint3& a, const uint3& b ) { return make_uint3( max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z ) ); }
inline uint4 max( const uint4& a, const uint4& b ) { return make_uint4( max( a.x, b.x ), max( a.y, b.y ), max( a.z, b.z ), max( a.w, b.w ) ); }

inline float lerp( float a, float b, float t ) { return a + t * (b - a); }
inline float2 lerp( const float2& a, const float2& b, float t ) { return a + t * (b - a); }
inline float3 lerp( const float3& a, const float3& b, float t ) { return a + t * (b - a); }
inline float4 lerp( const float4& a, const float4& b, float t ) { return a + t * (b - a); }

inline float clamp( float f, float a, float b ) { return fmaxf( a, fminf( f, b ) ); }
inline int clamp( int f, int a, int b ) { return max( a, min( f, b ) ); }
inline uint clamp( uint f, uint a, uint b ) { return max( a, min( f, b ) ); }
inline float2 clamp( const float2& v, float a, float b ) { return make_float2( clamp( v.x, a, b ), clamp( v.y, a, b ) ); }
inline float2 clamp( const float2& v, const float2& a, const float2& b ) { return make_float2( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ) ); }
inline float3 clamp( const float3& v, float a, float b ) { return make_float3( clamp( v.x, a, b ), clamp( v.y, a, b ), clamp( v.z, a, b ) ); }
inline float3 clamp( const float3& v, const float3& a, const float3& b ) { return make_float3( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ), clamp( v.z, a.z, b.z ) ); }
inline float4 clamp( const float4& v, float a, float b ) { return make_float4( clamp( v.x, a, b ), clamp( v.y, a, b ), clamp( v.z, a, b ), clamp( v.w, a, b ) ); }
inline float4 clamp( const float4& v, const float4& a, const float4& b ) { return make_float4( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ), clamp( v.z, a.z, b.z ), clamp( v.w, a.w, b.w ) ); }
inline int2 clamp( const int2& v, int a, int b ) { return make_int2( clamp( v.x, a, b ), clamp( v.y, a, b ) ); }
inline int2 clamp( const int2& v, const int2& a, const int2& b ) { return make_int2( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ) ); }
inline int3 clamp( const int3& v, int a, int b ) { return make_int3( clamp( v.x, a, b ), clamp( v.y, a, b ), clamp( v.z, a, b ) ); }
inline int3 clamp( const int3& v, const int3& a, const int3& b ) { return make_int3( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ), clamp( v.z, a.z, b.z ) ); }
inline int4 clamp( const int4& v, int a, int b ) { return make_int4( clamp( v.x, a, b ), clamp( v.y, a, b ), clamp( v.z, a, b ), clamp( v.w, a, b ) ); }
inline int4 clamp( const int4& v, const int4& a, const int4& b ) { return make_int4( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ), clamp( v.z, a.z, b.z ), clamp( v.w, a.w, b.w ) ); }
inline uint2 clamp( const uint2& v, uint a, uint b ) { return make_uint2( clamp( v.x, a, b ), clamp( v.y, a, b ) ); }
inline uint2 clamp( const uint2& v, const uint2& a, const uint2& b ) { return make_uint2( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ) ); }
inline uint3 clamp( const uint3& v, uint a, uint b ) { return make_uint3( clamp( v.x, a, b ), clamp( v.y, a, b ), clamp( v.z, a, b ) ); }
inline uint3 clamp( const uint3& v, const uint3& a, const uint3& b ) { return make_uint3( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ), clamp( v.z, a.z, b.z ) ); }
inline uint4 clamp( const uint4& v, uint a, uint b ) { return make_uint4( clamp( v.x, a, b ), clamp( v.y, a, b ), clamp( v.z, a, b ), clamp( v.w, a, b ) ); }
inline uint4 clamp( const uint4& v, const uint4& a, const uint4& b ) { return make_uint4( clamp( v.x, a.x, b.x ), clamp( v.y, a.y, b.y ), clamp( v.z, a.z, b.z ), clamp( v.w, a.w, b.w ) ); }

inline float dot( const float2& a, const float2& b ) { return a.x * b.x + a.y * b.y; }
inline float dot( const float3& a, const float3& b ) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline float dot( const float4& a, const float4& b ) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
inline int dot( const int2& a, const int2& b ) { return a.x * b.x + a.y * b.y; }
inline int dot( const int3& a, const int3& b ) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline int dot( const int4& a, const int4& b ) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
inline uint dot( const uint2& a, const uint2& b ) { return a.x * b.x + a.y * b.y; }
inline uint dot( const uint3& a, const uint3& b ) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline uint dot( const uint4& a, const uint4& b ) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

inline float sqrLength( const float2& v ) { return dot( v, v ); }
inline float sqrLength( const float3& v ) { return dot( v, v ); }
inline float sqrLength( const float4& v ) { return dot( v, v ); }

inline float length( const float2& v ) { return sqrtf( dot( v, v ) ); }
inline float length( const float3& v ) { return sqrtf( dot( v, v ) ); }
inline float length( const float4& v ) { return sqrtf( dot( v, v ) ); }

inline float length( const int2& v ) { return sqrtf( (float)dot( v, v ) ); }
inline float length( const int3& v ) { return sqrtf( (float)dot( v, v ) ); }
inline float length( const int4& v ) { return sqrtf( (float)dot( v, v ) ); }

inline float2 normalize( const float2& v ) { float invLen = rsqrtf( dot( v, v ) ); return v * invLen; }
inline float3 normalize( const float3& v ) { float invLen = rsqrtf( dot( v, v ) ); return v * invLen; }
inline float4 normalize( const float4& v ) { float invLen = rsqrtf( dot( v, v ) ); return v * invLen; }

inline uint dominantAxis( const float2& v ) { float x = fabs( v.x ), y = fabs( v.y ); return x > y ? 0 : 1; } // for coherent grid traversal
inline uint dominantAxis( const float3& v ) { float x = fabs( v.x ), y = fabs( v.y ), z = fabs( v.z ); float m = max( max( x, y ), z ); return m == x ? 0 : (m == y ? 1 : 2); }

inline float2 floorf( const float2& v ) { return make_float2( floorf( v.x ), floorf( v.y ) ); }
inline float3 floorf( const float3& v ) { return make_float3( floorf( v.x ), floorf( v.y ), floorf( v.z ) ); }
inline float4 floorf( const float4& v ) { return make_float4( floorf( v.x ), floorf( v.y ), floorf( v.z ), floorf( v.w ) ); }

inline float fracf( float v ) { return v - floorf( v ); }
inline float2 fracf( const float2& v ) { return make_float2( fracf( v.x ), fracf( v.y ) ); }
inline float3 fracf( const float3& v ) { return make_float3( fracf( v.x ), fracf( v.y ), fracf( v.z ) ); }
inline float4 fracf( const float4& v ) { return make_float4( fracf( v.x ), fracf( v.y ), fracf( v.z ), fracf( v.w ) ); }

inline float2 fmodf( const float2& a, const float2& b ) { return make_float2( fmodf( a.x, b.x ), fmodf( a.y, b.y ) ); }
inline float3 fmodf( const float3& a, const float3& b ) { return make_float3( fmodf( a.x, b.x ), fmodf( a.y, b.y ), fmodf( a.z, b.z ) ); }
inline float4 fmodf( const float4& a, const float4& b ) { return make_float4( fmodf( a.x, b.x ), fmodf( a.y, b.y ), fmodf( a.z, b.z ), fmodf( a.w, b.w ) ); }

inline float2 fabs( const float2& v ) { return make_float2( fabs( v.x ), fabs( v.y ) ); }
inline float3 fabs( const float3& v ) { return make_float3( fabs( v.x ), fabs( v.y ), fabs( v.z ) ); }
inline float4 fabs( const float4& v ) { return make_float4( fabs( v.x ), fabs( v.y ), fabs( v.z ), fabs( v.w ) ); }
inline int2 abs( const int2& v ) { return make_int2( abs( v.x ), abs( v.y ) ); }
inline int3 abs( const int3& v ) { return make_int3( abs( v.x ), abs( v.y ), abs( v.z ) ); }
inline int4 abs( const int4& v ) { return make_int4( abs( v.x ), abs( v.y ), abs( v.z ), abs( v.w ) ); }

inline float3 reflect( const float3& i, const float3& n ) { return i - 2.0f * n * dot( n, i ); }

inline float3 cross( const float3& a, const float3& b ) { return make_float3( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x ); }

inline float smoothstep( float a, float b, float x )
{
	float y = clamp( (x - a) / (b - a), 0.0f, 1.0f );
	return (y * y * (3.0f - (2.0f * y)));
}
inline float2 smoothstep( float2 a, float2 b, float2 x )
{
	float2 y = clamp( (x - a) / (b - a), 0.0f, 1.0f );
	return (y * y * (make_float2( 3.0f ) - (make_float2( 2.0f ) * y)));
}
inline float3 smoothstep( float3 a, float3 b, float3 x )
{
	float3 y = clamp( (x - a) / (b - a), 0.0f, 1.0f );
	return (y * y * (make_float3( 3.0f ) - (make_float3( 2.0f ) * y)));
}
inline float4 smoothstep( float4 a, float4 b, float4 x )
{
	float4 y = clamp( (x - a) / (b - a), 0.0f, 1.0f );
	return (y * y * (make_float4( 3.0f ) - (make_float4( 2.0f ) * y)));
}

// axis aligned bounding box class
class aabb
{
public:
	aabb() = default;
	aabb( __m128 a, __m128 b ) { bmin4 = a, bmax4 = b; bmin[3] = bmax[3] = 0; }
	aabb( float3 a, float3 b ) { bmin[0] = a.x, bmin[1] = a.y, bmin[2] = a.z, bmin[3] = 0, bmax[0] = b.x, bmax[1] = b.y, bmax[2] = b.z, bmax[3] = 0; }
	__inline void Reset() { bmin4 = _mm_set_ps1( 1e34f ), bmax4 = _mm_set_ps1( -1e34f ); }
	bool Contains( const __m128& p ) const
	{
		union { __m128 va4; float va[4]; };
		union { __m128 vb4; float vb[4]; };
		va4 = _mm_sub_ps( p, bmin4 ), vb4 = _mm_sub_ps( bmax4, p );
		return ((va[0] >= 0) && (va[1] >= 0) && (va[2] >= 0) &&
			(vb[0] >= 0) && (vb[1] >= 0) && (vb[2] >= 0));
	}
	__inline void Grow( const aabb& bb ) { bmin4 = _mm_min_ps( bmin4, bb.bmin4 ); bmax4 = _mm_max_ps( bmax4, bb.bmax4 ); }
	__inline void Grow( const __m128& p ) { bmin4 = _mm_min_ps( bmin4, p ); bmax4 = _mm_max_ps( bmax4, p ); }
	__inline void Grow( const __m128 min4, const __m128 max4 ) { bmin4 = _mm_min_ps( bmin4, min4 ); bmax4 = _mm_max_ps( bmax4, max4 ); }
	__inline void Grow( const float3& p ) { __m128 p4 = _mm_setr_ps( p.x, p.y, p.z, 0 ); Grow( p4 ); }
	aabb Union( const aabb& bb ) const { aabb r; r.bmin4 = _mm_min_ps( bmin4, bb.bmin4 ), r.bmax4 = _mm_max_ps( bmax4, bb.bmax4 ); return r; }
	static aabb Union( const aabb& a, const aabb& b ) { aabb r; r.bmin4 = _mm_min_ps( a.bmin4, b.bmin4 ), r.bmax4 = _mm_max_ps( a.bmax4, b.bmax4 ); return r; }
	aabb Intersection( const aabb& bb ) const { aabb r; r.bmin4 = _mm_max_ps( bmin4, bb.bmin4 ), r.bmax4 = _mm_min_ps( bmax4, bb.bmax4 ); return r; }
	__inline float Extend( const int axis ) const { return bmax[axis] - bmin[axis]; }
	__inline float Minimum( const int axis ) const { return bmin[axis]; }
	__inline float Maximum( const int axis ) const { return bmax[axis]; }
	float Area() const
	{
		union { __m128 e4; float e[4]; };
		e4 = _mm_sub_ps( bmax4, bmin4 );
		return max( 0.0f, e[0] * e[1] + e[0] * e[2] + e[1] * e[2] );
	}
	int LongestAxis() const
	{
		int a = 0;
		if (Extend( 1 ) > Extend( 0 )) a = 1;
		if (Extend( 2 ) > Extend( a )) a = 2;
		return a;
	}
	// data members
	union
	{
		struct
		{
			union { __m128 bmin4; float bmin[4]; struct { float3 bmin3; }; };
			union { __m128 bmax4; float bmax[4]; struct { float3 bmax3; }; };
		};
		__m128 bounds[2] = { _mm_set_ps( 1e34f, 1e34f, 1e34f, 0 ), _mm_set_ps( -1e34f, -1e34f, -1e34f, 0 ) };
	};
	__inline void SetBounds( const __m128 min4, const __m128 max4 ) { bmin4 = min4; bmax4 = max4; }
	__inline __m128 Center() const { return _mm_mul_ps( _mm_add_ps( bmin4, bmax4 ), _mm_set_ps1( 0.5f ) ); }
	__inline float Center( uint axis ) const { return (bmin[axis] + bmax[axis]) * 0.5f; }
};

// matrix class
class mat4
{
public:
	mat4() = default;
	float cell[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	float& operator [] ( const int idx ) { return cell[idx]; }
	float operator()( const int i, const int j ) const { return cell[i * 4 + j]; }
	float& operator()( const int i, const int j ) { return cell[i * 4 + j]; }
	mat4& operator += ( const mat4& a )
	{
		for (int i = 0; i < 16; i++) cell[i] += a.cell[i];
		return *this;
	}
	bool operator==( const mat4& m )
	{
		for (int i = 0; i < 16; i++) if (m.cell[i] != cell[i]) return false; return true;
	}
	float3 GetTranslation() const { return make_float3( cell[3], cell[7], cell[11] ); }
	static mat4 FromColumnMajor( const mat4& T )
	{
		mat4 M;
		M.cell[0] = T.cell[0], M.cell[1] = T.cell[4], M.cell[2] = T.cell[8], M.cell[3] = T.cell[12];
		M.cell[4] = T.cell[1], M.cell[5] = T.cell[5], M.cell[6] = T.cell[9], M.cell[7] = T.cell[13];
		M.cell[8] = T.cell[2], M.cell[9] = T.cell[6], M.cell[10] = T.cell[10], M.cell[11] = T.cell[14];
		M.cell[12] = T.cell[3], M.cell[13] = T.cell[7], M.cell[14] = T.cell[11], M.cell[15] = T.cell[15];
		return M;
	}
	constexpr static mat4 Identity() { return mat4{}; }
	static mat4 ZeroMatrix() { mat4 r; memset( r.cell, 0, 64 ); return r; }
	static mat4 RotateX( const float a ) { mat4 r; r.cell[5] = cosf( a ); r.cell[6] = -sinf( a ); r.cell[9] = sinf( a ); r.cell[10] = cosf( a ); return r; };
	static mat4 RotateY( const float a ) { mat4 r; r.cell[0] = cosf( a ); r.cell[2] = sinf( a ); r.cell[8] = -sinf( a ); r.cell[10] = cosf( a ); return r; };
	static mat4 RotateZ( const float a ) { mat4 r; r.cell[0] = cosf( a ); r.cell[1] = -sinf( a ); r.cell[4] = sinf( a ); r.cell[5] = cosf( a ); return r; };
	static mat4 Scale( const float s ) { mat4 r; r.cell[0] = r.cell[5] = r.cell[10] = s; return r; }
	static mat4 Scale( const float3 s ) { mat4 r; r.cell[0] = s.x, r.cell[5] = s.y, r.cell[10] = s.z; return r; }
	static mat4 Scale( const float4 s ) { mat4 r; r.cell[0] = s.x, r.cell[5] = s.y, r.cell[10] = s.z, r.cell[15] = s.w; return r; }
	static mat4 Rotate( const float3& u, const float a ) { return Rotate( u.x, u.y, u.z, a ); }
	static mat4 Rotate( const float x, const float y, const float z, const float a )
	{
		const float c = cosf( a ), l_c = 1 - c, s = sinf( a );
		// row major
		mat4 m;
		m[0] = x * x + (1 - x * x) * c, m[1] = x * y * l_c + z * s, m[2] = x * z * l_c - y * s, m[3] = 0;
		m[4] = x * y * l_c - z * s, m[5] = y * y + (1 - y * y) * c, m[6] = y * z * l_c + x * s, m[7] = 0;
		m[8] = x * z * l_c + y * s, m[9] = y * z * l_c - x * s, m[10] = z * z + (1 - z * z) * c, m[11] = 0;
		m[12] = m[13] = m[14] = 0, m[15] = 1;
		return m;
	}
	static mat4 LookAt( const float3 P, const float3 T )
	{
		const float3 z = normalize( T - P );
		const float3 x = normalize( cross( z, make_float3( 0, 1, 0 ) ) );
		const float3 y = cross( x, z );
		mat4 M = Translate( P );
		M[0] = x.x, M[4] = x.y, M[8] = x.z;
		M[1] = y.x, M[5] = y.y, M[9] = y.z;
		M[2] = z.x, M[6] = z.y, M[10] = z.z;
		return M;
	}
	static mat4 LookAt( const float3& pos, const float3& look, const float3& up )
	{
		// PBRT's lookat
		mat4 cameraToWorld;
		// initialize fourth column of viewing matrix
		cameraToWorld( 0, 3 ) = pos.x;
		cameraToWorld( 1, 3 ) = pos.y;
		cameraToWorld( 2, 3 ) = pos.z;
		cameraToWorld( 3, 3 ) = 1;

		// initialize first three columns of viewing matrix
		float3 dir = normalize( look - pos );
		float3 right = cross( normalize( up ), dir );
		if (dot( right, right ) == 0)
		{
			printf(
				"\"up\" vector (%f, %f, %f) and viewing direction (%f, %f, %f) "
				"passed to LookAt are pointing in the same direction.  Using "
				"the identity transformation.\n",
				up.x, up.y, up.z, dir.x, dir.y, dir.z );
			return mat4();
		}
		right = normalize( right );
		float3 newUp = cross( dir, right );
		cameraToWorld( 0, 0 ) = right.x, cameraToWorld( 1, 0 ) = right.y;
		cameraToWorld( 2, 0 ) = right.z, cameraToWorld( 3, 0 ) = 0.;
		cameraToWorld( 0, 1 ) = newUp.x, cameraToWorld( 1, 1 ) = newUp.y;
		cameraToWorld( 2, 1 ) = newUp.z, cameraToWorld( 3, 1 ) = 0.;
		cameraToWorld( 0, 2 ) = dir.x, cameraToWorld( 1, 2 ) = dir.y;
		cameraToWorld( 2, 2 ) = dir.z, cameraToWorld( 3, 2 ) = 0.;
		return cameraToWorld.Inverted();
	}
	static mat4 Translate( const float x, const float y, const float z ) { mat4 r; r.cell[3] = x; r.cell[7] = y; r.cell[11] = z; return r; };
	static mat4 Translate( const float3 P ) { mat4 r; r.cell[3] = P.x; r.cell[7] = P.y; r.cell[11] = P.z; return r; };
	float Trace3() const { return cell[0] + cell[5] + cell[10]; }
	CHECK_RESULT mat4 Transposed() const
	{
		mat4 M;
		M[0] = cell[0], M[1] = cell[4], M[2] = cell[8];
		M[4] = cell[1], M[5] = cell[5], M[6] = cell[9];
		M[8] = cell[2], M[9] = cell[6], M[10] = cell[10];
		return M;
	}
	CHECK_RESULT mat4 Inverted() const
	{
		// from MESA, via http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		const float inv[16] = {
			cell[5] * cell[10] * cell[15] - cell[5] * cell[11] * cell[14] - cell[9] * cell[6] * cell[15] +
			cell[9] * cell[7] * cell[14] + cell[13] * cell[6] * cell[11] - cell[13] * cell[7] * cell[10],
			-cell[1] * cell[10] * cell[15] + cell[1] * cell[11] * cell[14] + cell[9] * cell[2] * cell[15] -
			cell[9] * cell[3] * cell[14] - cell[13] * cell[2] * cell[11] + cell[13] * cell[3] * cell[10],
			cell[1] * cell[6] * cell[15] - cell[1] * cell[7] * cell[14] - cell[5] * cell[2] * cell[15] +
			cell[5] * cell[3] * cell[14] + cell[13] * cell[2] * cell[7] - cell[13] * cell[3] * cell[6],
			-cell[1] * cell[6] * cell[11] + cell[1] * cell[7] * cell[10] + cell[5] * cell[2] * cell[11] -
			cell[5] * cell[3] * cell[10] - cell[9] * cell[2] * cell[7] + cell[9] * cell[3] * cell[6],
			-cell[4] * cell[10] * cell[15] + cell[4] * cell[11] * cell[14] + cell[8] * cell[6] * cell[15] -
			cell[8] * cell[7] * cell[14] - cell[12] * cell[6] * cell[11] + cell[12] * cell[7] * cell[10],
			cell[0] * cell[10] * cell[15] - cell[0] * cell[11] * cell[14] - cell[8] * cell[2] * cell[15] +
			cell[8] * cell[3] * cell[14] + cell[12] * cell[2] * cell[11] - cell[12] * cell[3] * cell[10],
			-cell[0] * cell[6] * cell[15] + cell[0] * cell[7] * cell[14] + cell[4] * cell[2] * cell[15] -
			cell[4] * cell[3] * cell[14] - cell[12] * cell[2] * cell[7] + cell[12] * cell[3] * cell[6],
			cell[0] * cell[6] * cell[11] - cell[0] * cell[7] * cell[10] - cell[4] * cell[2] * cell[11] +
			cell[4] * cell[3] * cell[10] + cell[8] * cell[2] * cell[7] - cell[8] * cell[3] * cell[6],
			cell[4] * cell[9] * cell[15] - cell[4] * cell[11] * cell[13] - cell[8] * cell[5] * cell[15] +
			cell[8] * cell[7] * cell[13] + cell[12] * cell[5] * cell[11] - cell[12] * cell[7] * cell[9],
			-cell[0] * cell[9] * cell[15] + cell[0] * cell[11] * cell[13] + cell[8] * cell[1] * cell[15] -
			cell[8] * cell[3] * cell[13] - cell[12] * cell[1] * cell[11] + cell[12] * cell[3] * cell[9],
			cell[0] * cell[5] * cell[15] - cell[0] * cell[7] * cell[13] - cell[4] * cell[1] * cell[15] +
			cell[4] * cell[3] * cell[13] + cell[12] * cell[1] * cell[7] - cell[12] * cell[3] * cell[5],
			-cell[0] * cell[5] * cell[11] + cell[0] * cell[7] * cell[9] + cell[4] * cell[1] * cell[11] -
			cell[4] * cell[3] * cell[9] - cell[8] * cell[1] * cell[7] + cell[8] * cell[3] * cell[5],
			-cell[4] * cell[9] * cell[14] + cell[4] * cell[10] * cell[13] + cell[8] * cell[5] * cell[14] -
			cell[8] * cell[6] * cell[13] - cell[12] * cell[5] * cell[10] + cell[12] * cell[6] * cell[9],
			cell[0] * cell[9] * cell[14] - cell[0] * cell[10] * cell[13] - cell[8] * cell[1] * cell[14] +
			cell[8] * cell[2] * cell[13] + cell[12] * cell[1] * cell[10] - cell[12] * cell[2] * cell[9],
			-cell[0] * cell[5] * cell[14] + cell[0] * cell[6] * cell[13] + cell[4] * cell[1] * cell[14] -
			cell[4] * cell[2] * cell[13] - cell[12] * cell[1] * cell[6] + cell[12] * cell[2] * cell[5],
			cell[0] * cell[5] * cell[10] - cell[0] * cell[6] * cell[9] - cell[4] * cell[1] * cell[10] +
			cell[4] * cell[2] * cell[9] + cell[8] * cell[1] * cell[6] - cell[8] * cell[2] * cell[5]
		};
		const float det = cell[0] * inv[0] + cell[1] * inv[4] + cell[2] * inv[8] + cell[3] * inv[12];
		mat4 retVal;
		if (det != 0)
		{
			const float invdet = 1.0f / det;
			for (int i = 0; i < 16; i++) retVal.cell[i] = inv[i] * invdet;
		}
		return retVal;
	}

	CHECK_RESULT mat4 Inverted3x3() const
	{
		// via https://stackoverflow.com/questions/983999/simple-3x3-matrix-inverse-code-c
		const float invdet = 1.0f / (cell[0] * (cell[5] * cell[10] - cell[6] * cell[9]) -
		 cell[4] * (cell[1] * cell[10] - cell[9] * cell[2]) +
		 cell[8] * (cell[1] * cell[6] - cell[5] * cell[2]));
		mat4 R;
		R.cell[0] = (cell[5] * cell[10] - cell[6] * cell[9]) * invdet;
		R.cell[4] = (cell[8] * cell[6] - cell[4] * cell[10]) * invdet;
		R.cell[8] = (cell[4] * cell[9] - cell[8] * cell[5]) * invdet;
		R.cell[1] = (cell[9] * cell[2] - cell[1] * cell[10]) * invdet;
		R.cell[5] = (cell[0] * cell[10] - cell[8] * cell[2]) * invdet;
		R.cell[9] = (cell[1] * cell[8] - cell[0] * cell[9]) * invdet;
		R.cell[2] = (cell[1] * cell[6] - cell[2] * cell[5]) * invdet;
		R.cell[6] = (cell[2] * cell[4] - cell[0] * cell[6]) * invdet;
		R.cell[10] = (cell[0] * cell[5] - cell[1] * cell[4]) * invdet;
		return R;
	}

	inline float3 TransformVector( const float3& v ) const
	{
		return make_float3( cell[0] * v.x + cell[1] * v.y + cell[2] * v.z,
			cell[4] * v.x + cell[5] * v.y + cell[6] * v.z,
			cell[8] * v.x + cell[9] * v.y + cell[10] * v.z );
	}

	inline float3 TransformPoint( const float3& v ) const
	{
		const float3 res = make_float3(
			cell[0] * v.x + cell[1] * v.y + cell[2] * v.z + cell[3],
			cell[4] * v.x + cell[5] * v.y + cell[6] * v.z + cell[7],
			cell[8] * v.x + cell[9] * v.y + cell[10] * v.z + cell[11] );
		const float w = cell[12] * v.x + cell[13] * v.y + cell[14] * v.z + cell[15];
		if (w == 1) return res;
		return res * (1.f / w);
	}
};

mat4 operator * ( const mat4& a, const mat4& b );
mat4 operator + ( const mat4& a, const mat4& b );
mat4 operator * ( const mat4& a, const float s );
mat4 operator * ( const float s, const mat4& a );
bool operator == ( const mat4& a, const mat4& b );
bool operator != ( const mat4& a, const mat4& b );
float4 operator * ( const mat4& a, const float4& b );
float4 operator * ( const float4& a, const mat4& b );

float3 TransformPosition( const float3& a, const mat4& M );
float3 TransformVector( const float3& a, const mat4& M );

class quat // based on https://github.com/adafruit
{
public:
	quat() = default;
	quat( float _w, float _x, float _y, float _z ) : w( _w ), x( _x ), y( _y ), z( _z ) {}
	quat( float _w, float3 v ) : w( _w ), x( v.x ), y( v.y ), z( v.z ) {}
	float magnitude() const { return sqrtf( w * w + x * x + y * y + z * z ); }
	void normalize() { float m = magnitude(); *this = this->scale( 1 / m ); }
	quat conjugate() const { return quat( w, -x, -y, -z ); }
	void fromAxisAngle( const float3& axis, float theta )
	{
		w = cosf( theta / 2 );
		const float s = sinf( theta / 2 );
		x = axis.x * s, y = axis.y * s, z = axis.z * s;
	}
	void fromMatrix( const mat4& m )
	{
		float tr = m.Trace3(), S;
		if (tr > 0)
		{
			S = sqrtf( tr + 1.0f ) * 2, w = 0.25f * S;
			x = (m( 2, 1 ) - m( 1, 2 )) / S, y = (m( 0, 2 ) - m( 2, 0 )) / S;
			z = (m( 1, 0 ) - m( 0, 1 )) / S;
		}
		else if (m( 0, 0 ) > m( 1, 1 ) && m( 0, 0 ) > m( 2, 2 ))
		{
			S = sqrt( 1.0f + m( 0, 0 ) - m( 1, 1 ) - m( 2, 2 ) ) * 2;
			w = (m( 2, 1 ) - m( 1, 2 )) / S, x = 0.25f * S;
			y = (m( 0, 1 ) + m( 1, 0 )) / S, z = (m( 0, 2 ) + m( 2, 0 )) / S;
		}
		else if (m( 1, 1 ) > m( 2, 2 ))
		{
			S = sqrt( 1.0f + m( 1, 1 ) - m( 0, 0 ) - m( 2, 2 ) ) * 2;
			w = (m( 0, 2 ) - m( 2, 0 )) / S;
			x = (m( 0, 1 ) + m( 1, 0 )) / S, y = 0.25f * S;
			z = (m( 1, 2 ) + m( 2, 1 )) / S;
		}
		else
		{
			S = sqrt( 1.0f + m( 2, 2 ) - m( 0, 0 ) - m( 1, 1 ) ) * 2;
			w = (m( 1, 0 ) - m( 0, 1 )) / S, x = (m( 0, 2 ) + m( 2, 0 )) / S;
			y = (m( 1, 2 ) + m( 2, 1 )) / S, z = 0.25f * S;
		}
	}
	void toAxisAngle( float3& axis, float& angle ) const
	{
		float s = sqrtf( 1 - w * w );
		if (s == 0) return;
		angle = 2 * acosf( w );
		axis.x = x / s, axis.y = y / s, axis.z = z / s;
	}
	mat4 toMatrix() const
	{
		mat4 ret;
		ret.cell[0] = 1 - 2 * y * y - 2 * z * z;
		ret.cell[1] = 2 * x * y - 2 * w * z, ret.cell[2] = 2 * x * z + 2 * w * y, ret.cell[4] = 2 * x * y + 2 * w * z;
		ret.cell[5] = 1 - 2 * x * x - 2 * z * z;
		ret.cell[6] = 2 * y * z - 2 * w * x, ret.cell[8] = 2 * x * z - 2 * w * y, ret.cell[9] = 2 * y * z + 2 * w * x;
		ret.cell[10] = 1 - 2 * x * x - 2 * y * y;
		return ret;
	}
	float3 toEuler() const
	{
		float3 ret;
		float sqw = w * w, sqx = x * x, sqy = y * y, sqz = z * z;
		ret.x = atan2f( 2.0f * (x * y + z * w), (sqx - sqy - sqz + sqw) );
		ret.y = asinf( -2.0f * (x * z - y * w) / (sqx + sqy + sqz + sqw) );
		ret.z = atan2f( 2.0f * (y * z + x * w), (-sqx - sqy + sqz + sqw) );
		return ret;
	}
	float3 toAngularVelocity( float dt ) const
	{
		float3 ret;
		quat one( 1, 0, 0, 0 ), delta = one - *this, r = (delta / dt);
		r = r * 2, r = r * one, ret.x = r.x, ret.y = r.y, ret.z = r.z;
		return ret;
	}
	float3 rotateVector( const float3& v ) const
	{
		float3 qv = make_float3( x, y, z ), t = cross( qv, v ) * 2.0f;
		return v + t * w + cross( qv, t );
	}
	quat operator * ( const quat& q ) const
	{
		return quat(
			w * q.w - x * q.x - y * q.y - z * q.z, w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x, w * q.z + x * q.y - y * q.x + z * q.w
		);
	}
	static quat slerp( const quat& a, const quat& b, const float t )
	{
		// from https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
		quat qm;
		float cosHalfTheta = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
		if (abs( cosHalfTheta ) >= 1.0)
		{
			qm.w = a.w, qm.x = a.x, qm.y = a.y, qm.z = a.z;
			return qm;
		}
		float halfTheta = acosf( cosHalfTheta );
		float sinHalfTheta = sqrtf( 1.0f - cosHalfTheta * cosHalfTheta );
		if (fabs( sinHalfTheta ) < 0.001f)
		{
			qm.w = a.w * 0.5f + b.w * 0.5f, qm.x = a.x * 0.5f + b.x * 0.5f;
			qm.y = a.y * 0.5f + b.y * 0.5f, qm.z = a.z * 0.5f + b.z * 0.5f;
			return qm;
		}
		float ratioA = sinf( (1 - t) * halfTheta ) / sinHalfTheta;
		float ratioB = sinf( t * halfTheta ) / sinHalfTheta;
		qm.w = (a.w * ratioA + b.w * ratioB), qm.x = (a.x * ratioA + b.x * ratioB);
		qm.y = (a.y * ratioA + b.y * ratioB), qm.z = (a.z * ratioA + b.z * ratioB);
		return qm;
	}
	quat operator + ( const quat& q ) const { return quat( w + q.w, x + q.x, y + q.y, z + q.z ); }
	quat operator - ( const quat& q ) const { return quat( w - q.w, x - q.x, y - q.y, z - q.z ); }
	quat operator / ( float s ) const { return quat( w / s, x / s, y / s, z / s ); }
	quat operator * ( float s ) const { return scale( s ); }
	quat scale( float s ) const { return quat( w * s, x * s, y * s, z * s ); }
	float w = 1, x = 0, y = 0, z = 0;
};

// OpenCL buffer
class Buffer
{
public:
	enum { DEFAULT = 0, TEXTURE = 8, TARGET = 16, READONLY = 1, WRITEONLY = 2 };
	// constructor / destructor
	Buffer() : hostBuffer( 0 ) {}
	Buffer( unsigned int N, unsigned int t = DEFAULT, void* ptr = 0 );
	~Buffer();
	cl_mem* GetDevicePtr() { return &deviceBuffer; }
	unsigned int* GetHostPtr() { return hostBuffer; }
	void CopyToDevice( bool blocking = true );
	void CopyToDevice2( bool blocking, cl_event* e = 0, const size_t s = 0 );
	void CopyFromDevice( bool blocking = true );
	void CopyTo( Buffer* buffer );
	void Clear();
	// data members
	unsigned int* hostBuffer;
	cl_mem deviceBuffer = 0;
	unsigned int type, size, textureID;
	bool ownData;
};

// OpenCL kernel
class Kernel
{
	friend class Buffer;
public:
	// constructor / destructor
	Kernel( char* file, char* entryPoint );
	Kernel( cl_program& existingProgram, char* entryPoint );
	~Kernel();
	// get / set
	cl_kernel& GetKernel() { return kernel; }
	cl_program& GetProgram() { return program; }
	static cl_command_queue& GetQueue() { return queue; }
	static cl_command_queue& GetQueue2() { return queue2; }
	static cl_context& GetContext() { return context; }
	static cl_device_id& GetDevice() { return device; }
	// methods
	void Run( cl_event* eventToWaitFor = 0, cl_event* eventToSet = 0 );
	void Run( cl_mem* buffers, const int count = 1, cl_event* eventToWaitFor = 0, cl_event* eventToSet = 0, cl_event* acq = 0, cl_event* rel = 0 );
	void Run( Buffer* buffer, const int2 localSize = make_int2( 32, 2 ), cl_event* eventToWaitFor = 0, cl_event* eventToSet = 0, cl_event* acq = 0, cl_event* rel = 0 );
	void Run( Buffer* buffer, const int count = 1, cl_event* eventToWaitFor = 0, cl_event* eventToSet = 0, cl_event* acq = 0, cl_event* rel = 0 );
	void Run( const size_t count, const size_t localSize = 0, cl_event* eventToWaitFor = 0, cl_event* eventToSet = 0 );
	void Run2D( const int2 count, const int2 lsize, cl_event* eventToWaitFor = 0, cl_event* eventToSet = 0 );
	void SetArgument( int idx, cl_mem* buffer );
	void SetArgument( int idx, Buffer* buffer );
	void SetArgument( int idx, Buffer& buffer );
	void SetArgument( int idx, float );
	void SetArgument( int idx, int );
	void SetArgument( int idx, float2 );
	void SetArgument( int idx, float3 );
	void SetArgument( int idx, float4 );
	void SetFirstArgument( cl_mem* buffer ) { argIdx = 0; SetArgument( 0, buffer ); }
	void SetFirstArgument( Buffer* buffer ) { argIdx = 0; SetArgument( 0, buffer ); }
	void SetFirstArgument( float v ) { argIdx = 0; SetArgument( 0, v ); }
	void SetFirstArgument( int v ) { argIdx = 0; SetArgument( 0, v ); }
	void SetFirstArgument( float2 v ) { argIdx = 0; SetArgument( 0, v ); }
	void SetFirstArgument( float3 v ) { argIdx = 0; SetArgument( 0, v ); }
	void SetFirstArgument( float4 v ) { argIdx = 0; SetArgument( 0, v ); }
	void SetNextArgument( cl_mem* buffer ) { SetArgument( ++argIdx, buffer ); }
	void SetNextArgument( Buffer* buffer ) { SetArgument( ++argIdx, buffer ); }
	void SetNextArgument( float v ) { SetArgument( ++argIdx, v ); }
	void SetNextArgument( int v ) { SetArgument( ++argIdx, v ); }
	void SetNextArgument( float2 v ) { SetArgument( ++argIdx, v ); }
	void SetNextArgument( float3 v ) { SetArgument( ++argIdx, v ); }
	void SetNextArgument( float4 v ) { SetArgument( ++argIdx, v ); }
	static bool InitCL();
	static void CheckCLStarted();
	static void KillCL();
private:
	// data members
	cl_kernel kernel;
	cl_mem vbo_cl;
	cl_program program;
	bool arg0set = false;
	int argIdx = 0;
	inline static cl_device_id device;
	inline static cl_context context; // simplifies some things, but limits us to one device
	inline static cl_command_queue queue, queue2;
	inline static char* log = 0;
	inline static bool isNVidia = false, isAMD = false, isIntel = false, isOther = false;
	inline static bool isAmpere = false, isTuring = false, isPascal = false;
	inline static int vendorLines = 0;
public:
	inline static bool candoInterop = false, clStarted = false;
};

// global project settigs; shared with OpenCL
#include "common.h"

// Add your headers here; they will be able to use all previously defined classes and namespaces.
// In your own .cpp files just add #include "precomp.h".
// #include "my_include.h"



// InstructionSet.cpp
// Compile by using: cl /EHsc /W4 InstructionSet.cpp
// processor: x86, x64
// Uses the __cpuid intrinsic to get information about
// CPU extended instruction set support.

#include <iostream>
#include <bitset>
#include <array>
#include <intrin.h>

// instruction set detection
#ifdef _WIN32
#define cpuid(info, x) __cpuidex(info, x, 0)
#else
#include <cpuid.h>
void cpuid( int info[4], int InfoType ) { __cpuid_count( InfoType, 0, info[0], info[1], info[2], info[3] ); }
#endif
class CPUCaps // from https://github.com/Mysticial/FeatureDetector
{
public:
	static inline bool HW_MMX = false;
	static inline bool HW_x64 = false;
	static inline bool HW_ABM = false;
	static inline bool HW_RDRAND = false;
	static inline bool HW_BMI1 = false;
	static inline bool HW_BMI2 = false;
	static inline bool HW_ADX = false;
	static inline bool HW_PREFETCHWT1 = false;
	// SIMD: 128-bit
	static inline bool HW_SSE = false;
	static inline bool HW_SSE2 = false;
	static inline bool HW_SSE3 = false;
	static inline bool HW_SSSE3 = false;
	static inline bool HW_SSE41 = false;
	static inline bool HW_SSE42 = false;
	static inline bool HW_SSE4a = false;
	static inline bool HW_AES = false;
	static inline bool HW_SHA = false;
	// SIMD: 256-bit
	static inline bool HW_AVX = false;
	static inline bool HW_XOP = false;
	static inline bool HW_FMA3 = false;
	static inline bool HW_FMA4 = false;
	static inline bool HW_AVX2 = false;
	// SIMD: 512-bit
	static inline bool HW_AVX512F = false;    //  AVX512 Foundation
	static inline bool HW_AVX512CD = false;   //  AVX512 Conflict Detection
	static inline bool HW_AVX512PF = false;   //  AVX512 Prefetch
	static inline bool HW_AVX512ER = false;   //  AVX512 Exponential + Reciprocal
	static inline bool HW_AVX512VL = false;   //  AVX512 Vector Length Extensions
	static inline bool HW_AVX512BW = false;   //  AVX512 Byte + Word
	static inline bool HW_AVX512DQ = false;   //  AVX512 Doubleword + Quadword
	static inline bool HW_AVX512IFMA = false; //  AVX512 Integer 52-bit Fused Multiply-Add
	static inline bool HW_AVX512VBMI = false; //  AVX512 Vector Byte Manipulation Instructions
	// constructor
	CPUCaps()
	{
		int info[4];
		cpuid( info, 0 );
		int nIds = info[0];
		cpuid( info, 0x80000000 );
		unsigned nExIds = info[0];
		// detect Features
		if (nIds >= 0x00000001)
		{
			cpuid( info, 0x00000001 );
			HW_MMX = (info[3] & ((int)1 << 23)) != 0;
			HW_SSE = (info[3] & ((int)1 << 25)) != 0;
			HW_SSE2 = (info[3] & ((int)1 << 26)) != 0;
			HW_SSE3 = (info[2] & ((int)1 << 0)) != 0;
			HW_SSSE3 = (info[2] & ((int)1 << 9)) != 0;
			HW_SSE41 = (info[2] & ((int)1 << 19)) != 0;
			HW_SSE42 = (info[2] & ((int)1 << 20)) != 0;
			HW_AES = (info[2] & ((int)1 << 25)) != 0;
			HW_AVX = (info[2] & ((int)1 << 28)) != 0;
			HW_FMA3 = (info[2] & ((int)1 << 12)) != 0;
			HW_RDRAND = (info[2] & ((int)1 << 30)) != 0;
		}
		if (nIds >= 0x00000007)
		{
			cpuid( info, 0x00000007 );
			HW_AVX2 = (info[1] & ((int)1 << 5)) != 0;
			HW_BMI1 = (info[1] & ((int)1 << 3)) != 0;
			HW_BMI2 = (info[1] & ((int)1 << 8)) != 0;
			HW_ADX = (info[1] & ((int)1 << 19)) != 0;
			HW_SHA = (info[1] & ((int)1 << 29)) != 0;
			HW_PREFETCHWT1 = (info[2] & ((int)1 << 0)) != 0;
			HW_AVX512F = (info[1] & ((int)1 << 16)) != 0;
			HW_AVX512CD = (info[1] & ((int)1 << 28)) != 0;
			HW_AVX512PF = (info[1] & ((int)1 << 26)) != 0;
			HW_AVX512ER = (info[1] & ((int)1 << 27)) != 0;
			HW_AVX512VL = (info[1] & ((int)1 << 31)) != 0;
			HW_AVX512BW = (info[1] & ((int)1 << 30)) != 0;
			HW_AVX512DQ = (info[1] & ((int)1 << 17)) != 0;
			HW_AVX512IFMA = (info[1] & ((int)1 << 21)) != 0;
			HW_AVX512VBMI = (info[2] & ((int)1 << 1)) != 0;
		}
		if (nExIds >= 0x80000001)
		{
			cpuid( info, 0x80000001 );
			HW_x64 = (info[3] & ((int)1 << 29)) != 0;
			HW_ABM = (info[2] & ((int)1 << 5)) != 0;
			HW_SSE4a = (info[2] & ((int)1 << 6)) != 0;
			HW_FMA4 = (info[2] & ((int)1 << 16)) != 0;
			HW_XOP = (info[2] & ((int)1 << 11)) != 0;
		}
	}
};

// application base class
class TheApp
{
public:
	virtual void Init() = 0;
	virtual void Tick( float deltaTime ) = 0;
	virtual void Shutdown() = 0;
	virtual void MouseUp( int button ) = 0;
	virtual void MouseDown( int button ) = 0;
	virtual void MouseMove( int x, int y ) = 0;
	virtual void MouseWheel( float y ) = 0;
	virtual void KeyUp( int key ) = 0;
	virtual void KeyDown( int key ) = 0;
	Surface* screen = 0;
};

// EOF