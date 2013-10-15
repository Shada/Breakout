#include "resource.fx"
Texture2D tex	: register( t0 );

struct PSSceneIn
{
    float4 pos : SV_Position;
    float2 tex : TEXTURE0;
    float4 color : COLOR0;
};


//
// PS for particles
//
float4 PSScenemain(PSSceneIn input) : SV_Target
{   
//    return float4(0,0,0,1);

    return tex.Sample( samLinear, input.tex ) * input.color;
}
