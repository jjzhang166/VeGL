#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"

class Transform
{
public:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 euler;
	glm::vec3 scale;

	Transform (glm::vec3 pos)
		: position (pos), scale (1.0f), euler(0), rotation (glm::toQuat (glm::orientate3 (glm::vec3 (0.0f))))
	{}

	void Rotate (const glm::vec3& euler)
	{
		rotation = glm::rotate (rotation, glm::radians (euler.x), glm::vec3 (1.0f, 0, 0));
		rotation = glm::rotate (rotation, glm::radians (euler.y), glm::vec3 (0, 1.0f, 0));
		rotation = glm::rotate (rotation, glm::radians (euler.z), glm::vec3 (0, 0, 1.0f));
		this->euler = glm::eulerAngles (rotation) * glm::pi<float> () / 180.0f;
	}

	glm::mat4 get_model_matrix ()
	{
		glm::mat4 rot = glm::toMat4 (rotation);
		glm::mat4 trans = glm::translate (glm::mat4 (1.0f), position);
		glm::mat4 scal = glm::scale (glm::mat4 (1.0f), scale);
		return trans * rot * scal;
	}

	void ShowUIMenu (const char* title, int posScale = 1000)
	{
		ImGui::Begin (title, nullptr, ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowSize (ImVec2 (300 * 1.5, 300 * 1.5), ImGuiCond_FirstUseEver);
		ImGui::SetWindowPos (ImVec2 (10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetWindowFontScale (1.5f);

		ImGui::Spacing ();

		float pos_buff[3] = { position.x, position.y, position.z };
		ImGui::SliderFloat3 ("Position", &pos_buff[0], -posScale, posScale);
		position = glm::vec3 (pos_buff[0], pos_buff[1], pos_buff[2]);
		ImGui::Spacing ();
		ImGui::Spacing ();

		float euler_buff[3] = { euler.x, euler.y, euler.z };
		ImGui::SliderFloat3 ("Euler Angles", &euler_buff[0], -180.0f, 180.0f);
		euler = glm::vec3 (euler_buff[0], euler_buff[1], euler_buff[2]);
		rotation = glm::toQuat (glm::orientate3 (glm::vec3 (euler_buff[0], euler_buff[1], euler_buff[2]) * glm::pi<float>() / 180.0f));

		ImGui::Spacing ();
		ImGui::Text ("Quaternion [x: %.3f, y: %.3f, z: %.3f, w: %.3f]", rotation.x, rotation.y, rotation.z, rotation.w);
		
		ImGui::Spacing ();
		ImGui::Spacing ();

		float scale_buff[3] = { scale.x, scale.y, scale.z };
		ImGui::SliderFloat3 ("Scale", &scale_buff[0], 0.0f, posScale);
		scale = glm::vec3 (scale_buff[0], scale_buff[1], scale_buff[2]);

		ImGui::Spacing ();
		ImGui::End ();
	}
};