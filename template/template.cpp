// Template, IGAD version 2
// IGAD/NHTV/UU - Jacco Bikker - 2006-2020

#include "precomp.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_PIC
#define STBI_NO_PNM
#include "lib/stb_image.h"

#pragma comment( linker, "/subsystem:windows /ENTRY:mainCRTStartup" )

using namespace Tmpl8;

// Enable usage of dedicated GPUs in notebooks
// Note: this does cause the linker to produce a .lib and .exp file;
// see http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
#ifdef WIN32
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

static GLFWwindow* window = 0;
static bool hasFocus = true, running = true;
static GLTexture* renderTarget = 0;
static int scrwidth = 0, scrheight = 0;
static TheApp* app = 0;

// static member data for instruction set support class
static const CPUCaps cpucaps;

// find the app implementation
TheApp* CreateApp();

// GLFW callbacks
void InitRenderTarget( int w, int h )
{
	// allocate render target and surface
	scrwidth = w, scrheight = h;
	renderTarget = new GLTexture( scrwidth, scrheight, GLTexture::INTTARGET );
}
void ReshapeWindowCallback( GLFWwindow* window, int w, int h )
{
	glViewport( 0, 0, w, h );
}
void KeyEventCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if (key == GLFW_KEY_ESCAPE) running = false;
	if (action == GLFW_PRESS) { if (app) if (key >= 0) app->KeyDown( key ); }
	else if (action == GLFW_RELEASE) { if (app) if (key >= 0) app->KeyUp( key ); }
}
void CharEventCallback( GLFWwindow* window, uint code ) { /* nothing here yet */ }
void WindowFocusCallback( GLFWwindow* window, int focused ) { hasFocus = (focused == GL_TRUE); }
void MouseButtonCallback( GLFWwindow* window, int button, int action, int mods )
{
	if (action == GLFW_PRESS) { if (app) app->MouseDown( button ); }
	else if (action == GLFW_RELEASE) { if (app) app->MouseUp( button ); }
}
void MouseScrollCallback( GLFWwindow* window, double x, double y )
{
	app->MouseWheel( (float)y );
}
void MousePosCallback( GLFWwindow* window, double x, double y )
{
	if (app) app->MouseMove( (int)x, (int)y );
}
void ErrorCallback( int error, const char* description )
{
	fprintf( stderr, "GLFW Error: %s\n", description );
}

// Application entry point
void main()
{
	// open a window
	if (!glfwInit()) FatalError( "glfwInit failed." );
	glfwSetErrorCallback( ErrorCallback );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); // 3.3 is enough for our needs
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_STENCIL_BITS, GL_FALSE );
	glfwWindowHint( GLFW_RESIZABLE, GL_FALSE /* easier :) */ );
#ifdef FULLSCREEN
	window = glfwCreateWindow( SCRWIDTH, SCRHEIGHT, "Tmpl8-2022", glfwGetPrimaryMonitor(), 0 );
#else
	window = glfwCreateWindow( SCRWIDTH, SCRHEIGHT, "Tmpl8-2022", 0, 0 );
#endif
	if (!window) FatalError( "glfwCreateWindow failed." );
	glfwMakeContextCurrent( window );
	// register callbacks
	glfwSetWindowSizeCallback( window, ReshapeWindowCallback );
	glfwSetKeyCallback( window, KeyEventCallback );
	glfwSetWindowFocusCallback( window, WindowFocusCallback );
	glfwSetMouseButtonCallback( window, MouseButtonCallback );
	glfwSetScrollCallback( window, MouseScrollCallback );
	glfwSetCursorPosCallback( window, MousePosCallback );
	glfwSetCharCallback( window, CharEventCallback );
	// initialize GLAD
	if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress )) FatalError( "gladLoadGLLoader failed." );
	glfwSwapInterval( 0 );
	// prepare OpenGL state
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );
	glDisable( GL_BLEND );
	CheckGL();
	// we want a console window for text output
#ifndef FULLSCREEN
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	AllocConsole();
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &coninfo );
	coninfo.dwSize.X = 1280;
	coninfo.dwSize.Y = 800;
	SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coninfo.dwSize );
	FILE* file = nullptr;
	freopen_s( &file, "CON", "w", stdout );
	freopen_s( &file, "CON", "w", stderr );
	SetWindowPos( GetConsoleWindow(), HWND_TOP, 0, 0, 1280, 800, 0 );
	glfwShowWindow( window );
#endif
	// initialize application
	InitRenderTarget( SCRWIDTH, SCRHEIGHT );
	Surface* screen = new Surface( SCRWIDTH, SCRHEIGHT );
	app = CreateApp();
	app->screen = screen;
	app->Init();
	// done, enter main loop
#if 1
	// basic shader: apply gamma correction
	Shader* shader = new Shader(
		"#version 330\nin vec4 p;\nin vec2 t;out vec2 u;void main(){u=t;gl_Position=p;}",
		"#version 330\nuniform sampler2D c;in vec2 u;out vec4 f;void main(){f=/*sqrt*/(texture(c,u));}", true );
#else
	// fxaa shader
	Shader* shader = new Shader(
		"#version 330\nin vec4 p;\nin vec2 t;out vec2 uv;void main(){uv=t;gl_Position=p;}",
		// FXAA 3.11 Implementation - effendiian
		// https://www.shadertoy.com/view/ttXGzn
		"#version 330\nuniform sampler2D tex;\nin vec2 uv;out vec4 f; \n"							\
		"#define FXAA_LUMINANCE 			1					\n"									\
		"#define FXAA_EDGE_THRESHOLD	  	(1.0/8.0)			\n"									\
		"#define FXAA_EDGE_THRESHOLD_MIN  	(1.0/24.0)			\n"									\
		"#define FXAA_SEARCH_STEPS			32					\n"									\
		"#define FXAA_SEARCH_ACCELERATION 	1					\n"									\
		"#define FXAA_SEARCH_THRESHOLD		(1.0/4.0)			\n"									\
		"#define FXAA_SUBPIX				2 // 1 is crisper	\n"									\
		"#define FXAA_SUBPIX_CAP			(3.0/4.0)			\n"									\
		"#define FXAA_SUBPIX_TRIM			(1.0/4.0)			\n"									\
		"#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))	\n"							\
		"float lum( vec3 color ) {\n #if FXAA_LUMINANCE == 0 \n"									\
		"	return color.x * 0.2126729 + color.y * 0.7151522 + color.z * 0.0721750; \n"				\
		"#else \n return color.g * (0.587 / 0.299) + color.r; \n #endif \n }"						\
		"float vertEdge( float lumaO, float lumaN, float lumaE, float lumaS,"						\
		"	float lumaW, float lumaNW, float lumaNE, float lumaSW, float lumaSE )"					\
		"{	float top = (0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE);"						\
		"	float middle = (0.50 * lumaW) + (-1.0 * lumaO) + (0.50 * lumaE);"						\
		"	float bottom = (0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE);"						\
		"	return abs( top ) + abs( middle ) + abs( bottom ); }"									\
		"float horEdge( float lumaO, float lumaN, float lumaE, float lumaS,"						\
		"	float lumaW, float lumaNW, float lumaNE, float lumaSW, float lumaSE )"					\
		"{	float top = (0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW);"						\
		"	float middle = (0.50 * lumaN) + (-1.0 * lumaO) + (0.50 * lumaS);"						\
		"	float bottom = (0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE);"						\
		"	return abs( top ) + abs( middle ) + abs( bottom ); }"									\
		"vec3 fxaa( vec2 textureDimensions, vec2 uv )"												\
		"{	vec2 texel = vec2( 1.0, 1.0 ) / textureDimensions;"										\
		"	vec3 rgbN = texture( tex, uv + vec2( 0, -texel.y ) ).rgb,"								\
		"		 rgbW = texture( tex, uv + vec2( -texel.x, 0 ) ).rgb,"								\
		"		 rgbO = texture( tex, uv + vec2( 0, 0 ) ).rgb,"										\
		"		 rgbE = texture( tex, uv + vec2( texel.x, 0 ) ).rgb,"								\
		"		 rgbS = texture( tex, uv + vec2( 0, texel.y ) ).rgb;"								\
		"	float lumaN = lum( rgbN ), lumaW = lum( rgbW );"										\
		"	float lumaO = lum( rgbO ), lumaE = lum( rgbE ), lumaS = lum( rgbS );"					\
		"	float minLuma = min( lumaO, min( min( lumaN, lumaW ), min( lumaS, lumaE ) ) );"			\
		"	float maxLuma = max( lumaO, max( max( lumaN, lumaW ), max( lumaS, lumaE ) ) );"			\
		"	float localContrast = maxLuma - minLuma;"												\
		"	if (localContrast < max( FXAA_EDGE_THRESHOLD_MIN, maxLuma* FXAA_EDGE_THRESHOLD ))"		\
		"		return rgbO;"																		\
		"	vec3 rgbL = rgbN + rgbW + rgbO + rgbE + rgbS;"											\
		"	float lumaL = (lumaN + lumaW + lumaS + lumaE) * 0.25;"									\
		"	float pixelContrast = abs( lumaL - lumaO );"											\
		"	float contrastRatio = pixelContrast / localContrast;"									\
		"	float lowpassBlend = 0;			\n"														\
		"#if FXAA_SUBPIX == 1				\n"														\
		"	lowpassBlend = max( 0.0, contrastRatio - FXAA_SUBPIX_TRIM ) * FXAA_SUBPIX_TRIM_SCALE;"	\
		"	lowpassBlend = min( FXAA_SUBPIX_CAP, lowpassBlend );	\n"								\
		"#elif FXAA_SUBPIX == 2				\n"														\
		"	lowpassBlend = contrastRatio;	\n"														\
		"#endif								\n"														\
		"	vec3 rgbNW = texture( tex, uv + vec2( -texel.x, -texel.y ) ).rgb,"						\
		"		 rgbNE = texture( tex, uv + vec2( texel.x, -texel.y ) ).rgb,"						\
		"		 rgbSW = texture( tex, uv + vec2( -texel.x, texel.y ) ).rgb,"						\
		"		 rgbSE = texture( tex, uv + vec2( texel.x, texel.y ) ).rgb;"						\
		"	rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);"												\
		"	rgbL *= (1.0 / 9.0);"																	\
		"	float lumaNW = lum( rgbNW ), lumaNE = lum( rgbNE );"									\
		"	float lumaSW = lum( rgbSW ), lumaSE = lum( rgbSE );"									\
		"	float edgeVert = vertEdge( lumaO, lumaN, lumaE, lumaS, lumaW, lumaNW, lumaNE, lumaSW, lumaSE );" \
		"	float edgeHori = horEdge( lumaO, lumaN, lumaE, lumaS, lumaW, lumaNW, lumaNE, lumaSW, lumaSE );" \
		"	bool isHorizontal = edgeHori >= edgeVert;"												\
		"	float edgeSign = isHorizontal ? -texel.y : -texel.x;"									\
		"	float gradNeg = isHorizontal ? abs( lumaN - lumaO ) : abs( lumaW - lumaO );"			\
		"	float gradPos = isHorizontal ? abs( lumaS - lumaO ) : abs( lumaE - lumaO );"			\
		"	float lumaNeg = isHorizontal ? ((lumaN + lumaO) * 0.5) : ((lumaW + lumaO) * 0.5);"		\
		"	float lumaPos = isHorizontal ? ((lumaS + lumaO) * 0.5) : ((lumaE + lumaO) * 0.5);"		\
		"	bool isNegative = (gradNeg >= gradPos);"												\
		"	float gradientHighest = isNegative ? gradNeg : gradPos;"								\
		"	float lumaHighest = isNegative ? lumaNeg : lumaPos;"									\
		"	if (isNegative) edgeSign *= -1.0;"														\
		"	vec2 pointN = vec2( 0.0, 0.0 );"														\
		"	pointN.x = uv.x + (isHorizontal ? 0.0 : edgeSign * 0.5);"								\
		"	pointN.y = uv.y + (isHorizontal ? edgeSign * 0.5 : 0.0);"								\
		"	gradientHighest *= FXAA_SEARCH_THRESHOLD;"												\
		"	vec2 pointP = pointN;"																	\
		"	vec2 offset = isHorizontal ? vec2( texel.x, 0.0 ) : vec2( 0.0, texel.y );"				\
		"	float lumaNegEnd = lumaNeg, lumaPosEnd = lumaPos;"										\
		"	bool searchNeg = false, searchPos = false;\n"											\
		"#if FXAA_SEARCH_ACCELERATION == 1\n"														\
		"	pointN -= offset, pointP += offset;\n"													\
		"#elif FXAA_SEARCH_ACCELERATION == 2\n"														\
		"	pointN -= offset * 1.5, pointP += offset * 1.5, offset *= 2;\n"							\
		"#elif FXAA_SEARCH_ACCELERATION == 3\n"														\
		"	pointN -= offset * 2, pointP += offset * 2, offset *= 3;\n"								\
		"#elif FXAA_SEARCH_ACCELERATION == 4\n"														\
		"	pointN -= offset * 2.5, pointP += offset * 2.5, offset *= 4;\n"							\
		"#endif\n"																					\
		"	for (int i = 0; i < FXAA_SEARCH_STEPS; i++) {\n"										\
		"	#if FXAA_SEARCH_ACCELERATION == 1\n"													\
		"		if (!searchNeg) lumaNegEnd = lum( texture( tex, pointN ).rgb );"					\
		"		if (!searchPos) lumaPosEnd = lum( texture( tex, pointP ).rgb );\n"					\
		"	#else \n"																				\
		"		if (!searchNeg) lumaNegEnd = lum( textureGrad( tex, pointN, offset, offset ).rgb );"	\
		"		if (!searchPos) lumaPosEnd = lum( textureGrad( tex, pointP, offset, offset ).rgb );\n"	\
		"	#endif \n"																				\
		"		searchNeg = searchNeg || (abs( lumaNegEnd - lumaHighest ) >= gradientHighest);"		\
		"		searchPos = searchPos || (abs( lumaPosEnd - lumaPos ) >= gradPos);"					\
		"		if (searchNeg && searchPos) break;"													\
		"		if (!searchNeg) pointN -= offset;"													\
		"		if (!searchPos) pointP += offset; }"												\
		"	float distanceNeg = isHorizontal ? uv.x - pointN.x : uv.y - pointN.y;"					\
		"	float distancePos = isHorizontal ? pointP.x - uv.x : pointP.y - uv.y;"					\
		"	bool isCloserToNegative = distanceNeg < distancePos;"									\
		"	float lumaEnd = isCloserToNegative ? lumaNegEnd : lumaPosEnd;"							\
		"	if (((lumaO - lumaNeg) < 0.0) == ((lumaEnd - lumaNeg) < 0.0)) edgeSign = 0.0;"			\
		"	float spanLen = distancePos + distanceNeg;"												\
		"	float dist = isCloserToNegative ? distanceNeg : distancePos;"							\
		"	float subOffs = (0.5 + (dist * (-1.0 / spanLen))) * edgeSign;"							\
		"	vec3 rgbOffset = textureLod( tex, vec2( uv.x + (isHorizontal ? 0.0 :"					\
		"		subOffs), uv.y + (isHorizontal ? subOffs : 0.0) ), 0.0 ).rgb;"						\
		"	return mix( rgbOffset, rgbL, lowpassBlend ); }"											\
		"void main(){f=vec4(sqrt(fxaa(vec2(1240,800),uv)),1);}", true );
#endif
	float deltaTime = 0;
	static int frameNr = 0;
	static Timer timer;
	while (!glfwWindowShouldClose( window ))
	{
		deltaTime = min( 500.0f, 1000.0f * timer.elapsed() );
		timer.reset();
		app->Tick( deltaTime );
		// send the rendering result to the screen using OpenGL
		if (frameNr++ > 1)
		{
			renderTarget->CopyFrom( app->screen );
			shader->Bind();
			shader->SetInputTexture( 0, "c", renderTarget );
			DrawQuad();
			shader->Unbind();
			glfwSwapBuffers( window );
			glfwPollEvents();
		}
		if (!running) break;
	}
	// close down
	app->Shutdown();
	Kernel::KillCL();
	glfwDestroyWindow( window );
	glfwTerminate();
}

// Jobmanager implementation
DWORD JobThreadProc( LPVOID lpParameter )
{
	JobThread* JobThreadInstance = (JobThread*)lpParameter;
	JobThreadInstance->BackgroundTask();
	return 0;
}

void JobThread::CreateAndStartThread( unsigned int threadId )
{
	m_GoSignal = CreateEvent( 0, FALSE, FALSE, 0 );
	m_ThreadHandle = CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)&JobThreadProc, (LPVOID)this, 0, 0 );
	m_ThreadID = threadId;
}
void JobThread::BackgroundTask()
{
	while (1)
	{
		WaitForSingleObject( m_GoSignal, INFINITE );
		while (1)
		{
			Job* job = JobManager::GetJobManager()->GetNextJob();
			if (!job)
			{
				JobManager::GetJobManager()->ThreadDone( m_ThreadID );
				break;
			}
			job->RunCodeWrapper();
		}
	}
}

void JobThread::Go()
{
	SetEvent( m_GoSignal );
}

void Job::RunCodeWrapper()
{
	Main();
}

JobManager* JobManager::m_JobManager = 0;

JobManager::JobManager( unsigned int threads ) : m_NumThreads( threads )
{
	InitializeCriticalSection( &m_CS );
}

