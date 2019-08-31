#ifndef _RWK_CAMERA_H_
#define _RWK_CAMERA_H_

#include <glm/glm.hpp>

class cOpenGL;

class cCamera
{

public:

	cCamera(cOpenGL* opengl);
	~cCamera();

	void SetPosition(glm::vec4);
	void SetRotation(glm::vec4);
	void UpdateViewMatrix();
	void MatrixRotationYawPitchRoll(glm::mat4& matrix, float yaw, float pitch, float roll);
	glm::vec4 TransformCoord(glm::vec4& vector, glm::mat4& matrix);
	void BuildViewMatrix(glm::vec4 position, glm::vec4 lookAt, glm::vec4 up);
	glm::mat4 GetViewMatrix();

	inline glm::vec4 GetPosition() { return m_position; }
	
private:
	cOpenGL* m_opengl;

	glm::vec4 m_position;
	glm::vec4 m_rotation;
	glm::mat4 m_view_matrix;
};



#endif
