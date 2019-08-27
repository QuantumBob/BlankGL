#include <windows.h>
//#include "gl/gl.h"
//#include "gl/glext.h"
//#include "gl/glxext.h"
//#include "gl/wglext.h"
#include "cVertexArray.h"
#include "cVertexBuffer.h"
#include "cBufferLayout.h"
#include "cOpenGL.h"
#include "rwkLoader.h"

///////////////////////////////////////////////////////////

cVertexArray::cVertexArray(cOpenGL* opengl) : m_opengl (opengl)
{
	glGenVertexArrays(1, &m_renderer_ID);
	glBindVertexArray(m_renderer_ID);
}

///////////////////////////////////////////////////////////

cVertexArray::~cVertexArray()
{
	glDeleteVertexArrays(1, &m_renderer_ID);
}

///////////////////////////////////////////////////////////

void cVertexArray::Bind() const
{
	glBindVertexArray(m_renderer_ID);
}

///////////////////////////////////////////////////////////

void cVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

///////////////////////////////////////////////////////////

void cVertexArray::AddBuffer(const cVertexBuffer& vertex_buffer, const cBufferLayout& layout)
{
	
	Bind();
	vertex_buffer.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		
		glVertexAttribPointer(element.index, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		glEnableVertexAttribArray(element.index);
		
		offset += element.count * sBufferElement::GetTypeSize(element.type);
	}
	
}


