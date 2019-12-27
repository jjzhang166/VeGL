#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static bool GLLogCall (const char* func, const char* file, int line)
{
    while (auto err = glGetError ())
    {
        std::cerr << "[OPENGL][ERROR] OpenGL Error at func \"" << func << "\", file \"" << file << "\", line #" << line << ": " << err << std::endl;
        return false;
    }
    return true;
}

#define ASSERT(x)\
    if(!(x))\
        __debugbreak ();

#define GLCall(x) while (glGetError () != GL_NO_ERROR);\
    x;\
    if(!GLLogCall(#x, __FILE__, __LINE__)) __debugbreak()\

