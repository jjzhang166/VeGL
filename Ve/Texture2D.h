#pragma once

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Debug.h"

class Texture2D
{
public:
	Texture2D (const std::string& p_name, GLuint id, int width, int height, int channels)
		: m_name (p_name), m_textureId(id), m_width(width), m_height(height), m_nchannels(channels)
	{
	}

	Texture2D (const std::string& p_name = "none", GLenum p_type = GL_RGBA, int nchannels=4)
		: m_name(p_name), m_width(1), m_height(1), m_nchannels(nchannels)
	{
		GLCall (glGenTextures (1, &m_textureId));

		GLubyte data[] = { 255, 255, 255, 255 };

		GLCall (glBindTexture (GL_TEXTURE_2D, m_textureId));
		GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall (glTexImage2D (GL_TEXTURE_2D, 0, p_type, 1, 1, 0, p_type, GL_UNSIGNED_BYTE, data));
	}

	Texture2D (const std::string& p_name, const std::string& p_path)
		: m_name(p_name)
	{
		stbi_set_flip_vertically_on_load (true);
		auto data = stbi_load (p_path.c_str(), &m_width, &m_height, &m_nchannels, 0);

		if (data)
		{
			GLCall (glGenTextures (1, &m_textureId));
			GLCall (glBindTexture (GL_TEXTURE_2D, m_textureId));

			GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall (glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall (glPixelStorei (GL_UNPACK_ALIGNMENT, 4));

			switch (m_nchannels)
			{
			case 1:
				GLCall (glTexImage2D (GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, data));
				break;
			case 2:
				GLCall (glTexImage2D (GL_TEXTURE_2D, 0, GL_RG, m_width, m_height, 0, GL_RG, GL_UNSIGNED_BYTE, data));
				break;
			case 3:
				GLCall (glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
				break;
			case 4: 
				GLCall (glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
				break;
			default:
				std::cerr << "[STBI][ERROR] Failed to create opengl texture from image at '" << p_path << "', invalid # of channels: " << m_nchannels << std::endl;
				break;
			}

			GLCall (glGenerateMipmap (GL_TEXTURE_2D));
			stbi_image_free (data);
		}
		else
			std::cerr << "[STBI][ERROR] Failed to load image '" << p_path << "'" << std::endl;
	}

	/*
	Texture2D (const Texture2D& tex)
		: m_width (tex.m_width),
		m_height (tex.m_height),
		m_nchannels (tex.m_nchannels),
		m_name (tex.m_name),
		m_textureId (tex.m_textureId)
	{}


	Texture2D (Texture2D& tex) 
		: m_width(std::move(tex.m_width)),
		m_height (std::move (tex.m_height)),
		m_nchannels (std::move (tex.m_nchannels)),
		m_name (std::move (tex.m_name)),
		m_textureId (std::move (tex.m_textureId))
	{}*/

	~Texture2D ()
	{
		GLCall (glDeleteTextures (1, &m_textureId));
	}
	/*
	Texture2D& operator=(Texture2D& other)
	{
		GLCall (glDeleteTextures (1, &m_textureId));
		m_width = other.m_width;
		m_height = other.m_height;
		m_nchannels = other.m_nchannels;
		m_name = other.m_name;
		m_textureId = other.m_textureId;
		return *this;
	}

	Texture2D& operator=(Texture2D&& other)
	{
		GLCall (glDeleteTextures (1, &m_textureId));
		m_width = std::move (other.m_width);
		m_height = std::move (other.m_height);
		m_name = std::move (other.m_name);
		m_textureId = std::move (other.m_textureId);
		return *this;
	}*/

	void Bind (unsigned unit = 0)
	{
		GLCall (glActiveTexture (GL_TEXTURE0 + unit));
		GLCall (glBindTexture (GL_TEXTURE_2D, m_textureId));
	}

	int GetWidth () const { return m_width; }
	int GetHeight () const { return m_height; }
	int GetNChannels () const { return m_nchannels; }
	const std::string& GetName () const { return m_name; }

private:
	int m_width, m_height, m_nchannels;
	std::string m_name;
	GLuint m_textureId=NULL;
};