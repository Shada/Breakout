#version 430 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 position_modelspace;
out vec3 normal_viewspace;
out vec3 eyedirection_viewspace;
out vec3 lightdir_viewspace;

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
	mat4 modelView = view * world;
	mat4 mvp = projection * modelView;

    // Output position of the vertex in clip space
    gl_Position = mvp * vec4(vertexPos, 1);

    // position of vertex in world space
    position_modelspace = (world * vec4(vertexPos, 1)).xyz;

    // vector between vertex and camera in view space
    vec3 vertexPos_viewspace = (modelView * vec4(vertexPos, 1)).xyz;
    eyedirection_viewspace = cameraPos - vertexPos_viewspace; /// TODO: use camerapos instead

    // vector that goes from vertex to light, in view space
    vec3 lightpos_viewspace = (view * vec4(100,100,-100,1)).xyz; /// TODO: use lightpos instead
    lightdir_viewspace = lightpos_viewspace + eyedirection_viewspace;

    //normal of vertex in view space
    normal_viewspace = (view * world * vec4(vertexNorm, 0)).xyz; // Maybe need to use inverse transpose of model matrix. depends on scaling

    UV = vec2(vertexUV.x, 1 - vertexUV.y);
}
