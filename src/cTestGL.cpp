#include <windows.h>
#include <gl/gl.h>
#include "gl/glext.h"
#include "gl/wglext.h"
#include <string>
#include <fstream>
#include <sstream>
#include "cTestGL.h"

///////////////////////////////////////////////////////////

cTestGL::cTestGL()
{
}

///////////////////////////////////////////////////////////

cTestGL::~cTestGL()
{
	// Cleanup VBOm_vertex_buffer_IDm_vertex_buffer_ID
	glDeleteBuffers(1, &m_vertex_buffer_ID);
	glDeleteVertexArrays(1, &vertex_array_object_ID);
	glDeleteProgram(m_program_ID);
}

///////////////////////////////////////////////////////////

void cTestGL::DoEverything()
{

	// A Vertex Array Object(VAO) is an OpenGL Object that stores all of the state needed to supply vertex data.
	// It stores the format of the vertex data as well as the Buffer Objects providing the vertex data arrays.
	glGenVertexArrays(1, &vertex_array_object_ID);
	glBindVertexArray(vertex_array_object_ID);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Create and compile our GLSL program from the shaders
	m_program_ID = LoadShaders("resources/shaders/SimpleVertexShader.vshader", "resources/shaders/SimpleFragmentShader.fshader");

	/*
	#######################################
	Vertex Data
	*/
	vertex_buffer_data = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,

		 0.0f, -0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f
	};

	glGenBuffers(1, &m_vertex_buffer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float), &vertex_buffer_data[0], GL_STATIC_DRAW);

	
	// attribute index
	// size
	// type
	// normalized?
	// stride
	// array buffer offset
	/* Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);

	/*
	#######################################
	Index Data
	*/
	index_buffer_data = {
		0, 1, 2
	};

	index_buffer_data_2 = {
		3, 4, 5,
	};

	glGenBuffers(1, &m_index_buffer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof (unsigned int), &index_buffer_data[0], GL_STATIC_DRAW);

	/*
	#######################################
	Colour Data
	*/
	std::vector<vec3> colours3 = {
		{  1.0,  0.0,  0.0 }, /* vertex 1 */
		{  0.0,  1.0,  0.0 }, /* vertex 2 */
		{  0.0,  0.0,  1.0 }, /* vertex 3 */
		{  1.0,  1.0, 0.0 }, /* vertex 4 */
		{ 0.0,  1.0,  1.0 }, /* vertex 5 */
		{ 1.0,  0.0,  1.0 } }; /* vertex 6 */

	std::vector<vec4> colours4 = {
		{  1.0,  0.0,  0.0, 1.0  }, /* Red */
		{  0.0,  1.0,  0.0 , 1.0 }, /* Green */
		{  0.0,  0.0,  1.0, 1.0  }, /* Blue */
		{  1.0,  1.0, 0.0 , 0.5 }, /* vertex 4 */
		{ 0.0,  1.0,  1.0 , 0.5 }, /* vertex 5 */
		{ 1.0,  0.0,  1.0, 0.5  } }; /* vertex 6 */

	unsigned int size = colours4.size() * (sizeof(vec4) / sizeof(float));
	glGenBuffers(1, &m_colour_buffer_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_colour_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), &colours4[0], GL_STATIC_DRAW);

	/* Specify that our color data is going into attribute index 1, and contains four floats per vertex (rgba as vec4) */
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);
}

///////////////////////////////////////////////////////////

void cTestGL::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(m_program_ID);

	/*int location = glGetUniformLocation(m_program_ID, "u_colour");
	if (location != -1)
	{
		glUniform4f(location, 0.2f, 0.4f, 0.5f, 1.0f);
	}*/

	/*glEnableVertexAttribArray(0);*/

	// use for non-indexed vertex buffer
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_ID);

	// use for indexed vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer_ID);

	//// attribute 0. No particular reason for 0, but must match the layout in the shader.
	//// size
	//// type
 //  // normalized?
	//// stride
	//// array buffer offset
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0	);

	// Draw the triangle

	// use for non-indexed vertex array
	//glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

	// use for indexed vertex array
	// mode
	// count
	// type
	// element array buffer offset
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);
	//glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)0);

	//glDisableVertexAttribArray(0);

}

///////////////////////////////////////////////////////////

unsigned int cTestGL::LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	unsigned int vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);

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
	glShaderSource(vertex_shader_ID, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertex_shader_ID);

	// Check Vertex Shader
	glGetShaderiv(vertex_shader_ID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(vertex_shader_ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vertex_shader_ID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::string msg = VertexShaderErrorMessage[0] + "\n";
		OutputDebugString(msg.c_str());
	}

	// Compile Fragment Shader
	msg = "Compiling shader : " + std::string(fragment_file_path) + "\n";
	OutputDebugString(msg.c_str());
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(fragment_shader_ID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fragment_shader_ID);

	// Check Fragment Shader
	glGetShaderiv(fragment_shader_ID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(fragment_shader_ID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fragment_shader_ID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::string msg(FragmentShaderErrorMessage.begin(), FragmentShaderErrorMessage.end());
		OutputDebugString(msg.c_str());
	}

	// Link the program
	OutputDebugString("Linking program\n");
	unsigned int ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertex_shader_ID);
	glAttachShader(ProgramID, fragment_shader_ID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::string msg = ProgramErrorMessage[0] + "\n";
		OutputDebugString(msg.c_str());
	}

	glDetachShader(ProgramID, vertex_shader_ID);
	glDetachShader(ProgramID, fragment_shader_ID);

	glDeleteShader(vertex_shader_ID);
	glDeleteShader(fragment_shader_ID);

	return ProgramID;
}
