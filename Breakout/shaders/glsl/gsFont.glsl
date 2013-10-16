#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in float	gpos[];
in vec4		gtex[];

out vec2	UV;


layout(std140, binding = 4) uniform CameraOnce
{
	mat4 projection;
	mat4 projInv;
	vec4 lightPos;
	vec2 resolution;
};

layout(std140, binding = 3) uniform FontBuffer
{
	vec2	textPos;
	vec2	textSize;
	float	textRotation;
	vec4	textTintAlpha;
	vec2	imageSize;
};

void main()
{
	vec2 pos =  vec2(gpos[0], 0 );
	vec4 tex = vec4(gtex[0].x / imageSize.x, gtex[0].y / imageSize.y, (gtex[0].x + gtex[0].z) / imageSize.x, (gtex[0].y + gtex[0].w) / imageSize.y);

	//also a textpos as uniform
	gl_Position	= vec4( (pos.x / resolution.x) * 2 - 1, ((resolution.y - pos.y - gtex[0].w) / resolution.y) * 2 - 1, 0, 1 );
	UV	= vec2( tex.x, 1-tex.w );
	EmitVertex();

	gl_Position = vec4( (pos.x/resolution.x) * 2 - 1, ((resolution.y - pos.y) / resolution.y) * 2 - 1, 0, 1 );
	UV = vec2( tex.x, 1-tex.y );
	EmitVertex();

	gl_Position = vec4( ((pos.x + gtex[0].z) / resolution.x) * 2 - 1, ((resolution.y - pos.y - gtex[0].w) / resolution.y) * 2 - 1, 0, 1 );
	UV = vec2( tex.z, 1-tex.w );
	EmitVertex();

	gl_Position = vec4( ((pos.x + gtex[0].z) / resolution.x) * 2 - 1, ((resolution.y - pos.y) / resolution.y) * 2 - 1, 0, 1 );
	UV = vec2( tex.z, 1- tex.y );
	EmitVertex();

	EndPrimitive();
}