#include "resource.fx"
struct VS_Input
{
	float3 pos	: POSITION;
	float3 norm	: NORMAL;
	float2 tex	: TEXCOORD0;
};
float4 vs_quad(VS_Input input) : SV_POSITION
{
	return float4(0.2f,0.2f,0.2f,0.2f);
}