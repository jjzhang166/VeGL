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

uniform int fogRanging; //0 plane based, 1 range based
uniform int fogInterpolation; //0 linear (doesn't use density, uses fogEnd &fogStart), 1 exp, 2 exp sq

uniform vec3 fogColor;
uniform float fogDensity;

uniform float fogEnd;
uniform float fogStart;

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

    float dist = 0;
    float fogFactor = 0;
    vec3 finalColor = vec3(0,0,0);

    if(fogRanging == 0)
    {
    	dist = abs(fs_in.FragPos.z);
    }
    else
    {
    	dist = length(fs_in.FragPos);
    }

    if(fogInterpolation == 0)
    {
    	//20 start, 80 fog ends
    	fogFactor = (fogEnd- dist)/(fogEnd-fogStart);
    	fogFactor = clamp(fogFactor, 0.0, 1.0);
    	finalColor = mix(fogColor, result, fogFactor);
    }
    else if(fogInterpolation == 1)
    {
    	fogFactor = 1.0 / exp(dist * (fogDensity/20));
    	fogFactor = clamp(fogFactor, 0.0, 1.0);
    	finalColor = mix(fogColor, result, fogFactor);
    }
    else if(fogInterpolation == 2)
    {
    	fogFactor = 1.0 / exp((dist * (fogDensity/20)) * (dist * (fogDensity/20)));
    	fogFactor = clamp(fogFactor, 0.0, 1.0);
    	finalColor = mix(fogColor, result, fogFactor);
    }

	color = vec4(finalColor, 1.0);
};