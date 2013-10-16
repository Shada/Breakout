#version 430 core

in vec2 UV;
in vec3 norm;
in vec4 posW;

out vec4 color;

uniform sampler2D textureSampler;

layout(std140, binding = 0) uniform CameraOnce
{
	mat4 projection;
	mat4 projInv;
	vec4 lightPos;
	vec2 resolution;
};

layout(std140, binding = 1) uniform CameraMove
{
	mat4 view;
	mat4 viewInv;
	vec3 cameraPos;
	vec3 cameraDir;
};

void main()
{
	if(posW.y < 50 ) //no waterlevel sent in yet
		return;

	vec3 kd = vec3(0.9280,0.9280,0.9280);
	vec3 ka = vec3(0.08,	0.08,	0.08);
	vec3 ks = vec3(0.2,	0.2,	0.2);
	
	// light calculations done in world space
	vec3 s = normalize(lightPos.xyz - posW.xyz);
	vec4 texColor  = texture2D(textureSampler, UV);
	
	vec4 outColor = vec4(ka, 0) + vec4(kd,0) * max(dot(s, norm), 0.0) +
					vec4(ks, 0) * pow(max(dot(normalize(-s + 2 *dot(s, norm) * norm),
									normalize( cameraPos.xyz - posW.xyz)), 0.0),5);
	color = texColor * outColor;
}