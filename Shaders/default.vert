#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out VS_OUT {
    vec2 TexCoords;
    vec4 FragPos;
} vs_out;

vec3 rand3(vec3 c) {
	float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
	vec3 r;
	r.z = fract(512.0*j);
	j *= .125;
	r.x = fract(512.0*j);
	j *= .125;
	r.y = fract(512.0*j);
	return r-0.5;
}

const float F3 =  0.3333333;
const float G3 =  0.1666667;
float simplex(vec3 p) 
{

	vec3 s = floor(p + dot(p, vec3(F3)));
	vec3 x = p - s + dot(s, vec3(G3));
	 
	vec3 e = step(vec3(0.0), x - x.yzx);
	vec3 i1 = e*(1.0 - e.zxy);
	vec3 i2 = 1.0 - e.zxy*(1.0 - e);
	 	
	vec3 x1 = x - i1 + G3;
	vec3 x2 = x - i2 + 2.0*G3;
	vec3 x3 = x - 1.0 + 3.0*G3;
	 
	vec4 w, d;
	 
	w.x = dot(x, x);
	w.y = dot(x1, x1);
	w.z = dot(x2, x2);
	w.w = dot(x3, x3);
	 
	w = max(0.6 - w, 0.0);
	 
	d.x = dot(rand3(s), x);
	d.y = dot(rand3(s + i1), x1);
	d.z = dot(rand3(s + i2), x2);
	d.w = dot(rand3(s + 1.0), x3);
	 
	w *= w;
	w *= w;
	d *= w;
	 
	return dot(d, vec4(52.0));
}

float FBN(vec3 m, float freq=1, float amp=0.53, float lacunarity=2.0, float gain=0.5, int octaves = 22) 
{
	float sum = 0;

	while(octaves-- > 0)
	{
		sum += amp * simplex(freq*m);
		freq *= lacunarity;
		amp *= gain;
	}


	return sum;
}

void main ()
{
	vec3 np = position;
	np.y = abs(FBN(vec3(position.x/33, 0, position.z/33)) * 12) + (0.4*FBN(vec3(position.x/16, 0, position.z/16))) + (0.2*FBN(vec3(position.x/8, 0, position.z/8)));

    vs_out.TexCoords = uv;
    vs_out.FragPos =  projection * view * model * vec4(np,1.0);

    gl_Position = vs_out.FragPos;
};