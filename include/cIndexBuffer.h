#ifndef _RWK_INDEXBUFFER_H_
#define _RWK_INDEXBUFFER_H_

class cOpenGL;

class cIndexBuffer
{
public:
	cIndexBuffer(cOpenGL*);
	cIndexBuffer(cOpenGL*, const void* data, unsigned int count);
	~cIndexBuffer();

	void Bind() const;
	void UnBind() const;

	void SetIndexData(unsigned int size, const void* data);

	inline unsigned int GetCount() const { return m_count; }
	inline unsigned int GetIndexBufferObject() { return m_renderer_ID; }
	inline unsigned int GetIndexBufferSize() { return m_buffer_size; }

private:
	unsigned int m_renderer_ID;
	unsigned int m_count;

	unsigned int m_buffer_size;

	cOpenGL* m_opengl;
};

#endif