JobManager::~JobManager()
{
	DeleteCriticalSection( &m_CS );
}

void JobManager::CreateJobManager( unsigned int numThreads )
{
	m_JobManager = new JobManager( numThreads );
	m_JobManager->m_JobThreadList = new JobThread[numThreads];
	for (unsigned int i = 0; i < numThreads; i++)
	{
		m_JobManager->m_JobThreadList[i].CreateAndStartThread( i );
		m_JobManager->m_ThreadDone[i] = CreateEvent( 0, FALSE, FALSE, 0 );
	}
	m_JobManager->m_JobCount = 0;
}

void JobManager::AddJob2( Job* a_Job )
{
	m_JobList[m_JobCount++] = a_Job;
}

Job* JobManager::GetNextJob()
{
	Job* job = 0;
	EnterCriticalSection( &m_CS );
	if (m_JobCount > 0) job = m_JobList[--m_JobCount];
	LeaveCriticalSection( &m_CS );
	return job;
}

void JobManager::RunJobs()
{
	for (unsigned int i = 0; i < m_NumThreads; i++) m_JobThreadList[i].Go();
	WaitForMultipleObjects( m_NumThreads, m_ThreadDone, TRUE, INFINITE );
}

void JobManager::ThreadDone( unsigned int n )
{
	SetEvent( m_ThreadDone[n] );
}

DWORD CountSetBits( ULONG_PTR bitMask )
{
	DWORD LSHIFT = sizeof( ULONG_PTR ) * 8 - 1, bitSetCount = 0;
	ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
	for (DWORD i = 0; i <= LSHIFT; ++i) bitSetCount += ((bitMask & bitTest) ? 1 : 0), bitTest /= 2;
	return bitSetCount;
}

void JobManager::GetProcessorCount( uint& cores, uint& logical )
{
	// https://github.com/GPUOpen-LibrariesAndSDKs/cpu-core-counts
	cores = logical = 0;
	char* buffer = NULL;
	DWORD len = 0;
	if (FALSE == GetLogicalProcessorInformationEx( RelationAll, (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)buffer, &len ))
	{
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			buffer = (char*)malloc( len );
			if (GetLogicalProcessorInformationEx( RelationAll, (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)buffer, &len ))
			{
				DWORD offset = 0;
				char* ptr = buffer;
				while (ptr < buffer + len)
				{
					PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX pi = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX)ptr;
					if (pi->Relationship == RelationProcessorCore)
					{
						cores++;
						for (size_t g = 0; g < pi->Processor.GroupCount; ++g)
							logical += CountSetBits( pi->Processor.GroupMask[g].Mask );
					}
					ptr += pi->Size;
				}
			}
			free( buffer );
		}
	}
}

JobManager* JobManager::GetJobManager()
{
	if (!m_JobManager)
	{
		uint c, l;
		GetProcessorCount( c, l );
		CreateJobManager( l );
	}
	return m_JobManager;
}

// OpenGL helper functions
void _CheckGL( const char* f, int l )
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		const char* errStr = "UNKNOWN ERROR";
		if (error == 0x500) errStr = "INVALID ENUM";
		else if (error == 0x502) errStr = "INVALID OPERATION";
		else if (error == 0x501) errStr = "INVALID VALUE";
		else if (error == 0x506) errStr = "INVALID FRAMEBUFFER OPERATION";
		FatalError( "GL error %d: %s at %s:%d\n", error, errStr, f, l );
	}
}

GLuint CreateVBO( const GLfloat* data, const uint size )
{
	GLuint id;
	glGenBuffers( 1, &id );
	glBindBuffer( GL_ARRAY_BUFFER, id );
	glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
	CheckGL();
	return id;
}

void BindVBO( const uint idx, const uint N, const GLuint id )
{
	glEnableVertexAttribArray( idx );
	glBindBuffer( GL_ARRAY_BUFFER, id );
	glVertexAttribPointer( idx, N, GL_FLOAT, GL_FALSE, 0, (void*)0 );
	CheckGL();
}

void CheckShader( GLuint shader, const char* vshader, const char* fshader )
{
	char buffer[1024];
	memset( buffer, 0, sizeof( buffer ) );
	GLsizei length = 0;
	glGetShaderInfoLog( shader, sizeof( buffer ), &length, buffer );
	CheckGL();
	FATALERROR_IF( length > 0 && strstr( buffer, "ERROR" ), "Shader compile error:\n%s", buffer );
}

void CheckProgram( GLuint id, const char* vshader, const char* fshader )
{
	char buffer[1024];
	memset( buffer, 0, sizeof( buffer ) );
	GLsizei length = 0;
	glGetProgramInfoLog( id, sizeof( buffer ), &length, buffer );
	CheckGL();
	FATALERROR_IF( length > 0, "Shader link error:\n%s", buffer );
}

void DrawQuad()
{
	static GLuint vao = 0;
	if (!vao)
	{
		// generate buffers
		static const GLfloat verts[] = { -1, 1, 0, 1, 1, 0, -1, -1, 0, 1, 1, 0, -1, -1, 0, 1, -1, 0 };
		static const GLfloat uvdata[] = { 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1 };
		GLuint vertexBuffer = CreateVBO( verts, sizeof( verts ) );
		GLuint UVBuffer = CreateVBO( uvdata, sizeof( uvdata ) );
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
		BindVBO( 0, 3, vertexBuffer );
		BindVBO( 1, 2, UVBuffer );
		glBindVertexArray( 0 );
		CheckGL();
	}
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	glBindVertexArray( 0 );
}

// OpenGL texture wrapper class
GLTexture::GLTexture( uint w, uint h, uint type )
{
	width = w;
	height = h;
	glGenTextures( 1, &ID );
	glBindTexture( GL_TEXTURE_2D, ID );
	if (type == DEFAULT)
	{
		// regular texture
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, 0 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}
	else if (type == INTTARGET)
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	}
	else /* type == FLOAT */
	{
		// floating point texture
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0 );
	}
	glBindTexture( GL_TEXTURE_2D, 0 );
	CheckGL();
}

GLTexture::~GLTexture()
{
	glDeleteTextures( 1, &ID );
	CheckGL();
}

void GLTexture::Bind( const uint slot )
{
	glActiveTexture( GL_TEXTURE0 + slot );
	glBindTexture( GL_TEXTURE_2D, ID );
	CheckGL();
}

void GLTexture::CopyFrom( Surface* src )
{
	glBindTexture( GL_TEXTURE_2D, ID );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, src->pixels );
	CheckGL();
}

void GLTexture::CopyTo( Surface* dst )
{
	glBindTexture( GL_TEXTURE_2D, ID );
	glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst->pixels );
	CheckGL();
}

// Shader class implementation
Shader::Shader( const char* vfile, const char* pfile, bool fromString )
{
	if (fromString)
	{
		Compile( vfile, pfile );
	}
	else
	{
		Init( vfile, pfile );
	}
}

Shader::~Shader()
{
	glDetachShader( ID, pixel );
	glDetachShader( ID, vertex );
	glDeleteShader( pixel );
	glDeleteShader( vertex );
	glDeleteProgram( ID );
	CheckGL();
}

void Shader::Init( const char* vfile, const char* pfile )
{
	string vsText = TextFileRead( vfile );
	string fsText = TextFileRead( pfile );
	FATALERROR_IF( vsText.size() == 0, "File %s not found", vfile );
	FATALERROR_IF( fsText.size() == 0, "File %s not found", pfile );
	const char* vertexText = vsText.c_str();
	const char* fragmentText = fsText.c_str();
	Compile( vertexText, fragmentText );
}

void Shader::Compile( const char* vtext, const char* ftext )
{
	vertex = glCreateShader( GL_VERTEX_SHADER );
	pixel = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( vertex, 1, &vtext, 0 );
	glCompileShader( vertex );
	CheckShader( vertex, vtext, ftext );
	glShaderSource( pixel, 1, &ftext, 0 );
	glCompileShader( pixel );
	CheckShader( pixel, vtext, ftext );
	ID = glCreateProgram();
	glAttachShader( ID, vertex );
	glAttachShader( ID, pixel );
	glBindAttribLocation( ID, 0, "pos" );
	glBindAttribLocation( ID, 1, "tuv" );
	glLinkProgram( ID );
	CheckProgram( ID, vtext, ftext );
	CheckGL();
}

void Shader::Bind()
{
	glUseProgram( ID );
	CheckGL();
}

void Shader::Unbind()
{
	glUseProgram( 0 );
	CheckGL();
}

void Shader::SetInputTexture( uint slot, const char* name, GLTexture* texture )
{
	glActiveTexture( GL_TEXTURE0 + slot );
	glBindTexture( GL_TEXTURE_2D, texture->ID );
	glUniform1i( glGetUniformLocation( ID, name ), slot );
	CheckGL();
}

void Shader::SetInputMatrix( const char* name, const mat4& matrix )
{
	const GLfloat* data = (const GLfloat*)&matrix;
	glUniformMatrix4fv( glGetUniformLocation( ID, name ), 1, GL_FALSE, data );
	CheckGL();
}

void Shader::SetFloat( const char* name, const float v )
{
	glUniform1f( glGetUniformLocation( ID, name ), v );
	CheckGL();
}

void Shader::SetInt( const char* name, const int v )
{
	glUniform1i( glGetUniformLocation( ID, name ), v );
	CheckGL();
}

void Shader::SetUInt( const char* name, const uint v )
{
	glUniform1ui( glGetUniformLocation( ID, name ), v );
	CheckGL();
}

// RNG - Marsaglia's xor32
static uint seed = 0x12345678;
uint RandomUInt()
{
	seed ^= seed << 13;
	seed ^= seed >> 17;
	seed ^= seed << 5;
	return seed;
}
float RandomFloat() { return RandomUInt() * 2.3283064365387e-10f; }
float Rand( float range ) { return RandomFloat() * range; }
// local seed
uint RandomUInt( uint& seed )
{
	seed ^= seed << 13;
	seed ^= seed >> 17;
	seed ^= seed << 5;
	return seed;
}
float RandomFloat( uint& seed ) { return RandomUInt( seed ) * 2.3283064365387e-10f; }

// Perlin noise implementation - https://stackoverflow.com/questions/29711668/perlin-noise-generation
static int numX = 512, numY = 512, numOctaves = 7, primeIndex = 0;
static float persistence = 0.5f;
static int primes[10][3] = {
	{ 995615039, 600173719, 701464987 }, { 831731269, 162318869, 136250887 }, { 174329291, 946737083, 245679977 },
	{ 362489573, 795918041, 350777237 }, { 457025711, 880830799, 909678923 }, { 787070341, 177340217, 593320781 },
	{ 405493717, 291031019, 391950901 }, { 458904767, 676625681, 424452397 }, { 531736441, 939683957, 810651871 },
	{ 997169939, 842027887, 423882827 }
};
static float Noise( const int i, const int x, const int y )
{
	int n = x + y * 57;
	n = (n << 13) ^ n;
	const int a = primes[i][0], b = primes[i][1], c = primes[i][2];
	const int t = (n * (n * n * a + b) + c) & 0x7fffffff;
	return 1.0f - (float)t / 1073741824.0f;
}
static float SmoothedNoise( const int i, const int x, const int y )
{
	const float corners = (Noise( i, x - 1, y - 1 ) + Noise( i, x + 1, y - 1 ) + Noise( i, x - 1, y + 1 ) + Noise( i, x + 1, y + 1 )) / 16;
	const float sides = (Noise( i, x - 1, y ) + Noise( i, x + 1, y ) + Noise( i, x, y - 1 ) + Noise( i, x, y + 1 )) / 8;
	const float center = Noise( i, x, y ) / 4;
	return corners + sides + center;
}
static float Interpolate( const float a, const float b, const float x )
{
	const float ft = x * 3.1415927f, f = (1 - cosf( ft )) * 0.5f;
	return a * (1 - f) + b * f;
}
static float InterpolatedNoise( const int i, const float x, const float y )
{
	const int integer_X = (int)x, integer_Y = (int)y;
	const float fractional_X = x - integer_X, fractional_Y = y - integer_Y;
	const float v1 = SmoothedNoise( i, integer_X, integer_Y );
	const float v2 = SmoothedNoise( i, integer_X + 1, integer_Y );
	const float v3 = SmoothedNoise( i, integer_X, integer_Y + 1 );
	const float v4 = SmoothedNoise( i, integer_X + 1, integer_Y + 1 );
	const float i1 = Interpolate( v1, v2, fractional_X );
	const float i2 = Interpolate( v3, v4, fractional_X );
	return Interpolate( i1, i2, fractional_Y );
}
float noise2D( const float x, const float y )
{
	float total = 0, frequency = (float)(2 << numOctaves), amplitude = 1;
	for (int i = 0; i < numOctaves; ++i)
	{
		frequency /= 2, amplitude *= persistence;
		total += InterpolatedNoise( (primeIndex + i) % 10, x / frequency, y / frequency ) * amplitude;
	}
	return total / frequency;
}

// math implementations
mat4 operator*( const mat4& a, const mat4& b )
{
	mat4 r;
	for (uint i = 0; i < 16; i += 4)
		for (uint j = 0; j < 4; ++j)
		{
			r[i + j] =
				(a.cell[i + 0] * b.cell[j + 0]) +
				(a.cell[i + 1] * b.cell[j + 4]) +
				(a.cell[i + 2] * b.cell[j + 8]) +
				(a.cell[i + 3] * b.cell[j + 12]);
		}
	return r;
}
mat4 operator*( const mat4& a, const float s )
{
	mat4 r;
	for (uint i = 0; i < 16; i += 4) r.cell[i] = a.cell[i] * s;
	return r;
}
mat4 operator*( const float s, const mat4& a )
{
	mat4 r;
	for (uint i = 0; i < 16; i++) r.cell[i] = a.cell[i] * s;
	return r;
}
mat4 operator+( const mat4& a, const mat4& b )
{
	mat4 r;
	for (uint i = 0; i < 16; i += 4) r.cell[i] = a.cell[i] + b.cell[i];
	return r;
}
bool operator==( const mat4& a, const mat4& b )
{
	for (uint i = 0; i < 16; i++)
		if (a.cell[i] != b.cell[i]) return false;
	return true;
}
bool operator!=( const mat4& a, const mat4& b ) { return !(a == b); }
float4 operator*( const mat4& a, const float4& b )
{
	return make_float4( a.cell[0] * b.x + a.cell[1] * b.y + a.cell[2] * b.z + a.cell[3] * b.w,
		a.cell[4] * b.x + a.cell[5] * b.y + a.cell[6] * b.z + a.cell[7] * b.w,
		a.cell[8] * b.x + a.cell[9] * b.y + a.cell[10] * b.z + a.cell[11] * b.w,
		a.cell[12] * b.x + a.cell[13] * b.y + a.cell[14] * b.z + a.cell[15] * b.w );
}
float4 operator*( const float4& b, const mat4& a )
{
	return make_float4( a.cell[0] * b.x + a.cell[1] * b.y + a.cell[2] * b.z + a.cell[3] * b.w,
		a.cell[4] * b.x + a.cell[5] * b.y + a.cell[6] * b.z + a.cell[7] * b.w,
		a.cell[8] * b.x + a.cell[9] * b.y + a.cell[10] * b.z + a.cell[11] * b.w,
		a.cell[12] * b.x + a.cell[13] * b.y + a.cell[14] * b.z + a.cell[15] * b.w );
}
float3 TransformPosition( const float3& a, const mat4& M )
{
	return make_float3( make_float4( a, 1 ) * M );
}
float3 TransformVector( const float3& a, const mat4& M )
{
	return make_float3( make_float4( a, 0 ) * M );
}

// Helper functions
bool FileIsNewer( const char* file1, const char* file2 )
{
	struct stat f1;
	struct stat f2;

	auto ret = stat( file1, &f1 );
	FATALERROR_IF( ret, "File %s not found!", file1 );

	if (stat( file2, &f2 )) return true; // second file does not exist

#ifdef _MSC_VER
	return f1.st_mtime >= f2.st_mtime;
#else
	if (f1.st_mtim.tv_sec >= f2.st_mtim.tv_sec)
		return true;
	return f1.st_mtim.tv_nsec >= f2.st_mtim.tv_nsec;
#endif
}

bool FileExists( const char* f )
{
	ifstream s( f );
	return s.good();
}

bool RemoveFile( const char* f )
{
	if (!FileExists( f )) return false;
	return !remove( f );
}

uint FileSize( string filename )
{
	ifstream s( filename );
	return s.good();
}

string TextFileRead( const char* _File )
{
	ifstream s( _File );
	string str( (istreambuf_iterator<char>( s )), istreambuf_iterator<char>() );
	s.close();
	return str;
}

int LineCount( const string s )
{
	const char* p = s.c_str();
	int lines = 0;
	while (*p) if (*p++ == '\n') lines++;
	return lines;
}

void TextFileWrite( const string& text, const char* _File )
{
	ofstream s( _File, ios::binary );
	int len = (int)text.size();
	s.write( (const char*)&len, sizeof( len ) );
	s.write( text.c_str(), len );
}

void FatalError( const char* fmt, ... )
{
	char t[16384];
	va_list args;
	va_start( args, fmt );
	vsnprintf( t, sizeof( t ), fmt, args );
	va_end( args );
#ifdef _MSC_VER
	MessageBox( NULL, t, "Fatal error", MB_OK );
#else
	fprintf( stderr, t );
#endif
	while (1) exit( 0 );
}

