#include "resource.fx"
struct VS_Input
{
	float3 pos	: POSITION;
	float3 norm	: NORMAL;
	float2 tex	: TEXCOORD0;
};
struct PS_Input
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD0;
	float3 norm		: NORMAL;
	float4 posW		: TEXCOORD1;
};
PS_Input vs_refl(VS_Input input)
{
	PS_Input output = (PS_Input)0;
	output.posW = mul(world, float4(input.pos,1.0f));
	output.pos = mul(viewRefl, output.posW);
	output.pos = mul(projection, output.pos);
	output.norm = normalize(mul(world, float4(input.norm,0)).xyz);
	output.tex = input.tex;
	return output;
}