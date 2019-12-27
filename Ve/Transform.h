#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

class Transform
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform (glm::vec3 pos)
		: position (pos), scale (1.0f), rotation (glm::toQuat (glm::orientate3 (glm::vec3 (0.0f))))
	{}

	void Rotate (const glm::vec3& euler)
	{
		rotation = glm::rotate (rotation, glm::radians (euler.x), glm::vec3 (1.0f, 0, 0));
		rotation = glm::rotate (rotation, glm::radians (euler.y), glm::vec3 (0, 1.0f, 0));
		rotation = glm::rotate (rotation, glm::radians (euler.z), glm::vec3 (0, 0, 1.0f));
	}

	glm::mat4 get_model_matrix ()
	{
		glm::mat4 rot = glm::toMat4 (rotation);
		glm::mat4 trans = glm::translate (glm::mat4 (1.0f), position);
		glm::mat4 scal = glm::scale (glm::mat4 (1.0f), scale);
		return trans * rot;
	}
};