// source file information
static int sourceFiles = 0;
static char* sourceFile[64]; // yup, ugly constant

// default worksize
static size_t workSize[] = { SCRWIDTH, SCRHEIGHT };
static size_t localSize[] = { 32, 4 };

using namespace std;

#define CHECKCL(r) CheckCL( r, __FILE__, __LINE__ )

// CHECKCL method
// OpenCL error handling.
// ----------------------------------------------------------------------------
bool CheckCL( cl_int result, const char* file, int line )
{
	if (result == CL_SUCCESS) return true;
	if (result == CL_DEVICE_NOT_FOUND) FatalError( "Error: CL_DEVICE_NOT_FOUND\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_DEVICE_NOT_AVAILABLE) FatalError( "Error: CL_DEVICE_NOT_AVAILABLE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_COMPILER_NOT_AVAILABLE) FatalError( "Error: CL_COMPILER_NOT_AVAILABLE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_MEM_OBJECT_ALLOCATION_FAILURE) FatalError( "Error: CL_MEM_OBJECT_ALLOCATION_FAILURE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_OUT_OF_RESOURCES) FatalError( "Error: CL_OUT_OF_RESOURCES\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_OUT_OF_HOST_MEMORY) FatalError( "Error: CL_OUT_OF_HOST_MEMORY\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_PROFILING_INFO_NOT_AVAILABLE) FatalError( "Error: CL_PROFILING_INFO_NOT_AVAILABLE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_MEM_COPY_OVERLAP) FatalError( "Error: CL_MEM_COPY_OVERLAP\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_IMAGE_FORMAT_MISMATCH) FatalError( "Error: CL_IMAGE_FORMAT_MISMATCH\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_IMAGE_FORMAT_NOT_SUPPORTED) FatalError( "Error: CL_IMAGE_FORMAT_NOT_SUPPORTED\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_BUILD_PROGRAM_FAILURE) FatalError( "Error: CL_BUILD_PROGRAM_FAILURE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_MAP_FAILURE) FatalError( "Error: CL_MAP_FAILURE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_MISALIGNED_SUB_BUFFER_OFFSET) FatalError( "Error: CL_MISALIGNED_SUB_BUFFER_OFFSET\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST) FatalError( "Error: CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_VALUE) FatalError( "Error: CL_INVALID_VALUE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_DEVICE_TYPE) FatalError( "Error: CL_INVALID_DEVICE_TYPE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_PLATFORM) FatalError( "Error: CL_INVALID_PLATFORM\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_DEVICE) FatalError( "Error: CL_INVALID_DEVICE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_CONTEXT) FatalError( "Error: CL_INVALID_CONTEXT\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_QUEUE_PROPERTIES) FatalError( "Error: CL_INVALID_QUEUE_PROPERTIES\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_COMMAND_QUEUE) FatalError( "Error: CL_INVALID_COMMAND_QUEUE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_HOST_PTR) FatalError( "Error: CL_INVALID_HOST_PTR\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_MEM_OBJECT) FatalError( "Error: CL_INVALID_MEM_OBJECT\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_IMAGE_FORMAT_DESCRIPTOR) FatalError( "Error: CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_IMAGE_SIZE) FatalError( "Error: CL_INVALID_IMAGE_SIZE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_SAMPLER) FatalError( "Error: CL_INVALID_SAMPLER\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_BINARY) FatalError( "Error: CL_INVALID_BINARY\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_BUILD_OPTIONS) FatalError( "Error: CL_INVALID_BUILD_OPTIONS\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_PROGRAM) FatalError( "Error: CL_INVALID_PROGRAM\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_PROGRAM_EXECUTABLE) FatalError( "Error: CL_INVALID_PROGRAM_EXECUTABLE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_KERNEL_NAME) FatalError( "Error: CL_INVALID_KERNEL_NAME\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_KERNEL_DEFINITION) FatalError( "Error: CL_INVALID_KERNEL_DEFINITION\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_KERNEL) FatalError( "Error: CL_INVALID_KERNEL\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_ARG_INDEX) FatalError( "Error: CL_INVALID_ARG_INDEX\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_ARG_VALUE) FatalError( "Error: CL_INVALID_ARG_VALUE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_ARG_SIZE) FatalError( "Error: CL_INVALID_ARG_SIZE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_KERNEL_ARGS) FatalError( "Error: CL_INVALID_KERNEL_ARGS\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_WORK_DIMENSION) FatalError( "Error: CL_INVALID_WORK_DIMENSION\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_WORK_GROUP_SIZE) FatalError( "Error: CL_INVALID_WORK_GROUP_SIZE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_WORK_ITEM_SIZE) FatalError( "Error: CL_INVALID_WORK_ITEM_SIZE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_GLOBAL_OFFSET) FatalError( "Error: CL_INVALID_GLOBAL_OFFSET\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_EVENT_WAIT_LIST) FatalError( "Error: CL_INVALID_EVENT_WAIT_LIST\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_EVENT) FatalError( "Error: CL_INVALID_EVENT\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_OPERATION) FatalError( "Error: CL_INVALID_OPERATION\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_GL_OBJECT) FatalError( "Error: CL_INVALID_GL_OBJECT\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_BUFFER_SIZE) FatalError( "Error: CL_INVALID_BUFFER_SIZE\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_MIP_LEVEL) FatalError( "Error: CL_INVALID_MIP_LEVEL\n%s, line %i", file, line, "OpenCL error" );
	if (result == CL_INVALID_GLOBAL_WORK_SIZE) FatalError( "Error: CL_INVALID_GLOBAL_WORK_SIZE\n%s, line %i", file, line, "OpenCL error" );
	return false;
}

// getFirstDevice
// ----------------------------------------------------------------------------
static cl_device_id getFirstDevice( cl_context context )
{
	size_t dataSize;
	cl_device_id* devices;
	clGetContextInfo( context, CL_CONTEXT_DEVICES, 0, NULL, &dataSize );
	devices = (cl_device_id*)malloc( dataSize );
	clGetContextInfo( context, CL_CONTEXT_DEVICES, dataSize, devices, NULL );
	cl_device_id first = devices[0];
	free( devices );
	return first;
}

// getPlatformID
// ----------------------------------------------------------------------------
static cl_int getPlatformID( cl_platform_id* platform )
{
	char chBuffer[1024];
	cl_uint num_platforms, devCount;
	cl_platform_id* clPlatformIDs;
	cl_int error;
	*platform = NULL;
	CHECKCL( error = clGetPlatformIDs( 0, NULL, &num_platforms ) );
	if (num_platforms == 0) CHECKCL( -1 );
	clPlatformIDs = (cl_platform_id*)malloc( num_platforms * sizeof( cl_platform_id ) );
	error = clGetPlatformIDs( num_platforms, clPlatformIDs, NULL );
	cl_uint deviceType[2] = { CL_DEVICE_TYPE_GPU, CL_DEVICE_TYPE_CPU };
	char* deviceOrder[2][3] = { { "NVIDIA", "AMD", "" }, { "", "", "" } };
	printf( "available OpenCL platforms:\n" );
	for (cl_uint i = 0; i < num_platforms; ++i)
	{
		CHECKCL( error = clGetPlatformInfo( clPlatformIDs[i], CL_PLATFORM_NAME, 1024, &chBuffer, NULL ) );
		printf( "#%i: %s\n", i, chBuffer );
	}
	for (cl_uint j = 0; j < 2; j++) for (int k = 0; k < 3; k++) for (cl_uint i = 0; i < num_platforms; ++i)
	{
		error = clGetDeviceIDs( clPlatformIDs[i], deviceType[j], 0, NULL, &devCount );
		if ((error != CL_SUCCESS) || (devCount == 0)) continue;
		CHECKCL( error = clGetPlatformInfo( clPlatformIDs[i], CL_PLATFORM_NAME, 1024, &chBuffer, NULL ) );
		if (deviceOrder[j][k][0]) if (!strstr( chBuffer, deviceOrder[j][k] )) continue;
		printf( "OpenCL device: %s\n", chBuffer );
		*platform = clPlatformIDs[i], j = 2, k = 3;
		break;
	}
	free( clPlatformIDs );
	return CL_SUCCESS;
}

// Buffer constructor
// ----------------------------------------------------------------------------
Buffer::Buffer( unsigned int N, unsigned int t, void* ptr )
{
	type = t;
	ownData = false;
	int rwFlags = CL_MEM_READ_WRITE;
	if (t & READONLY) rwFlags = CL_MEM_READ_ONLY;
	if (t & WRITEONLY) rwFlags = CL_MEM_WRITE_ONLY;
	if ((t & (TEXTURE | TARGET)) == 0)
	{
		size = N;
		textureID = 0; // not representing a texture
		deviceBuffer = clCreateBuffer( Kernel::GetContext(), rwFlags, size * 4, 0, 0 );
		hostBuffer = (uint*)ptr;
	}
	else
	{
		textureID = N; // representing texture N
		if (!Kernel::candoInterop) FatalError( "didn't expect to get here." );
		int error = 0;
		if (t == TARGET) deviceBuffer = clCreateFromGLTexture( Kernel::GetContext(), CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, N, &error );
		else deviceBuffer = clCreateFromGLTexture( Kernel::GetContext(), CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, N, &error );
		CHECKCL( error );
		hostBuffer = 0;
	}
}

// Buffer destructor
// ----------------------------------------------------------------------------
Buffer::~Buffer()
{
	if (ownData)
	{
		delete hostBuffer;
		hostBuffer = 0;
	}
	if ((type & (TEXTURE | TARGET)) == 0) clReleaseMemObject( deviceBuffer );
}

// CopyToDevice method
// ----------------------------------------------------------------------------
void Buffer::CopyToDevice( bool blocking )
{
	cl_int error;
	CHECKCL( error = clEnqueueWriteBuffer( Kernel::GetQueue(), deviceBuffer, blocking, 0, size * 4, hostBuffer, 0, 0, 0 ) );
}

// CopyToDevice2 method (uses 2nd queue)
// ----------------------------------------------------------------------------
void Buffer::CopyToDevice2( bool blocking, cl_event* eventToSet, const size_t s )
{
	cl_int error;
	CHECKCL( error = clEnqueueWriteBuffer( Kernel::GetQueue2(), deviceBuffer, blocking ? CL_TRUE : CL_FALSE, 0, s == 0 ? (size * 4) : (s * 4), hostBuffer, 0, 0, eventToSet ) );
}

// CopyFromDevice method
// ----------------------------------------------------------------------------
void Buffer::CopyFromDevice( bool blocking )
{
	cl_int error;
	if (!hostBuffer) hostBuffer = new uint[size], ownData = true;
	CHECKCL( error = clEnqueueReadBuffer( Kernel::GetQueue(), deviceBuffer, blocking, 0, size * 4, hostBuffer, 0, 0, 0 ) );
}

// CopyTo
// ----------------------------------------------------------------------------
void Buffer::CopyTo( Buffer* buffer )
{
	clEnqueueCopyBuffer( Kernel::GetQueue(), deviceBuffer, buffer->deviceBuffer, 0, 0, size * 4, 0, 0, 0 );
}

// Clear
// ----------------------------------------------------------------------------
void Buffer::Clear()
{
	uint value = 0;
#if 0
	memset( hostBuffer, 0, size * 4 );
	CopyToDevice();
#else
	cl_int error;
	CHECKCL( error = clEnqueueFillBuffer( Kernel::GetQueue(), deviceBuffer, &value, 4, 0, size * 4, 0, 0, 0 ) );
#endif
}

// Kernel constructor
// ----------------------------------------------------------------------------
Kernel::Kernel( char* file, char* entryPoint )
{
	CheckCLStarted();
	// load a cl file
	string csText = TextFileRead( file );
	if (csText.size() == 0) FatalError( "File %s not found", file );
	// add vendor defines
	vendorLines = 0;
	if (isNVidia) csText = "#define ISNVIDIA\n" + csText, vendorLines++;
	if (isAMD) csText = "#define ISAMD\n" + csText, vendorLines++;
	if (isIntel) csText = "#define ISINTEL\n" + csText, vendorLines++;
	if (isOther) csText = "#define ISOTHER\n" + csText, vendorLines++;
	if (isAmpere) csText = "#define ISAMPERE\n" + csText, vendorLines++;
	if (isTuring) csText = "#define ISTURING\n" + csText, vendorLines++;
	if (isPascal) csText = "#define ISPASCAL\n" + csText, vendorLines++;
	// expand #include directives: cl compiler doesn't support these natively
	// warning: this simple system does not handle nested includes.
	struct Include { int start, end; string file; } includes[64];
	int Ninc = 0;
	while (1)
	{
		// see if any #includes remain
		size_t pos = csText.find( "#include" );
		if (pos == string::npos) break;
		// start of expanded source construction
		string tmp;
		if (pos > 0)
			tmp = csText.substr( 0, pos - 1 ) + "\n",
			includes[Ninc].start = LineCount( tmp ); // record first line of #include content
		else
			includes[Ninc].start = 0;
		// parse filename of include file
		pos = csText.find( "\"", pos + 1 );
		if (pos == string::npos) FatalError( "Expected \" after #include in shader." );
		size_t end = csText.find( "\"", pos + 1 );
		if (end == string::npos) FatalError( "Expected second \" after #include in shader." );
		string file = csText.substr( pos + 1, end - pos - 1 );
		// load include file content
		string incText = TextFileRead( file.c_str() );
		includes[Ninc].end = includes[Ninc].start + LineCount( incText );
		includes[Ninc++].file = file;
		if (incText.size() == 0) FatalError( "#include file not found:\n%s", file.c_str() );
		// cleanup include file content: we get some crap first sometimes, but why?
		int firstValidChar = 0;
		while (incText[firstValidChar] < 0) firstValidChar++;
		// add include file content and remainder of source to expanded source string
		tmp += incText.substr( firstValidChar, string::npos );
		tmp += csText.substr( end + 1, string::npos ) + "\n";
		// repeat until no #includes left
		csText = tmp;
	}
	// attempt to compile the loaded and expanded source text
	const char* source = csText.c_str();
	size_t size = strlen( source );
	cl_int error;
	program = clCreateProgramWithSource( context, 1, (const char**)&source, &size, &error );
	CHECKCL( error );
	// why does the nvidia compiler not support these:
	// -cl-nv-maxrregcount=64 not faster than leaving it out (same for 128)
	// -cl-no-subgroup-ifp ? fails on nvidia.
	error = clBuildProgram( program, 0, NULL, "-cl-nv-verbose -cl-fast-relaxed-math -cl-mad-enable -cl-single-precision-constant", NULL, NULL );
	// handle errors
	if (error == CL_SUCCESS)
	{
		// dump PTX via: https://forums.developer.nvidia.com/t/pre-compiling-opencl-kernels-tutorial/17089
		// and: https://stackoverflow.com/questions/12868889/clgetprograminfo-cl-program-binary-sizes-incorrect-results
		cl_uint devCount;
		CHECKCL( clGetProgramInfo( program, CL_PROGRAM_NUM_DEVICES, sizeof( cl_uint ), &devCount, NULL ) );
		size_t* size = new size_t[devCount];
		size[0] = 0;
		size_t received;
		CHECKCL( clGetProgramInfo( program, CL_PROGRAM_BINARY_SIZES /* wrong data... */, devCount * sizeof( size_t ), size, &received ) );
		char** binaries = new char* [devCount];
		for (uint i = 0; i < devCount; i++)
			binaries[i] = new char[size[i] + 1];
		CHECKCL( clGetProgramInfo( program, CL_PROGRAM_BINARIES, devCount * sizeof( size_t ), binaries, NULL ) );
		FILE* f = fopen( "buildlog.txt", "wb" );
		for (uint i = 0; i < devCount; i++)
			fwrite( binaries[i], 1, size[i] + 1, f );
		fclose( f );
	}
	else
	{
		// obtain the error log from the cl compiler
		if (!log) log = new char[256 * 1024]; // can be quite large
		log[0] = 0;
		clGetProgramBuildInfo( program, getFirstDevice( context ), CL_PROGRAM_BUILD_LOG, 256 * 1024, log, &size );
		// save error log for closer inspection
		FILE* f = fopen( "errorlog.txt", "wb" );
		fwrite( log, 1, size, f );
		fclose( f );
		// find and display the first error. Note: platform specific sadly; code below is for NVIDIA
		char* error = strstr( log, ": error:" );
		if (error)
		{
			int errorPos = (int)(error - log);
			while (errorPos > 0) if (log[errorPos - 1] == '\n') break; else errorPos--;
			// translate file and line number of error and report
			log[errorPos + 2048] = 0;
			int lineNr = 0, linePos = 0;
			char* lns = strstr( log + errorPos, ">:" ), * eol;
			if (!lns) FatalError( "unkown error text format", log + errorPos ); else
			{
				lns += 2;
				while (*lns >= '0' && *lns <= '9') lineNr = lineNr * 10 + (*lns++ - '0');
				lns++; // proceed to line number
				while (*lns >= '0' && *lns <= '9') linePos = linePos * 10 + (*lns++ - '0');
				lns += 9; // proceed to error message
				eol = lns;
				while (*eol != '\n' && *eol > 0) eol++;
				*eol = 0;
				lineNr--; // we count from 0 instead of 1
				// adjust file and linenr based on include file data
				string errorFile = file;
				bool errorInInclude = false;
				for (int i = Ninc - 1; i >= 0; i--)
				{
					if (lineNr > includes[i].end)
					{
						for (int j = 0; j <= i; j++) lineNr -= includes[j].end - includes[j].start;
						break;
					}
					else if (lineNr > includes[i].start)
					{
						errorFile = includes[i].file;
						lineNr -= includes[i].start;
						errorInInclude = true;
						break;
					}
				}
				if (!errorInInclude) lineNr -= vendorLines;
				// present error message
				char t[1024];
				sprintf( t, "file %s, line %i, pos %i:\n%s", errorFile.c_str(), lineNr + 1, linePos, lns );
				FatalError( t, "Build error" );
			}
		}
		else
		{
			// error string has unknown format; just dump it to a window
			log[2048] = 0; // truncate very long logs
			FatalError( log, "Build error" );
		}
	}
	kernel = clCreateKernel( program, entryPoint, &error );
	if (kernel == 0) FatalError( "clCreateKernel failed: entry point not found." );
	CHECKCL( error );
}

