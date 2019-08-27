#include <windows.h>
//#include <gl/gl.h>
//#include "gl/glext.h"
//#include "gl/wglext.h"
#include "cVertexBuffer.h"
#include "cOpenGL.h"
#include "rwkLoader.h"

///////////////////////////////////////////////////////////

cVertexBuffer::cVertexBuffer(cOpenGL* opengl) : m_opengl(opengl)
{
	glGenBuffers(1, &m_renderer_ID);
}

///////////////////////////////////////////////////////////

cVertexBuffer::cVertexBuffer(const void* data, unsigned int size)
{
	SetVertexData(size, data);
}

///////////////////////////////////////////////////////////

cVertexBuffer::~cVertexBuffer()
{
	glDeleteBuffers(1, &m_renderer_ID);
}

///////////////////////////////////////////////////////////

void cVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_renderer_ID);
}

///////////////////////////////////////////////////////////

void cVertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

///////////////////////////////////////////////////////////
// Calls glBindBuffer then glBufferData on sent data. Caller must ensure VAO is bound before this call
void cVertexBuffer::SetVertexData(unsigned int size, const void* data, unsigned int in_renderer)
{
	// May want to keep copy of data in m_vertex_buffer_data
	if (m_renderer_ID == 0)
	{
		glGenBuffers(1, &m_renderer_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderer_ID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
}


