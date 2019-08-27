#ifndef  _BLANK1_RWKERROR_H_
#define  _BLANK1_RWKERROR_H_

#include <windows.h>
#include <gl/gl.h>
#include "gl/glext.h"

class rwkError
{
public:
	rwkError();
	~rwkError();

	static void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

};
#endif


