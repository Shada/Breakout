#version 430 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;

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
	mat4 viewRefl;
	vec3 cameraPos;
	vec3 cameraDir;
};

void main()
{
	mat4 vp = projection * view;
	vec4 pos = vp * vec4(vertexPos, 0);
	gl_Position = pos.xyww;

	UV = vec2(vertexUV.x, 1 - vertexUV.y);
}
