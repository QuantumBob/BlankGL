#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <string>
#include <list>

#include <glm/gtx/transform.hpp>

#include "cOpenGL.h"
#include "cModel.h"
#include "cVertexArray.h"
#include "cVertexBuffer.h"
#include "cIndexBuffer.h"
#include "cShader.h"
#include "cBufferLayout.h"
#include "cCamera.h"

#include "rwkLoader.h"

///////////////////////////////////////////////////////////

cModel::cModel(cOpenGL* opengl, const std::string& filepath)
	: m_opengl (opengl), m_filepath(filepath), m_model_matrix (glm::mat4(1.0f))
{
	m_draw_type = GL_TRIANGLES;
	m_draw_type_name = "GL_TRIANGLES";

	m_vertex_array = new cVertexArray(opengl);
	m_vertex_buffer = new cVertexBuffer(opengl);
	m_colour_buffer = new cVertexBuffer(opengl);
	m_index_buffer = new cIndexBuffer(opengl);

	m_shader = new cShader(opengl, filepath);
	m_shader->Bind();

	std::vector<float> vertex_data;
	std::vector<unsigned int> index_data;
	std::vector<float> colour_data;

	m_model_matrix = glm::scale(glm::vec3(100.0f, 100.0f, 100.0f));
	//LoadTriangleData(vertex_data, index_data, colour_data);
	LoadCubeData(vertex_data, index_data, colour_data);

	m_vertex_buffer->SetVertexData(vertex_data.size() * sizeof(float), &vertex_data[0]);
	m_colour_buffer->SetVertexData(colour_data.size() * sizeof(float), &colour_data[0]);
	m_index_buffer->SetIndexData(index_data.size() * sizeof(unsigned int), &index_data[0]);

	cBufferLayout position_layout;
	position_layout.Push<float>(0, 3);
	m_vertex_array->AddBuffer(m_vertex_buffer[0], position_layout);

	cBufferLayout colour_layout;
	colour_layout.Push<float>(1, 4);
	m_vertex_array->AddBuffer(m_colour_buffer[0], colour_layout);

}

///////////////////////////////////////////////////////////

cModel::cModel(cOpenGL* opengl, const std::string& filepath, unsigned int in) 
	: m_opengl (opengl), m_filepath(filepath), m_constructor_flag (in), m_model_matrix(glm::mat4(1.0f))
{
	m_vertex_array = new cVertexArray(opengl);
	cVertexBuffer* m_position_colour_buffer = new cVertexBuffer(opengl);
	m_index_buffer = new cIndexBuffer(opengl);
	m_shader = new cShader(opengl, filepath);
	
	std::vector<float> vertex_data;
	std::vector<unsigned int> index_data;

	LoadTriangleData(vertex_data, index_data);

	m_position_colour_buffer->SetVertexData(vertex_data.size() * sizeof(float), &vertex_data[0]);
	m_index_buffer->SetIndexData(index_data.size() * sizeof(unsigned int), &index_data[0]);

	cBufferLayout vertex_layout;
	vertex_layout.Push<float>(0, 3);
	vertex_layout.Push<float>(1, 4);

	m_vertex_array->AddBuffer(m_position_colour_buffer[0], vertex_layout);
}

///////////////////////////////////////////////////////////

cModel::~cModel()
{
	m_vertex_buffer->UnBind();
	m_colour_buffer->UnBind();
	m_index_buffer->UnBind();
	m_vertex_array->Unbind();
}

///////////////////////////////////////////////////////////

void cModel::Bind()
{
	// all models use indexed buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetIndexBufferID());
}

///////////////////////////////////////////////////////////

cVertexArray* cModel::GetVertexArray()
{
	return m_vertex_array;
}

///////////////////////////////////////////////////////////

cVertexBuffer* cModel::GetVertexBuffer()
{
	return m_vertex_buffer;
}

///////////////////////////////////////////////////////////

cVertexBuffer* cModel::GetColourBuffer()
{
	return m_colour_buffer;
}

///////////////////////////////////////////////////////////

cIndexBuffer* cModel::GetIndexBuffer()
{
	return m_index_buffer;
}

///////////////////////////////////////////////////////////

unsigned int cModel::GetVertexBufferID()
{
	return m_vertex_buffer->GetVertexBufferObject();
}

