#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 UV;

void main()
{
	gl_Position = vec4( -1, -1, 0, 1 );
	UV = vec2( 0, 1 );
	EmitVertex();

	gl_Position = vec4( -1, 1, 0, 1 );
	UV = vec2( 0, 0 );
	EmitVertex();

	gl_Position = vec4( 1, -1, 0, 1);
	UV = vec2( 1, 1 );
	EmitVertex();

	gl_Position = vec4( 1, 1, 0, 1 );
	UV = vec2( 1, 0 );
	EmitVertex();

	EndPrimitive();
}