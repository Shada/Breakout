#include "resource.fx"

Texture2D texScene			:register( t0 );
Texture2D texDepth			:register( t1 );
Texture2D reflectionMap		:register( t2 );
Texture2D heightMap			:register( t3 );
Texture2D normalMap			:register( t4 );
Texture2D foamMap			:register( t5 );
Texture2D texLava			:register( t6 );
Texture2D lavaGlowMap		:register( t7 );


float3x3 computeTangentMatrix(float3 normal, float3 position, float2 texCoord)
{
	float3 dp1	= ddx(position);
	float3 dp2	= ddy(position);
	float2 duv1	= ddx(texCoord);
	float2 duv2	= ddy(texCoord);
	
	float3x3 m			= float3x3( dp1, dp2, cross( dp1, dp2 ) );
	float2x3 inverseM	= float2x3( cross( m[1], m[2] ), cross( m[2], m[0] ) );
	float3 tangent		= mul( float2( duv1.x, duv2.x ), inverseM );
	float3 biTangent	= mul( float2( duv1.y, duv2.y ), inverseM );
	
	return transpose(float3x3(normalize(tangent), normalize(biTangent), normal));
}
float fresnelTerm(float3 normal, float3 eyeDir)
{
		float angle		= 1.0f - saturate( dot( normal, eyeDir ) );
		float fresnel	= angle * angle;
		fresnel			= fresnel * fresnel;
		fresnel			= fresnel * angle;
		return saturate( fresnel * ( 1.0f - saturate( .5f ) ) + .5f - refractionStrength );
}

