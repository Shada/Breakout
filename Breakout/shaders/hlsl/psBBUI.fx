#include "resource.fx"
Texture2DArray tex		: register( t0 );

struct PS_Input
{
	float4	pos			: SV_POSITION;
	float4	tintAlpha	: TINTALPHA;
	float2	texCoords	: TEXCOORD0;
	int		texIndex	: TEXINDEX;
};

float4 ps_simple(PS_Input input) :SV_TARGET0
{
	float4 texColor  = tex[input.texIndex].Sample(samLinear, input.texCoord);
	return texColor * input.tintAlpha;
}