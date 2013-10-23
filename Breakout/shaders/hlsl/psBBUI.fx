#include "resource.fx"
Texture2D texArray[5]		: register( t0 );

struct PS_Input
{
	float4	pos			: SV_POSITION;
	float4	tintAlpha	: TINTALPHA;
	float2	texCoords	: TEXCOORD;
	int		texIndex	: TEXINDEX;
};

float4 ps(PS_Input input) :SV_TARGET0
{
	//stupid hlsl wont let me access the arrays directly with the input
	float4 texColor[5];
	texColor[0] = texArray[0].Sample(samLinear, input.texCoords );
	texColor[1] = texArray[1].Sample(samLinear, input.texCoords );
	texColor[2] = texArray[2].Sample(samLinear, input.texCoords );
	texColor[3] = texArray[3].Sample(samLinear, input.texCoords );
	texColor[4] = texArray[4].Sample(samLinear, input.texCoords );

	return texColor[input.texIndex] * input.tintAlpha;// * input.tintAlpha;
}