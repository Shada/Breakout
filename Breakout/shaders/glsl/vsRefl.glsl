#version 430 core

layout(location = 0) in vec3 vpos;
layout(location = 1) in vec3 vnorm;
layout(location = 2) in vec2 vtex;

out vec2 gtex;
out vec3 gnorm;
out vec4 gposW;

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

layout(std140, binding = 2) uniform WorldBuffer
{
	mat4 world;
	mat4 worldInv;
};

void main()
{

	gnorm = vec3(0,0,0);
	gtex = vec2(0,0);
	gposW = world * vec4(vpos, 1.0f);
	gl_Position = projection * view * gposW;
	gnorm = normalize((world * vec4(vnorm, 0)).xyz);
	gtex = vtex;
}