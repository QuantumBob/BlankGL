#ifndef _RWK_BUFFERLAYOUT_H
#define _RWK_BUFFERLAYOUT_H

#include "gl/gl.h"
#include "gl/glext.h"
#include "glm/glm.hpp"

#include <vector>

struct sBufferElement
{
	unsigned int index;
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int GetTypeSize(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			case GL_FLOAT_VEC4:		return 16;
		}
		return 0;
	}
};

class cBufferLayout
{
public:
	cBufferLayout() : m_stride(0) {};
	~cBufferLayout() {};

	template<typename T>
	void Push(unsigned int index, unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int index, unsigned int count)
	{
		m_stride += sBufferElement::GetTypeSize(GL_FLOAT) * count;
		m_elements.push_back({ index, GL_FLOAT, count, GL_FALSE });
	}

	template<>
	void Push<unsigned int>(unsigned int index, unsigned int count)
	{
		m_stride += sBufferElement::GetTypeSize(GL_UNSIGNED_INT);
		m_elements.push_back({ index, GL_UNSIGNED_INT, count, GL_FALSE });
	}

	template<>
	void Push<unsigned char>(unsigned int index, unsigned int count)
	{
		m_stride += sBufferElement::GetTypeSize(GL_UNSIGNED_BYTE);
		m_elements.push_back({ index, GL_UNSIGNED_BYTE, count, GL_TRUE });
	}

	template<>
	void Push<glm::vec4>(unsigned int index, unsigned int count)
	{
		m_stride += sBufferElement::GetTypeSize(GL_FLOAT_VEC4);
		m_elements.push_back({ index, GL_FLOAT_VEC4, count, GL_FALSE });
	}

	inline unsigned int GetStride() const { return m_stride; }
	inline const std::vector<sBufferElement> GetElements() const& { return m_elements; }

private:
	unsigned int m_stride;
	std::vector<sBufferElement> m_elements;

};

#endif
