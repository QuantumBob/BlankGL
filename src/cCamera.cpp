#include "cCamera.h"
#include "cOpenGL.h"
#include "cWindow.h"

#include <glm/gtx/transform.hpp>

///////////////////////////////////////////////////////////

cCamera::cCamera(cOpenGL* opengl) : m_opengl (opengl)
{

	m_position = glm::vec4(glm::vec3(0.0f), 1.0f);
	m_rotation = glm::vec4(0.0f);
}

///////////////////////////////////////////////////////////

cCamera::~cCamera()
{
}

///////////////////////////////////////////////////////////

void cCamera::SetPosition(glm::vec4 in_position)
{
	m_position = in_position;
	return;
}

///////////////////////////////////////////////////////////

void cCamera::SetRotation(glm::vec4 in_rotation)
{
	m_rotation = in_rotation;
	return;
}

///////////////////////////////////////////////////////////

void cCamera::UpdateViewMatrix()
{
	glm::vec4 up, position, lookAt;
	float yaw, pitch, roll;
	/*float rotationMatrix[9];*/
	glm::mat4 rotation_matrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	up.w = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_position.x;
	position.y = m_position.y;
	position.z = m_position.z;
	position.w = 1.0f;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;
	lookAt.w = 0.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	MatrixRotationYawPitchRoll(rotation_matrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	TransformCoord(lookAt, rotation_matrix);
	TransformCoord(up, rotation_matrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt.x = position.x + lookAt.x;
	lookAt.y = position.y + lookAt.y;
	lookAt.z = position.z + lookAt.z;

	// Finally create the view matrix from the three updated vectors.
	BuildViewMatrix(position, lookAt, up);

	return;
}

///////////////////////////////////////////////////////////

void cCamera::MatrixRotationYawPitchRoll(glm::mat4& matrix, float yaw, float pitch, float roll)
{
	float cYaw, cPitch, cRoll, sYaw, sPitch, sRoll;


	// Get the cosine and sin of the yaw, pitch, and roll.
	cYaw = cosf(yaw);
	cPitch = cosf(pitch);
	cRoll = cosf(roll);

	sYaw = sinf(yaw);
	sPitch = sinf(pitch);
	sRoll = sinf(roll);

	// Calculate the yaw, pitch, roll rotation matrix.
	matrix[0][0] = (cRoll * cYaw) + (sRoll * sPitch * sYaw);
	matrix[1][0] = (sRoll * cPitch);
	matrix[2][0] = (cRoll * -sYaw) + (sRoll * sPitch * cYaw);
	matrix[3][0] = 0.0f;

	matrix[0][1] = (-sRoll * cYaw) + (cRoll * sPitch * sYaw);
	matrix[1][1] = (cRoll * cPitch);
	matrix[2][1] = (sRoll * sYaw) + (cRoll * sPitch * cYaw);
	matrix[3][1] = 0.0f;

	matrix[0][2] = (cPitch * sYaw);
	matrix[1][2] = -sPitch;
	matrix[2][2] = (cPitch * cYaw);
	matrix[3][2] = 0.0f;

	matrix[0][2] = 0.0f;
	matrix[1][2] = 0.0f;
	matrix[2][2] = 0.0f;
	matrix[3][2] = 1.0f;

	return;
}

///////////////////////////////////////////////////////////

glm::vec4 cCamera::TransformCoord(glm::vec4& vector, glm::mat4& matrix)
{
	//float x, y, z;

	return matrix * vector;
	// Transform the vector by the 3x3 matrix.
	/*x = (vector.x * matrix[0]) + (vector.y * matrix[3]) + (vector.z * matrix[6]);
	y = (vector.x * matrix[1]) + (vector.y * matrix[4]) + (vector.z * matrix[7]);
	z = (vector.x * matrix[2]) + (vector.y * matrix[5]) + (vector.z * matrix[8]);*/

	// Store the result in the reference.
	/*vector.x = x;
	vector.y = y;
	vector.z = z;*/

}

///////////////////////////////////////////////////////////

void cCamera::BuildViewMatrix(glm::vec4 position, glm::vec4 lookAt, glm::vec4 up)
{
	glm::vec3 zAxis, xAxis, yAxis;
	float length, result1, result2, result3;

	// zAxis = normal(lookAt - position)
	zAxis.x = lookAt.x - position.x;
	zAxis.y = lookAt.y - position.y;
	zAxis.z = lookAt.z - position.z;
	length = sqrt((zAxis.x * zAxis.x) + (zAxis.y * zAxis.y) + (zAxis.z * zAxis.z));
	zAxis.x = zAxis.x / length;
	zAxis.y = zAxis.y / length;
	zAxis.z = zAxis.z / length;

	// xAxis = normal(cross(up, zAxis))
	xAxis.x = (up.y * zAxis.z) - (up.z * zAxis.y);
	xAxis.y = (up.z * zAxis.x) - (up.x * zAxis.z);
	xAxis.z = (up.x * zAxis.y) - (up.y * zAxis.x);
	length = sqrt((xAxis.x * xAxis.x) + (xAxis.y * xAxis.y) + (xAxis.z * xAxis.z));
	xAxis.x = xAxis.x / length;
	xAxis.y = xAxis.y / length;
	xAxis.z = xAxis.z / length;

	// yAxis = cross(zAxis, xAxis)
	yAxis.x = (zAxis.y * xAxis.z) - (zAxis.z * xAxis.y);
	yAxis.y = (zAxis.z * xAxis.x) - (zAxis.x * xAxis.z);
	yAxis.z = (zAxis.x * xAxis.y) - (zAxis.y * xAxis.x);

	// -dot(xAxis, position)
	result1 = ((xAxis.x * position.x) + (xAxis.y * position.y) + (xAxis.z * position.z)) * -1.0f;

	// -dot(yaxis, eye)
	result2 = ((yAxis.x * position.x) + (yAxis.y * position.y) + (yAxis.z * position.z)) * -1.0f;

	// -dot(zaxis, eye)
	result3 = ((zAxis.x * position.x) + (zAxis.y * position.y) + (zAxis.z * position.z)) * -1.0f;

	// Set the computed values in the view matrix.
	m_view_matrix[0][0] = xAxis.x;
	m_view_matrix[1][0] = yAxis.x;
	m_view_matrix[2][0] = zAxis.x;
	m_view_matrix[3][0] = 0.0f;

	m_view_matrix[0][1] = xAxis.y;
	m_view_matrix[1][1] = yAxis.y;
	m_view_matrix[2][1] = zAxis.y;
	m_view_matrix[3][1] = 0.0f;

	m_view_matrix[0][2] = xAxis.z;
	m_view_matrix[1][2] = yAxis.z;
	m_view_matrix[2][2] = zAxis.z;
	m_view_matrix[3][2] = 0.0f;

	m_view_matrix[0][2] = result1;
	m_view_matrix[1][2] = result2;
	m_view_matrix[2][2] = result3;
	m_view_matrix[3][2] = 1.0f;

	return;
}

///////////////////////////////////////////////////////////

glm::mat4 cCamera::GetViewMatrix()
{
	return m_view_matrix;
}