Kernel::Kernel( cl_program& existingProgram, char* entryPoint )
{
	CheckCLStarted();
	cl_int error;
	program = existingProgram;
	kernel = clCreateKernel( program, entryPoint, &error );
	if (kernel == 0) FatalError( "clCreateKernel failed: entry point not found." );
	CHECKCL( error );
}

// Kernel destructor
// ----------------------------------------------------------------------------
Kernel::~Kernel()
{
	if (kernel) clReleaseKernel( kernel );
	// if (program) clReleaseProgram( program ); // NOTE: may be shared with other kernels
	kernel = 0;
	// program = 0;
}

// InitCL method
// ----------------------------------------------------------------------------
bool Kernel::InitCL()
{
	cl_platform_id platform;
	cl_device_id* devices;
	cl_uint devCount;
	cl_int error;
	if (!CHECKCL( error = getPlatformID( &platform ) )) return false;
	if (!CHECKCL( error = clGetDeviceIDs( platform, CL_DEVICE_TYPE_ALL, 0, NULL, &devCount ) )) return false;
	devices = new cl_device_id[devCount];
	if (!CHECKCL( error = clGetDeviceIDs( platform, CL_DEVICE_TYPE_ALL, devCount, devices, NULL ) )) return false;
	uint deviceUsed = -1;
	// search a capable OpenCL device
	char device_string[1024], device_platform[1024];
	for (uint i = 0; i < devCount; i++)
	{
		// CHECKCL( error = clGetDeviceInfo( devices[i], CL_DEVICE_NAME, 1024, &device_string, NULL ) );
		// if (strstr( device_string, "AMD" ) == 0) continue; // I insist on AMD
		size_t extensionSize;
		CHECKCL( error = clGetDeviceInfo( devices[i], CL_DEVICE_EXTENSIONS, 0, NULL, &extensionSize ) );
		if (extensionSize > 0)
		{
			char* extensions = (char*)malloc( extensionSize );
			CHECKCL( error = clGetDeviceInfo( devices[i], CL_DEVICE_EXTENSIONS, extensionSize, extensions, &extensionSize ) );
			string deviceList( extensions );
			free( extensions );
			string mustHave[] = {
				"cl_khr_gl_sharing",
				"cl_khr_global_int32_base_atomics"
			};
			bool hasAll = true;
			for (int j = 0; j < 2; j++)
			{
				size_t o = 0, s = deviceList.find( ' ', o );
				bool hasFeature = false;
				while (s != deviceList.npos)
				{
					string subs = deviceList.substr( o, s - o );
					if (strcmp( mustHave[j].c_str(), subs.c_str() ) == 0) hasFeature = true;
					do { o = s + 1, s = deviceList.find( ' ', o ); } while (s == o);
				}
				if (!hasFeature) hasAll = false;
			}
			if (hasAll)
			{
				cl_context_properties props[] =
				{
					CL_GL_CONTEXT_KHR, (cl_context_properties)glfwGetWGLContext( window ),
					CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
					CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0
				};
				// attempt to create a context with the requested features
				context = clCreateContext( props, 1, &devices[i], NULL, NULL, &error );
				if (error == CL_SUCCESS)
				{
					candoInterop = true;
					deviceUsed = i;
					break;
				}
			}
			if (deviceUsed > -1) break;
		}
	}
	if (deviceUsed == -1) FatalError( "No capable OpenCL device found." );
	device = getFirstDevice( context );
	if (!CHECKCL( error )) return false;
	// print device name
	clGetDeviceInfo( devices[deviceUsed], CL_DEVICE_NAME, 1024, &device_string, NULL );
	clGetDeviceInfo( devices[deviceUsed], CL_DEVICE_VERSION, 1024, &device_platform, NULL );
	printf( "Device # %u, %s (%s)\n", deviceUsed, device_string, device_platform );
	// digest device string
	char* d = device_string;
	for (int i = 0; i < strlen( d ); i++) if (d[i] >= 'A' && d[i] <= 'Z') d[i] -= 'A' - 'a';
	if (strstr( d, "nvidia" ))
	{
		isNVidia = true;
		if (strstr( d, "rtx" ))
		{
			// detect Ampere GPUs
			if (strstr( d, "3050" ) || strstr( d, "3060" ) || strstr( d, "3070" ) || strstr( d, "3080" ) || strstr( d, "3090" )) isAmpere = true;
			if (strstr( d, "a2000" ) || strstr( d, "a3000" ) || strstr( d, "a4000" ) || strstr( d, "a5000" ) || strstr( d, "a6000" )) isAmpere = true;
			// detect Turing GPUs
			if (strstr( d, "2060" ) || strstr( d, "2070" ) || strstr( d, "2080" )) isTuring = true;
			// detect Titan RTX
			if (strstr( d, "titan rtx" )) isTuring = true;
			// detect Turing Quadro
			if (strstr( d, "quadro" ))
			{
				if (strstr( d, "3000" ) || strstr( d, "4000" ) || strstr( d, "5000" ) || strstr( d, "6000" ) || strstr( d, "8000" )) isTuring = true;
			}
		}
		else if (strstr( d, "gtx" ))
		{
			// detect Turing GPUs
			if (strstr( d, "1650" ) || strstr( d, "1660" )) isTuring = true;
			// detect Pascal GPUs
			if (strstr( d, "1010" ) || strstr( d, "1030" ) || strstr( d, "1050" ) || strstr( d, "1060" ) || strstr( d, "1070" ) || strstr( d, "1080" )) isPascal = true;
		}
		else if (strstr( d, "quadro" ))
		{
			// detect Pascal GPUs
			if (strstr( d, "p2000" ) || strstr( d, "p1000" ) || strstr( d, "p600" ) || strstr( d, "p400" ) || strstr( d, "p5000" ) || strstr( d, "p100" )) isPascal = true;
		}
		else
		{
			// detect Pascal GPUs
			if (strstr( d, "titan x" )) isPascal = true;
		}
	}
	else if (strstr( d, "amd" ))
	{
		isAMD = true;
	}
	else if (strstr( d, "intel" ))
	{
		isIntel = true;
	}
	else
	{
		isOther = true;
	}
	// report on findings
	printf( "hardware detected: " );
	if (isNVidia)
	{
		printf( "NVIDIA, " );
		if (isAmpere) printf( "AMPERE class.\n" );
		else if (isTuring) printf( "TURING class.\n" );
		else if (isPascal) printf( "PASCAL class.\n" );
		else printf( "PRE-PASCAL hardware (warning: slow).\n" );
	}
	else if (isAMD)
	{
		printf( "AMD.\n" );
	}
	else if (isIntel)
	{
		printf( "Intel.\n" );
	}
	else
	{
		printf( "identification failed.\n" );
	}
	// create a command-queue
	queue = clCreateCommandQueue( context, devices[deviceUsed], CL_QUEUE_PROFILING_ENABLE, &error );
	if (!CHECKCL( error )) return false;
	// create a second command queue for asynchronous copies
	queue2 = clCreateCommandQueue( context, devices[deviceUsed], CL_QUEUE_PROFILING_ENABLE, &error );
	if (!CHECKCL( error )) return false;
	// cleanup
	delete devices;
	clStarted = true;
	return true;
}

// KillCL method
// ----------------------------------------------------------------------------
void Kernel::KillCL()
{
	if (!clStarted) return;
	clReleaseCommandQueue( queue2 );
	clReleaseCommandQueue( queue );
	clReleaseContext( context );
}

// CheckCLStarted method
// ----------------------------------------------------------------------------
void Kernel::CheckCLStarted()
{
	if (!clStarted) FatalError( "Call InitCL() before using OpenCL functionality." );
}

// SetArgument methods
// ----------------------------------------------------------------------------
void Kernel::SetArgument( int idx, cl_mem* buffer ) { CheckCLStarted(); clSetKernelArg( kernel, idx, sizeof( cl_mem ), buffer ); arg0set |= idx == 0; argIdx = idx; }
void Kernel::SetArgument( int idx, Buffer* buffer ) { CheckCLStarted(); clSetKernelArg( kernel, idx, sizeof( cl_mem ), buffer->GetDevicePtr() ); arg0set |= idx == 0; argIdx = idx; }
void Kernel::SetArgument( int idx, Buffer& buffer ) { CheckCLStarted(); clSetKernelArg( kernel, idx, sizeof( cl_mem ), buffer.GetDevicePtr() ); arg0set |= idx == 0; argIdx = idx; }
void Kernel::SetArgument( int idx, int value ) { CheckCLStarted(); clSetKernelArg( kernel, idx, sizeof( int ), &value ); arg0set |= idx == 0; argIdx = idx; }
void Kernel::SetArgument( int idx, float value ) { CheckCLStarted(); clSetKernelArg( kernel, idx, sizeof( float ), &value ); arg0set |= idx == 0; argIdx = idx; }
void Kernel::SetArgument( int idx, float2 value ) { CheckCLStarted(); clSetKernelArg( kernel, idx, sizeof( float2 ), &value ); arg0set |= idx == 0; argIdx = idx; }
void Kernel::SetArgument( int idx, float3 value ) { CheckCLStarted(); clSetKernelArg( kernel, idx, sizeof( float3 ), &value ); arg0set |= idx == 0; argIdx = idx; }
void Kernel::SetArgument( int idx, float4 value ) { CheckCLStarted(); clSetKernelArg( kernel, idx, sizeof( float4 ), &value ); arg0set |= idx == 0; argIdx = idx; }

// Run method
// ----------------------------------------------------------------------------
void Kernel::Run( cl_event* eventToWaitFor, cl_event* eventToSet )
{
	CheckCLStarted();
	glFinish();
	cl_int error = clEnqueueNDRangeKernel( queue, kernel, 2, 0, workSize, localSize, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet );
	CHECKCL( error );
	clFinish( queue );
}

void Kernel::Run( cl_mem* buffers, const int count, cl_event* eventToWaitFor, cl_event* eventToSet, cl_event* acq, cl_event* rel )
{
	CheckCLStarted();
	cl_int error;
	if (Kernel::candoInterop)
	{
		CHECKCL( error = clEnqueueAcquireGLObjects( queue, count, buffers, 0, 0, acq ) );
		CHECKCL( error = clEnqueueNDRangeKernel( queue, kernel, 2, 0, workSize, localSize, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet ) );
		CHECKCL( error = clEnqueueReleaseGLObjects( queue, count, buffers, 0, 0, rel ) );
	}
	else
	{
		CHECKCL( error = clEnqueueNDRangeKernel( queue, kernel, 2, 0, workSize, localSize, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet ) );
	}
}

void Kernel::Run( Buffer* buffer, const int2 tileSize, cl_event* eventToWaitFor, cl_event* eventToSet, cl_event* acq, cl_event* rel )
{
	// execute a kernel for each pixel of a screen buffer, 1 thread per pixel
	CheckCLStarted();
	cl_int error;
	if (!arg0set) FatalError( "Kernel expects at least 1 argument, none set." );
	if (Kernel::candoInterop)
	{
		size_t localSize[2] = { (size_t)tileSize.x, (size_t)tileSize.y };
		CHECKCL( error = clEnqueueAcquireGLObjects( queue, 1, buffer->GetDevicePtr(), 0, 0, acq ) );
		CHECKCL( error = clEnqueueNDRangeKernel( queue, kernel, 2, 0, workSize, localSize, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet ) );
		CHECKCL( error = clEnqueueReleaseGLObjects( queue, 1, buffer->GetDevicePtr(), 0, 0, rel ) );
	}
	else
	{
		CHECKCL( error = clEnqueueNDRangeKernel( queue, kernel, 2, 0, workSize, localSize, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet ) );
	}
}

void Kernel::Run( Buffer* buffer, const int count, cl_event* eventToWaitFor, cl_event* eventToSet, cl_event* acq, cl_event* rel )
{
	// execute a 1D kernel that outputs to a screen buffer
	CheckCLStarted();
	cl_int error;
	size_t workSize = (size_t)count;
	if (Kernel::candoInterop)
	{
		CHECKCL( error = clEnqueueAcquireGLObjects( queue, 1, buffer->GetDevicePtr(), 0, 0, acq ) );
		CHECKCL( error = clEnqueueNDRangeKernel( queue, kernel, 1, 0, &workSize, 0, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet ) );
		CHECKCL( error = clEnqueueReleaseGLObjects( queue, 1, buffer->GetDevicePtr(), 0, 0, rel ) );
	}
	else
	{
		CHECKCL( error = clEnqueueNDRangeKernel( queue, kernel, 1, 0, &workSize, 0, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet ) );
	}
}

void Kernel::Run2D( const int2 count, const int2 lsize, cl_event* eventToWaitFor, cl_event* eventToSet )
{
	CheckCLStarted();
	size_t localSize[2] = { (size_t)lsize.x, (size_t)lsize.y };
	size_t workSize[2] = { (size_t)count.x, (size_t)count.y };
	cl_int error;
	CHECKCL( error = clEnqueueNDRangeKernel( queue, kernel, 2, 0, workSize, localSize, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet ) );
}

void Kernel::Run( const size_t count, const size_t localSize, cl_event* eventToWaitFor, cl_event* eventToSet )
{
	CheckCLStarted();
	cl_int error;
	CHECKCL( error = clEnqueueNDRangeKernel( queue, kernel, 1, 0, &count, localSize == 0 ? 0 : &localSize, eventToWaitFor ? 1 : 0, eventToWaitFor, eventToSet ) );
}

// surface implementation
// ----------------------------------------------------------------------------

static char s_Font[51][5][6];
static bool fontInitialized = false;
static int s_Transl[256];

Surface::Surface( int w, int h, uint* b ) : pixels( b ), width( w ), height( h ) {}
Surface::Surface( int w, int h ) : width( w ), height( h )
{
	pixels = (uint*)MALLOC64( w * h * sizeof( uint ) );
	ownBuffer = true; // needs to be deleted in destructor
}
Surface::Surface( const char* file ) : pixels( 0 ), width( 0 ), height( 0 )
{
	FILE* f = fopen( file, "rb" );
	if (!f) FatalError( "File not found: %s", file );
	fclose( f );
	LoadImage( file );
}

void Surface::LoadImage( const char* file )
{
	int n;
	unsigned char* data = stbi_load( file, &width, &height, &n, 0 );
	if (data)
	{
		pixels = (uint*)MALLOC64( width * height * sizeof( uint ) );
		ownBuffer = true; // needs to be deleted in destructor
		const int s = width * height;
		if (n == 1) // greyscale
		{
			for (int i = 0; i < s; i++)
			{
				const unsigned char p = data[i];
				pixels[i] = p + (p << 8) + (p << 16);
			}
		}
		else
		{
			for (int i = 0; i < s; i++) pixels[i] = (data[i * n + 0] << 16) + (data[i * n + 1] << 8) + data[i * n + 2];
		}
	}
	stbi_image_free( data );
}

Surface::~Surface()
{
	if (ownBuffer) FREE64( pixels ); // free only if we allocated the buffer ourselves
}

void Surface::Clear( uint c )
{
	const int s = width * height;
	for (int i = 0; i < s; i++) pixels[i] = c;
}

void Surface::Plot( int x, int y, uint c )
{
	if (x < 0 || y < 0 || x >= width || y >= height) return;
	pixels[x + y * width] = c;
}

void Surface::Print( const char* s, int x1, int y1, uint c )
{
	if (!fontInitialized)
	{
		InitCharset();
		fontInitialized = true;
	}
	uint* t = pixels + x1 + y1 * width;
	for (int i = 0; i < (int)(strlen( s )); i++, t += 6)
	{
		int pos = 0;
		if ((s[i] >= 'A') && (s[i] <= 'Z')) pos = s_Transl[(unsigned short)(s[i] - ('A' - 'a'))];
		else pos = s_Transl[(unsigned short)s[i]];
		uint* a = t;
		const char* u = (const char*)s_Font[pos];
		for (int v = 0; v < 5; v++, u++, a += width)
			for (int h = 0; h < 5; h++) if (*u++ == 'o') *(a + h) = c, * (a + h + width) = 0;
	}
}

#define OUTCODE(x,y) (((x)<xmin)?1:(((x)>xmax)?2:0))+(((y)<ymin)?4:(((y)>ymax)?8:0))

