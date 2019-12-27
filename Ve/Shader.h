#pragma once
#include "Debug.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <cstdarg>

#include "Texture2D.h"
#include <glm/gtc/type_ptr.hpp>

static int textureIdx = 0;

class Shader
{
public:
    Shader ()
    {
        GLCall (m_programId = glCreateProgram (););
    }

    ~Shader ()
    {
        GLCall (glDeleteProgram (m_programId));
    }

    static void CheckStatus (GLuint obj)
    {
        GLint status = GL_FALSE;
        GLCall(if (glIsShader (obj)) glGetShaderiv (obj, GL_COMPILE_STATUS, &status));
        GLCall (if (glIsProgram (obj)) glGetProgramiv (obj, GL_LINK_STATUS, &status););
        GLCall (if (status == GL_TRUE) return;);
        GLchar log[1 << 15] = { 0 };
        GLCall (if (glIsShader (obj)) glGetShaderInfoLog (obj, sizeof (log), NULL, log););
        GLCall (if (glIsProgram (obj)) glGetProgramInfoLog (obj, sizeof (log), NULL, log););
        std::cerr << "[OPENGL][ERROR] Shader status check failed: " << log << std::endl;
        std::exit (EXIT_FAILURE);
    }

    void LoadShaders (const char* shader_path, ...)
    {
        va_list args;
        va_start (args, shader_path);
        while (shader_path)
        {
            const GLenum type = va_arg (args, GLenum);

            LoadShader (shader_path, type);

            shader_path = va_arg (args, const char*);
        }
        va_end (args);
    }

    void LoadShader (const std::string& path, const GLenum type)
    {
        std::ifstream t (path);
        std::string shader_src ((std::istreambuf_iterator<char> (t)),
            std::istreambuf_iterator<char> ());
        AttachShader (type, shader_src.c_str ());
    }

    void CompileShader ()
    {
        GLCall (glLinkProgram (m_programId));
        GLCall (glValidateProgram (m_programId));
        CheckStatus (m_programId);
    }

    void AttachShader (GLenum type, const char* src)
    {
        GLuint shader_id = glCreateShader (type);
        GLCall (glShaderSource (shader_id, 1, &src, NULL));
        GLCall (glCompileShader (shader_id));
        CheckStatus (shader_id);
        GLCall (glAttachShader (m_programId, shader_id));
        GLCall (glDeleteShader (shader_id));
    }

    void Bind ()
    {
        GLCall (glUseProgram (m_programId));
    }

    void Disable ()
    {
        GLCall (glUseProgram (0));
    }

    GLint GetUniformLocation (const std::string& m_name)
    {
        auto search = m_uniformCache.find (m_name);
        if (search != m_uniformCache.end ())
            return search->second;

        GLCall (GLint location = glGetUniformLocation (m_programId, m_name.c_str ()));

        if (location == -1)
            std::cerr << "[Error][NkGL] Could not retrieve location of uniform '" << m_name << "' to set it" << std::endl;

        m_uniformCache[m_name] = location;
        return location;
    }

    template <class T = int>
    void SetUniform (const std::string& m_name, T value)
    {
        glUniform1i (GetUniformLocation (m_name), value);
    }

    template <class T = float>
    void SetUniform (const std::string& m_name, float value)
    {
        glUniform1f (GetUniformLocation (m_name), value);
    }

    template <class T = glm::vec2>
    void SetUniform (const std::string& m_name, glm::vec2 value)
    {
        glUniform2f (GetUniformLocation (m_name), value.x, value.y);
    }

    template <class T = glm::vec3>
    void SetUniform (const std::string& m_name, glm::vec3 value)
    {
        glUniform3f (GetUniformLocation (m_name), value.x, value.y, value.z);
    }

    template <class T = glm::vec4>
    void SetUniform (const std::string& m_name, glm::vec4 value)
    {
        glUniform4f (GetUniformLocation (m_name), value.x, value.y, value.z, value.w);
    }

    template <class T = glm::mat3>
    void SetUniform (const std::string& m_name, glm::mat3 value)
    {
        glUniformMatrix3fv (GetUniformLocation (m_name), 1, GL_FALSE, glm::value_ptr(value));
    }

    template <class T = glm::mat4>
    void SetUniform (const std::string& m_name, glm::mat4 value)
    {
        glUniformMatrix4fv (GetUniformLocation (m_name), 1, GL_FALSE, glm::value_ptr (value));
    }

    void BeginLoadingTextures ()
    {
        textureIdx = 0;
    }

    void LoadTexture(Texture2D &value)
    {
        value.Bind (textureIdx);
        glUniform1i (GetUniformLocation (value.GetName()), textureIdx++);
    }

private:
    GLuint m_programId;
    std::unordered_map<std::string, GLuint> m_uniformCache;
};