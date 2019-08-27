#include <windows.h>
#include "dbghelp.h"
#include "KHR/khrplatform.h"
#include <gl/gl.h>
#include "gl/glext.h"
#include "gl/wglext.h"
#include "rwkError.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>

#include "glm/glm.hpp"

#include "cShader.h"
#include "cOpenGL.h"
#include "rwkLoader.h"

///////////////////////////////////////////////////////////

cShader::cShader(cOpenGL* opengl, const std::string& filepath)
	: m_opengl (opengl), m_filepath (filepath), m_renderer_ID (0)
{
	ShaderProgramSource shader_source = ParseShader(filepath);
	m_renderer_ID = CreateShader(shader_source.VertexSource, shader_source.FragmentSource);
}

///////////////////////////////////////////////////////////

cShader::~cShader()
{
}

///////////////////////////////////////////////////////////

void cShader::Bind() const
{
	glUseProgram(m_renderer_ID);
}

///////////////////////////////////////////////////////////

void cShader::UnBind()
{
	glUseProgram(0);
}

///////////////////////////////////////////////////////////

unsigned int cShader::LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	unsigned int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		std::string error = "Impossible to open " + std::string(vertex_file_path) + ". Are you in the right directory ? Don't forget to read the FAQ !\n";
		OutputDebugString(error.c_str());
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else {
		std::string error = "Impossible to open " + std::string(fragment_file_path) + ". Are you in the right directory ? Don't forget to read the FAQ !\n";
		OutputDebugString(error.c_str());
		return 0;
	}

	int Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	std::string msg = "Compiling shader : " + std::string(vertex_file_path) + "\n";
	OutputDebugString(msg.c_str());
	char const* VertexSourcePointer = VertexShaderCode.c_str();

	//// Print shader code to output
	//msg = VertexShaderCode.c_str();
	//OutputDebugString(msg.c_str());


	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::string msg(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end());
		msg += "\n";
		OutputDebugString(msg.c_str());
	}

	// Compile Fragment Shader
	msg = "Compiling shader : " + std::string(fragment_file_path) + "\n";
	OutputDebugString(msg.c_str());
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::string msg(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end());
		msg += "\n";
		OutputDebugString(msg.c_str());
	}

	// Link the program
	OutputDebugString("Linking program\n");
	unsigned int ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		msg += "\n";
		OutputDebugString(msg.c_str());
		return 0;
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

///////////////////////////////////////////////////////////

ShaderProgramSource cShader::ParseShader(const std::string file_path)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(file_path);

	if (!stream.is_open())
	{
		std::string error = "Impossible to open " + std::string(file_path) + ". Are you in the right directory ?\n";
		OutputDebugString(error.c_str());
		return {};
	}

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

///////////////////////////////////////////////////////////

unsigned int cShader::CompileShader(const std::string& source, unsigned int type)
{
	// Compile Vertex Shader
	std::string msg = "Compiling shader \n";// : " + std::string(vertex_file_path) + "\n";
	OutputDebugString(msg.c_str());
	unsigned int ID = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(ID, 1, &src, nullptr);
	glCompileShader(ID);

	return ID;
}

///////////////////////////////////////////////////////////

int cShader::CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	unsigned int program_ID = glCreateProgram();
	unsigned int vertex_shader_ID = CompileShader(vertex_shader, GL_VERTEX_SHADER);

	//// Print shader source to output
	//std::string msg = vertex_shader;// source.c_str();
	//OutputDebugString(msg.c_str());

	int Result = GL_FALSE;
	int InfoLogLength;

	// Check Vertex Shader
	glGetShaderiv(vertex_shader_ID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertex_shader_ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vertex_shader_ID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::string msg(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end());
		msg += "\n";
		OutputDebugString(msg.c_str());
		return 0;
	}

	unsigned int fragment_shader_ID = CompileShader(fragment_shader, GL_FRAGMENT_SHADER);

	//// Print shader source to output
	//msg = fragment_shader;// source.c_str();
	//OutputDebugString(msg.c_str());

	// Check Fragment Shader
	glGetShaderiv(fragment_shader_ID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragment_shader_ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fragment_shader_ID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::string msg(VertexShaderErrorMessage.begin(), VertexShaderErrorMessage.end());
		msg += "\n";
		OutputDebugString(msg.c_str());
		return 0;
	}

	// Link the program
	OutputDebugString("Linking program\n");
	glAttachShader(program_ID, vertex_shader_ID);
	glAttachShader(program_ID, fragment_shader_ID);
	glLinkProgram(program_ID);

	// Check the program
	glGetProgramiv(program_ID, GL_LINK_STATUS, &Result);
	glGetProgramiv(program_ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(program_ID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::string msg(ProgramErrorMessage.begin(), ProgramErrorMessage.end());
		msg += "\n";
		OutputDebugString(msg.c_str());
		return 0;
	}

	glDetachShader(program_ID, vertex_shader_ID);
	glDetachShader(program_ID, fragment_shader_ID);

	glDeleteShader(vertex_shader_ID);
	glDeleteShader(fragment_shader_ID);

	return program_ID;
}

///////////////////////////////////////////////////////////

//bool cShader::SetShaderParameters(cOpenGL* opengl, float* worldMatrix, float* viewMatrix, float* projectionMatrix)
bool cShader::SetShaderParameters(glm::mat4& mvp_matrix, unsigned int solid_colour_mode)
{
	SetUniformMatrix4f("u_mvp_matrix", mvp_matrix);
	SetUniform1ui("solid_colour_mode", solid_colour_mode);
	
	return true;
}

///////////////////////////////////////////////////////////

void cShader::DestroyShader()
{
	glDeleteProgram(m_renderer_ID);
}

///////////////////////////////////////////////////////////

void cShader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocations(name), v0, v1, v2, v3);
}

///////////////////////////////////////////////////////////

void cShader::SetUniformMatrix4f(const std::string& name, const glm::mat4& in_matrix)
{
	glUniformMatrix4fv(GetUniformLocations(name), 1, GL_FALSE, &in_matrix[0][0]);
}

void cShader::SetUniform1ui(const std::string& name, unsigned int v1)
{
	glUniform1ui(GetUniformLocations(name), v1);
}

///////////////////////////////////////////////////////////

int cShader::GetUniformLocations(const std::string& name)
{
	if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
		return m_uniform_location_cache[name];

	int location = glGetUniformLocation(m_renderer_ID, name.c_str());
	if (location == -1)
	{
		std::string msg = "Warning : uniform " + name + " doesn't exist.";
		OutputDebugString(msg.c_str());
	}
	
	m_uniform_location_cache[name] = location;
	
	return location;
}
