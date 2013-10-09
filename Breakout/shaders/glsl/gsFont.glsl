#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in float	gpos[];
in vec4		gtex[];

out vec2	tex;

void main()
{
	//also a textpos as uniform
	vec2 pos = vec2(gpos[0], 0);
	vec4 texCoords = vec4(gtex[0].x / 1420, gtex[0].y / 250, (gtex[0].x + gtex[0].z) / 1420, (gtex[0].y + gtex[0].w) / 250);

	gl_Position = vec4( (pos.x / 1024) * 2 - 1, ((768 - pos.y - gtex[0].w)/768) * 2 - 1,	 0, 1 );
	tex = vec2( texCoords.x, texCoords.w );
	EmitVertex();

	gl_Position = vec4( (pos.x / 1024) * 2 - 1, ((768 - pos.y)/768) * 2 - 1,	 0, 1 );
	tex = vec2( texCoords.x, texCoords.y );
	EmitVertex();

	gl_Position = vec4( ((pos.x + gtex[0].z) / 1024) * 2 - 1, ((768 - pos.y - gtex[0].w)/768) * 2 - 1,	 0, 1 );
	tex = vec2( texCoords.z, texCoords.w );
	EmitVertex();

	gl_Position = vec4( ((pos.x + gtex[0].z) / 1024) * 2 - 1, ((768 - pos.y)/768) * 2 - 1,	 0, 1 );
	tex = vec2( texCoords.z, texCoords.y );
	EmitVertex();

	EndPrimitive();
}