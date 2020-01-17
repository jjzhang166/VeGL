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
		Ao = 4,
		Heightmap = 5
	};

	~Material ()
	{
		/*std::map<std::string, std::pair<Texture2D*, glm::vec3>>::iterator it;
		for (it = maps.begin (); it != maps.end (); it++) {
			delete it->second.first;
		}
		maps.clear ();*/
	}

	void AddMap (MapType type, const std::string& path, float coefficient = 1.0f)
	{
		std::string type_str = typeToStr (type, "Map");
		maps[type_str] = std::make_pair (new Texture2D (type_str, path), glm::vec3(coefficient));
	}

	void AddMap (MapType type, float coefficient = 1.0f)
	{
		maps[typeToStr (type, "Map")] = std::make_pair (new Texture2D (), glm::vec3(coefficient));
	}

	void AddMap (MapType type, glm::vec3 coefficient)
	{
		maps[typeToStr (type, "Map")] = std::make_pair (new Texture2D (), coefficient);
	}


	void AddMap (MapType type, Texture2D* map, float coefficient = 1.0f)
	{
		maps[typeToStr (type, "Map")] = std::make_pair (map, glm::vec3(coefficient));
	}
	void LoadToShader (MapType type, Shader &shader)
	{
		std::string type_str = typeToStr (type);
		std::string coeff_str = type_str + "Coeff";
		type_str += "Map";

		auto search = maps.find (type_str);
		if (search != maps.end())
		{
			shader.LoadTexture (search->second.first);
			shader.SetUniform (coeff_str, search->second.second);
			return;
		}

		std::cerr << "[VE][ERROR] Material does not contain map '" << type_str << "'" << std::endl;
	}

	void ShowMenuForMap (MapType type, const std::string &hint, bool coeff_3e = false)
	{
		glm::vec3& vref = maps[typeToStr (type, "Map")].second;

		if (coeff_3e)
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

	std::string typeToStr (MapType type, const std::string& suffix = "")
	{
		switch ((int)type)
		{
		case Albedo:
			return "material.albedo" + suffix;
		case Normal:
			return "material.normal" + suffix;
		case Metallic:
			return "material.metallic" + suffix;
		case Roughness:
			return "material.roughness" + suffix;
		case Ao:
			return "material.ao" + suffix;
		case Heightmap:
			return "heightmap" + suffix;
		default:
			std::cerr << "[VE][ERROR] Invalid map type #" << (int)type << std::endl;
			return "";
		}
	}
};

struct Light
{
	glm::vec3 pos;
	glm::vec3 col;
	float intensity;
};

enum FogRanging
{
	Plane, Range
};

enum FogInterpolation
{
	Linear, Exponential, ExponentialSquared
};

struct FogSettings
{
	FogRanging rangeMode;
	FogInterpolation interpolationMode;
	float start, end;
	float density;
	glm::vec3 color;
};

class Renderer
{
public:
	Renderer (unsigned max_lights = 100)
		: max_lights(max_lights)
	{
		m_stdShader.LoadShader ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/pbr.vert", GL_VERTEX_SHADER);
	    m_stdShader.LoadShader ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/pbr.frag", GL_FRAGMENT_SHADER, 
	    "#version 430 core\n"
	    "const uint max_lights = " + std::to_string(max_lights) + ";\n"
		"uniform struct LightingData"
		"{"
			"vec3 lightPositions[" + std::to_string(max_lights) + "];"
			"vec3 lightColors[" + std::to_string(max_lights) + "];"
			"float lightIntensities[" + std::to_string(max_lights) + "];"
		"} lighting_data;");
		m_stdShader.CompileShader ();

