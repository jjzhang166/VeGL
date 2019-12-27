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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>

GLFWwindow* window;

int width = 1600, height = 1200;
double previousTime = 0;
double lastTimeMeasured = 0;
double deltaTime = 1;
std::vector<double> fpsMeasureBuffer;
double fpsAvg = 0;

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
	glfwSwapInterval (1);

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
}

void terminateGL ()
{
	glfwDestroyWindow (window);
	glfwTerminate ();
}

int main()
{
	initGL ();

	Texture2D diffuse ("ground_diff", "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/sand.png");
	Texture2D normal ("ground_norm", "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Res/sand_normal.png");

	Mesh mesh = Mesh::Primitive_Plane (glm::vec2 (500, 500));
	Shader shader;
	shader.LoadShaders ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/default.vert", 
		GL_VERTEX_SHADER, "E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/default.frag", GL_FRAGMENT_SHADER, NULL);
	shader.CompileShader ();

	Camera camera (glm::vec3 (0)); 
	camera.zFar = 100;
	camera.Rotate (glm::vec3 (-22.0f, 0,0));

	Transform terrain_transform (glm::vec3 (-250, -12, -500));
	glm::vec3 lightPos (0, 0, 0); 
	glm::vec3 lightColor (0.2f, 0.2f, 0);

	ImGui::CreateContext ();
	ImGuiIO& io = ImGui::GetIO (); (void)io;
	ImGui::StyleColorsLight ();
	ImGui_ImplGlfw_InitForOpenGL (window, true);
	ImGui_ImplOpenGL3_Init ("#version 330 core");

	shader.Bind ();
	shader.LoadTexture (diffuse);
	shader.LoadTexture (normal);

	int fogDepth = 1;
	int fogSelector = 1;
	float fogDensity = 0.01f;
	glm::vec3 fogColor (0.5f);

	while (!glfwWindowShouldClose (window))
	{
		double currentTime = glfwGetTime ();
		deltaTime = (currentTime - previousTime);
		previousTime = currentTime;
		fpsMeasureBuffer.push_back (1.0f / deltaTime);

		if (currentTime - lastTimeMeasured >= 0.1)
		{
			fpsAvg = 0;
			for (auto i = 0; i < fpsMeasureBuffer.size (); i++)
				fpsAvg += fpsMeasureBuffer[i];
			fpsAvg /= fpsMeasureBuffer.size ();
			fpsMeasureBuffer.clear ();
			lastTimeMeasured = currentTime;
		}

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplGlfw_NewFrame ();
		ImGui_ImplOpenGL3_NewFrame ();
		ImGui::NewFrame ();

		ImGui::Begin ("Performance");  
		ImGui::SetWindowSize (ImVec2 (300, 600));
		ImGui::Text ("FPS: %.2f", fpsAvg);

		ImGui::CollapsingHeader ("Fog");
		ImGui::Text ("type");
		ImGui::RadioButton ("Linear", &fogSelector, 0); ImGui::SameLine ();
		ImGui::RadioButton ("Exp", &fogSelector, 1); ImGui::SameLine ();
		ImGui::RadioButton ("Exp^2", &fogSelector, 2);
		ImGui::Text ("Dist mode");
		ImGui::RadioButton ("Plane", &fogDepth, 0); ImGui::SameLine ();
		ImGui::RadioButton ("Range", &fogDepth, 1);

		ImGui::SliderFloat ("Density", &fogDensity, 0.0f, 1.0f);

		float cols[3] = { fogColor.x, fogColor.y, fogColor.z };
		ImGui::SliderFloat3 ("Color", cols, 0.0f, 1.0f);
		fogColor = glm::vec3 (cols[0], cols[1], cols[2]);

		ImGui::End ();



		//camera.Rotate (glm::vec3 (0,0, 45.0f) * (float)deltaTime, false);

		shader.Bind ();
		mesh.Bind ();
		shader.SetUniform ("model", terrain_transform.get_model_matrix ());
		shader.SetUniform ("view", camera.GetView ());
		shader.SetUniform ("projection", camera.GetProjection (width / height));
		shader.SetUniform ("lightPos", lightPos);
		shader.SetUniform ("lightColor", lightColor);

		shader.SetUniform ("depthFog", fogDepth);
		shader.SetUniform ("fogSelector", fogSelector);
		shader.SetUniform ("fogDensity", fogDensity);
		shader.SetUniform ("fogColor", fogColor);

		glDrawElements (GL_TRIANGLES, mesh.GetIndexCount (), GL_UNSIGNED_INT, nullptr);

		ImGui::Render ();
		ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData ());

		glfwSwapBuffers (window);
		glfwPollEvents ();
	}

	ImGui_ImplGlfw_Shutdown ();
	ImGui_ImplOpenGL3_Shutdown ();
	terminateGL ();

	return 0;
}