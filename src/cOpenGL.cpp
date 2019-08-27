#include <windows.h>
#include "dbghelp.h"

#include <string>

#include "glm/gtx/transform.hpp"

#include "cApp.h"
#include "cOpenGL.h"
#include "cCamera.h"
#include "cModel.h"
#include "cVertexArray.h"
#include "cShader.h"
#include "rwkError.h"

#define RWK_GL_LOADER_IMPLEMENTATION
#include "rwkLoader.h"


///////////////////////////////////////////////////////////

cOpenGL::cOpenGL ()
{
	GLsizeiptr test = 0;
}

///////////////////////////////////////////////////////////

cOpenGL::~cOpenGL()
{
}

///////////////////////////////////////////////////////////

void cOpenGL::CreateGLDrawTypeMap()
{
	gl_draw_types_map = { {"GL_POINTS", GL_POINTS} , {"GL_LINE_STRIP", GL_LINE_STRIP}, {"GL_LINE_LOOP", GL_LINE_LOOP} ,{"GL_LINES", GL_LINES} ,{"GL_TRIANGLE_STRIP", GL_TRIANGLE_STRIP} ,{"GL_TRIANGLE_FAN", GL_TRIANGLE_FAN} ,{"GL_TRIANGLES", GL_TRIANGLES} };// , { "GL_QUAD_STRIP", GL_QUAD_STRIP }, { "GL_QUADS", GL_QUADS }, { "GL_POLYGON", GL_POLYGON }
}



///////////////////////////////////////////////////////////

void cOpenGL::Destroy()
{

}

///////////////////////////////////////////////////////////

bool cOpenGL::InitializeExtensions(HWND fake_hwnd)
{
	HDC fake_DC;
	PIXELFORMATDESCRIPTOR fake_pixel_format;
	int error;
	HGLRC fake_RC;
	bool result;


	// Get the device context for this temporary window.
	fake_DC = GetDC(fake_hwnd);
	if (!fake_DC)
	{
		return false;
	}

	ZeroMemory(&fake_pixel_format, sizeof(fake_pixel_format));
	fake_pixel_format.nSize = sizeof(fake_pixel_format);
	fake_pixel_format.nVersion = 1;
	fake_pixel_format.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fake_pixel_format.iPixelType = PFD_TYPE_RGBA;
	fake_pixel_format.cColorBits = 32;
	fake_pixel_format.cAlphaBits = 8;
	fake_pixel_format.cDepthBits = 24;

	const int fake_pixel_format_ID = ChoosePixelFormat(fake_DC, &fake_pixel_format);
	if (fake_pixel_format_ID == 0) {
		cApp::App()->ShowMessage("ChoosePixelFormat() failed.", " in InitializeExtensions(...)");
		return 1;
	}

	// Set a temporary default pixel format.
	error = SetPixelFormat(fake_DC, fake_pixel_format_ID, &fake_pixel_format);
	if (error != 1)
	{
		return false;
	}

	// Create a temporary rendering context.
	fake_RC = wglCreateContext(fake_DC);
	if (!fake_RC)
	{
		return false;
	}

	// Set the temporary rendering context as the current rendering context for this window.
	error = wglMakeCurrent(fake_DC, fake_RC);
	if (error != 1)
	{
		return false;
	}

	// Initialize the OpenGL extensions needed for this application.  Note that a temporary rendering context was needed to do so.
	result = LoadExtensionList();
	if (!result)
	{
		return false;
	}

	// initialize the opengl extensions
	rwk_loader_init();

	// Release the temporary rendering context now that the extensions have been loaded.
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(fake_RC);
	fake_RC = NULL;

	//// Release the device context for this window.
	ReleaseDC(fake_hwnd, fake_DC);
	fake_DC = 0;

	return true;
}

///////////////////////////////////////////////////////////

