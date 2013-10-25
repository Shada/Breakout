#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2		gpos[];
in vec2		gsize[];
in float	grotation[];
in vec4		gtintAlpha[];
in int		gtexIndex[];

out vec2 UV;
out int ftexIndex;
out vec4 ftintAlpha;

layout(std140, binding = 0) uniform CameraOnce
{
	mat4 projection;
	mat4 projInv;
	vec4 lightPos;
	vec2 resolution;
};

void main()
{
    vec2 pos =  vec2( gpos[0].x, gpos[0].y );
	vec2 size = vec2( gsize[0].x, gsize[0].y );

	gl_Position = vec4( pos.x / resolution.x * 2 - 1,
                (resolution.y - pos.y) / resolution.y * 2 - 1, 0, 1 );
	ftintAlpha = gtintAlpha[0];
	UV	= vec2( 0, 1 );
	ftexIndex = gtexIndex[0];
	EmitVertex();

	gl_Position = vec4( pos.x / resolution.x * 2 - 1,
                (resolution.y - pos.y - size.y) / resolution.y * 2 - 1, 0, 1 );
	ftintAlpha = gtintAlpha[0];
	UV = vec2( 0, 0 );
	ftexIndex = gtexIndex[0];
	EmitVertex();

	gl_Position = vec4( (pos.x + size.x) / resolution.x * 2 - 1,
                (resolution.y-pos.y) / resolution.y * 2 - 1, 0, 1 );
	ftintAlpha = gtintAlpha[0];
	UV = vec2( 1, 1 );
	ftexIndex = gtexIndex[0];
	EmitVertex();

	gl_Position = vec4( (pos.x + size.x) / resolution.x * 2 - 1,
                (resolution.y - pos.y - size.y ) / resolution.y * 2 - 1, 0, 1);
	ftintAlpha = gtintAlpha[0];
	UV = vec2( 1, 0 );
	ftexIndex = gtexIndex[0];
	EmitVertex();

	EndPrimitive();
}
