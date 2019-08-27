#ifndef  _BLANK1_CTESTGL_H_
#define  _BLANK1_CTESTGL_H_

#include <vector>

struct vec3 {
	float red;
	float green;
	float blue;
};

struct vec4 {
	float red;
	float green;
	float blue;
	float alpha;
};

class cTestGL
{
public:
	cTestGL();
	~cTestGL();

	void DoEverything();
	void Render();

	unsigned int LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

private:

	unsigned int vertex_array_object_ID;
	unsigned int m_program_ID;
	unsigned int m_vertex_buffer_ID;
	unsigned int m_index_buffer_ID;
	unsigned int m_colour_buffer_ID;

	std::vector<float> vertex_buffer_data;
	std::vector<unsigned int> index_buffer_data;
	std::vector<unsigned int> index_buffer_data_2;

};



#endif
