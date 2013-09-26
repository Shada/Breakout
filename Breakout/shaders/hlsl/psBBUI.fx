#include "resource.fx"
//Texture2DArray tex		: register( t0 );

struct PS_Input
{
	float4	pos			: SV_POSITION;
	float4	tintAlpha	: TINTALPHA;
	float2	texCoords	: TEXCOORD;
	int		texIndex	: TEXINDEX;
};

float4 ps(PS_Input input) :SV_TARGET0
{
	float4 texColor = float4(1,1,1,1);//tex[input.texIndex].Sample(samLinear, input.texCoord);
	return texColor * input.tintAlpha;
}