void Surface::Line( float x1, float y1, float x2, float y2, uint c )
{
	// clip (Cohen-Sutherland, https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm)
	const float xmin = 0, ymin = 0, xmax = (float)width - 1, ymax = (float)height - 1;
	int c0 = OUTCODE( x1, y1 ), c1 = OUTCODE( x2, y2 );
	bool accept = false;
	while (1)
	{
		if (!(c0 | c1)) { accept = true; break; }
		else if (c0 & c1) break; else
		{
			float x, y;
			const int co = c0 ? c0 : c1;
			if (co & 8) x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1), y = ymax;
			else if (co & 4) x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1), y = ymin;
			else if (co & 2) y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1), x = xmax;
			else if (co & 1) y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1), x = xmin;
			if (co == c0) x1 = x, y1 = y, c0 = OUTCODE( x1, y1 );
			else x2 = x, y2 = y, c1 = OUTCODE( x2, y2 );
		}
	}
	if (!accept) return;
	float b = x2 - x1;
	float h = y2 - y1;
	float l = fabsf( b );
	if (fabsf( h ) > l) l = fabsf( h );
	int il = (int)l;
	float dx = b / (float)l;
	float dy = h / (float)l;
	for (int i = 0; i <= il; i++)
	{
		*(pixels + (int)x1 + (int)y1 * width) = c;
		x1 += dx, y1 += dy;
	}
}

void Surface::CopyTo( Surface* d, int x, int y )
{
	uint* dst = d->pixels;
	uint* src = pixels;
	if ((src) && (dst))
	{
		int srcwidth = width;
		int srcheight = height;
		int dstwidth = d->width;
		int dstheight = d->height;
		if ((srcwidth + x) > dstwidth) srcwidth = dstwidth - x;
		if ((srcheight + y) > dstheight) srcheight = dstheight - y;
		if (x < 0) src -= x, srcwidth += x, x = 0;
		if (y < 0) src -= y * srcwidth, srcheight += y, y = 0;
		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += x + dstwidth * y;
			for (int y = 0; y < srcheight; y++)
			{
				memcpy( dst, src, srcwidth * 4 );
				dst += dstwidth, src += srcwidth;
			}
		}
	}
}

void Surface::SetChar( int c, const char* c1, const char* c2, const char* c3, const char* c4, const char* c5 )
{
	strcpy( s_Font[c][0], c1 );
	strcpy( s_Font[c][1], c2 );
	strcpy( s_Font[c][2], c3 );
	strcpy( s_Font[c][3], c4 );
	strcpy( s_Font[c][4], c5 );
}

