#include "cIndexBuffer.h"
#include "cOpenGL.h"
#include "rwkLoader.h"

///////////////////////////////////////////////////////////

cIndexBuffer::cIndexBuffer(cOpenGL* opengl) : m_opengl (opengl), m_count (0)
{
	glGenBuffers(1, &m_renderer_ID);
}

///////////////////////////////////////////////////////////

cIndexBuffer::cIndexBuffer(cOpenGL* opengl, const void* data, unsigned int count)
	: m_opengl(opengl), m_count(count)
{
	glGenBuffers(1, &m_renderer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

///////////////////////////////////////////////////////////

cIndexBuffer::~cIndexBuffer()
{
	glDeleteBuffers(1, &m_renderer_ID);
}

///////////////////////////////////////////////////////////

void cIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID);
}

///////////////////////////////////////////////////////////

void cIndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

///////////////////////////////////////////////////////////

void cIndexBuffer::SetIndexData(unsigned int size, const void* data)
{
	if (m_renderer_ID != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
	m_buffer_size = size;
}

