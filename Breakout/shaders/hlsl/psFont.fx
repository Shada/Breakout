#include "resource.fx"
struct PS_Input
{
	float4	pos			: SV_POSITION;
	float2	tex			: TEXCOORD;
};

float4 ps(PS_Input input) :SV_TARGET0
{
	float4 texColor = float4(1,1,1,1);//tex[input.texIndex].Sample(samLinear, input.texCoord);
	return texColor;
}