#ifndef _BLANK1_VERTEXBUFFER_H_
#define _BLANK1_VERTEXBUFFER_H_

#include <vector>

class cOpenGL;

class cVertexBuffer
{
public:
	cVertexBuffer(cOpenGL* opengl);
	cVertexBuffer(const void* data, unsigned int size);
	~cVertexBuffer();
	
	void Bind() const;
	void UnBind() const;

	void SetVertexData(unsigned int size, const void* data, unsigned int in_renderer = 0);

	inline unsigned int GetVertexBufferObject() { return m_renderer_ID; }
	inline std::vector<float> GetVertexData() { return m_vertex_buffer_data; }

private:
	unsigned int m_renderer_ID;
	std::vector<float> m_vertex_buffer_data;
	cOpenGL* m_opengl;
};

#endif

