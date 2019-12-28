#pragma once

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw.h"
#include "Vendor/imgui/imgui_impl_opengl3.h"

class Core
{
public:
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
		glClearColor (0.5, 0.5, 0.5, 1);
		glClearDepth (1);

		ImGui::CreateContext ();
		ImGuiIO& io = ImGui::GetIO (); (void)io;
		ImGui_ImplGlfw_InitForOpenGL (window, true);
		ImGui_ImplOpenGL3_Init ("#version 330 core");

		SetupImGuiStyleVe ();
	}

	void terminateGL ()
	{
		ImGui_ImplGlfw_Shutdown ();
		ImGui_ImplOpenGL3_Shutdown ();
		glfwDestroyWindow (window);
		glfwTerminate ();
	}

	float getWindowAspectRatio ()
	{
		return width / height;
	}

	bool isRunning ()
	{
		return !glfwWindowShouldClose (window);
	}

	void Refresh ()
	{
		glfwSwapBuffers (window);
		glfwPollEvents ();
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

private:
	GLFWwindow* window;
	float width=1600, height=1200;

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
};