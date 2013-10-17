#version 430 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 pos = projection * view * vec4(vertexPos, 0);
	gl_Position = pos.xyww;

	UV = vec2(vertexUV.x, 1 - vertexUV.y);
}
