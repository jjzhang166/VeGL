#version 330 core
layout (location = 0) out vec4 o_color;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
	vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;


uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int fogRanging; //0 plane based, 1 range based
uniform int fogInterpolation; //0 linear (doesn't use density, uses fogEnd &fogStart), 1 exp, 2 exp sq

uniform vec3 fogColor;
uniform float fogDensity;

uniform float fogEnd;
uniform float fogStart;

void main ()
{
	vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    // get diffuse color
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    vec3 result = ambient + diffuse + specular;

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

	o_color = vec4(finalColor, 1.0);
};