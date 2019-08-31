#ifndef  _RWK_OPENGL_H_
#define  _RWK_OPENGL_H_
#include <windows.h>

#include "gl/gl.h"
#include "gl/glext.h"
#include "gl/wglext.h"

#include <glm/glm.hpp>

#include <list>
#include <unordered_map>

class cWindow;

class cOpenGL
{
public:

	cOpenGL();
	~cOpenGL();

	void CreateGLDrawTypeMap();
	void Init();
	void Destroy();
	bool InitializeExtensions(HWND hwnd);
	bool InitializeOpenGL(HWND hwnd, cWindow* main_window, float screen_far, float screen_near, bool vsync);
	bool WGLExtensionSupported(const char* extension_name);
	void* GetAnyGLFunctionAddress(const char* name);
	void BeginScene(float red, float green, float blue, float alpha);
	void BeginScene(glm::vec4 clear_color);
	void EndScene();
	void GetVideoCardInfo(char* cardName);
	
	inline HDC Get_DC() { return m_DC; }
	inline HGLRC get_GLRC() { return m_RC; }
	inline cWindow* GetMainWindow() { return m_main_window; }
	inline std::unordered_map<std::string, int>* GetOGLDrawTypeMap() { return &gl_draw_types_map; }

private:

	HDC m_DC;
	HGLRC m_RC;

	cWindow* m_main_window;

	char m_video_card_description[128];

	std::unordered_map<std::string, int> gl_draw_types_map;

	bool LoadExtensionList();

	PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

public:

	/*PFNGLACTIVETEXTUREPROC glActiveTexture;
	PFNGLATTACHSHADERPROC glAttachShader;	
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
	PFNGLBINDBUFFERPROC glBindBuffer;
	PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
	PFNGLBINDSAMPLERPROC glBindSampler;
	PFNGLBINDTEXTURESPROC glBindTexture;
	PFNGLBLENDEQUATIONPROC glBlendEquation;
	PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
	PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
	PFNGLBUFFERDATAPROC glBufferData;
	PFNGLCOMPILESHADERPROC glCompileShader;
	PFNGLCREATEPROGRAMPROC glCreateProgram;
	PFNGLCREATESHADERPROC glCreateShader;
	PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers;
	PFNGLDELETEPROGRAMPROC glDeleteProgram;
	PFNGLDELETESHADERPROC glDeleteShader;
	PFNGLDELETETEXTURESEXTPROC glDeleteTextures;
	PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
	PFNGLDETACHSHADERPROC glDetachShader;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
	PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
	PFNGLGENBUFFERSPROC glGenBuffers;
	PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
	PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
	PFNGLGETPROGRAMIVPROC glGetProgramiv;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
	PFNGLGETSHADERIVPROC glGetShaderiv;
	PFNGLGETSTRINGIPROC glGetStringi;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
	PFNGLLINKPROGRAMPROC glLinkProgram;
	PFNGLSHADERSOURCEPROC glShaderSource;
	PFNGLUNIFORM1IPROC glUniform1i;
	PFNGLUNIFORM1UIPROC glUniform1ui;
	PFNGLUNIFORM3FVPROC glUniform3fv;
	PFNGLUNIFORM4FPROC glUniform4f;
	PFNGLUNIFORM4FVPROC glUniform4fv;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
	PFNGLUSEPROGRAMPROC glUseProgram;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;*/
	
};


#endif

