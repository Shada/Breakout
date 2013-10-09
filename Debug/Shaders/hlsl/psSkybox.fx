#include "resource.fx" 
Texture2D tex01			: register( t0 );

struct PS_Input
{
	float4 pos	: SV_POSITION;
	float2 tex	: TEXCOORD0;
};

float4 ps_skybox( PS_Input input ) : SV_TARGET0
{
	return tex01.Sample(samSkybox, input.tex);
}