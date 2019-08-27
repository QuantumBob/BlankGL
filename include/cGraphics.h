#ifndef _BLANK1_GRAPHICS_H_
#define _BLANK1_GRAPHICS_H_

#include "windows.h"
#include <list>

#include <glm/glm.hpp>

class cOpenGL;
class cCamera;
class cModel;

// GLOBALS 
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_FAR = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float FOV = glm::radians(45.0f);

class cGraphics
{
public:
	cGraphics(cOpenGL* opengl);
	~cGraphics();

	bool Initialize(HWND hwnd, float aspect_ratio);
	void Shutdown();
	bool Frame();
	bool Render();
	void SetProjectionMatrix(float fov, float aspect_ratio);

	inline glm::mat4 GetProjectionMatrix() { return m_projection_matrix; }
	inline std::list<cModel*>& GetModelList() { return m_model_list; }

private:
	cOpenGL* m_opengl;
	cCamera* m_camera;

	glm::vec4 m_clear_colour;

	glm::mat4 m_projection_matrix;

	std::list<cModel*> m_model_list;

	bool show_demo_window = true;
	bool show_another_window = false;
};

#endif
