#define GLEW_STATIC 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <cstring>

#include "Texture2D.h"
#include "VertexSpecification.h"

#include "Mesh.h"
#include "Shader.h"
#include "Renderer.h"

#include "Camera.h"
#include "Transform.h"

#include"HeightmapGen.h"
#include "Core.h"

#include <vector>

int main ()
{
	Core core;

	core.initGL ();

	Material obj;
	std::string tex = "tile fancy";
	obj.AddMap (Material::Albedo, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/" + tex + "/albedo.png");
	obj.AddMap (Material::Normal, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/" + tex + "/normal.png");
	obj.AddMap (Material::Metallic, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/" + tex + "/metal.png");
	obj.AddMap (Material::Roughness, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/" + tex + "/roughness.png");
	obj.AddMap (Material::Ao, 0.5);

	Material terrain;
	terrain.AddMap (Material::Albedo, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/" + tex + "/albedo.png");
	terrain.AddMap (Material::Normal, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/" + tex + "/normal.png");
	terrain.AddMap (Material::Metallic, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/" + tex + "/metal.png");
	terrain.AddMap (Material::Roughness, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/" + tex + "/roughness.png");
	terrain.AddMap (Material::Ao, 0.5);


	Mesh plane = Mesh::Primitive_Plane ();
	Mesh cube = Mesh::Primitive_Sphere ();

	Camera camera (glm::vec3 (0));
	camera.zFar = 100;
	camera.fov = 45;
	camera.Rotate (glm::vec3 (-22.0f, 0, 0));

	Transform terrain_transform (glm::vec3 (-250, -24, -250));
	terrain_transform.scale = glm::vec3 (500, 1, 500);

	Transform cube_transform (glm::vec3 (0, -3, -102));

	HeightmapGen gen(500,500);
	gen.Setup (0);
	gen.Run ();

	auto ht = gen.GetTex ("heightmap");
	terrain.AddMap (Material::Heightmap, &ht, 1.0f);

	int polymode = GL_FILL, swapint = 1, shadMod = GL_SMOOTH;
	bool rot = false;

	Renderer renderer;

	std::vector<Light> lights;
	lights.push_back (Light{ glm::vec3 (0), glm::vec3 (1), 10.0f });
	lights.push_back (Light{ glm::vec3 (0), glm::vec3 (1), 10.0f });
	lights.push_back (Light{ glm::vec3 (0), glm::vec3 (1), 10.0f });
	lights.push_back (Light{ glm::vec3 (0), glm::vec3 (1), 10.0f });

	FogSettings fogSettings;
	fogSettings.rangeMode = FogRanging::Range;
	fogSettings.interpolationMode = FogInterpolation::Exponential;
	fogSettings.density = 0.01f;
	fogSettings.end = 80;
	fogSettings.start = 20;
    fogSettings.color = glm::vec3 (0.5f);
	
	while (core.isRunning())
	{
		core.Refresh ();

		if(rot)
			camera.Rotate (glm::vec3 (0, 0, 45.0f) * (float)core.GetDeltaTime(), false);
		
		renderer.RenderMesh (camera, cube, obj, core, cube_transform, lights, fogSettings);
		renderer.RenderTerrain (camera, plane, terrain, core, terrain_transform, lights, fogSettings);
		
		//ImGui
		ImGui_ImplGlfw_NewFrame ();
		ImGui_ImplOpenGL3_NewFrame ();
		ImGui::NewFrame ();

		ImGui::Begin ("Control", nullptr, ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowSize (ImVec2 (300 * 1.5, 600 * 1.5), ImGuiCond_FirstUseEver);
		ImGui::SetWindowPos (ImVec2 (10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetWindowFontScale (1.5f);
		
		core.ShowPerformanceUI ("Performance");

		ImGui::Spacing ();

		if (ImGui::CollapsingHeader ("Fog"))
		{
			ImGui::Spacing ();
			ImGui::Text ("Type");
			ImGui::RadioButton ("Linear", (int*)&fogSettings.interpolationMode, 0); ImGui::SameLine ();
			ImGui::RadioButton ("Exp", (int*)&fogSettings.interpolationMode, 1); ImGui::SameLine ();
			ImGui::RadioButton ("Exp^2", (int*)&fogSettings.interpolationMode, 2);
			ImGui::Spacing ();
			ImGui::Text ("Distance mode");
			ImGui::RadioButton ("Plane", (int*)&fogSettings.rangeMode, 0); ImGui::SameLine ();
			ImGui::RadioButton ("Range", (int*)&fogSettings.rangeMode, 1);
			ImGui::Spacing ();

			if (fogSettings.interpolationMode == 0)
			{
				ImGui::InputFloat ("Start", &fogSettings.start);
				ImGui::InputFloat ("End", &fogSettings.end);
			}
			else
				ImGui::SliderFloat ("Density", &fogSettings.density, 0.0f, 1.f);

			float cols[3] = { fogSettings.color.x,  fogSettings.color.y,  fogSettings.color.z };
			ImGui::SliderFloat3 ("Color", cols, 0.0f, 1.0f);
			fogSettings.color = glm::vec3 (cols[0], cols[1], cols[2]);
			ImGui::Spacing ();
		}

		if (ImGui::CollapsingHeader ("Lighting"))
		{
			renderer.ShowMenu (obj, "Material");
			ImGui::Spacing ();

			for (unsigned i = 0; i < lights.size(); ++i)
			{
				std::string title = "Light #" + std::to_string(i+1);

				ImGui::PushID (i);
				if (ImGui::CollapsingHeader (title.c_str()))
				{
					Light& light = lights[i];

					ImGui::Spacing ();
					float lpb[3] = { light.pos.x, light.pos.y, light.pos.z };
					ImGui::SliderFloat3 ("Light Pos", lpb, -20.0f, 20.0f);
					light.pos = glm::vec3 (lpb[0], lpb[1], lpb[2]);
					ImGui::Spacing ();

					float lcb[3] = { light.col.x, light.col.y, light.col.z };
					ImGui::SliderFloat3 ("Light Col", lcb, 0.0f, 1.0f);
					light.col = glm::vec3 (lcb[0], lcb[1], lcb[2]);
					ImGui::Spacing ();

					float inte = light.intensity;
					ImGui::SliderFloat ("Intensity", &inte, 0.0, 300.0f);
					light.intensity = inte;
					ImGui::Spacing ();
				}
				ImGui::PopID ();
				ImGui::Spacing ();
			}

			if (ImGui::Button ("Add Light"))
			{
				lights.push_back (Light{ glm::vec3 (0), glm::vec3 (1), 10.0f });
			}

			ImGui::Spacing ();
		}

		if (ImGui::CollapsingHeader ("Debug"))
		{
			ImGui::RadioButton ("Wireframe", &polymode, GL_LINE); ImGui::SameLine ();
			ImGui::RadioButton ("Shaded", &polymode, GL_FILL);
			ImGui::SliderInt ("Swap Interval", &swapint, 0, 6);
			ImGui::RadioButton ("Smooth shading", &shadMod,  GL_SMOOTH);
			ImGui::RadioButton ("Flat shading", &shadMod, GL_FLAT);
			ImGui::Checkbox ("Rotate Camera", &rot);

		    glPolygonMode (GL_FRONT_AND_BACK, polymode);
			glfwSwapInterval (swapint);
			glShadeModel (shadMod);
		}

		terrain_transform.ShowUIMenu ("Terrain - Transform");
		cube_transform.ShowUIMenu ("Cube - Transform", 10);

		ImGui::End ();
		ImGui::Render ();
		ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData ());
	}

	core.terminateGL ();

	return 0;
}