#ifndef _BLANK1_VERTEXARRAY_H
#define _BLANK1_VERTEXARRAY_H


class cOpenGL;
class cVertexBuffer;
class cBufferLayout;

class cVertexArray
{
public:
	cVertexArray(cOpenGL*);
	~cVertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const cVertexBuffer& vb, const cBufferLayout& layout);
	
	inline unsigned int GetVertexArrayObject() { return m_renderer_ID; }

private:
	unsigned int m_renderer_ID;
	cOpenGL* m_opengl;
};

#endif