struct PS_Input
{
	float4 pos	: SV_POSITION;
	float2 tex	: TEXCOORD0;
};
float4 ps_water(PS_Input input) : SV_TARGET0
{
	float4 originalColor	= texScene.Sample( samLinear, input.tex );
	if(cameraPos.y < waterLevel) //save work if submerged
		return texScene.Sample( samSkybox, float2( input.tex.x + 2*sin(timer * 0.002f + 3.0f * abs(input.tex.y)) * (refractionScale * min(input.tex.y, 1.0f)), input.tex.y ) ) + float4(0,0,0.3f,1);
	
	float depth				= texDepth.Sample( samLinear, input.tex ).r;
	float4 clipPos = float4(input.tex.x * 2.0 - 1.0,
						(1-input.tex.y) * 2.0 - 1.0,
						depth, 1);
	float4 viewPos = mul(projInv, clipPos);
	float4 worldPos = mul(viewInv, float4( (viewPos / viewPos.w).xyz,1) );
	
	if(worldPos.y > waterLevel)// + maxAmplitude)//if the pixel is above the water surface, then discard.
	{
		if(waterType ==1)
		{
			if(worldPos.y < waterLevel + 10 && depth < 1)
			{
				float oldRange = 10.0f - 0.0f;
				float newRange = 1.0f - 0.0f;
				float glowFactor =waterLevel +10 - worldPos.y;
				glowFactor = (((glowFactor-0) * newRange)/ oldRange) + 0.0f;
				return saturate(originalColor + float4(1,0.3,0.15,1) * glowFactor );
			}
		}
		return originalColor;
	}
	float level = waterLevel;
	//find the position of the point where the pixel meets the surface of the water.

	float3 eyeDir = normalize(worldPos.xyz - cameraPos);
	float distanceFromSurface = level - worldPos.y;
	float distanceFromCamera = cameraPos.y - worldPos.y;

	float t = (level - cameraPos.y) / eyeDir.y;
	float3 surfacePos = cameraPos + eyeDir * t;

	float2 texCoord;
	int temp;
	//calculate the position in regards to waves.
	for(int i = 0; i < 10; i++)
	{
		texCoord = (surfacePos.xz + eyeDir.xz * 0.1f) * scale + timer * 0.0000005f * windDirection;

		float bias = heightMap.Sample( samLinear, texCoord ).r;

		bias *= 0.1f;
		level += bias * maxAmplitude;
		t = (level - cameraPos.y) / eyeDir.y;
		surfacePos = cameraPos + eyeDir * t;
	}


	depth = length(worldPos.xyz - surfacePos);
	float depth2 = surfacePos.y - worldPos.y;
	eyeDir = normalize(cameraPos - surfacePos);
	//calculate normal

	float normal1 = heightMap.Sample( samLinear, (texCoord + float2(-1,0)) / 256).r;
	float normal2 = heightMap.Sample( samLinear, (texCoord + float2(1,0)) / 256).r;
	float normal3 = heightMap.Sample( samLinear, (texCoord + float2(0,-1)) / 256).r;
	float normal4 = heightMap.Sample( samLinear, (texCoord + float2(0,1)) / 256).r;

	float3 surfaceNormal = normalize( float3( (normal1 - normal2) * maxAmplitude,
												normalScaling,
												(normal3 - normal4) * maxAmplitude) );
	//calculate tangent
	texCoord			= (surfacePos.xz * 0.016 + windDirection * timer * 0.00016);
	float3x3 mTangent	= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	float3 normal0a		= normalize( mul(mTangent, 2.0f * normalMap.Sample( samLinear, texCoord ).xyz - 1.0f ) );

	texCoord			= (surfacePos.xz * 0.008 + windDirection * timer * 0.00008);
	 mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	float3 normal1a		= normalize( mul(mTangent, 2.0f * normalMap.Sample( samLinear, texCoord ).xyz - 1.0f ) );

	texCoord			= (surfacePos.xz * 0.004 + windDirection * timer * 0.00004);
	 mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	float3 normal2a		= normalize( mul(mTangent, 2.0f * normalMap.Sample( samLinear, texCoord ).xyz - 1.0f ) );

	texCoord			= (surfacePos.xz * 0.001 + windDirection * timer * 0.00002);
	 mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	float3 normal3a		= normalize( mul(mTangent, 2.0f * normalMap.Sample( samLinear, texCoord ).xyz - 1.0f ) );

	float3 normal		= normalize(normal0a * normalModifier.x + normal1a * normalModifier.y + normal2a * normalModifier.z + normal3a * normalModifier.w);

	//calculate refraction
	texCoord	= input.tex;
	texCoord.x	+= sin(timer * 0.002f + 1.0f * abs(worldPos.y)) * (refractionScale * min(depth2, 1.0f));

	float3 refraction	= texScene.Sample( samLinear, texCoord ).rgb;
	float depthRefr		= texDepth.Sample( samLinear, texCoord ).r;

	float4 clipPosRefr	= float4(texCoord.x * 2.0 - 1.0,
						(1-texCoord.y) * 2.0 - 1.0,
						depthRefr, 1);
	float4 viewPosRefr	= mul(projInv, clipPosRefr);
	float4 worldPosRefr	= mul(viewInv, float4( (viewPosRefr / clipPosRefr.w).xyz,1) );

	if( worldPosRefr.y < level)
		refraction = originalColor.xyz;

	//float3 reflection		= reflectionMap.Sample(samLinear, texCoordProj);
	float3 reflection		= reflectionMap.Sample(samLinear, float2(1-texCoord.x, texCoord.y)).xyz;

	float fresnel = fresnelTerm(normal, eyeDir);
	float depthN = depth * waterFade;


	float3 waterColor = saturate(length(float3(1.0f,1.0f,1.0f)) / 3.0f );
	refraction = lerp(lerp(refraction, waterSurfaceColor.xyz * waterColor, 1-saturate(depthN / transparency)),
						waterDepthColor.xyz * waterColor, saturate(depth2 / extinction.xyz));
	float4 foam = float4( 0.0f,0.0f,0.0f,0.0f);

	texCoord			= (surfacePos.xz + eyeDir.xz * 0.1) * 0.005 + timer * 0.00001f * windDirection + sin(timer * 0.001 + worldPos.x) * 0.000005;
	float2 texCoord2	= (surfacePos.xz + eyeDir.xz * 0.1) * 0.005 + timer * 0.00002f * windDirection + sin(timer * 0.001 + worldPos.z) * 0.000005;

	if(depth2 < foamOptions.x)
		foam = ( foamMap.Sample(samLinear, texCoord) + foamMap.Sample(samLinear, texCoord2) ) * 0.5f;
	else if(depth2 < foamOptions.y)
	{
		foam = lerp((foamMap.Sample(samLinear,texCoord) + foamMap.Sample(samLinear,texCoord2)) * 0.5f, 0.0f, (depth2 - foamOptions.x) / (foamOptions.y - foamOptions.x));
	}
	if(maxAmplitude - foamOptions.z > 0.0001f)
	{
		foam += (foamMap.Sample(samLinear, texCoord) + foamMap.Sample(samLinear, texCoord2)) * 0.05f * saturate((level - (waterLevel + foamOptions.z)) / (maxAmplitude - foamOptions.z));
	}

	float3 tnorm = mul(view, float4(normal,0) ).xyz;
	float3 lightDist = mul(view, float4(-10,2000,4800,1)).xyz - float4(( viewPosRefr.xyz/viewPosRefr.w),1).xyz;
	float3 s = normalize( lightDist );
	float3 cameraPosWV	= mul( view, float4(cameraPos,1.0) ).xyz;
	float3 specular = pow(max(dot(normalize(-s + 2 *dot(s, tnorm ) * tnorm ), normalize( cameraPosWV - float4(( viewPosRefr.xyz/viewPosRefr.w),1).xyz )), 0.0),40);
	if(waterType == 0)
		return saturate(lerp(originalColor, lerp(float4(refraction,1.0f), float4(reflection,1.0f),  0.7*fresnel) + float4(specular,1) + foam ,saturate(depth2 * shoreTransition)));

	
	float4 lavaColor = texLava.Sample( samLinear, texCoord);
	float glowFactor = lavaGlowMap.Sample(samLinear, texCoord).r;

	float oldRange = 1.0f - (-1.0f);
	float newRange = 1.0f - 0.0f;
	float pulse =sin(timer*0.0004);
	pulse = (((pulse-(-1)) * newRange)/ oldRange) + 0.5f;
	float4 outColor = saturate(lerp(lerp(originalColor*glowFactor, lerp(float4(refraction,1.0f), float4(reflection,1.0f)*glowFactor,  0.7*fresnel) + float4(specular,1)*0 + foam ,saturate(depth2 * shoreTransition)),lavaColor,0.6));
	return saturate(outColor + float4(1,0.49,0.15,1)*pulse*glowFactor);
}