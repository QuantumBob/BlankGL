#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <unordered_map>

class cOpenGL;

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class cShader
{
public:
	cShader(cOpenGL* opengl, const std::string& fielpath);
	~cShader();

	void Bind() const;
	void UnBind();

	unsigned int LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	ShaderProgramSource ParseShader(const std::string file_path);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	//bool SetShaderParameters(cOpenGL* opengl, float* worldMatrix, float* viewMatrix, float* projectionMatrix);
	bool SetShaderParameters(glm::mat4& mvp_matrix, unsigned int solid_colour_mode);

	void DestroyShader();

	// Set Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMatrix4f(const std::string& name, const glm::mat4& in_matrix);
	void SetUniform1ui(const std::string& name, unsigned int v1);

	inline unsigned int GetProgramID() { return m_renderer_ID; }

private:

	unsigned int m_renderer_ID;
	const std::string& m_filepath;
	std::unordered_map<std::string, int> m_uniform_location_cache;

	int GetUniformLocations(const std::string& name);

	cOpenGL* m_opengl;
};


#endif

