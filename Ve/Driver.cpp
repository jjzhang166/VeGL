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

int main()
{
	Core core;

	core.initGL ();

	Material obj;
	obj.AddMap (Material::Albedo, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/rusted iron 2/albedo.png");
	obj.AddMap (Material::Normal, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/rusted iron 2/normal.png");
	obj.AddMap (Material::Metallic, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/rusted iron 2/metallic.png");
	obj.AddMap (Material::Roughness, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/rusted iron 2/roughness.png");
	obj.AddMap (Material::Ao, 0.5);

	Mesh plane = Mesh::Primitive_Plane ();
	Mesh cube = Mesh::Primitive_Cube ();

	Camera camera (glm::vec3 (0)); 
	camera.zFar = 100;
	camera.fov = 45;
	camera.Rotate (glm::vec3 (-22.0f, 0,0));

	Transform terrain_transform (glm::vec3 (-250, -24, -250));
	terrain_transform.scale = glm::vec3(500,1,500);

    Transform cube_transform (glm::vec3 (0, -3, -102));

	int fogRanging = 1;
	int fogInterpolation = 1;
	float fogDensity = 0.01f;
	float fogEnd = 80, fogStart = 20;
	glm::vec3 fogColor (0.5f);

	double previousTime = 0;
	double deltaTime = 1;
	std::vector<double> fpsMeasureBuffer;
	double fpsAvg = 0;
	int samples = 100;
	int cur_samples = 0;

	//HeightmapGen gen(500,500);
	//gen.Setup (0);
	//gen.Run ();

	//auto tex = gen.GetTex ("heightmap");

	int polymode = GL_FILL, swapint = 1, shadMod = GL_SMOOTH;
	bool rot = false;

	glm::vec3 lightPositions[] = {
	   glm::vec3 (-10.0f,  10.0f, 10.0f),
	   glm::vec3 (-10.0f,  10.0f, 10.0f),
	   glm::vec3 (-10.0f,  10.0f, 10.0f),
	   glm::vec3 (-10.0f,  10.0f, 10.0f)
	};
	glm::vec3 lightColors[] = {
		glm::vec3 (300.0f, 300.0f, 300.0f),
		glm::vec3 (300.0f, 300.0f, 300.0f),
		glm::vec3 (300.0f, 300.0f, 300.0f),
		glm::vec3 (300.0f, 300.0f, 300.0f)
	};

	float lightIntensities[] = {
		1,1,1,1
	};

	glm::vec3 albedo(1,0,0);
	float metallic=1, roughness=1, ao=1.0f;

	Renderer renderer;
	
	while (core.isRunning())
	{
		//Timing
		double currentTime = glfwGetTime ();
		deltaTime = (currentTime - previousTime);
		previousTime = currentTime;
		fpsMeasureBuffer.push_back (1.0f / deltaTime);

		if (cur_samples++>=samples)
		{
			cur_samples = 0;
			fpsAvg = 0;
			for (auto i = 0; i < fpsMeasureBuffer.size (); i++)
				fpsAvg += fpsMeasureBuffer[i];
			fpsAvg /= fpsMeasureBuffer.size ();
			fpsMeasureBuffer.clear ();
		}

		//GL
		core.Refresh ();

		if(rot)
			camera.Rotate (glm::vec3 (0, 0, 45.0f) * (float)deltaTime, false);
		
		renderer.Render (camera, cube, obj, core, cube_transform, lightPositions, lightColors, lightIntensities, 4);

		//def_shader.SetUniform ("fogRanging", fogRanging);
		//def_shader.SetUniform ("fogInterpolation", fogInterpolation);
		//def_shader.SetUniform ("fogEnd", fogEnd);
		//def_shader.SetUniform ("fogStart", fogStart);
		//def_shader.SetUniform ("fogDensity", fogDensity);
		//def_shader.SetUniform ("fogColor", fogColor);
		
		//ImGui
		ImGui_ImplGlfw_NewFrame ();
		ImGui_ImplOpenGL3_NewFrame ();
		ImGui::NewFrame ();

		ImGui::Begin ("Control", nullptr, ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowSize (ImVec2 (300 * 1.5, 600 * 1.5), ImGuiCond_FirstUseEver);
		ImGui::SetWindowPos (ImVec2 (10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetWindowFontScale (1.5f);
		
		if (ImGui::CollapsingHeader ("Performance"))
		{
			ImGui::Spacing ();
			ImGui::Text ("Avg. fps: %.2f", fpsAvg);
			ImGui::Text ("Avg. frametime (ms): %.2f", 1000.0f/fpsAvg);
			ImGui::Spacing ();
			ImGui::Text ("Last fps: %.2f", 1.0f / deltaTime);
			ImGui::Text ("Last frametime (ms): %.2f", 1000.0*deltaTime);
			ImGui::Spacing ();
		}

		ImGui::Spacing ();

		if (ImGui::CollapsingHeader ("Fog"))
		{
			ImGui::Spacing ();
			ImGui::Text ("Type");
			ImGui::RadioButton ("Linear", &fogInterpolation, 0); ImGui::SameLine ();
			ImGui::RadioButton ("Exp", &fogInterpolation, 1); ImGui::SameLine ();
			ImGui::RadioButton ("Exp^2", &fogInterpolation, 2);
			ImGui::Spacing ();
			ImGui::Text ("Distance mode");
			ImGui::RadioButton ("Plane", &fogRanging, 0); ImGui::SameLine ();
			ImGui::RadioButton ("Range", &fogRanging, 1);
			ImGui::Spacing ();

			if (fogInterpolation == 0)
			{
				ImGui::InputFloat ("Start", &fogStart);
				ImGui::InputFloat ("End", &fogEnd);
			}
			else
				ImGui::SliderFloat ("Density", &fogDensity, 0.0f, 1.0f);

			float cols[3] = { fogColor.x, fogColor.y, fogColor.z };
			ImGui::SliderFloat3 ("Color", cols, 0.0f, 1.0f);
			fogColor = glm::vec3 (cols[0], cols[1], cols[2]);
			ImGui::Spacing ();
		}

		if (ImGui::CollapsingHeader ("Lighting"))
		{
			renderer.ShowMenu (obj, "Material");
			ImGui::Spacing ();

			for (unsigned i = 0; i < sizeof (lightPositions) / sizeof (lightPositions[0]); ++i)
			{
				std::string title = "Light #" + std::to_string(i+1);

				ImGui::PushID (i);
				if (ImGui::CollapsingHeader (title.c_str()))
				{
					ImGui::Spacing ();
					float lpb[3] = { lightPositions[i].x,  lightPositions[i].y,  lightPositions[i].z };
					ImGui::SliderFloat3 ("Light Pos", lpb, -20.0f, 20.0f);
					lightPositions[i] = glm::vec3 (lpb[0], lpb[1], lpb[2]);
					ImGui::Spacing ();

					float lcb[3] = { lightColors[i].x,  lightColors[i].y,  lightColors[i].z };
					ImGui::SliderFloat3 ("Light Color", lcb, -20.0f, 20.0f);
					lightColors[i] = glm::vec3 (lcb[0], lcb[1], lcb[2]);
					ImGui::Spacing ();

					float inte = lightIntensities[i];
					ImGui::SliderFloat ("Intensity", &inte, 0.0, 10);
					lightIntensities[i] = inte;
					ImGui::Spacing ();
				}
				ImGui::PopID ();
				ImGui::Spacing ();
			}
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