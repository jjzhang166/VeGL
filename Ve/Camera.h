#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

class Camera
{
public:
	glm::vec3 right, up, forward;
	glm::vec3 position;
	glm::quat rotation;

	float fov = 45.0f;
	float zNear = 0.1f, zFar = 100.0f;

	Camera (const glm::vec3& pos)
		: position (pos), rotation (glm::toQuat (glm::orientate3 (glm::vec3 (0.0f))))
	{
	}

	void Rotate (const glm::vec3& euler, bool local = true)
	{
		glm::fquat new_rot = glm::toQuat (glm::orientate3 (euler * (3.141592f / 180.0f)));
		rotation = local ? rotation * new_rot : new_rot * rotation;
	}

	void LookAt (const glm::vec3& pos)
	{
		glm::mat4 lookMat = glm::lookAt (position, glm::vec3 (pos.x, pos.y, pos.z), glm::vec3 (0, 1, 0));
		rotation = glm::conjugate (glm::quat_cast (lookMat));
	}

	glm::mat4 GetView ()
	{
		forward = rotation * glm::vec3 (0, 0, 1); // +Z is forward direction
		right = rotation * glm::vec3 (1, 0, 0); // +X is right direction
		up = rotation * glm::vec3 (0, 1, 0); // +Y is up direction

		return glm::lookAtLH (position, position + forward, up);
	}

	glm::mat4 GetProjection (const float aspectRatio)
	{
		return glm::perspective (glm::radians (fov), aspectRatio, zNear, zFar);
	}
}; 
