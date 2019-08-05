#include "MyGL.h"

MyGL::MyGL()
{
}

MyGL::~MyGL()
{
}

void MyGL::Init() {
	window = new Window();
}

Window* MyGL::getWindow()
{
	return window;
}

void MyGL::setWindow(Window* in)
{
	window = in;
}

Renderer* MyGL::getRenderer()
{
	return renderer;
}

void MyGL::setRenderer(Renderer* in)
{
	renderer = in;
}
