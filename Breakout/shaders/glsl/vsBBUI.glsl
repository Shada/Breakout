#version 430 core
layout (location = 0) in vec2	vpos;
layout (location = 1) in vec2	vsize;
layout (location = 2) in float	rotation;
layout (location = 3) in vec4	tintAlpha;
layout (location = 4) in int	texIndex;

out vec2	gpos;
out vec2	gsize;
out float	grotation;
out vec4	gtintAlpha;
out int		gtexIndex;

void main()
{
	gpos		= vpos;
	gsize		= vsize;
	grotation	= rotation;
	gtintAlpha	= tintAlpha;
	gtexIndex	= texIndex;
}
