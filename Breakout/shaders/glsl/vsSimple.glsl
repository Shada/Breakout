#version 430 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 position_modelspace;
out vec3 normal_viewspace;
out vec3 eyedirection_viewspace;
out vec3 lightdir_viewspace;

uniform mat4 model;
uniform mat4 modelInvTrans;
uniform mat4 projection;
uniform mat4 view;

///uniform vec3 ka;
///uniform vec3 kd;
///uniform vec3 ks;
//uniform vec3 lightpos;
//uniform vec3 camerapos;

void main()
{
	mat4 negZscale;
	negZscale = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1);
	mat4 modelView = view * model * negZscale;
	
    // Output position of the vertex in clip space
    gl_Position = projection * modelView * vec4(vertexPos, 1);

    // position of vertex in world space
    position_modelspace = (model * vec4(vertexPos, 1)).xyz;

    // vector between vertex and camera in view space
    vec3 vertexPos_viewspace = (modelView * vec4(vertexPos, 1)).xyz;
    eyedirection_viewspace = vec3(0,0,0) - vertexPos_viewspace; /// TODO: use camerapos instead

    // vector that goes from vertex to light, in view space
    vec3 lightpos_viewspace = (view * vec4(100,200,-100, 1)).xyz; /// TODO: use lightpos instead
    lightdir_viewspace = lightpos_viewspace + eyedirection_viewspace;

    //normal of vertex in view space
    normal_viewspace = (view * model * negZscale * vec4(vertexNorm, 0)).xyz; // Maybe need to use inverse transpose of model matrix. depends on scaling

    UV = vertexUV;
}
