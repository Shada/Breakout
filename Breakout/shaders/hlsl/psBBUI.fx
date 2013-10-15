#include "resource.fx"
Texture2DArray texArray		: register( t0 );

struct PS_Input
{
	float4	pos			: SV_POSITION;
	float4	tintAlpha	: TINTALPHA;
	float2	texCoords	: TEXCOORD;
	int		texIndex	: TEXINDEX;
};

float4 ps(PS_Input input) :SV_TARGET0
{
	float4 texColor = texArray.Sample(samLinear, float3(input.texCoords,input.texIndex) );
	return texColor;// * input.tintAlpha;
}