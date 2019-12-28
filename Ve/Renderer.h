#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Transform.h"
#include "Core.h"
#include <map>

class Material
{
public:
	enum MapType
	{
		Albedo = 0,
		Normal = 1,
		Metallic = 2,
		Roughness = 3,
		Ao = 4
	};

	void AddMap (MapType type, const std::string& path, float coefficient = 1.0f)
	{
		std::string type_str = typeToStr (type) + "Map";
		maps[type_str] = std::make_pair (new Texture2D (type_str, path), glm::vec3(coefficient));
	}

	void AddMap (MapType type, float coefficient = 1.0f)
	{
		std::string type_str = typeToStr(type) + "Map";
		maps[type_str] = std::make_pair (new Texture2D (), glm::vec3(coefficient));
	}

	void AddMap (MapType type, glm::vec3 coefficient)
	{
		std::string type_str = typeToStr (type) + "Map";
		maps[type_str] = std::make_pair (new Texture2D (), coefficient);
	}

	void LoadToShader (MapType type, Shader &shader)
	{
		std::string ot = typeToStr (type);
		std::string type_str = ot + "Map";
		std::string coeff_str = ot + "Coeff";

		auto search = maps.find (type_str);
		if (search != maps.end())
		{
			shader.LoadTexture (search->second.first);
			shader.SetUniform (coeff_str, search->second.second);
			return;
		}

		std::cerr << "[VE][ERROR] Material does not contain map '" << type_str << "'" << std::endl;
	}

	void ShowMenuForMap (MapType type, const std::string &hint, bool rgb = false)
	{
		std::string type_str = typeToStr (type);
		glm::vec3& vref = maps[type_str + "Map"].second;

		if (rgb)
		{
			float color_buff[3] = { vref.x, vref.y, vref.z };
			ImGui::SliderFloat3 (hint.c_str(), &color_buff[0], 0.0, 1.0);
			vref = glm::vec3 (color_buff[0], color_buff[1], color_buff[2]);
		}
		else
			ImGui::SliderFloat (hint.c_str (), &vref.x, 0.0f, 1.0f);
	}

private:
	std::map<std::string, std::pair<Texture2D*, glm::vec3>> maps;

	std::string typeToStr (MapType type)
	{
		switch ((int)type)
		{
		case Albedo:
			return "material.albedo";
		case Normal:
			return "material.normal";
		case Metallic:
			return "material.metallic";
		case Roughness:
			return "material.roughness";
		case Ao:
			return "material.ao";
		default:
			std::cerr << "[VE][ERROR] Invalid map type #" << (int)type << std::endl;
			return "";
		}
	}
};

struct Light
{
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
};

class Renderer
{
public:
	Renderer ()
	{
		m_stdShader.LoadShaders ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/pbr.vert", GL_VERTEX_SHADER,
			"E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/pbr.frag", GL_FRAGMENT_SHADER,
			NULL);
		m_stdShader.CompileShader ();

		m_terrainShader.LoadShaders ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/terrain.vert", GL_VERTEX_SHADER, 
			"E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/terrain.frag", GL_FRAGMENT_SHADER, 
			NULL);
		m_terrainShader.CompileShader ();
		
		glGenBuffers (1, &lights_ssbo);
		glBindBuffer (GL_SHADER_STORAGE_BUFFER, lights_ssbo);
		glBufferData (GL_SHADER_STORAGE_BUFFER, sizeof (Light), &lights[0], GL_DYNAMIC_COPY);
		glBindBuffer (GL_SHADER_STORAGE_BUFFER, 0);
	}

	void ShowMenu (Material& mat, const std::string& title)
	{
		if (ImGui::CollapsingHeader (title.c_str ()))
		{
			mat.ShowMenuForMap (Material::Albedo, "Albedo", true);
			mat.ShowMenuForMap (Material::Normal, "Normal");
			mat.ShowMenuForMap (Material::Metallic, "Metallic");
			mat.ShowMenuForMap (Material::Ao, "Ao");
			mat.ShowMenuForMap (Material::Roughness, "Roughness");
		}
	}

	template<typename T = Mesh>
	void Render (Camera& camera, T& renderable, Material& mat, Core& core,
		Transform trans, glm::vec3* lightPositions, glm::vec3* lightColors, float* lightIntensities, unsigned n_lights)
	{
		m_stdShader.Bind ();
		m_stdShader.BeginLoadingTextures ();
		
		mat.LoadToShader (Material::Albedo, m_stdShader);
		mat.LoadToShader (Material::Normal, m_stdShader);
		mat.LoadToShader (Material::Metallic, m_stdShader);
		mat.LoadToShader (Material::Ao, m_stdShader);
		mat.LoadToShader (Material::Roughness, m_stdShader);

		m_stdShader.SetUniform ("model", trans.get_model_matrix ());
		m_stdShader.SetUniform ("view", camera.GetView ());
		m_stdShader.SetUniform ("projection", camera.GetProjection (core.getWindowAspectRatio ()));
		m_stdShader.SetUniform ("camPos", camera.position);

	//	glBindBuffer (GL_SHADER_STORAGE_BUFFER, lights_ssbo);
	//	GLvoid* p = glMapBuffer (GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	//	memcpy (p, &lights[0], sizeof (Light) * lights.size())
	//	glUnmapBuffer (GL_SHADER_STORAGE_BUFFER);

		for (unsigned int i = 0; i < n_lights; ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3 (sin (glfwGetTime () * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			m_stdShader.SetUniform ("lightPositions[" + std::to_string (i) + "]", newPos);
			m_stdShader.SetUniform ("lightColors[" + std::to_string (i) + "]", lightColors[i] * lightIntensities[i]);
		}

		renderable.Bind ();
		glDrawElements (GL_TRIANGLE_STRIP, renderable.GetIndexCount (), GL_UNSIGNED_INT, nullptr);
	}

private:
	Shader m_stdShader;
	Shader m_terrainShader;
	GLuint lights_ssbo;
	std::vector<Light> lights;
};