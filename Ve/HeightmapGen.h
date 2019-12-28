#pragma once
#include <GL/glew.h>
#include "Debug.h"
#include "Shader.h"
#include "Texture2D.h"

class HeightmapGen
{
public:
	HeightmapGen (GLsizei width, GLsizei height)
		: width (width), height (height)
	{
		compute.LoadShader ("E:/OneDrive - www.saaint.com/Dev/Repositories/GLDev/Ve/Shaders/heightmap_gen.comp", GL_COMPUTE_SHADER);
		compute.CompileShader ();

		GLCall (glGenTextures (1, &compute_texID));
		GLCall (glActiveTexture (GL_TEXTURE0));
		GLCall (glBindTexture (GL_TEXTURE_2D, compute_texID));
		GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall (glTexImage2D (GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, NULL));
		GLCall (glBindImageTexture (0, compute_texID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F));
	}

	~HeightmapGen ()
	{
		if(!gotAsTex)
			GLCall (glDeleteTextures (1, &compute_texID));
	}

	void Setup (GLint unit)
	{
		GLCall (glActiveTexture (GL_TEXTURE0 + unit));
		GLCall (glBindTexture (GL_TEXTURE_2D, compute_texID));

		compute.Bind ();
		compute.SetUniform ("destTex", unit);
	}

	void Run ()
	{
		compute.Bind ();
		GLCall(glDispatchCompute (width, height, 1));
	}

	Texture2D GetTex (const std::string &name)
	{
		gotAsTex = true;
		return Texture2D (name, compute_texID, width, height, 1);
	}

private:
	bool gotAsTex = false;
	GLuint compute_texID;
	GLsizei width, height;
	Shader compute;
};