		m_terrainShader.LoadShader ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/pbr_terrain.vert", GL_VERTEX_SHADER);
		m_terrainShader.LoadShader ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/pbr.frag", GL_FRAGMENT_SHADER,
			"#version 430 core\n"
			"const uint max_lights = " + std::to_string (max_lights) + ";\n"
			"uniform struct LightingData"
			"{"
			"vec3 lightPositions[" + std::to_string (max_lights) + "];"
			"vec3 lightColors[" + std::to_string (max_lights) + "];"
			"float lightIntensities[" + std::to_string (max_lights) + "];"
			"} lighting_data;");
		m_terrainShader.CompileShader ();
	}

	~Renderer ()
	{
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

	void RenderMesh (Camera& camera, Mesh& renderable, Material& mat, Core& core,
		Transform trans, const std::vector<Light> &lights, const FogSettings& fogSettings)
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

		unsigned sz_lights = lights.size () >= max_lights ? max_lights : lights.size ();
		for (unsigned idx = 0; idx < lights.size(); ++idx)
		{
			m_stdShader.SetUniform ("lighting_data.lightPositions[" + std::to_string (idx) + "]", lights[idx].pos);
			m_stdShader.SetUniform ("lighting_data.lightColors[" + std::to_string (idx) + "]", lights[idx].col);
			m_stdShader.SetUniform ("lighting_data.lightIntensities[" + std::to_string(idx) + "]", lights[idx].intensity);
		}

		m_stdShader.SetUniform ("fog_settings.fogRanging", fogSettings.rangeMode);
		m_stdShader.SetUniform ("fog_settings.fogInterpolation", fogSettings.interpolationMode);
		m_stdShader.SetUniform ("fog_settings.fogEnd", fogSettings.end);
		m_stdShader.SetUniform ("fog_settings.fogStart", fogSettings.start);
		m_stdShader.SetUniform ("fog_settings.fogDensity", fogSettings.density);
		m_stdShader.SetUniform ("fog_settings.fogColor", fogSettings.color);

		renderable.Bind ();
		glDrawElements (renderable.GetDrawMode(), renderable.GetIndexCount (), GL_UNSIGNED_INT, nullptr);
	}

	void RenderTerrain (Camera& camera, Mesh& renderable, Material& mat, Core& core,
		Transform trans, const std::vector<Light>& lights, const FogSettings& fogSettings)
	{
		m_terrainShader.Bind ();
		m_terrainShader.BeginLoadingTextures ();

		mat.LoadToShader (Material::Albedo, m_terrainShader);
		mat.LoadToShader (Material::Normal, m_terrainShader);
		mat.LoadToShader (Material::Metallic, m_terrainShader);
		mat.LoadToShader (Material::Ao, m_terrainShader);
		mat.LoadToShader (Material::Roughness, m_terrainShader);
		mat.LoadToShader (Material::Heightmap, m_terrainShader);

		m_terrainShader.SetUniform ("model", trans.get_model_matrix ());
		m_terrainShader.SetUniform ("view", camera.GetView ());
		m_terrainShader.SetUniform ("projection", camera.GetProjection (core.getWindowAspectRatio ()));
		m_terrainShader.SetUniform ("camPos", camera.position);

		unsigned sz_lights = lights.size () >= max_lights ? max_lights : lights.size ();
		for (unsigned idx = 0; idx < lights.size (); ++idx)
		{
			m_terrainShader.SetUniform ("lighting_data.lightPositions[" + std::to_string (idx) + "]", lights[idx].pos);
			m_terrainShader.SetUniform ("lighting_data.lightColors[" + std::to_string (idx) + "]", lights[idx].col);
			m_terrainShader.SetUniform ("lighting_data.lightIntensities[" + std::to_string (idx) + "]", lights[idx].intensity);
		}

		m_terrainShader.SetUniform ("fog_settings.fogDensity", fogSettings.density);
		m_terrainShader.SetUniform ("fog_settings.fogRanging", fogSettings.rangeMode);
		m_terrainShader.SetUniform ("fog_settings.fogInterpolation", fogSettings.interpolationMode);
		m_terrainShader.SetUniform ("fog_settings.fogEnd", fogSettings.end);
		m_terrainShader.SetUniform ("fog_settings.fogStart", fogSettings.start);
		m_terrainShader.SetUniform ("fog_settings.fogColor", fogSettings.color);

		renderable.Bind ();
		glDrawElements (renderable.GetDrawMode (), renderable.GetIndexCount (), GL_UNSIGNED_INT, nullptr);
	}
private:

	unsigned max_lights;
	Shader m_stdShader;
	Shader m_terrainShader;

};