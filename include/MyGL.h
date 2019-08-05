#ifndef _PROJECT01_GENERAL_H_
#define _PROJECT01_GENERAL_H_


#include "Window.h"
#include "Renderer.h"

class MyGL
{
public:
	MyGL();
	~MyGL();

	void Init();

	Window* getWindow();
	void setWindow(Window*);

	Renderer* getRenderer();
	void setRenderer(Renderer*);

private:
	Window* window;
	Renderer* renderer;

};

#endif

