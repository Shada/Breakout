#define PI 3.14159265f

SamplerState samLinear	: register( s0 );

cbuffer cbWorld			: register( b0 )
{
	matrix world;
	matrix worldInv;
};

cbuffer cbCameraMove	: register( b1 )
{
	matrix view;
	matrix viewInv;
	float3 cameraPos;
	float3 cameraDir;
};
cbuffer cbOnce			: register( b2 )
{
	matrix projection;
	matrix projInv;
};