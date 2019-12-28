#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D heightmap;

out VS_OUT {
    vec2 TexCoords;
    vec4 FragPos;
} vs_out;


void main ()
{
	float height = texture(heightmap, position.xz).x;

	vec3 posBuff = position;
	posBuff.y = height;

    vs_out.TexCoords = uv;
    vs_out.FragPos =  projection * view * model * vec4(posBuff,1.0);

    gl_Position = vs_out.FragPos;
};