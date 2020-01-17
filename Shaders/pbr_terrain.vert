#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform sampler2D heightmap;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = uv;
    Normal = mat3(model) * normal;   

    float height = texture(heightmap, position.xz).x;

	vec3 posBuff = position;
	posBuff.y = height;
    WorldPos = vec3(model * vec4(posBuff, 1.0));

    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}