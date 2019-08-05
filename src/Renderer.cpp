#include <Windows.h>
#include <gl/gl.h>
#include "gl/glext.h"
#include "gl/wglext.h"
#define GL_LITE_IMPLEMENTATION
#include "gl_lite.h"
#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

///////////////////////////////////////////////////////////

void Renderer::render() {

	glClearColor(0.129f, 0.586f, 0.0f, 1.0f);	// rgb(33,150,243)
	glClear(GL_COLOR_BUFFER_BIT);
}

///////////////////////////////////////////////////////////

void Renderer::swapBuffers(HDC DC) {

	SwapBuffers(DC);
}