out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform struct Material
{
    sampler2D albedoMap;
    vec3 albedoCoeff;

    sampler2D normalMap;
    vec3 normalCoeff;

    sampler2D metallicMap;
    vec3 metallicCoeff;

    sampler2D roughnessMap;
    vec3 roughnessCoeff;

    sampler2D aoMap;
    vec3 aoCoeff;
} material;

uniform struct FogSettings
{
	int fogRanging; //0 plane based, 1 range based
	int fogInterpolation; //0 linear (doesn't use density, uses fogEnd &fogStart), 1 exp, 2 exp sq

	vec3 fogColor;
	float fogDensity;

	float fogEnd;
	float fogStart;
} fog_settings;

uniform vec3 camPos;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); 
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 getNormalFromMap()
{
	if(textureSize(material.roughnessMap, 0) == ivec2(1,1))
		return vec3(0,0,1);

    vec3 tangentNormal = texture(material.normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{		
 	vec3 albedo = material.albedoCoeff;
 	if(textureSize(material.albedoMap,0) != ivec2(1,1))
 		albedo *= pow(texture(material.albedoMap, TexCoords).rgb, vec3(2.2));

    float metallic  = material.metallicCoeff.r;
 	if(textureSize(material.metallicMap,0) != ivec2(1,1))
    	metallic *= texture(material.metallicMap, TexCoords).r;

    float roughness = material.roughnessCoeff.r;
 	if(textureSize(material.roughnessMap,0) != ivec2(1,1))
    	roughness *= texture(material.roughnessMap, TexCoords).r;

    float ao = material.aoCoeff.r;
 	if(textureSize(material.aoMap,0) != ivec2(1,1))
        ao *= texture(material.aoMap, TexCoords).r;

    vec3 N = normalize(getNormalFromMap()) * material.normalCoeff;
    vec3 V = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04); //dialectric
    F0 = mix(F0, albedo, metallic); //metallic workflow

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < max_lights; i++) 
    {
		vec3 light_pos = lighting_data.lightPositions[i];
		vec3 light_col = clamp(lighting_data.lightColors[i].rgb,0.,1.);
		float light_int = lighting_data.lightIntensities[i];

    	vec3 lMinusw = light_pos - WorldPos;
        vec3 L = normalize(lMinusw);
        vec3 H = normalize(V + L);
        float distance = length(lMinusw);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light_col * attenuation * light_int;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); 
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	  

        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  
    }   
    

    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping and gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 

    // Fog
    float dist = 0;
    float fogFactor = 0;
    vec3 finalColor = vec3(0,0,0);
    vec3 result = color;

    if(fog_settings.fogRanging == 0)
    {
    	dist = abs(WorldPos.z);
    }
    else
    {
    	dist = length(WorldPos);
    }

    if(fog_settings.fogInterpolation == 0)
    {
    	fogFactor = (fog_settings.fogEnd - dist)/(fog_settings.fogEnd-fog_settings.fogStart);
    	fogFactor = clamp(fogFactor, 0.0, 1.0);
    	finalColor = mix(fog_settings.fogColor, result, fogFactor);
    }
    else if(fog_settings.fogInterpolation == 1)
    {
    	fogFactor = 1.0 / exp(dist * (fog_settings.fogDensity/20.0));
    	fogFactor = clamp(fogFactor, 0.0, 1.0);
    	finalColor = mix(fog_settings.fogColor, result, fogFactor);
    }
    else if(fog_settings.fogInterpolation == 2)
    {
    	fogFactor = 1.0 / exp((dist * (fog_settings.fogDensity/20.0)) * (dist * (fog_settings.fogDensity/20.0)));
    	fogFactor = clamp(fogFactor, 0.0, 1.0);
    	finalColor = mix(fog_settings.fogColor, result, fogFactor);
    }
    FragColor = vec4(finalColor, 1.0);
}