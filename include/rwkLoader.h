/*
	rwkLoader.h - Single-header multi-platform OpenGL function loader

	----------------------------------------------------------------------------
	USAGE
	----------------------------------------------------------------------------
	1) Add the following lines in exactly one of your cpp files to compile the
	   implementation:

		   #define RWK_GL_LOADER_IMPLEMENTATION
		   #include "rwkLoader.h"

	2) In all other files in which you want to use OpenGL functions, simply
	   include this header file as follows:

		   #include "rwkLoader.h"

	3) Call rwk_gl_loader_init() before using any OpenGL function and after you have a
	   valid OpenGL context.
*/
#ifndef _RWK_LOADER_H_
#define _RWK_LOADER_H_

#if defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#define GLDECL WINAPI

#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_ARRAY_BUFFER                   0x8892 // Acquired from:
#define GL_ARRAY_BUFFER_BINDING           0x8894 // https://www.opengl.org/registry/api/GL/glext.h
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COMPILE_STATUS                 0x8B81
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FUNC_ADD                       0x8006
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_LINK_STATUS                    0x8B82
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_STATIC_DRAW                    0x88E4
#define GL_STREAM_DRAW                    0x88E0
#define GL_TEXTURE0                       0x84C0
#define GL_VERTEX_ARRAY_BINDING           0x85B5
#define GL_VERTEX_SHADER                  0x8B31

#ifdef _WIN64
typedef signed   long long int khronos_intptr_t;
typedef unsigned long long int khronos_uintptr_t;
typedef signed   long long int khronos_ssize_t;
typedef unsigned long long int khronos_usize_t;
#else
typedef signed   long  int     khronos_intptr_t;
typedef unsigned long  int     khronos_uintptr_t;
typedef signed   long  int     khronos_ssize_t;
typedef unsigned long  int     khronos_usize_t;
#endif

// khronos_ssize_t signed size
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_intptr_t GLintptr;

typedef char GLchar;

#endif // _WIN32

#include <GL/gl.h>


#define RWK_GL_LIST \
    /* ret, name, params */ \
	GLE(void, ActiveTexture, GLenum texture) \
    GLE(void, AttachShader, GLuint program, GLuint shader) \
    GLE(void, BindBuffer, GLenum target, GLuint buffer) \
    GLE(void, BindFramebuffer, GLenum target, GLuint framebuffer) \
	GLE(void, BindSampler, GLuint unit, GLuint sampler) \
	GLE(void, BindVertexArray, GLuint array) \
	GLE(void, BlendEquation, GLenum mode) \
	GLE(void, BlendEquationSeparate, GLenum modeRGB, GLenum modeAlpha) \
	GLE(void, BlendFuncSeparate, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) \
    GLE(void, BufferData, GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) \
    GLE(void, BufferSubData, GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data) \
    GLE(GLenum, CheckFramebufferStatus, GLenum target) \
    GLE(void, ClearBufferfv, GLenum buffer, GLint drawbuffer, const GLfloat * value) \
    GLE(void, CompileShader, GLuint shader) \
    GLE(GLuint, CreateProgram, void) \
    GLE(GLuint, CreateShader, GLenum type) \
    GLE(void, DeleteBuffers, GLsizei n, const GLuint *buffers) \
    GLE(void, DeleteFramebuffers, GLsizei n, const GLuint *framebuffers) \
	GLE(void, DeleteProgram, GLuint program) \
	GLE(void, DeleteShader, GLuint shader) \
	GLE(void, DeleteVertexArrays, GLsizei n, const GLuint* arrays) \
	GLE(void, DetachShader, GLuint program, GLuint shader) \
	GLE(void, DisableVertexAttribArray, GLuint index)  \
	GLE(void, DrawElementsBaseVertex, GLenum mode, GLsizei count, GLenum type, GLvoid* indices, GLint basevertex) \
    GLE(void, EnableVertexAttribArray, GLuint index) \
    GLE(void, DrawBuffers, GLsizei n, const GLenum *bufs) \
    GLE(void, FramebufferTexture2D, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) \
    GLE(void, GenBuffers, GLsizei n, GLuint *buffers) \
    GLE(void, GenFramebuffers, GLsizei n, GLuint * framebuffers) \
	GLE(void, GenVertexArrays, GLsizei n, GLuint *arrays) \
    GLE(GLint, GetAttribLocation, GLuint program, const GLchar *name) \
	GLE(void, GetProgramInfoLog, GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog) \
	GLE(void, GetProgramiv, GLuint program, GLenum pname, GLint* params) \
    GLE(void, GetShaderInfoLog, GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) \
    GLE(void, GetShaderiv, GLuint shader, GLenum pname, GLint *params) \
	GLE(const GLubyte*, GetStringi, GLenum name​, GLuint index​) \
    GLE(GLint, GetUniformLocation, GLuint program, const GLchar *name) \
    GLE(void, LinkProgram, GLuint program) \
    GLE(void, ShaderSource, GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length) \
    GLE(void, Uniform1i, GLint location, GLint v0) \
	GLE(void, Uniform1ui, GLint location, GLuint v0) \
    GLE(void, Uniform1f, GLint location, GLfloat v0) \
    GLE(void, Uniform2f, GLint location, GLfloat v0, GLfloat v1) \
    GLE(void, Uniform4f, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) \
    GLE(void, UniformMatrix4fv, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    GLE(void, UseProgram, GLuint program) \
    GLE(void, VertexAttribPointer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer) \
    /* end */

#define GLE(ret, name, ...) typedef ret GLDECL name##Proc(__VA_ARGS__); extern name##Proc * gl##name;
RWK_GL_LIST
#undef GLE

bool rwk_loader_init();

#endif //_RWK_LOADER_H_

// =============================================================================
#ifdef RWK_GL_LOADER_IMPLEMENTATION

#define GLE(ret, name, ...) name##Proc * gl##name;
RWK_GL_LIST
#undef GLE

bool rwk_loader_init()
{
	std::string msg;
	HINSTANCE dll = LoadLibraryA("opengl32.dll");
	typedef PROC WINAPI wglGetProcAddressProc(LPCSTR lpszProc);
	if (!dll) {
		OutputDebugStringA("opengl32.dll not found.\n");
		return false;
	}
	wglGetProcAddressProc* wglGetProcAddress = (wglGetProcAddressProc*)GetProcAddress(dll, "wglGetProcAddress");

#define GLE(ret, name, ...)																	\
	gl##name = (name##Proc *)wglGetProcAddress("gl" #name);									\
	msg = "Function gl" #name " loaded from wglGetProcAddress()\n";							\
	if (gl##name == 0 || (gl##name == (void*)0x1) || (gl##name == (void*)0x2) ||			\
	(gl##name == (void*)0x3) || (gl##name == (void*)-1)){									\
		gl##name = (name##Proc *)GetProcAddress(dll, "gl" #name);							\
		msg = "Function gl" #name " loaded from opengl32.dll\n";							\
	}																						\
	if (!gl##name) {																		\
		OutputDebugStringA("Function gl" #name " couldn't be loaded\n");					\
		return false;																		\
	}																						
	//OutputDebugStringA(msg.c_str());
	RWK_GL_LIST
#undef GLE
	return true;
}

#endif //RWK_GL_LOADER_IMPLEMENTATION

