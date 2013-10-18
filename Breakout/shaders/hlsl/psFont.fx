#include "resource.fx"
Texture2D tex01	: register( t0 );
struct PS_Input
{
	float4	pos			: SV_POSITION;
	float2	tex			: TEXCOORD;
};

float4 ps(PS_Input input) :SV_TARGET0
{
	float4 texColor =  tex01.Sample(samLinear, input.tex);//tex[input.texIndex].Sample(samLinear, input.texCoord);
	return texColor * textTintAlpha;
}