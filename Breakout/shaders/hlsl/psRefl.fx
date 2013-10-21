#include "resource.fx"
Texture2D tex01	: register( t0 );
struct PS_Input
{
	float4 pos		: SV_POSITION;
	float2 tex		: TEXCOORD0;
	float3 norm		: NORMAL;
	float4 posW		: TEXCOORD1;
};
float4 ps_refl(PS_Input input) : SV_TARGET0
{
	if(input.posW.y < waterLevel ) //no waterlevel sent in yet
		discard;

	float3 kd = float3(0.9280,0.9280,0.9280);
	float3 ka = float3(0.08,	0.08,	0.08);
	float3 ks = float3(0.2,	0.2,	0.2);

	// light calculations done in world space
	float3 s = normalize(lightPos.xyz - input.posW.xyz);
	float4 texColor  = tex01.Sample(samLinear, input.tex);

	float4 outColor = float4(ka,0) + float4(kd,0) * max(dot(s,input.norm),0.0) +
					float4(ks,0) * pow(max(dot(normalize(-s + 2 *dot(s, input.norm) * input.norm),
									normalize( cameraPos.xyz - input.posW.xyz)), 0.0),5);
	return saturate(texColor *outColor);
}