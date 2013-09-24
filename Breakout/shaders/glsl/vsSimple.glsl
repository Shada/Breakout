#version 430 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexNorm;
layout(location = 2) in vec2 vertexTexCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 projectioninverse;
uniform mat4 view;
uniform mat4 viewinverse;

uniform mat4 MVP;

void main()
{
    vec4 v = vec4(vertexPos, 1);
    mat4 mvp = projection * view;
    gl_Position = MVP * v;
}
