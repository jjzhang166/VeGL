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

#include "Camera.h"
#include "Transform.h"

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"

#include"HeightmapGen.h"

#include <vector>

GLFWwindow* window;

int width = 1600, height = 1200;

void initGL ()
{
	if (!glfwInit ())
	{
		const char* error;
		glfwGetError (&error);
		std::cerr << "[GLFW][ERROR] Failed to initialize GLFW: " << error << std::endl;
		std::exit (EXIT_FAILURE);
	}

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint (GLFW_SAMPLES, 4);
	glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow (width, height, "Ve", NULL, NULL);
	if (window == NULL)
	{
		const char* error;
		glfwGetError (&error);
		std::cout << "[GLFW][ERROR] Failed to create GLFW window: " << error << std::endl;
		glfwTerminate ();
		std::exit (EXIT_FAILURE);
	}

	glfwMakeContextCurrent (window);

	GLenum glewStatus = glewInit ();
	if (glewStatus != GLEW_OK)
	{
		std::cout << "[GLEW][ERROR] Failed to initialize GLEW: " << glewGetErrorString (glewStatus) << std::endl;
		glfwTerminate ();
		std::exit (EXIT_FAILURE);
	}

	std::cout << "[GLFW][MESSAGE] OpenGL successfully initialized with version: " << glGetString (GL_VERSION) << std::endl;

	glEnable (GL_MULTISAMPLE);
	glEnable (GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);
	//glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

	glClearColor (0.5, 0.5, 0.5, 1);
	glClearDepth (1);
}

void terminateGL ()
{
	glfwDestroyWindow (window);
	glfwTerminate ();
}

ImVec4 fromV (const glm::vec4& v)
{
	return ImVec4 (v.x, v.y, v.z, v.w);
}

struct ImVec3 { float x, y, z; ImVec3 (float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) { x = _x; y = _y; z = _z; } };

void imgui_easy_theming (ImGuiStyle& style, ImVec3 color_for_text, ImVec3 color_for_head, ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops, float alpha = 1.0f, float mul = 1.0f)
{
	color_for_text.x *= mul;
	color_for_text.y *= mul;
	color_for_text.z *= mul;
	color_for_head.x *= mul;
	color_for_head.y *= mul;
	color_for_head.z *= mul;
	color_for_area.x *= mul;
	color_for_area.y *= mul;
	color_for_area.z *= mul;
	color_for_body.x *= mul;
	color_for_body.y *= mul;
	color_for_body.z *= mul;
	color_for_pops.x *= mul;
	color_for_pops.y *= mul;
	color_for_pops.z *= mul;

	style.Colors[ImGuiCol_Text] = ImVec4 (color_for_text.x, color_for_text.y, color_for_text.z, 1.00f * alpha);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4 (color_for_text.x, color_for_text.y, color_for_text.z, 0.58f * alpha);
	style.Colors[ImGuiCol_WindowBg] = ImVec4 (color_for_body.x, color_for_body.y, color_for_body.z, 0.95f * alpha);
	style.Colors[ImGuiCol_Border] = ImVec4 (color_for_body.x, color_for_body.y, color_for_body.z, 0.00f * alpha);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4 (color_for_body.x, color_for_body.y, color_for_body.z, 0.00f * alpha);
	style.Colors[ImGuiCol_FrameBg] = ImVec4 (color_for_area.x, color_for_area.y, color_for_area.z, 1.00f * alpha);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.78f * alpha);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_TitleBg] = ImVec4 (color_for_area.x, color_for_area.y, color_for_area.z, 1.00f * alpha);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4 (color_for_area.x, color_for_area.y, color_for_area.z, 0.75f * alpha);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4 (color_for_area.x, color_for_area.y, color_for_area.z, 0.47f * alpha);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4 (color_for_area.x, color_for_area.y, color_for_area.z, 1.00f * alpha);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.21f * alpha);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.78f * alpha);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_CheckMark] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.80f * alpha);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.50f * alpha);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_Button] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.50f * alpha);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.86f * alpha);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_Header] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.76f * alpha);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.89f * alpha);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.15f * alpha);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.78f * alpha);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_PlotLines] = ImVec4 (color_for_text.x, color_for_text.y, color_for_text.z, 0.63f * alpha);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4 (color_for_text.x, color_for_text.y, color_for_text.z, 0.63f * alpha);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 1.00f * alpha);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4 (color_for_head.x, color_for_head.y, color_for_head.z, 0.43f * alpha);
	style.Colors[ImGuiCol_PopupBg] = ImVec4 (color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f * alpha);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4 (color_for_area.x, color_for_area.y, color_for_area.z, 0.73f * alpha);
}

void SetupImGuiStyleVe ()
{
	static ImVec3 color_for_text = ImVec3 (236.f / 255.f, 240.f / 255.f, 241.f / 255.f);
	static ImVec3 color_for_head = ImVec3 (41.f / 255.f, 128.f / 255.f, 185.f / 255.f);
	static ImVec3 color_for_area = ImVec3 (57.f / 255.f, 79.f / 255.f, 105.f / 255.f);
	static ImVec3 color_for_body = ImVec3 (44.f / 255.f, 62.f / 255.f, 80.f / 255.f);
	static ImVec3 color_for_pops = ImVec3 (33.f / 255.f, 46.f / 255.f, 60.f / 255.f);

	ImGuiStyle& style = ImGui::GetStyle ();

	style.WindowRounding = 5.6f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;
	style.WindowBorderSize = 0;

	float mul = 1.4f;

	imgui_easy_theming (style, color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops, 0.8, 1.2);
}

