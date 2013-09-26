#include "resource.fx"

struct VS_Input
{
	float2	pos			: POSITION;
	float2	size		: SIZE;
	float	rotation	: ROTATION;
	float4	tintAlpha	: TINTALPHA;
	int		texIndex	: TEXINDEX;
};

VS_Input vs(VS_Input input)
{
	return input;
}