bool cOpenGL::InitializeOpenGL(HWND hwnd, cWindow* main_window, float screen_far, float screen_near, bool vsync)
{
	CreateGLDrawTypeMap();

	m_DC = GetDC(hwnd);

	const int pixelAttribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, 4,
		0
	};

	int pixelFormatID; UINT numFormats;
	const bool status = wglChoosePixelFormatARB(m_DC, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);

	if (status == false || numFormats == 0) {
		cApp::App()->ShowMessage("wglChoosePixelFormatARB() failed.");
		return false;
	}

	PIXELFORMATDESCRIPTOR PFD;
	DescribePixelFormat(m_DC, pixelFormatID, sizeof(PFD), &PFD);
	SetPixelFormat(m_DC, pixelFormatID, &PFD);

	const int major_min = 4, minor_min = 0;
	const int contextAttribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
//		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
		0
	};

	m_RC = wglCreateContextAttribsARB(m_DC, 0, contextAttribs);
	if (m_RC == NULL) {
		cApp::App()->ShowMessage("wglCreateContextAttribsARB() failed.");
		return false;
	}

	if (!wglMakeCurrent(m_DC, m_RC)) {
		cApp::App()->ShowMessage("wglMakeCurrent() failed.");
		return false;
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(rwkError::MessageCallback, 0);

	// Set the depth buffer to be entirely cleared to 1.0 values.
	glClearDepth(1.0f);

	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	char* vendorString, * rendererString;

	// Get the name of the video card.
	vendorString = (char*)glGetString(GL_VENDOR);
	rendererString = (char*)glGetString(GL_RENDERER);

	// Store the video card name in a class member variable so it can be retrieved later.
	strcpy_s(m_video_card_description, vendorString);
	strcat_s(m_video_card_description, " - ");
	strcat_s(m_video_card_description, rendererString);

	wglSwapIntervalEXT(1);
	
	m_main_window = main_window;
		
	return true;
}

///////////////////////////////////////////////////////////

bool cOpenGL::WGLExtensionSupported(const char* extension_name)
{
	// this is pointer to function which returns pointer to string with list of all wgl extensions
	PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

	// determine pointer to wglGetExtensionsStringEXT function
	_wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

	if (strstr(_wglGetExtensionsStringEXT(), extension_name) == NULL)
	{
		// string was not found
		return false;
	}

	// extension is supported
	return true;
}

///////////////////////////////////////////////////////////
 
void* cOpenGL::GetAnyGLFunctionAddress(const char* name)
{
	void* p = (void*)wglGetProcAddress(name);
	if (p == 0 ||
		(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
		(p == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void*)GetProcAddress(module, name);
	}

	return p;
}

///////////////////////////////////////////////////////////

void cOpenGL::BeginScene(float red, float green, float blue, float alpha)
{
	// Set the color to clear the screen to.
	glClearColor(red, green, blue, alpha);

	// Clear the screen and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return;
}

///////////////////////////////////////////////////////////


void cOpenGL::BeginScene(glm::vec4 clear_color)
{
	// Set the color to clear the screen to.
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);

	// Clear the screen and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return;
}

///////////////////////////////////////////////////////////

void cOpenGL::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	SwapBuffers(m_DC);

	return;
}

///////////////////////////////////////////////////////////

void cOpenGL::GetVideoCardInfo(char* cardName)
{
	strcpy_s(cardName, 128, m_video_card_description);
	return;
}

/////////////////////////////////////////////////////////