int main()
{
	initGL ();

	Texture2D tex_sand ("ground_diff", "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/sand.png");
	Texture2D tex_container ("diffuseMap", "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/container.jpg");
	Texture2D tex_container_norm ("normalMap", "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/container_normal.png");

	Mesh plane = Mesh::Primitive_Plane ();
	Mesh cube = Mesh::Primitive_Cube ();

	Shader terrain_shader;
	terrain_shader.LoadShaders ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/terrain.vert",
		GL_VERTEX_SHADER, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/terrain.frag", GL_FRAGMENT_SHADER, NULL);
	terrain_shader.CompileShader ();

	Shader def_shader;
	def_shader.LoadShaders ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/default.vert",
		GL_VERTEX_SHADER, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/default.frag", GL_FRAGMENT_SHADER, NULL);
	def_shader.CompileShader ();


	Camera camera (glm::vec3 (0)); 
	camera.zFar = 100;
	camera.fov = 60;
	camera.Rotate (glm::vec3 (-22.0f, 0,0));

	Transform terrain_transform (glm::vec3 (-250, -24, -250));
	terrain_transform.scale = glm::vec3(500,1,500);

    Transform cube_transform (glm::vec3 (0, -3, -102));

	glm::vec3 lightPos (0, 0, 0); 
	glm::vec3 lightColor (0.2f, 0.2f, 0);

	ImGui::CreateContext ();
	ImGuiIO& io = ImGui::GetIO (); (void)io;
	ImGui_ImplGlfw_InitForOpenGL (window, true);
	ImGui_ImplOpenGL3_Init ("#version 330 core");	

	SetupImGuiStyleVe ();

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

	HeightmapGen gen(500,500);
	gen.Setup (0);
	gen.Run ();

	auto tex = gen.GetTex ("heightmap");

	int polymode = GL_FILL, swapint = 1, shadMod = GL_SMOOTH;
	bool rot = false;
	
	while (!glfwWindowShouldClose (window))
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
		glfwSwapBuffers (window);
		glfwPollEvents ();
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(rot)
			camera.Rotate (glm::vec3 (0, 0, 45.0f) * (float)deltaTime, false);

		terrain_shader.Bind ();
		terrain_shader.BeginLoadingTextures ();
		terrain_shader.LoadTexture (tex_sand);
		terrain_shader.LoadTexture (tex_container_norm);
		terrain_shader.LoadTexture (tex);

		terrain_shader.SetUniform ("model", terrain_transform.get_model_matrix ());
		terrain_shader.SetUniform ("view", camera.GetView ());
		terrain_shader.SetUniform ("projection", camera.GetProjection (width / height));
		terrain_shader.SetUniform ("lightPos", lightPos);
		terrain_shader.SetUniform ("lightColor", lightColor);
		terrain_shader.SetUniform ("fogRanging", fogRanging);
		terrain_shader.SetUniform ("fogInterpolation", fogInterpolation);
		terrain_shader.SetUniform ("fogEnd", fogEnd);
		terrain_shader.SetUniform ("fogStart", fogStart);
		terrain_shader.SetUniform ("fogDensity", fogDensity);
		terrain_shader.SetUniform ("fogColor", fogColor);

		plane.Bind ();

		glDrawElements (GL_TRIANGLES, plane.GetIndexCount (), GL_UNSIGNED_INT, nullptr);
		
		def_shader.Bind ();
		def_shader.BeginLoadingTextures ();
		def_shader.LoadTexture (tex_container);
		def_shader.LoadTexture (tex_container_norm);

		def_shader.SetUniform ("model", cube_transform.get_model_matrix ());
		def_shader.SetUniform ("view", camera.GetView ());
		def_shader.SetUniform ("lightPos", lightPos);
		def_shader.SetUniform ("viewPos", camera.position);

		def_shader.SetUniform ("projection", camera.GetProjection (width / height));
		def_shader.SetUniform ("lightPos", lightPos);
		def_shader.SetUniform ("lightColor", lightColor);
		def_shader.SetUniform ("fogRanging", fogRanging);
		def_shader.SetUniform ("fogInterpolation", fogInterpolation);
		def_shader.SetUniform ("fogEnd", fogEnd);
		def_shader.SetUniform ("fogStart", fogStart);
		def_shader.SetUniform ("fogDensity", fogDensity);
		def_shader.SetUniform ("fogColor", fogColor);
		cube.Bind ();

		glDrawElements (GL_TRIANGLES, cube.GetIndexCount (), GL_UNSIGNED_INT, nullptr);

		
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

			float po[3] = { lightPos.x, lightPos.y, lightPos.z };
			ImGui::SliderFloat3 ("Light Pos", po, -20.0f, 20.0f);
			lightPos = glm::vec3 (po[0], po[1], po[2]);
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
		cube_transform.ShowUIMenu ("Cube - Transform", 30);

		ImGui::End ();
		ImGui::Render ();
		ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData ());
	}

	ImGui_ImplGlfw_Shutdown ();
	ImGui_ImplOpenGL3_Shutdown ();
	terminateGL ();

	return 0;
}