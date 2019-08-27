 #ifndef  _BLANK1_CMODEL_H_
#define  _BLANK1_CMODEL_H_

#include <windows.h>
#include "dbghelp.h"

#include "KHR/khrplatform.h"
#include <gl/gl.h>
#include "gl/glext.h"
#include "gl/wglext.h"

#include <string>
#include <array>
#include <vector>

class cOpenGL;
class cVertexArray;
class cVertexBuffer;
class cIndexBuffer;
class cShader;

class cModel
{
public:
	cModel(cOpenGL*, const std::string&);
	cModel(cOpenGL*, const std::string&, unsigned int);
	~cModel();

	void Bind();

	cVertexArray* GetVertexArray();
	cVertexBuffer* GetVertexBuffer();
	cVertexBuffer* GetColourBuffer();
	cIndexBuffer* GetIndexBuffer();
		
	/*unsigned int LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	ShaderProgramSource ParseShader(std::string);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);*/

	unsigned int GetVertexBufferID();
	unsigned int GetIndexBufferID();
	unsigned int GetProgramID();
	
	void LoadTriangleData(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data, std::vector<float>& colour_data);
	void LoadTriangleData(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data);

	void LoadCubeData(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data, std::vector<float>& colour_data);

	glm::mat4 Transform(glm::mat4 translation_matrix, glm::mat4 rotation_matrix, glm::mat4 scaling_matrix);
	void Draw();

	inline std::string GetName() { return m_model_name; }
	inline void SetName(std::string in) { m_model_name = in; }
	inline cShader* GetShader() { return m_shader; }

	inline glm::mat4 GetModelMatrix() { return m_model_matrix; }

private:

	cOpenGL* m_opengl;
	const std::string m_filepath;
	unsigned int m_constructor_flag;

	cVertexArray* m_vertex_array;
	cVertexBuffer* m_vertex_buffer;
	cVertexBuffer* m_colour_buffer;
	cIndexBuffer* m_index_buffer;
	cShader* m_shader;

	std::string m_model_name;
	glm::mat4 m_model_matrix;
	//unsigned int m_program_ID;
};

#endif
