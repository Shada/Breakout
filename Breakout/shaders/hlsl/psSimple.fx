#include "resource.fx"
Texture2D tex01	: register( t0 );

struct PS_Input
{
	float4 pos		: SV_POSITION;
	float4 posWV	: TEXCOORD0;
	float3 norm		: NORMAL;
	float2 tex		: TEXCOORD1;
};

float4 ps_simple(PS_Input input) :SV_TARGET0
{
	float4 texColor  = tex01.Sample(samLinear, input.tex);
	
	return texColor;
}