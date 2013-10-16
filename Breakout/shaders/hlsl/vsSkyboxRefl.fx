#include "resource.fx" 
struct VS_Input
{
	float3 pos	: POSITION;
	float3 norm	: NORMAL;
	float2 tex	: TEXCOORD0;
};

struct PS_Input
{
	float4 pos	: SV_POSITION;
	float2 tex	: TEXCOORD0;
};

PS_Input vs_skybox( VS_Input input )
{
	PS_Input output = (PS_Input)0;
	output.tex = input.tex;
	output.pos = float4(mul((float3x3)viewRefl, input.pos ),1);
	output.pos = mul(projection, output.pos );

	return output;
}