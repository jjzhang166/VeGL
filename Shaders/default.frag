#version 330 core
layout (location = 0) out vec4 color;

uniform sampler2D ground_diff;
uniform sampler2D ground_norm;

in VS_OUT {
    vec2 TexCoords;
	vec4 FragPos;
} fs_in;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 camPos;

void main ()
{
	vec2 uv = fs_in.TexCoords;

	vec3 ambient = vec3(1);
    vec3 normal = texture(ground_norm,  uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);   

    vec3 lightDir = normalize(lightPos - fs_in.FragPos.xyz);  
    float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

    vec3 objectColor = texture(ground_diff, uv).rgb;
    vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0);
};