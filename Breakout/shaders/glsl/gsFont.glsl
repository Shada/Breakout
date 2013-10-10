#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in float	gpos[];
in vec4		gtex[];

out vec2	UV;

void main()
{
	vec2 pos =  vec2( gpos[0], 0 );
	vec4 tex = vec4(gtex[0].x / 1420.0, gtex[0].y / 250.0, (gtex[0].x + gtex[0].z) / 1420.0, (gtex[0].y + gtex[0].w) / 250.0);

	//also a textpos as uniform
	gl_Position	= vec4( (pos.x / 1024) * 2 - 1, ((768 - pos.y - gtex[0].w) / 768) * 2 - 1, 0, 1 );
	UV	= vec2( tex.x, 1-tex.w );
	EmitVertex();

	gl_Position = vec4( (pos.x/1024) * 2 - 1, ((768 - pos.y) / 768) * 2 - 1, 0, 1 );
	UV = vec2( tex.x, 1-tex.y );
	EmitVertex();

	gl_Position = vec4( ((pos.x + gtex[0].z) / 1024) * 2 - 1, ((768 - pos.y - gtex[0].w) / 768) * 2 - 1, 0, 1 );
	UV = vec2( tex.z, 1-tex.w );
	EmitVertex();

	gl_Position = vec4( ((pos.x + gtex[0].z) / 1024) * 2 - 1, ((768 - pos.y) / 768) * 2 - 1, 0, 1 );
	UV = vec2( tex.z, 1- tex.y );
	EmitVertex();

	EndPrimitive();
}