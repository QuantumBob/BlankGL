#include "cApp.h"
#include "cGraphics.h"
#include "cOpenGL.h"
#include "cWindow.h"
#include "cCamera.h"
#include "cModel.h"
#include "cShader.h"
#include "cVertexArray.h"

#include <glm/gtx/transform.hpp>
//#include "glm/gtx/string_cast.hpp"

#include "rwkLoader.h"

#include "imGui/imgui.h"
#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_opengl3.h"

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

///////////////////////////////////////////////////////////

cGraphics::cGraphics(cOpenGL* opengl) : m_opengl (opengl)
{
}

///////////////////////////////////////////////////////////

cGraphics::~cGraphics()
{
}

///////////////////////////////////////////////////////////

bool cGraphics::Initialize(HWND hwnd, float aspect_ratio)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplWin32_Init(hwnd);

	const char* glsl_version = "#version 150";
	ImGui_ImplOpenGL3_Init(glsl_version);

	//Set Window bg color
	m_clear_colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Set the projection matrix
	SetProjectionMatrix(FOV, aspect_ratio);

	m_camera = new cCamera(m_opengl);
	m_camera->SetPosition(glm::vec4(100.0f, 200.0f, 500.0f, 1.0f));
	
	cModel* newModel = new cModel(m_opengl, "resources/shaders/dualshader2.shader");
	m_model_list.push_back(newModel);
	return true;
}

///////////////////////////////////////////////////////////

void cGraphics::Shutdown()
{
	// Cleanup imGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();


	// Cleanup VBO
	std::list <cModel*> ::iterator model = m_model_list.begin();
	unsigned int buffer = 0;
	while (model != m_model_list.end())
	{
		buffer = (*model)->GetVertexBufferID();
		glDeleteBuffers(1, &buffer);
		buffer = (*model)->GetIndexBufferID();
		(*model)->GetShader()->UnBind();
		buffer = (*model)->GetVertexArray()->GetVertexArrayObject();
		glDeleteVertexArrays(1, &buffer);
		model++;
	}
	glDisableVertexAttribArray(0);
}

///////////////////////////////////////////////////////////

bool cGraphics::Frame()
{
	// UpdateViewMatrix the graphics scene.
	if (!Render())
	{
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////

bool cGraphics::Render()
{

	glm::mat4 model_matrix;
	glm::mat4 view_matrix;
	
	// Clear the buffers to begin the scene.
	m_opengl->BeginScene(m_clear_colour);

	// Build the view matrix
	view_matrix = glm::lookAt(glm::vec3(m_camera->GetPosition()), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
	std::list <cModel*> ::iterator model = m_model_list.begin();
	glm::mat4 mvp_matrix;

	// Update the model using the color shader.
	while (model != m_model_list.end())
	{
		// Finish the MVP matrix
		model_matrix = (*model)->GetModelMatrix();
		mvp_matrix = m_projection_matrix * view_matrix * model_matrix;

		// Set the color shader as the current shader program and set the matrices that it will use for rendering.
		(*model)->GetShader()->SetShaderParameters(mvp_matrix, 1);

		// Draw the model
		(*model)->Draw();
		model++;
	}

	// Do imGui stuff	
	// Feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)& m_clear_colour); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Present the rendered scene to the screen.
	m_opengl->EndScene();

	return true;
}

///////////////////////////////////////////////////////////

void cGraphics::SetProjectionMatrix(float fov, float aspect_ratio)
{
	// Build the perspective projection matrix.
	m_projection_matrix = glm::perspective(fov, aspect_ratio, SCREEN_NEAR, SCREEN_FAR);
}