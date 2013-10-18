#define PI 3.14159265f

SamplerState samLinear	: register( s0 );
SamplerState samSkybox	: register( s1 );

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
};
cbuffer cbOnce			: register( b2 )
{
	matrix projection;
	matrix projInv;
	float4 lightPos;
	float2 resolution;
	float2 padding2;
};
cbuffer cbFont			: register( b3 )
{
	float4	textTintAlpha;
	float2	textPos;
	float2	textSize;
	float2	imageSize;
	float	textRotation;
	float padda;
};

cbuffer cbWater			: register( b4 )
{
	float waterLevel; //height of sea level
	float timer;
	float2 windDirection;
};

cbuffer cbWaterOnce		: register( b5 )
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
	float2 scale;

	float4 normalModifier; //multiplier for the different normals. first one is for small waves.

	float4 foamOptions; //depth of which foam starts to fade out, depth of which foam is invisible, height of which foam appears for waves.

	float3 waterSurfaceColor;
	int	   waterType;

	float4 waterDepthColor;

	float4 extinction;
};