///////////////////////////////////////////////////////////

unsigned int cModel::GetIndexBufferID()
{
	return m_index_buffer->GetIndexBufferObject();
}

///////////////////////////////////////////////////////////

unsigned int cModel::GetProgramID()
{
	return m_shader->GetProgramID();
}

///////////////////////////////////////////////////////////

void cModel::LoadTriangleData(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data, std::vector<float>& colour_data)
{
	/*vertex_data = {
		-0.5f, -0.5f, 0.5f,
		 0.5f, -0.5f, 0.5f,
		 0.0f,  0.5f, 0.5f
	};*/

	/*vertex_data = {
		50.0f, 50.0f, 0.0f,
		 250.5f, 50.0f, 0.0f,
		 150.0f,  250.0f, 0.0f
	};*/

	vertex_data = {
		-100.0f, -100.f, 0.0f,
		 100.0f, -100.0f, 0.0f,
		 0.0f,  100.0f, 0.0f
	};

	colour_data = {
		1.0, 0.0, 0.0, 1.0,		/* v1 */
		0.0, 1.0, 0.0, 1.0,		/* v2 */
		0.0, 0.0, 1.0, 1.0		/* v3 */
	};	

	index_data = {
		0, 1, 2
	};
}

///////////////////////////////////////////////////////////

void cModel::LoadTriangleData(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data)
{
	vertex_data = {
		-0.5f, -0.5f, 0.0f, 1.0, 0.0, 0.0, 1.0,
		 0.5f, -0.5f, 0.0f, 0.0, 0.0, 0.0, 1.0,
		 0.0f,  0.5f, 0.0f, 0.0, 0.0, 1.0, 1.0
	};

	index_data = {
		0, 1, 2
	};
}

///////////////////////////////////////////////////////////

void cModel::LoadCubeData(std::vector<float>& vertex_data, std::vector<unsigned int>& index_data, std::vector<float>& colour_data)
{
	vertex_data = {
		-1.0f, -1.0f, -1.0f, // index 0
		 1.0f, -1.0f, -1.0f, // index 1
		 1.0f,  1.0f, -1.0f, // index 2
		 -1.0f,  1.0f, -1.0f, // index 3, square 1

		 1.0f, -1.0f, 1.0f, // index 4
		 -1.0f, -1.0f, 1.0f, // index 5
		 -1.0f,  1.0f, 1.0f, // index 6
		 1.0f,  1.0f, 1.0f, // index 7, square 2
	};

	colour_data = {
		1.0, 0.0, 0.0, 1.0,		/* v0 */
		0.0, 1.0, 0.0, 1.0,		/* v1 */
		0.0, 0.0, 1.0, 1.0,		/* v2 */
		0.5, 0.5, 0.5, 1.0,		/* v3 */

		1.0, 0.0, 0.0, 1.0,		/* v4 */
		0.0, 1.0, 0.0, 1.0,		/* v5 */
		0.0, 0.0, 1.0, 1.0,		/* v6 */
		0.0, 0.0, 0.0, 1.0		/* v7 */
	};

	index_data = {
		0, 1, 3,
		3, 1, 2,

		2, 1, 4,
		4, 7, 2, 

		2, 7, 6,
		6, 3, 2,

		0, 3, 6,
		6, 5, 0,

		0, 5, 4,
		4, 1, 0,

		4, 5, 6,
		6, 7, 4
	};
}

///////////////////////////////////////////////////////////

glm::mat4 cModel::Transform(glm::mat4 translation_matrix, glm::mat4 rotation_matrix, glm::mat4 scaling_matrix)
{
	return translation_matrix * rotation_matrix * scaling_matrix;
}

///////////////////////////////////////////////////////////

void cModel::Draw()
{
	glUseProgram(GetShader()->GetProgramID());

	unsigned int buffer_size = m_index_buffer->GetIndexBufferSize();
	GetIndexBuffer()->Bind();

	// Draw the triangle !
	//glDrawElements(GL_TRIANGLES, buffer_size, GL_UNSIGNED_INT, (void*)0);
	glPointSize(4);
	glDrawElements(m_draw_type, buffer_size, GL_UNSIGNED_INT, (void*)0);
	/*GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP, GL_QUADS, GL_POLYGON*/


}






