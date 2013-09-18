#include "resource.fx"

struct VS_Input
{
	float2	pos			: POSITION;
	float2	size		: SIZE;
	float	rotation	: ROTATION;
	float4	tintAlpha	: TINTALPHA;
	float2	texCoords	: TEXCOORD0;
	int		texIndex	: TEXINDEX;
};

VS_Input vs_simple(VS_Input input)
{
	return input;
}