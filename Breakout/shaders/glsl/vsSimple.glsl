#version 430 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexTexCoord;

//send in all or pre-calculate on CPU?
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    gl_Position.xyz = vertexPos;
    gl_Position.w = 1.0;
}
