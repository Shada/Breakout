#include "resource.fx"
struct VS_Input
{
	float pos	: POSITION;
	float4 tex	: TEXCOORD0;
};

VS_Input vs(VS_Input input)
{
	return input;
}