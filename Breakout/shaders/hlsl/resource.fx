#ifndef _RESOURCE_FX_
#define _RESOURCE_FX_

#define PI 3.14159265f

SamplerState samLinear	: register( s0 );

SamplerState samPoint	: register( s1 );

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
	float2 padding;
};
cbuffer cbOnce			: register( b2 )
{
	matrix projection;
	matrix projInv;
	float4 lightPos;
};

cbuffer cbParticles		: register( b3 )
{
	float globalTime;
    float elapsedTime;
    float particlesPerStage;		//How many particles that should be spawned between the stages
	float stages;
    float4 frameGravity;
	float4 color;
	float4 fadeColor;
	float3 startPos;					//The center pos of the particle system might be changed if the particle system is moved
	float radie;						//The radie in which you want the particles to spawn
	float3 startSpeed;					//The direction in which the particle should start to and the speed in which the should do it in
	float randDir;
	float3 radieDirections;				//The direction the radie should make the particles spawn in(for example (1,0,1) making the particles spawn from a flat circle as rain for example)
	float shellLife;
};

#endif