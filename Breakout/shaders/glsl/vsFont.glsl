#version 430 core
layout (location = 0) in float	vpos;
layout (location = 1) in vec4	vtex;

out float	gpos;
out vec4	gtex;

void main()
{
	gpos		= vpos;
	gtex		= gtex;
}