void Surface::InitCharset()
{
	SetChar( 0, ":ooo:", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 1, "oooo:", "o:::o", "oooo:", "o:::o", "oooo:" );
	SetChar( 2, ":oooo", "o::::", "o::::", "o::::", ":oooo" );
	SetChar( 3, "oooo:", "o:::o", "o:::o", "o:::o", "oooo:" );
	SetChar( 4, "ooooo", "o::::", "oooo:", "o::::", "ooooo" );
	SetChar( 5, "ooooo", "o::::", "ooo::", "o::::", "o::::" );
	SetChar( 6, ":oooo", "o::::", "o:ooo", "o:::o", ":ooo:" );
	SetChar( 7, "o:::o", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 8, "::o::", "::o::", "::o::", "::o::", "::o::" );
	SetChar( 9, ":::o:", ":::o:", ":::o:", ":::o:", "ooo::" );
	SetChar( 10, "o::o:", "o:o::", "oo:::", "o:o::", "o::o:" );
	SetChar( 11, "o::::", "o::::", "o::::", "o::::", "ooooo" );
	SetChar( 12, "oo:o:", "o:o:o", "o:o:o", "o:::o", "o:::o" );
	SetChar( 13, "o:::o", "oo::o", "o:o:o", "o::oo", "o:::o" );
	SetChar( 14, ":ooo:", "o:::o", "o:::o", "o:::o", ":ooo:" );
	SetChar( 15, "oooo:", "o:::o", "oooo:", "o::::", "o::::" );
	SetChar( 16, ":ooo:", "o:::o", "o:::o", "o::oo", ":oooo" );
	SetChar( 17, "oooo:", "o:::o", "oooo:", "o:o::", "o::o:" );
	SetChar( 18, ":oooo", "o::::", ":ooo:", "::::o", "oooo:" );
	SetChar( 19, "ooooo", "::o::", "::o::", "::o::", "::o::" );
	SetChar( 20, "o:::o", "o:::o", "o:::o", "o:::o", ":oooo" );
	SetChar( 21, "o:::o", "o:::o", ":o:o:", ":o:o:", "::o::" );
	SetChar( 22, "o:::o", "o:::o", "o:o:o", "o:o:o", ":o:o:" );
	SetChar( 23, "o:::o", ":o:o:", "::o::", ":o:o:", "o:::o" );
	SetChar( 24, "o:::o", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar( 25, "ooooo", ":::o:", "::o::", ":o:::", "ooooo" );
	SetChar( 26, ":ooo:", "o::oo", "o:o:o", "oo::o", ":ooo:" );
	SetChar( 27, "::o::", ":oo::", "::o::", "::o::", ":ooo:" );
	SetChar( 28, ":ooo:", "o:::o", "::oo:", ":o:::", "ooooo" );
	SetChar( 29, "oooo:", "::::o", "::oo:", "::::o", "oooo:" );
	SetChar( 30, "o::::", "o::o:", "ooooo", ":::o:", ":::o:" );
	SetChar( 31, "ooooo", "o::::", "oooo:", "::::o", "oooo:" );
	SetChar( 32, ":oooo", "o::::", "oooo:", "o:::o", ":ooo:" );
	SetChar( 33, "ooooo", "::::o", ":::o:", "::o::", "::o::" );
	SetChar( 34, ":ooo:", "o:::o", ":ooo:", "o:::o", ":ooo:" );
	SetChar( 35, ":ooo:", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar( 36, "::o::", "::o::", "::o::", ":::::", "::o::" );
	SetChar( 37, ":ooo:", "::::o", ":::o:", ":::::", "::o::" );
	SetChar( 38, ":::::", ":::::", "::o::", ":::::", "::o::" );
	SetChar( 39, ":::::", ":::::", ":ooo:", ":::::", ":ooo:" );
	SetChar( 40, ":::::", ":::::", ":::::", ":::o:", "::o::" );
	SetChar( 41, ":::::", ":::::", ":::::", ":::::", "::o::" );
	SetChar( 42, ":::::", ":::::", ":ooo:", ":::::", ":::::" );
	SetChar( 43, ":::o:", "::o::", "::o::", "::o::", ":::o:" );
	SetChar( 44, "::o::", ":::o:", ":::o:", ":::o:", "::o::" );
	SetChar( 45, ":::::", ":::::", ":::::", ":::::", ":::::" );
	SetChar( 46, "ooooo", "ooooo", "ooooo", "ooooo", "ooooo" );
	SetChar( 47, "::o::", "::o::", ":::::", ":::::", ":::::" ); // Tnx Ferry
	SetChar( 48, "o:o:o", ":ooo:", "ooooo", ":ooo:", "o:o:o" );
	SetChar( 49, "::::o", ":::o:", "::o::", ":o:::", "o::::" );
	char c[] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/";
	int i;
	for (i = 0; i < 256; i++) s_Transl[i] = 45;
	for (i = 0; i < 50; i++) s_Transl[(unsigned char)c[i]] = i;
}

/*

	OpenGL loader generated by glad 0.1.35 on Fri Mar 18 11:02:23 2022.

	Language/Generator: C/C++
	Specification: gl
	APIs: gl=3.3
	Profile: core
	Extensions:

	Loader: True
	Local files: True
	Omit khrplatform: False
	Reproducible: False

	Commandline:
		--profile="core" --api="gl=3.3" --generator="c" --spec="gl" --local-files --extensions=""
	Online:
		https://glad.dav1d.de/#profile=core&language=c&specification=gl&loader=on&api=gl%3D3.3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "glad.h"

static void* get_proc( const char* namez );

#if defined(_WIN32) || defined(__CYGWIN__)
#ifndef _WINDOWS_
#undef APIENTRY
#endif
#include <windows.h>
static HMODULE libGL;

typedef void* (APIENTRYP PFNWGLGETPROCADDRESSPROC_PRIVATE)(const char*);
static PFNWGLGETPROCADDRESSPROC_PRIVATE gladGetProcAddressPtr;

#ifdef _MSC_VER
#ifdef __has_include
#if __has_include(<winapifamily.h>)
#define HAVE_WINAPIFAMILY 1
#endif
#elif _MSC_VER >= 1700 && !_USING_V110_SDK71_
#define HAVE_WINAPIFAMILY 1
#endif
#endif

#ifdef HAVE_WINAPIFAMILY
#include <winapifamily.h>
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#define IS_UWP 1
#endif
#endif

static
int open_gl( void ) {
#ifndef IS_UWP
	libGL = LoadLibraryW( L"opengl32.dll" );
	if (libGL != NULL) {
		void (*tmp)(void);
		tmp = (void(*)(void)) GetProcAddress( libGL, "wglGetProcAddress" );
		gladGetProcAddressPtr = (PFNWGLGETPROCADDRESSPROC_PRIVATE)tmp;
		return gladGetProcAddressPtr != NULL;
	}
#endif

	return 0;
}

static
void close_gl( void ) {
	if (libGL != NULL) {
		FreeLibrary( (HMODULE)libGL );
		libGL = NULL;
	}
}
#else
#include <dlfcn.h>
static void* libGL;

#if !defined(__APPLE__) && !defined(__HAIKU__)
typedef void* (APIENTRYP PFNGLXGETPROCADDRESSPROC_PRIVATE)(const char*);
static PFNGLXGETPROCADDRESSPROC_PRIVATE gladGetProcAddressPtr;
#endif

static
int open_gl( void ) {
#ifdef __APPLE__
	static const char* NAMES[] = {
		"../Frameworks/OpenGL.framework/OpenGL",
		"/Library/Frameworks/OpenGL.framework/OpenGL",
		"/System/Library/Frameworks/OpenGL.framework/OpenGL",
		"/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL"
	};
#else
	static const char* NAMES[] = { "libGL.so.1", "libGL.so" };
#endif

	unsigned int index = 0;
	for (index = 0; index < (sizeof( NAMES ) / sizeof( NAMES[0] )); index++) {
		libGL = dlopen( NAMES[index], RTLD_NOW | RTLD_GLOBAL );

		if (libGL != NULL) {
		#if defined(__APPLE__) || defined(__HAIKU__)
			return 1;
		#else
			gladGetProcAddressPtr = (PFNGLXGETPROCADDRESSPROC_PRIVATE)dlsym( libGL,
				"glXGetProcAddressARB" );
			return gladGetProcAddressPtr != NULL;
		#endif
		}
	}

	return 0;
}

static
void close_gl( void ) {
	if (libGL != NULL) {
		dlclose( libGL );
		libGL = NULL;
	}
}
#endif

static
void* get_proc( const char* namez ) {
	void* result = NULL;
	if (libGL == NULL) return NULL;

#if !defined(__APPLE__) && !defined(__HAIKU__)
	if (gladGetProcAddressPtr != NULL) {
		result = gladGetProcAddressPtr( namez );
	}
#endif
	if (result == NULL) {
	#if defined(_WIN32) || defined(__CYGWIN__)
		result = (void*)GetProcAddress( (HMODULE)libGL, namez );
	#else
		result = dlsym( libGL, namez );
	#endif
	}

	return result;
}

int gladLoadGL( void ) {
	int status = 0;

	if (open_gl()) {
		status = gladLoadGLLoader( &get_proc );
		close_gl();
	}

	return status;
}

struct gladGLversionStruct GLVersion = { 0, 0 };

#if defined(GL_ES_VERSION_3_0) || defined(GL_VERSION_3_0)
#define _GLAD_IS_SOME_NEW_VERSION 1
#endif

static int max_loaded_major;
static int max_loaded_minor;

static const char* exts = NULL;
static int num_exts_i = 0;
static char** exts_i = NULL;

static int get_exts( void ) {
#ifdef _GLAD_IS_SOME_NEW_VERSION
	if (max_loaded_major < 3) {
	#endif
		exts = (const char*)glGetString( GL_EXTENSIONS );
	#ifdef _GLAD_IS_SOME_NEW_VERSION
	}
	else {
		unsigned int index;

		num_exts_i = 0;
		glGetIntegerv( GL_NUM_EXTENSIONS, &num_exts_i );
		if (num_exts_i > 0) {
			exts_i = (char**)malloc( (size_t)num_exts_i * (sizeof * exts_i) );
		}

		if (exts_i == NULL) {
			return 0;
		}

		for (index = 0; index < (unsigned)num_exts_i; index++) {
			const char* gl_str_tmp = (const char*)glGetStringi( GL_EXTENSIONS, index );
			size_t len = strlen( gl_str_tmp );

			char* local_str = (char*)malloc( (len + 1) * sizeof( char ) );
			if (local_str != NULL) {
				memcpy( local_str, gl_str_tmp, (len + 1) * sizeof( char ) );
			}
			exts_i[index] = local_str;
		}
	}
#endif
	return 1;
}

static void free_exts( void ) {
	if (exts_i != NULL) {
		int index;
		for (index = 0; index < num_exts_i; index++) {
			free( (char*)exts_i[index] );
		}
		free( (void*)exts_i );
		exts_i = NULL;
	}
}

static int has_ext( const char* ext ) {
#ifdef _GLAD_IS_SOME_NEW_VERSION
	if (max_loaded_major < 3) {
	#endif
		const char* extensions;
		const char* loc;
		const char* terminator;
		extensions = exts;
		if (extensions == NULL || ext == NULL) {
			return 0;
		}

		while (1) {
			loc = strstr( extensions, ext );
			if (loc == NULL) {
				return 0;
			}

			terminator = loc + strlen( ext );
			if ((loc == extensions || *(loc - 1) == ' ') &&
				(*terminator == ' ' || *terminator == '\0')) {
				return 1;
			}
			extensions = terminator;
		}
	#ifdef _GLAD_IS_SOME_NEW_VERSION
	}
	else {
		int index;
		if (exts_i == NULL) return 0;
		for (index = 0; index < num_exts_i; index++) {
			const char* e = exts_i[index];

			if (exts_i[index] != NULL && strcmp( e, ext ) == 0) {
				return 1;
			}
		}
	}
#endif

	return 0;
}
int GLAD_GL_VERSION_1_0 = 0;
int GLAD_GL_VERSION_1_1 = 0;
int GLAD_GL_VERSION_1_2 = 0;
int GLAD_GL_VERSION_1_3 = 0;
int GLAD_GL_VERSION_1_4 = 0;
int GLAD_GL_VERSION_1_5 = 0;
int GLAD_GL_VERSION_2_0 = 0;
int GLAD_GL_VERSION_2_1 = 0;
int GLAD_GL_VERSION_3_0 = 0;
int GLAD_GL_VERSION_3_1 = 0;
int GLAD_GL_VERSION_3_2 = 0;
int GLAD_GL_VERSION_3_3 = 0;
PFNGLACTIVETEXTUREPROC glad_glActiveTexture = NULL;
PFNGLATTACHSHADERPROC glad_glAttachShader = NULL;
PFNGLBEGINCONDITIONALRENDERPROC glad_glBeginConditionalRender = NULL;
PFNGLBEGINQUERYPROC glad_glBeginQuery = NULL;
PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback = NULL;
PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation = NULL;
PFNGLBINDBUFFERPROC glad_glBindBuffer = NULL;
PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase = NULL;
PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange = NULL;
PFNGLBINDFRAGDATALOCATIONPROC glad_glBindFragDataLocation = NULL;
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glad_glBindFragDataLocationIndexed = NULL;
PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer = NULL;
PFNGLBINDSAMPLERPROC glad_glBindSampler = NULL;
PFNGLBINDTEXTUREPROC glad_glBindTexture = NULL;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = NULL;
PFNGLBLENDCOLORPROC glad_glBlendColor = NULL;
PFNGLBLENDEQUATIONPROC glad_glBlendEquation = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate = NULL;
PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate = NULL;
PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer = NULL;
PFNGLBUFFERDATAPROC glad_glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glad_glBufferSubData = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;
PFNGLCLAMPCOLORPROC glad_glClampColor = NULL;
PFNGLCLEARPROC glad_glClear = NULL;
PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi = NULL;
PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv = NULL;
PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv = NULL;
PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv = NULL;
PFNGLCLEARCOLORPROC glad_glClearColor = NULL;
PFNGLCLEARDEPTHPROC glad_glClearDepth = NULL;
PFNGLCLEARSTENCILPROC glad_glClearStencil = NULL;
PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync = NULL;
PFNGLCOLORMASKPROC glad_glColorMask = NULL;
PFNGLCOLORMASKIPROC glad_glColorMaski = NULL;
PFNGLCOLORP3UIPROC glad_glColorP3ui = NULL;
PFNGLCOLORP3UIVPROC glad_glColorP3uiv = NULL;
PFNGLCOLORP4UIPROC glad_glColorP4ui = NULL;
PFNGLCOLORP4UIVPROC glad_glColorP4uiv = NULL;
PFNGLCOMPILESHADERPROC glad_glCompileShader = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glad_glCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glad_glCompressedTexSubImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D = NULL;
PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData = NULL;
PFNGLCOPYTEXIMAGE1DPROC glad_glCopyTexImage1D = NULL;
PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE1DPROC glad_glCopyTexSubImage1D = NULL;
PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D = NULL;
PFNGLCREATEPROGRAMPROC glad_glCreateProgram = NULL;
PFNGLCREATESHADERPROC glad_glCreateShader = NULL;
PFNGLCULLFACEPROC glad_glCullFace = NULL;
PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = NULL;
PFNGLDELETEQUERIESPROC glad_glDeleteQueries = NULL;
PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers = NULL;
PFNGLDELETESAMPLERSPROC glad_glDeleteSamplers = NULL;
PFNGLDELETESHADERPROC glad_glDeleteShader = NULL;
PFNGLDELETESYNCPROC glad_glDeleteSync = NULL;
PFNGLDELETETEXTURESPROC glad_glDeleteTextures = NULL;
PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = NULL;
PFNGLDEPTHFUNCPROC glad_glDepthFunc = NULL;
PFNGLDEPTHMASKPROC glad_glDepthMask = NULL;
PFNGLDEPTHRANGEPROC glad_glDepthRange = NULL;
PFNGLDETACHSHADERPROC glad_glDetachShader = NULL;
PFNGLDISABLEPROC glad_glDisable = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray = NULL;
PFNGLDISABLEIPROC glad_glDisablei = NULL;
PFNGLDRAWARRAYSPROC glad_glDrawArrays = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced = NULL;
PFNGLDRAWBUFFERPROC glad_glDrawBuffer = NULL;
PFNGLDRAWBUFFERSPROC glad_glDrawBuffers = NULL;
PFNGLDRAWELEMENTSPROC glad_glDrawElements = NULL;
PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex = NULL;
PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements = NULL;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex = NULL;
PFNGLENABLEPROC glad_glEnable = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
PFNGLENABLEIPROC glad_glEnablei = NULL;
PFNGLENDCONDITIONALRENDERPROC glad_glEndConditionalRender = NULL;
PFNGLENDQUERYPROC glad_glEndQuery = NULL;
PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback = NULL;
PFNGLFENCESYNCPROC glad_glFenceSync = NULL;
PFNGLFINISHPROC glad_glFinish = NULL;
PFNGLFLUSHPROC glad_glFlush = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture = NULL;
PFNGLFRAMEBUFFERTEXTURE1DPROC glad_glFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3DPROC glad_glFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer = NULL;
PFNGLFRONTFACEPROC glad_glFrontFace = NULL;
PFNGLGENBUFFERSPROC glad_glGenBuffers = NULL;
PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
PFNGLGENQUERIESPROC glad_glGenQueries = NULL;
PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers = NULL;
PFNGLGENSAMPLERSPROC glad_glGenSamplers = NULL;
PFNGLGENTEXTURESPROC glad_glGenTextures = NULL;
PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = NULL;
PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap = NULL;
PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMNAMEPROC glad_glGetActiveUniformName = NULL;
PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv = NULL;
PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation = NULL;
PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v = NULL;
PFNGLGETBOOLEANVPROC glad_glGetBooleanv = NULL;
PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v = NULL;
PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv = NULL;
PFNGLGETBUFFERSUBDATAPROC glad_glGetBufferSubData = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glad_glGetCompressedTexImage = NULL;
PFNGLGETDOUBLEVPROC glad_glGetDoublev = NULL;
PFNGLGETERRORPROC glad_glGetError = NULL;
PFNGLGETFLOATVPROC glad_glGetFloatv = NULL;
PFNGLGETFRAGDATAINDEXPROC glad_glGetFragDataIndex = NULL;
PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv = NULL;
PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v = NULL;
PFNGLGETINTEGER64VPROC glad_glGetInteger64v = NULL;
PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v = NULL;
PFNGLGETINTEGERVPROC glad_glGetIntegerv = NULL;
PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = NULL;
PFNGLGETQUERYOBJECTI64VPROC glad_glGetQueryObjecti64v = NULL;
PFNGLGETQUERYOBJECTIVPROC glad_glGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUI64VPROC glad_glGetQueryObjectui64v = NULL;
PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv = NULL;
PFNGLGETQUERYIVPROC glad_glGetQueryiv = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv = NULL;
PFNGLGETSAMPLERPARAMETERIIVPROC glad_glGetSamplerParameterIiv = NULL;
PFNGLGETSAMPLERPARAMETERIUIVPROC glad_glGetSamplerParameterIuiv = NULL;
PFNGLGETSAMPLERPARAMETERFVPROC glad_glGetSamplerParameterfv = NULL;
PFNGLGETSAMPLERPARAMETERIVPROC glad_glGetSamplerParameteriv = NULL;
PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = NULL;
PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource = NULL;
PFNGLGETSHADERIVPROC glad_glGetShaderiv = NULL;
PFNGLGETSTRINGPROC glad_glGetString = NULL;
PFNGLGETSTRINGIPROC glad_glGetStringi = NULL;
PFNGLGETSYNCIVPROC glad_glGetSynciv = NULL;
PFNGLGETTEXIMAGEPROC glad_glGetTexImage = NULL;
PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv = NULL;
PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv = NULL;
PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv = NULL;
PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv = NULL;
PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv = NULL;
PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex = NULL;
PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices = NULL;
PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = NULL;
PFNGLGETUNIFORMFVPROC glad_glGetUniformfv = NULL;
PFNGLGETUNIFORMIVPROC glad_glGetUniformiv = NULL;
PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv = NULL;
PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv = NULL;
PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv = NULL;
PFNGLGETVERTEXATTRIBDVPROC glad_glGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv = NULL;
PFNGLHINTPROC glad_glHint = NULL;
PFNGLISBUFFERPROC glad_glIsBuffer = NULL;
PFNGLISENABLEDPROC glad_glIsEnabled = NULL;
PFNGLISENABLEDIPROC glad_glIsEnabledi = NULL;
PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer = NULL;
PFNGLISPROGRAMPROC glad_glIsProgram = NULL;
PFNGLISQUERYPROC glad_glIsQuery = NULL;
PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer = NULL;
PFNGLISSAMPLERPROC glad_glIsSampler = NULL;
PFNGLISSHADERPROC glad_glIsShader = NULL;
PFNGLISSYNCPROC glad_glIsSync = NULL;
PFNGLISTEXTUREPROC glad_glIsTexture = NULL;
PFNGLISVERTEXARRAYPROC glad_glIsVertexArray = NULL;
PFNGLLINEWIDTHPROC glad_glLineWidth = NULL;
PFNGLLINKPROGRAMPROC glad_glLinkProgram = NULL;
PFNGLLOGICOPPROC glad_glLogicOp = NULL;
PFNGLMAPBUFFERPROC glad_glMapBuffer = NULL;
PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange = NULL;
PFNGLMULTIDRAWARRAYSPROC glad_glMultiDrawArrays = NULL;
PFNGLMULTIDRAWELEMENTSPROC glad_glMultiDrawElements = NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glad_glMultiDrawElementsBaseVertex = NULL;
PFNGLMULTITEXCOORDP1UIPROC glad_glMultiTexCoordP1ui = NULL;
PFNGLMULTITEXCOORDP1UIVPROC glad_glMultiTexCoordP1uiv = NULL;
PFNGLMULTITEXCOORDP2UIPROC glad_glMultiTexCoordP2ui = NULL;
PFNGLMULTITEXCOORDP2UIVPROC glad_glMultiTexCoordP2uiv = NULL;
PFNGLMULTITEXCOORDP3UIPROC glad_glMultiTexCoordP3ui = NULL;
PFNGLMULTITEXCOORDP3UIVPROC glad_glMultiTexCoordP3uiv = NULL;
PFNGLMULTITEXCOORDP4UIPROC glad_glMultiTexCoordP4ui = NULL;
PFNGLMULTITEXCOORDP4UIVPROC glad_glMultiTexCoordP4uiv = NULL;
PFNGLNORMALP3UIPROC glad_glNormalP3ui = NULL;
PFNGLNORMALP3UIVPROC glad_glNormalP3uiv = NULL;
PFNGLPIXELSTOREFPROC glad_glPixelStoref = NULL;
PFNGLPIXELSTOREIPROC glad_glPixelStorei = NULL;
PFNGLPOINTPARAMETERFPROC glad_glPointParameterf = NULL;
PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv = NULL;
PFNGLPOINTPARAMETERIPROC glad_glPointParameteri = NULL;
PFNGLPOINTPARAMETERIVPROC glad_glPointParameteriv = NULL;
PFNGLPOINTSIZEPROC glad_glPointSize = NULL;
PFNGLPOLYGONMODEPROC glad_glPolygonMode = NULL;
PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset = NULL;
PFNGLPRIMITIVERESTARTINDEXPROC glad_glPrimitiveRestartIndex = NULL;
PFNGLPROVOKINGVERTEXPROC glad_glProvokingVertex = NULL;
PFNGLQUERYCOUNTERPROC glad_glQueryCounter = NULL;
PFNGLREADBUFFERPROC glad_glReadBuffer = NULL;
PFNGLREADPIXELSPROC glad_glReadPixels = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample = NULL;
PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage = NULL;
PFNGLSAMPLEMASKIPROC glad_glSampleMaski = NULL;
PFNGLSAMPLERPARAMETERIIVPROC glad_glSamplerParameterIiv = NULL;
PFNGLSAMPLERPARAMETERIUIVPROC glad_glSamplerParameterIuiv = NULL;
PFNGLSAMPLERPARAMETERFPROC glad_glSamplerParameterf = NULL;
PFNGLSAMPLERPARAMETERFVPROC glad_glSamplerParameterfv = NULL;
PFNGLSAMPLERPARAMETERIPROC glad_glSamplerParameteri = NULL;
PFNGLSAMPLERPARAMETERIVPROC glad_glSamplerParameteriv = NULL;
PFNGLSCISSORPROC glad_glScissor = NULL;
PFNGLSECONDARYCOLORP3UIPROC glad_glSecondaryColorP3ui = NULL;
PFNGLSECONDARYCOLORP3UIVPROC glad_glSecondaryColorP3uiv = NULL;
PFNGLSHADERSOURCEPROC glad_glShaderSource = NULL;
PFNGLSTENCILFUNCPROC glad_glStencilFunc = NULL;
PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate = NULL;
PFNGLSTENCILMASKPROC glad_glStencilMask = NULL;
PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate = NULL;
PFNGLSTENCILOPPROC glad_glStencilOp = NULL;
PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate = NULL;
PFNGLTEXBUFFERPROC glad_glTexBuffer = NULL;
PFNGLTEXCOORDP1UIPROC glad_glTexCoordP1ui = NULL;
PFNGLTEXCOORDP1UIVPROC glad_glTexCoordP1uiv = NULL;
PFNGLTEXCOORDP2UIPROC glad_glTexCoordP2ui = NULL;
PFNGLTEXCOORDP2UIVPROC glad_glTexCoordP2uiv = NULL;
PFNGLTEXCOORDP3UIPROC glad_glTexCoordP3ui = NULL;
PFNGLTEXCOORDP3UIVPROC glad_glTexCoordP3uiv = NULL;
PFNGLTEXCOORDP4UIPROC glad_glTexCoordP4ui = NULL;
PFNGLTEXCOORDP4UIVPROC glad_glTexCoordP4uiv = NULL;
PFNGLTEXIMAGE1DPROC glad_glTexImage1D = NULL;
PFNGLTEXIMAGE2DPROC glad_glTexImage2D = NULL;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glad_glTexImage2DMultisample = NULL;
PFNGLTEXIMAGE3DPROC glad_glTexImage3D = NULL;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glad_glTexImage3DMultisample = NULL;
PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv = NULL;
PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv = NULL;
PFNGLTEXPARAMETERFPROC glad_glTexParameterf = NULL;
PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv = NULL;
PFNGLTEXPARAMETERIPROC glad_glTexParameteri = NULL;
PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv = NULL;
PFNGLTEXSUBIMAGE1DPROC glad_glTexSubImage1D = NULL;
PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D = NULL;
PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings = NULL;
PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
PFNGLUNIFORM1FVPROC glad_glUniform1fv = NULL;
PFNGLUNIFORM1IPROC glad_glUniform1i = NULL;
PFNGLUNIFORM1IVPROC glad_glUniform1iv = NULL;
PFNGLUNIFORM1UIPROC glad_glUniform1ui = NULL;
PFNGLUNIFORM1UIVPROC glad_glUniform1uiv = NULL;
PFNGLUNIFORM2FPROC glad_glUniform2f = NULL;
PFNGLUNIFORM2FVPROC glad_glUniform2fv = NULL;
PFNGLUNIFORM2IPROC glad_glUniform2i = NULL;
PFNGLUNIFORM2IVPROC glad_glUniform2iv = NULL;
PFNGLUNIFORM2UIPROC glad_glUniform2ui = NULL;
PFNGLUNIFORM2UIVPROC glad_glUniform2uiv = NULL;
PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
PFNGLUNIFORM3FVPROC glad_glUniform3fv = NULL;
PFNGLUNIFORM3IPROC glad_glUniform3i = NULL;
PFNGLUNIFORM3IVPROC glad_glUniform3iv = NULL;
PFNGLUNIFORM3UIPROC glad_glUniform3ui = NULL;
PFNGLUNIFORM3UIVPROC glad_glUniform3uiv = NULL;
PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;
PFNGLUNIFORM4FVPROC glad_glUniform4fv = NULL;
PFNGLUNIFORM4IPROC glad_glUniform4i = NULL;
PFNGLUNIFORM4IVPROC glad_glUniform4iv = NULL;
PFNGLUNIFORM4UIPROC glad_glUniform4ui = NULL;
PFNGLUNIFORM4UIVPROC glad_glUniform4uiv = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding = NULL;
PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv = NULL;
PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv = NULL;
PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer = NULL;
PFNGLUSEPROGRAMPROC glad_glUseProgram = NULL;
PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram = NULL;
PFNGLVERTEXATTRIB1DPROC glad_glVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DVPROC glad_glVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1SPROC glad_glVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SVPROC glad_glVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB2DPROC glad_glVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DVPROC glad_glVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2SPROC glad_glVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SVPROC glad_glVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB3DPROC glad_glVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DVPROC glad_glVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3SPROC glad_glVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SVPROC glad_glVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB4NBVPROC glad_glVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NIVPROC glad_glVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NSVPROC glad_glVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NUBPROC glad_glVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBVPROC glad_glVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUIVPROC glad_glVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUSVPROC glad_glVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4BVPROC glad_glVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4DPROC glad_glVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DVPROC glad_glVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4IVPROC glad_glVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4SPROC glad_glVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SVPROC glad_glVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4UBVPROC glad_glVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UIVPROC glad_glVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4USVPROC glad_glVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor = NULL;
PFNGLVERTEXATTRIBI1IPROC glad_glVertexAttribI1i = NULL;
PFNGLVERTEXATTRIBI1IVPROC glad_glVertexAttribI1iv = NULL;
PFNGLVERTEXATTRIBI1UIPROC glad_glVertexAttribI1ui = NULL;
PFNGLVERTEXATTRIBI1UIVPROC glad_glVertexAttribI1uiv = NULL;
PFNGLVERTEXATTRIBI2IPROC glad_glVertexAttribI2i = NULL;
PFNGLVERTEXATTRIBI2IVPROC glad_glVertexAttribI2iv = NULL;
PFNGLVERTEXATTRIBI2UIPROC glad_glVertexAttribI2ui = NULL;
PFNGLVERTEXATTRIBI2UIVPROC glad_glVertexAttribI2uiv = NULL;
PFNGLVERTEXATTRIBI3IPROC glad_glVertexAttribI3i = NULL;
PFNGLVERTEXATTRIBI3IVPROC glad_glVertexAttribI3iv = NULL;
PFNGLVERTEXATTRIBI3UIPROC glad_glVertexAttribI3ui = NULL;
PFNGLVERTEXATTRIBI3UIVPROC glad_glVertexAttribI3uiv = NULL;
PFNGLVERTEXATTRIBI4BVPROC glad_glVertexAttribI4bv = NULL;
PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i = NULL;
PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv = NULL;
PFNGLVERTEXATTRIBI4SVPROC glad_glVertexAttribI4sv = NULL;
PFNGLVERTEXATTRIBI4UBVPROC glad_glVertexAttribI4ubv = NULL;
PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui = NULL;
PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv = NULL;
PFNGLVERTEXATTRIBI4USVPROC glad_glVertexAttribI4usv = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer = NULL;
PFNGLVERTEXATTRIBP1UIPROC glad_glVertexAttribP1ui = NULL;
PFNGLVERTEXATTRIBP1UIVPROC glad_glVertexAttribP1uiv = NULL;
PFNGLVERTEXATTRIBP2UIPROC glad_glVertexAttribP2ui = NULL;
PFNGLVERTEXATTRIBP2UIVPROC glad_glVertexAttribP2uiv = NULL;
PFNGLVERTEXATTRIBP3UIPROC glad_glVertexAttribP3ui = NULL;
PFNGLVERTEXATTRIBP3UIVPROC glad_glVertexAttribP3uiv = NULL;
PFNGLVERTEXATTRIBP4UIPROC glad_glVertexAttribP4ui = NULL;
PFNGLVERTEXATTRIBP4UIVPROC glad_glVertexAttribP4uiv = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = NULL;
PFNGLVERTEXP2UIPROC glad_glVertexP2ui = NULL;
PFNGLVERTEXP2UIVPROC glad_glVertexP2uiv = NULL;
PFNGLVERTEXP3UIPROC glad_glVertexP3ui = NULL;
PFNGLVERTEXP3UIVPROC glad_glVertexP3uiv = NULL;
PFNGLVERTEXP4UIPROC glad_glVertexP4ui = NULL;
PFNGLVERTEXP4UIVPROC glad_glVertexP4uiv = NULL;
PFNGLVIEWPORTPROC glad_glViewport = NULL;
PFNGLWAITSYNCPROC glad_glWaitSync = NULL;
static void load_GL_VERSION_1_0( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_1_0) return;
	glad_glCullFace = (PFNGLCULLFACEPROC)load( "glCullFace" );
	glad_glFrontFace = (PFNGLFRONTFACEPROC)load( "glFrontFace" );
	glad_glHint = (PFNGLHINTPROC)load( "glHint" );
	glad_glLineWidth = (PFNGLLINEWIDTHPROC)load( "glLineWidth" );
	glad_glPointSize = (PFNGLPOINTSIZEPROC)load( "glPointSize" );
	glad_glPolygonMode = (PFNGLPOLYGONMODEPROC)load( "glPolygonMode" );
	glad_glScissor = (PFNGLSCISSORPROC)load( "glScissor" );
	glad_glTexParameterf = (PFNGLTEXPARAMETERFPROC)load( "glTexParameterf" );
	glad_glTexParameterfv = (PFNGLTEXPARAMETERFVPROC)load( "glTexParameterfv" );
	glad_glTexParameteri = (PFNGLTEXPARAMETERIPROC)load( "glTexParameteri" );
	glad_glTexParameteriv = (PFNGLTEXPARAMETERIVPROC)load( "glTexParameteriv" );
	glad_glTexImage1D = (PFNGLTEXIMAGE1DPROC)load( "glTexImage1D" );
	glad_glTexImage2D = (PFNGLTEXIMAGE2DPROC)load( "glTexImage2D" );
	glad_glDrawBuffer = (PFNGLDRAWBUFFERPROC)load( "glDrawBuffer" );
	glad_glClear = (PFNGLCLEARPROC)load( "glClear" );
	glad_glClearColor = (PFNGLCLEARCOLORPROC)load( "glClearColor" );
	glad_glClearStencil = (PFNGLCLEARSTENCILPROC)load( "glClearStencil" );
	glad_glClearDepth = (PFNGLCLEARDEPTHPROC)load( "glClearDepth" );
	glad_glStencilMask = (PFNGLSTENCILMASKPROC)load( "glStencilMask" );
	glad_glColorMask = (PFNGLCOLORMASKPROC)load( "glColorMask" );
	glad_glDepthMask = (PFNGLDEPTHMASKPROC)load( "glDepthMask" );
	glad_glDisable = (PFNGLDISABLEPROC)load( "glDisable" );
	glad_glEnable = (PFNGLENABLEPROC)load( "glEnable" );
	glad_glFinish = (PFNGLFINISHPROC)load( "glFinish" );
	glad_glFlush = (PFNGLFLUSHPROC)load( "glFlush" );
	glad_glBlendFunc = (PFNGLBLENDFUNCPROC)load( "glBlendFunc" );
	glad_glLogicOp = (PFNGLLOGICOPPROC)load( "glLogicOp" );
	glad_glStencilFunc = (PFNGLSTENCILFUNCPROC)load( "glStencilFunc" );
	glad_glStencilOp = (PFNGLSTENCILOPPROC)load( "glStencilOp" );
	glad_glDepthFunc = (PFNGLDEPTHFUNCPROC)load( "glDepthFunc" );
	glad_glPixelStoref = (PFNGLPIXELSTOREFPROC)load( "glPixelStoref" );
	glad_glPixelStorei = (PFNGLPIXELSTOREIPROC)load( "glPixelStorei" );
	glad_glReadBuffer = (PFNGLREADBUFFERPROC)load( "glReadBuffer" );
	glad_glReadPixels = (PFNGLREADPIXELSPROC)load( "glReadPixels" );
	glad_glGetBooleanv = (PFNGLGETBOOLEANVPROC)load( "glGetBooleanv" );
	glad_glGetDoublev = (PFNGLGETDOUBLEVPROC)load( "glGetDoublev" );
	glad_glGetError = (PFNGLGETERRORPROC)load( "glGetError" );
	glad_glGetFloatv = (PFNGLGETFLOATVPROC)load( "glGetFloatv" );
	glad_glGetIntegerv = (PFNGLGETINTEGERVPROC)load( "glGetIntegerv" );
	glad_glGetString = (PFNGLGETSTRINGPROC)load( "glGetString" );
	glad_glGetTexImage = (PFNGLGETTEXIMAGEPROC)load( "glGetTexImage" );
	glad_glGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC)load( "glGetTexParameterfv" );
	glad_glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC)load( "glGetTexParameteriv" );
	glad_glGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC)load( "glGetTexLevelParameterfv" );
	glad_glGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC)load( "glGetTexLevelParameteriv" );
	glad_glIsEnabled = (PFNGLISENABLEDPROC)load( "glIsEnabled" );
	glad_glDepthRange = (PFNGLDEPTHRANGEPROC)load( "glDepthRange" );
	glad_glViewport = (PFNGLVIEWPORTPROC)load( "glViewport" );
}
static void load_GL_VERSION_1_1( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_1_1) return;
	glad_glDrawArrays = (PFNGLDRAWARRAYSPROC)load( "glDrawArrays" );
	glad_glDrawElements = (PFNGLDRAWELEMENTSPROC)load( "glDrawElements" );
	glad_glPolygonOffset = (PFNGLPOLYGONOFFSETPROC)load( "glPolygonOffset" );
	glad_glCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC)load( "glCopyTexImage1D" );
	glad_glCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC)load( "glCopyTexImage2D" );
	glad_glCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC)load( "glCopyTexSubImage1D" );
	glad_glCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC)load( "glCopyTexSubImage2D" );
	glad_glTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC)load( "glTexSubImage1D" );
	glad_glTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC)load( "glTexSubImage2D" );
	glad_glBindTexture = (PFNGLBINDTEXTUREPROC)load( "glBindTexture" );
	glad_glDeleteTextures = (PFNGLDELETETEXTURESPROC)load( "glDeleteTextures" );
	glad_glGenTextures = (PFNGLGENTEXTURESPROC)load( "glGenTextures" );
	glad_glIsTexture = (PFNGLISTEXTUREPROC)load( "glIsTexture" );
}
static void load_GL_VERSION_1_2( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_1_2) return;
	glad_glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)load( "glDrawRangeElements" );
	glad_glTexImage3D = (PFNGLTEXIMAGE3DPROC)load( "glTexImage3D" );
	glad_glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)load( "glTexSubImage3D" );
	glad_glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC)load( "glCopyTexSubImage3D" );
}
static void load_GL_VERSION_1_3( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_1_3) return;
	glad_glActiveTexture = (PFNGLACTIVETEXTUREPROC)load( "glActiveTexture" );
	glad_glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC)load( "glSampleCoverage" );
	glad_glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC)load( "glCompressedTexImage3D" );
	glad_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)load( "glCompressedTexImage2D" );
	glad_glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC)load( "glCompressedTexImage1D" );
	glad_glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)load( "glCompressedTexSubImage3D" );
	glad_glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)load( "glCompressedTexSubImage2D" );
	glad_glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)load( "glCompressedTexSubImage1D" );
	glad_glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC)load( "glGetCompressedTexImage" );
}
static void load_GL_VERSION_1_4( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_1_4) return;
	glad_glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)load( "glBlendFuncSeparate" );
	glad_glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)load( "glMultiDrawArrays" );
	glad_glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)load( "glMultiDrawElements" );
	glad_glPointParameterf = (PFNGLPOINTPARAMETERFPROC)load( "glPointParameterf" );
	glad_glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC)load( "glPointParameterfv" );
	glad_glPointParameteri = (PFNGLPOINTPARAMETERIPROC)load( "glPointParameteri" );
	glad_glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC)load( "glPointParameteriv" );
	glad_glBlendColor = (PFNGLBLENDCOLORPROC)load( "glBlendColor" );
	glad_glBlendEquation = (PFNGLBLENDEQUATIONPROC)load( "glBlendEquation" );
}
static void load_GL_VERSION_1_5( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_1_5) return;
	glad_glGenQueries = (PFNGLGENQUERIESPROC)load( "glGenQueries" );
	glad_glDeleteQueries = (PFNGLDELETEQUERIESPROC)load( "glDeleteQueries" );
	glad_glIsQuery = (PFNGLISQUERYPROC)load( "glIsQuery" );
	glad_glBeginQuery = (PFNGLBEGINQUERYPROC)load( "glBeginQuery" );
	glad_glEndQuery = (PFNGLENDQUERYPROC)load( "glEndQuery" );
	glad_glGetQueryiv = (PFNGLGETQUERYIVPROC)load( "glGetQueryiv" );
	glad_glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC)load( "glGetQueryObjectiv" );
	glad_glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC)load( "glGetQueryObjectuiv" );
	glad_glBindBuffer = (PFNGLBINDBUFFERPROC)load( "glBindBuffer" );
	glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)load( "glDeleteBuffers" );
	glad_glGenBuffers = (PFNGLGENBUFFERSPROC)load( "glGenBuffers" );
	glad_glIsBuffer = (PFNGLISBUFFERPROC)load( "glIsBuffer" );
	glad_glBufferData = (PFNGLBUFFERDATAPROC)load( "glBufferData" );
	glad_glBufferSubData = (PFNGLBUFFERSUBDATAPROC)load( "glBufferSubData" );
	glad_glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)load( "glGetBufferSubData" );
	glad_glMapBuffer = (PFNGLMAPBUFFERPROC)load( "glMapBuffer" );
	glad_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)load( "glUnmapBuffer" );
	glad_glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)load( "glGetBufferParameteriv" );
	glad_glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)load( "glGetBufferPointerv" );
}
static void load_GL_VERSION_2_0( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_2_0) return;
	glad_glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)load( "glBlendEquationSeparate" );
	glad_glDrawBuffers = (PFNGLDRAWBUFFERSPROC)load( "glDrawBuffers" );
	glad_glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC)load( "glStencilOpSeparate" );
	glad_glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC)load( "glStencilFuncSeparate" );
	glad_glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC)load( "glStencilMaskSeparate" );
	glad_glAttachShader = (PFNGLATTACHSHADERPROC)load( "glAttachShader" );
	glad_glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)load( "glBindAttribLocation" );
	glad_glCompileShader = (PFNGLCOMPILESHADERPROC)load( "glCompileShader" );
	glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC)load( "glCreateProgram" );
	glad_glCreateShader = (PFNGLCREATESHADERPROC)load( "glCreateShader" );
	glad_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)load( "glDeleteProgram" );
	glad_glDeleteShader = (PFNGLDELETESHADERPROC)load( "glDeleteShader" );
	glad_glDetachShader = (PFNGLDETACHSHADERPROC)load( "glDetachShader" );
	glad_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)load( "glDisableVertexAttribArray" );
	glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)load( "glEnableVertexAttribArray" );
	glad_glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)load( "glGetActiveAttrib" );
	glad_glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)load( "glGetActiveUniform" );
	glad_glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)load( "glGetAttachedShaders" );
	glad_glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)load( "glGetAttribLocation" );
	glad_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)load( "glGetProgramiv" );
	glad_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)load( "glGetProgramInfoLog" );
	glad_glGetShaderiv = (PFNGLGETSHADERIVPROC)load( "glGetShaderiv" );
	glad_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)load( "glGetShaderInfoLog" );
	glad_glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)load( "glGetShaderSource" );
	glad_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)load( "glGetUniformLocation" );
	glad_glGetUniformfv = (PFNGLGETUNIFORMFVPROC)load( "glGetUniformfv" );
	glad_glGetUniformiv = (PFNGLGETUNIFORMIVPROC)load( "glGetUniformiv" );
	glad_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)load( "glGetVertexAttribdv" );
	glad_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)load( "glGetVertexAttribfv" );
	glad_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)load( "glGetVertexAttribiv" );
	glad_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)load( "glGetVertexAttribPointerv" );
	glad_glIsProgram = (PFNGLISPROGRAMPROC)load( "glIsProgram" );
	glad_glIsShader = (PFNGLISSHADERPROC)load( "glIsShader" );
	glad_glLinkProgram = (PFNGLLINKPROGRAMPROC)load( "glLinkProgram" );
	glad_glShaderSource = (PFNGLSHADERSOURCEPROC)load( "glShaderSource" );
	glad_glUseProgram = (PFNGLUSEPROGRAMPROC)load( "glUseProgram" );
	glad_glUniform1f = (PFNGLUNIFORM1FPROC)load( "glUniform1f" );
	glad_glUniform2f = (PFNGLUNIFORM2FPROC)load( "glUniform2f" );
	glad_glUniform3f = (PFNGLUNIFORM3FPROC)load( "glUniform3f" );
	glad_glUniform4f = (PFNGLUNIFORM4FPROC)load( "glUniform4f" );
	glad_glUniform1i = (PFNGLUNIFORM1IPROC)load( "glUniform1i" );
	glad_glUniform2i = (PFNGLUNIFORM2IPROC)load( "glUniform2i" );
	glad_glUniform3i = (PFNGLUNIFORM3IPROC)load( "glUniform3i" );
	glad_glUniform4i = (PFNGLUNIFORM4IPROC)load( "glUniform4i" );
	glad_glUniform1fv = (PFNGLUNIFORM1FVPROC)load( "glUniform1fv" );
	glad_glUniform2fv = (PFNGLUNIFORM2FVPROC)load( "glUniform2fv" );
	glad_glUniform3fv = (PFNGLUNIFORM3FVPROC)load( "glUniform3fv" );
	glad_glUniform4fv = (PFNGLUNIFORM4FVPROC)load( "glUniform4fv" );
	glad_glUniform1iv = (PFNGLUNIFORM1IVPROC)load( "glUniform1iv" );
	glad_glUniform2iv = (PFNGLUNIFORM2IVPROC)load( "glUniform2iv" );
	glad_glUniform3iv = (PFNGLUNIFORM3IVPROC)load( "glUniform3iv" );
	glad_glUniform4iv = (PFNGLUNIFORM4IVPROC)load( "glUniform4iv" );
	glad_glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)load( "glUniformMatrix2fv" );
	glad_glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)load( "glUniformMatrix3fv" );
	glad_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)load( "glUniformMatrix4fv" );
	glad_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)load( "glValidateProgram" );
	glad_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)load( "glVertexAttrib1d" );
	glad_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)load( "glVertexAttrib1dv" );
	glad_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)load( "glVertexAttrib1f" );
	glad_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)load( "glVertexAttrib1fv" );
	glad_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)load( "glVertexAttrib1s" );
	glad_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)load( "glVertexAttrib1sv" );
	glad_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)load( "glVertexAttrib2d" );
	glad_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)load( "glVertexAttrib2dv" );
	glad_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)load( "glVertexAttrib2f" );
	glad_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)load( "glVertexAttrib2fv" );
	glad_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)load( "glVertexAttrib2s" );
	glad_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)load( "glVertexAttrib2sv" );
	glad_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)load( "glVertexAttrib3d" );
	glad_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)load( "glVertexAttrib3dv" );
	glad_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)load( "glVertexAttrib3f" );
	glad_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)load( "glVertexAttrib3fv" );
	glad_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)load( "glVertexAttrib3s" );
	glad_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)load( "glVertexAttrib3sv" );
	glad_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)load( "glVertexAttrib4Nbv" );
	glad_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)load( "glVertexAttrib4Niv" );
	glad_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)load( "glVertexAttrib4Nsv" );
	glad_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)load( "glVertexAttrib4Nub" );
	glad_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)load( "glVertexAttrib4Nubv" );
	glad_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)load( "glVertexAttrib4Nuiv" );
	glad_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)load( "glVertexAttrib4Nusv" );
	glad_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)load( "glVertexAttrib4bv" );
	glad_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)load( "glVertexAttrib4d" );
	glad_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)load( "glVertexAttrib4dv" );
	glad_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)load( "glVertexAttrib4f" );
	glad_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)load( "glVertexAttrib4fv" );
	glad_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)load( "glVertexAttrib4iv" );
	glad_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)load( "glVertexAttrib4s" );
	glad_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)load( "glVertexAttrib4sv" );
	glad_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)load( "glVertexAttrib4ubv" );
	glad_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)load( "glVertexAttrib4uiv" );
	glad_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)load( "glVertexAttrib4usv" );
	glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)load( "glVertexAttribPointer" );
}
static void load_GL_VERSION_2_1( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_2_1) return;
	glad_glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC)load( "glUniformMatrix2x3fv" );
	glad_glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC)load( "glUniformMatrix3x2fv" );
	glad_glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC)load( "glUniformMatrix2x4fv" );
	glad_glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC)load( "glUniformMatrix4x2fv" );
	glad_glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC)load( "glUniformMatrix3x4fv" );
	glad_glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC)load( "glUniformMatrix4x3fv" );
}
static void load_GL_VERSION_3_0( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_3_0) return;
	glad_glColorMaski = (PFNGLCOLORMASKIPROC)load( "glColorMaski" );
	glad_glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)load( "glGetBooleani_v" );
	glad_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)load( "glGetIntegeri_v" );
	glad_glEnablei = (PFNGLENABLEIPROC)load( "glEnablei" );
	glad_glDisablei = (PFNGLDISABLEIPROC)load( "glDisablei" );
	glad_glIsEnabledi = (PFNGLISENABLEDIPROC)load( "glIsEnabledi" );
	glad_glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)load( "glBeginTransformFeedback" );
	glad_glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC)load( "glEndTransformFeedback" );
	glad_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)load( "glBindBufferRange" );
	glad_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)load( "glBindBufferBase" );
	glad_glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC)load( "glTransformFeedbackVaryings" );
	glad_glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)load( "glGetTransformFeedbackVarying" );
	glad_glClampColor = (PFNGLCLAMPCOLORPROC)load( "glClampColor" );
	glad_glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)load( "glBeginConditionalRender" );
	glad_glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC)load( "glEndConditionalRender" );
	glad_glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)load( "glVertexAttribIPointer" );
	glad_glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC)load( "glGetVertexAttribIiv" );
	glad_glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC)load( "glGetVertexAttribIuiv" );
	glad_glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC)load( "glVertexAttribI1i" );
	glad_glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC)load( "glVertexAttribI2i" );
	glad_glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC)load( "glVertexAttribI3i" );
	glad_glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC)load( "glVertexAttribI4i" );
	glad_glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC)load( "glVertexAttribI1ui" );
	glad_glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC)load( "glVertexAttribI2ui" );
	glad_glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC)load( "glVertexAttribI3ui" );
	glad_glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC)load( "glVertexAttribI4ui" );
	glad_glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC)load( "glVertexAttribI1iv" );
	glad_glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC)load( "glVertexAttribI2iv" );
	glad_glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC)load( "glVertexAttribI3iv" );
	glad_glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC)load( "glVertexAttribI4iv" );
	glad_glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC)load( "glVertexAttribI1uiv" );
	glad_glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC)load( "glVertexAttribI2uiv" );
	glad_glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC)load( "glVertexAttribI3uiv" );
	glad_glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC)load( "glVertexAttribI4uiv" );
	glad_glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC)load( "glVertexAttribI4bv" );
	glad_glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC)load( "glVertexAttribI4sv" );
	glad_glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC)load( "glVertexAttribI4ubv" );
	glad_glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC)load( "glVertexAttribI4usv" );
	glad_glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC)load( "glGetUniformuiv" );
	glad_glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)load( "glBindFragDataLocation" );
	glad_glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC)load( "glGetFragDataLocation" );
	glad_glUniform1ui = (PFNGLUNIFORM1UIPROC)load( "glUniform1ui" );
	glad_glUniform2ui = (PFNGLUNIFORM2UIPROC)load( "glUniform2ui" );
	glad_glUniform3ui = (PFNGLUNIFORM3UIPROC)load( "glUniform3ui" );
	glad_glUniform4ui = (PFNGLUNIFORM4UIPROC)load( "glUniform4ui" );
	glad_glUniform1uiv = (PFNGLUNIFORM1UIVPROC)load( "glUniform1uiv" );
	glad_glUniform2uiv = (PFNGLUNIFORM2UIVPROC)load( "glUniform2uiv" );
	glad_glUniform3uiv = (PFNGLUNIFORM3UIVPROC)load( "glUniform3uiv" );
	glad_glUniform4uiv = (PFNGLUNIFORM4UIVPROC)load( "glUniform4uiv" );
	glad_glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC)load( "glTexParameterIiv" );
	glad_glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC)load( "glTexParameterIuiv" );
	glad_glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC)load( "glGetTexParameterIiv" );
	glad_glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC)load( "glGetTexParameterIuiv" );
	glad_glClearBufferiv = (PFNGLCLEARBUFFERIVPROC)load( "glClearBufferiv" );
	glad_glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC)load( "glClearBufferuiv" );
	glad_glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)load( "glClearBufferfv" );
	glad_glClearBufferfi = (PFNGLCLEARBUFFERFIPROC)load( "glClearBufferfi" );
	glad_glGetStringi = (PFNGLGETSTRINGIPROC)load( "glGetStringi" );
	glad_glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)load( "glIsRenderbuffer" );
	glad_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)load( "glBindRenderbuffer" );
	glad_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)load( "glDeleteRenderbuffers" );
	glad_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)load( "glGenRenderbuffers" );
	glad_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)load( "glRenderbufferStorage" );
	glad_glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)load( "glGetRenderbufferParameteriv" );
	glad_glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)load( "glIsFramebuffer" );
	glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)load( "glBindFramebuffer" );
	glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)load( "glDeleteFramebuffers" );
	glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)load( "glGenFramebuffers" );
	glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)load( "glCheckFramebufferStatus" );
	glad_glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)load( "glFramebufferTexture1D" );
	glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)load( "glFramebufferTexture2D" );
	glad_glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)load( "glFramebufferTexture3D" );
	glad_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)load( "glFramebufferRenderbuffer" );
	glad_glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)load( "glGetFramebufferAttachmentParameteriv" );
	glad_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)load( "glGenerateMipmap" );
	glad_glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)load( "glBlitFramebuffer" );
	glad_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)load( "glRenderbufferStorageMultisample" );
	glad_glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)load( "glFramebufferTextureLayer" );
	glad_glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)load( "glMapBufferRange" );
	glad_glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)load( "glFlushMappedBufferRange" );
	glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)load( "glBindVertexArray" );
	glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)load( "glDeleteVertexArrays" );
	glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)load( "glGenVertexArrays" );
	glad_glIsVertexArray = (PFNGLISVERTEXARRAYPROC)load( "glIsVertexArray" );
}
static void load_GL_VERSION_3_1( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_3_1) return;
	glad_glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)load( "glDrawArraysInstanced" );
	glad_glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)load( "glDrawElementsInstanced" );
	glad_glTexBuffer = (PFNGLTEXBUFFERPROC)load( "glTexBuffer" );
	glad_glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)load( "glPrimitiveRestartIndex" );
	glad_glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)load( "glCopyBufferSubData" );
	glad_glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)load( "glGetUniformIndices" );
	glad_glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)load( "glGetActiveUniformsiv" );
	glad_glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)load( "glGetActiveUniformName" );
	glad_glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)load( "glGetUniformBlockIndex" );
	glad_glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)load( "glGetActiveUniformBlockiv" );
	glad_glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)load( "glGetActiveUniformBlockName" );
	glad_glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)load( "glUniformBlockBinding" );
	glad_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)load( "glBindBufferRange" );
	glad_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)load( "glBindBufferBase" );
	glad_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)load( "glGetIntegeri_v" );
}
static void load_GL_VERSION_3_2( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_3_2) return;
	glad_glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)load( "glDrawElementsBaseVertex" );
	glad_glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)load( "glDrawRangeElementsBaseVertex" );
	glad_glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)load( "glDrawElementsInstancedBaseVertex" );
	glad_glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)load( "glMultiDrawElementsBaseVertex" );
	glad_glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)load( "glProvokingVertex" );
	glad_glFenceSync = (PFNGLFENCESYNCPROC)load( "glFenceSync" );
	glad_glIsSync = (PFNGLISSYNCPROC)load( "glIsSync" );
	glad_glDeleteSync = (PFNGLDELETESYNCPROC)load( "glDeleteSync" );
	glad_glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)load( "glClientWaitSync" );
	glad_glWaitSync = (PFNGLWAITSYNCPROC)load( "glWaitSync" );
	glad_glGetInteger64v = (PFNGLGETINTEGER64VPROC)load( "glGetInteger64v" );
	glad_glGetSynciv = (PFNGLGETSYNCIVPROC)load( "glGetSynciv" );
	glad_glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC)load( "glGetInteger64i_v" );
	glad_glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC)load( "glGetBufferParameteri64v" );
	glad_glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)load( "glFramebufferTexture" );
	glad_glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)load( "glTexImage2DMultisample" );
	glad_glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)load( "glTexImage3DMultisample" );
	glad_glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC)load( "glGetMultisamplefv" );
	glad_glSampleMaski = (PFNGLSAMPLEMASKIPROC)load( "glSampleMaski" );
}
static void load_GL_VERSION_3_3( GLADloadproc load ) {
	if (!GLAD_GL_VERSION_3_3) return;
	glad_glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)load( "glBindFragDataLocationIndexed" );
	glad_glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC)load( "glGetFragDataIndex" );
	glad_glGenSamplers = (PFNGLGENSAMPLERSPROC)load( "glGenSamplers" );
	glad_glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)load( "glDeleteSamplers" );
	glad_glIsSampler = (PFNGLISSAMPLERPROC)load( "glIsSampler" );
	glad_glBindSampler = (PFNGLBINDSAMPLERPROC)load( "glBindSampler" );
	glad_glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)load( "glSamplerParameteri" );
	glad_glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)load( "glSamplerParameteriv" );
	glad_glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)load( "glSamplerParameterf" );
	glad_glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)load( "glSamplerParameterfv" );
	glad_glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)load( "glSamplerParameterIiv" );
	glad_glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)load( "glSamplerParameterIuiv" );
	glad_glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)load( "glGetSamplerParameteriv" );
	glad_glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)load( "glGetSamplerParameterIiv" );
	glad_glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)load( "glGetSamplerParameterfv" );
	glad_glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)load( "glGetSamplerParameterIuiv" );
	glad_glQueryCounter = (PFNGLQUERYCOUNTERPROC)load( "glQueryCounter" );
	glad_glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC)load( "glGetQueryObjecti64v" );
	glad_glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC)load( "glGetQueryObjectui64v" );
	glad_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)load( "glVertexAttribDivisor" );
	glad_glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC)load( "glVertexAttribP1ui" );
	glad_glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC)load( "glVertexAttribP1uiv" );
	glad_glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC)load( "glVertexAttribP2ui" );
	glad_glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC)load( "glVertexAttribP2uiv" );
	glad_glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC)load( "glVertexAttribP3ui" );
	glad_glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC)load( "glVertexAttribP3uiv" );
	glad_glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC)load( "glVertexAttribP4ui" );
	glad_glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC)load( "glVertexAttribP4uiv" );
	glad_glVertexP2ui = (PFNGLVERTEXP2UIPROC)load( "glVertexP2ui" );
	glad_glVertexP2uiv = (PFNGLVERTEXP2UIVPROC)load( "glVertexP2uiv" );
	glad_glVertexP3ui = (PFNGLVERTEXP3UIPROC)load( "glVertexP3ui" );
	glad_glVertexP3uiv = (PFNGLVERTEXP3UIVPROC)load( "glVertexP3uiv" );
	glad_glVertexP4ui = (PFNGLVERTEXP4UIPROC)load( "glVertexP4ui" );
	glad_glVertexP4uiv = (PFNGLVERTEXP4UIVPROC)load( "glVertexP4uiv" );
	glad_glTexCoordP1ui = (PFNGLTEXCOORDP1UIPROC)load( "glTexCoordP1ui" );
	glad_glTexCoordP1uiv = (PFNGLTEXCOORDP1UIVPROC)load( "glTexCoordP1uiv" );
	glad_glTexCoordP2ui = (PFNGLTEXCOORDP2UIPROC)load( "glTexCoordP2ui" );
	glad_glTexCoordP2uiv = (PFNGLTEXCOORDP2UIVPROC)load( "glTexCoordP2uiv" );
	glad_glTexCoordP3ui = (PFNGLTEXCOORDP3UIPROC)load( "glTexCoordP3ui" );
	glad_glTexCoordP3uiv = (PFNGLTEXCOORDP3UIVPROC)load( "glTexCoordP3uiv" );
	glad_glTexCoordP4ui = (PFNGLTEXCOORDP4UIPROC)load( "glTexCoordP4ui" );
	glad_glTexCoordP4uiv = (PFNGLTEXCOORDP4UIVPROC)load( "glTexCoordP4uiv" );
	glad_glMultiTexCoordP1ui = (PFNGLMULTITEXCOORDP1UIPROC)load( "glMultiTexCoordP1ui" );
	glad_glMultiTexCoordP1uiv = (PFNGLMULTITEXCOORDP1UIVPROC)load( "glMultiTexCoordP1uiv" );
	glad_glMultiTexCoordP2ui = (PFNGLMULTITEXCOORDP2UIPROC)load( "glMultiTexCoordP2ui" );
	glad_glMultiTexCoordP2uiv = (PFNGLMULTITEXCOORDP2UIVPROC)load( "glMultiTexCoordP2uiv" );
	glad_glMultiTexCoordP3ui = (PFNGLMULTITEXCOORDP3UIPROC)load( "glMultiTexCoordP3ui" );
	glad_glMultiTexCoordP3uiv = (PFNGLMULTITEXCOORDP3UIVPROC)load( "glMultiTexCoordP3uiv" );
	glad_glMultiTexCoordP4ui = (PFNGLMULTITEXCOORDP4UIPROC)load( "glMultiTexCoordP4ui" );
	glad_glMultiTexCoordP4uiv = (PFNGLMULTITEXCOORDP4UIVPROC)load( "glMultiTexCoordP4uiv" );
	glad_glNormalP3ui = (PFNGLNORMALP3UIPROC)load( "glNormalP3ui" );
	glad_glNormalP3uiv = (PFNGLNORMALP3UIVPROC)load( "glNormalP3uiv" );
	glad_glColorP3ui = (PFNGLCOLORP3UIPROC)load( "glColorP3ui" );
	glad_glColorP3uiv = (PFNGLCOLORP3UIVPROC)load( "glColorP3uiv" );
	glad_glColorP4ui = (PFNGLCOLORP4UIPROC)load( "glColorP4ui" );
	glad_glColorP4uiv = (PFNGLCOLORP4UIVPROC)load( "glColorP4uiv" );
	glad_glSecondaryColorP3ui = (PFNGLSECONDARYCOLORP3UIPROC)load( "glSecondaryColorP3ui" );
	glad_glSecondaryColorP3uiv = (PFNGLSECONDARYCOLORP3UIVPROC)load( "glSecondaryColorP3uiv" );
}
static int find_extensionsGL( void ) {
	if (!get_exts()) return 0;
	(void)&has_ext;
	free_exts();
	return 1;
}

static void find_coreGL( void ) {

	/* Thank you @elmindreda
	 * https://github.com/elmindreda/greg/blob/master/templates/greg.c.in#L176
	 * https://github.com/glfw/glfw/blob/master/src/context.c#L36
	 */
	int i, major, minor;

	const char* version;
	const char* prefixes[] = {
		"OpenGL ES-CM ",
		"OpenGL ES-CL ",
		"OpenGL ES ",
		NULL
	};

	version = (const char*)glGetString( GL_VERSION );
	if (!version) return;

	for (i = 0; prefixes[i]; i++) {
		const size_t length = strlen( prefixes[i] );
		if (strncmp( version, prefixes[i], length ) == 0) {
			version += length;
			break;
		}
	}

	/* PR #18 */
#ifdef _MSC_VER
	sscanf_s( version, "%d.%d", &major, &minor );
#else
	sscanf( version, "%d.%d", &major, &minor );
#endif

	GLVersion.major = major; GLVersion.minor = minor;
	max_loaded_major = major; max_loaded_minor = minor;
	GLAD_GL_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
	GLAD_GL_VERSION_1_1 = (major == 1 && minor >= 1) || major > 1;
	GLAD_GL_VERSION_1_2 = (major == 1 && minor >= 2) || major > 1;
	GLAD_GL_VERSION_1_3 = (major == 1 && minor >= 3) || major > 1;
	GLAD_GL_VERSION_1_4 = (major == 1 && minor >= 4) || major > 1;
	GLAD_GL_VERSION_1_5 = (major == 1 && minor >= 5) || major > 1;
	GLAD_GL_VERSION_2_0 = (major == 2 && minor >= 0) || major > 2;
	GLAD_GL_VERSION_2_1 = (major == 2 && minor >= 1) || major > 2;
	GLAD_GL_VERSION_3_0 = (major == 3 && minor >= 0) || major > 3;
	GLAD_GL_VERSION_3_1 = (major == 3 && minor >= 1) || major > 3;
	GLAD_GL_VERSION_3_2 = (major == 3 && minor >= 2) || major > 3;
	GLAD_GL_VERSION_3_3 = (major == 3 && minor >= 3) || major > 3;
	if (GLVersion.major > 3 || (GLVersion.major >= 3 && GLVersion.minor >= 3)) {
		max_loaded_major = 3;
		max_loaded_minor = 3;
	}
}

int gladLoadGLLoader( GLADloadproc load ) {
	GLVersion.major = 0; GLVersion.minor = 0;
	glGetString = (PFNGLGETSTRINGPROC)load( "glGetString" );
	if (glGetString == NULL) return 0;
	if (glGetString( GL_VERSION ) == NULL) return 0;
	find_coreGL();
	load_GL_VERSION_1_0( load );
	load_GL_VERSION_1_1( load );
	load_GL_VERSION_1_2( load );
	load_GL_VERSION_1_3( load );
	load_GL_VERSION_1_4( load );
	load_GL_VERSION_1_5( load );
	load_GL_VERSION_2_0( load );
	load_GL_VERSION_2_1( load );
	load_GL_VERSION_3_0( load );
	load_GL_VERSION_3_1( load );
	load_GL_VERSION_3_2( load );
	load_GL_VERSION_3_3( load );

	if (!find_extensionsGL()) return 0;
	return GLVersion.major != 0 || GLVersion.minor != 0;
}

// EOF