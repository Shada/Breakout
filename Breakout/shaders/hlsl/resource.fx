#define PI 3.14159265f

SamplerState samLinear	: register( s0 );
SamplerState samSkybox	: register( s0 );

cbuffer cbWorld			: register( b0 )
{
	matrix world;
	matrix worldInv;
};

cbuffer cbCameraMove	: register( b1 )
{
	matrix view;
	matrix viewInv;
	matrix viewRefl;
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
cbuffer cbWater			: register( b3 )
{
	float waterLevel; //height of sea level
	float timer;
	float2 windDirection;
};
cbuffer cbWaterOnce		: register( b4 )
{
	float waterFade; //how deep anything need to be to fade out in the water
	float normalScaling; 
	float maxAmplitude; //max wave amplitude
	float shoreTransition; //how soft the water and ground fades
	float refractionStrength; //refraction strength
	float displacementStrength; //multiplier for the height of waves
	float shininess;
	float specularIntensity;
	float transparency;
	float refractionScale;

	float4 normalModifier; //multiplier for the different normals. first one is for small waves.
	float3 foamOptions; //depth of which foam starts to fade out, depth of which foam is invisible, height of which foam appears for waves.
	float3 waterSurfaceColor;
	float3 waterDepthColor;
	float3 extinction;
	float2 scale;
	float3 temp;
};