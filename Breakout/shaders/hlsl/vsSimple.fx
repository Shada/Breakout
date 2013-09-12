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
	float4 posWV	: TEXCOORD0;
	float3 norm		: NORMAL;
	float2 tex		: TEXCOORD1;
};

PS_Input vs_simple(VS_Input input)
{
	PS_Input output = (PS_Input)0;
	output.pos = mul(world, float4(input.pos,1.0f));
	output.posWV = mul(view, output.pos);
	output.pos = mul(projection, output.posWV);
	matrix worldView = mul(view, world);
	output.norm = normalize(mul(worldView, float4(input.norm,0)).xyz);
	output.tex = input.tex;
	return output;
}