bool cOpenGL::LoadExtensionList()
{
	// Load the OpenGL extensions that this application will be using.
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	if (!wglChoosePixelFormatARB)
	{
		return false;
	}

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (!wglCreateContextAttribsARB)
	{
		return false;
	}

	glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback");
	if (!glDebugMessageCallback)
	{
		return false;
	}

	/*######### FOR REFERENCE ################
	if (WGLExtensionSupported("WGL_EXT_swap_control"))
	{
		// Extension is supported, init pointers.
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	}
	####################################*/

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (!wglSwapIntervalEXT)
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////
//
//bool cOpenGL::LoadExtensionList()
//{
//	// Load the OpenGL extensions that this application will be using.
//	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
//	if (!wglChoosePixelFormatARB)
//	{
//		return false;
//	}
//
//	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
//	if (!wglCreateContextAttribsARB)
//	{
//		return false;
//	}
//
//	/*######### FOR REFERENCE ################
//	if (WGLExtensionSupported("WGL_EXT_swap_control"))
//	{
//		// Extension is supported, init pointers.
//		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
//	}
//	####################################*/
//
//	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
//	if (!wglSwapIntervalEXT)
//	{
//		return false;
//	}
//
//	glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
//	if (!glGetStringi)
//	{
//		return false;
//	}
//
//	glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback");
//	if (!glDebugMessageCallback)
//	{
//		return false;
//	}
//
//	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
//	if (!glAttachShader)
//	{
//		return false;
//	}
//
//	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
//	if (!glBindBuffer)
//	{
//		return false;
//	}
//
//	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
//	if (!glBindVertexArray)
//	{
//		return false;
//	}
//
//	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
//	if (!glBufferData)
//	{
//		return false;
//	}
//
//	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
//	if (!glCompileShader)
//	{
//		return false;
//	}
//
//	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
//	if (!glCreateProgram)
//	{
//		return false;
//	}
//
//	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
//	if (!glCreateShader)
//	{
//		return false;
//	}
//
//	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
//	if (!glDeleteBuffers)
//	{
//		return false;
//	}
//
//	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
//	if (!glDeleteProgram)
//	{
//		return false;
//	}
//
//	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
//	if (!glDeleteShader)
//	{
//		return false;
//	}
//
//	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
//	if (!glDeleteVertexArrays)
//	{
//		return false;
//	}
//
//	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
//	if (!glDetachShader)
//	{
//		return false;
//	}
//
//	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
//	if (!glEnableVertexAttribArray)
//	{
//		return false;
//	}
//
//	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
//	if (!glGenBuffers)
//	{
//		return false;
//	}
//
//	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
//	if (!glGenVertexArrays)
//	{
//		return false;
//	}
//
//	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
//	if (!glGetAttribLocation)
//	{
//		return false;
//	}
//
//	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
//	if (!glGetProgramInfoLog)
//	{
//		return false;
//	}
//
//	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
//	if (!glGetProgramiv)
//	{
//		return false;
//	}
//
//	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
//	if (!glGetShaderInfoLog)
//	{
//		return false;
//	}
//
//	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
//	if (!glGetShaderiv)
//	{
//		return false;
//	}
//
//	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
//	if (!glLinkProgram)
//	{
//		return false;
//	}
//
//	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
//	if (!glShaderSource)
//	{
//		return false;
//	}
//
//	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
//	if (!glUseProgram)
//	{
//		return false;
//	}
//
//	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
//	if (!glVertexAttribPointer)
//	{
//		return false;
//	}
//
//	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
//	if (!glBindAttribLocation)
//	{
//		return false;
//	}
//
//	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
//	if (!glGetUniformLocation)
//	{
//		return false;
//	}
//
//	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
//	if (!glUniformMatrix4fv)
//	{
//		return false;
//	}
//
//	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
//	if (!glActiveTexture)
//	{
//		return false;
//	}
//
//	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
//	if (!glUniform1i)
//	{
//		return false;
//	}
//
//	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
//	if (!glGenerateMipmap)
//	{
//		return false;
//	}
//
//	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
//	if (!glDisableVertexAttribArray)
//	{
//		return false;
//	}
//
//	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
//	if (!glUniform3fv)
//	{
//		return false;
//	}
//
//	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
//	if (!glUniform4fv)
//	{
//		return false;
//	}
//
//	glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");
//	if (!glUniform4f)
//	{
//		return false;
//	}
//
//	glUniform1ui = (PFNGLUNIFORM1UIPROC)wglGetProcAddress("glUniform1ui");
//	if (!glUniform1ui)
//	{
//		return false;
//	}
//
//	glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
//	if (!glBlendEquation)
//	{
//		return false;
//	}
//
//	PFNGLDELETETEXTURESEXTPROC glDeleteTextures;
//
//	return true;
//}
