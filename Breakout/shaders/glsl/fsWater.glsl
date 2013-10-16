#version 430 core

uniform sampler2D texScene;
uniform sampler2D texDepth;
uniform sampler2D reflectionMap;
uniform sampler2D heightMap;
uniform sampler2D normalMap;
uniform sampler2D foamMap;


mat3 computeTangentMatrix(vec3 normal, vec3 position, vec2 texCoord)
{
	vec3 dp1	= ddx(position);
	vec3 dp2	= ddy(position);
	vec2 duv1	= ddx(texCoord);
	vec2 duv2	= ddy(texCoord);
	
	mat3 m			= mat3( dp1, dp2, cross( dp1, dp2 ) );
	mat2x3 inverseM	= mat2x3( cross( m[1], m[2] ), cross( m[2], m[0] ) );
	vec3 tangent		= mul( vec2( duv1.x, duv2.x ), inverseM );
	vec3 biTangent	= mul( vec2( duv1.y, duv2.y ), inverseM );
	
	return transpose(mat3(normalize(tangent), normalize(biTangent), normal));
}
float fresnelTerm(vec3 normal, vec3 eyeDir)
{
		float angle		= 1.0f - saturate( dot( normal, eyeDir ) );
		float fresnel	= angle * angle;
		fresnel			= fresnel * fresnel;
		fresnel			= fresnel * angle;
		return saturate( fresnel * ( 1.0f - saturate( .5f ) ) + .5f - refractionStrength );
}

in vec2 UV;
out vec4 color;
void main()
{
	vec4 originalColor	= texture2D(textureSampler, UV);
	if(cameraPos.y < waterLevel) //save work if submerged
	{
		color = texScene.Sample( samSkybox, vec2( UV.x + 2*sin(timer * 0.002f + 3.0f * abs(UV.y)) * (refractionScale * min(UV.y, 1.0f)), UV.y ) ) + vec4(0,0,0.3f,1);
		return;
	}
	float depth				= texDepth.Sample( samLinear, input.tex ).r;
	vec4 clipPos = vec4(input.tex.x * 2.0 - 1.0,
						(1-input.tex.y) * 2.0 - 1.0,
						depth, 1);
	vec4 viewPos = mul(projInv, clipPos);
	vec4 worldPos = mul(viewInv, vec4( (viewPos / viewPos.w).xyz,1) );
	
	if(worldPos.y > waterLevel)// + maxAmplitude)//if the pixel is above the water surface, then discard.
	{
		color = originalColor;
		return;
	}
	float level = waterLevel;
	//find the position of the point where the pixel meets the surface of the water.

	vec3 eyeDir = normalize(worldPos.xyz - cameraPos);
	float distanceFromSurface = level - worldPos.y;
	float distanceFromCamera = cameraPos.y - worldPos.y;

	float t = (level - cameraPos.y) / eyeDir.y;
	vec3 surfacePos = cameraPos + eyeDir * t;

	vec2 texCoord;
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

	vec3 surfaceNormal = normalize( float3( (normal1 - normal2) * maxAmplitude,
												normalScaling,
												(normal3 - normal4) * maxAmplitude) );
	//calculate tangent
	texCoord			= (surfacePos.xz * 0.016 + windDirection * timer * 0.00016);
	mat3 mTangent	= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	vec3 normal0a		= normalize( mul(mTangent, 2.0f * normalMap.Sample( samLinear, texCoord ).xyz - 1.0f ) );

	texCoord			= (surfacePos.xz * 0.008 + windDirection * timer * 0.00008);
	 mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	vec3 normal1a		= normalize( mul(mTangent, 2.0f * normalMap.Sample( samLinear, texCoord ).xyz - 1.0f ) );

	texCoord			= (surfacePos.xz * 0.004 + windDirection * timer * 0.00004);
	 mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	vec3 normal2a		= normalize( mul(mTangent, 2.0f * normalMap.Sample( samLinear, texCoord ).xyz - 1.0f ) );

	texCoord			= (surfacePos.xz * 0.001 + windDirection * timer * 0.00002);
	 mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	vec3 normal3a		= normalize( mul(mTangent, 2.0f * normalMap.Sample( samLinear, texCoord ).xyz - 1.0f ) );

	vec3 normal		= normalize(normal0a * normalModifier.x + normal1a * normalModifier.y + normal2a * normalModifier.z + normal3a * normalModifier.w);

	//calculate refraction
	texCoord	= UV;
	texCoord.x	+= sin(timer * 0.002f + 1.0f * abs(worldPos.y)) * (refractionScale * min(depth2, 1.0f));

	vec3 refraction	= texScene.Sample( samLinear, texCoord ).rgb;
	float depthRefr		= texDepth.Sample( samLinear, texCoord ).r;

	vec4 clipPosRefr	= float4(texCoord.x * 2.0 - 1.0,
						(1-texCoord.y) * 2.0 - 1.0,
						depthRefr, 1);
	vec4 viewPosRefr	= mul(projInv, clipPosRefr);
	vec4 worldPosRefr	= mul(viewInv, vec4( (viewPosRefr / clipPosRefr.w).xyz,1) );

	if( worldPosRefr.y < level)
		refraction = originalColor.xyz;

	/*float4x4 mTextureProj	= mul( mul(view,projection), mReflection);
	vec3 waterPos			= surfacePos.xyz - level + waterLevel;
	vec4 texCoordProj		= mul(mTextureProj, vec4(waterPos,1.0f));

	vec4 disPos				= vec4( texCoordProj.x + displacementStrength * normal.x,
							texCoordProj.y,
							texCoordProj.z + displacementStrength * normal.z,
							texCoordProj.w );
	texCoordProj = disPos;*/

	//float3 reflection		= reflectionMap.Sample(samLinear, texCoordProj);
	vec3 reflection		= reflectionMap.Sample(samLinear, vec2(1-texCoord.x, texCoord.y)).xyz;

	float fresnel = fresnelTerm(normal, eyeDir);
	float depthN = depth * waterFade;


	vec3 waterColor = saturate(length(float3(1.0f,1.0f,1.0f)) / 3.0f );
	refraction = lerp(lerp(refraction, waterSurfaceColor.xyz * waterColor, 1-saturate(depthN / transparency)),
						waterDepthColor.xyz * waterColor, saturate(depth2 / extinction.xyz));
	vec4 foam = vec4( 0.0f,0.0f,0.0f,0.0f);

	texCoord			= (surfacePos.xz + eyeDir.xz * 0.1) * 0.005 + timer * 0.00001f * windDirection + sin(timer * 0.001 + worldPos.x) * 0.000005;
	vec2 texCoord2	= (surfacePos.xz + eyeDir.xz * 0.1) * 0.005 + timer * 0.00002f * windDirection + sin(timer * 0.001 + worldPos.z) * 0.000005;

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

	vec3 tnorm = mul(view, vec4(normal,0) ).xyz;
	vec3 lightDist = mul(view, vec4(-10,2000,4800,1)).xyz - vec4(( viewPosRefr.xyz/viewPosRefr.w),1).xyz;
	vec3 s = normalize( lightDist );
	vec3 cameraPosWV	= mul( view, vec4(cameraPos,1.0) ).xyz;
	vec3 specular = pow(max(dot(normalize(-s + 2 *dot(s, tnorm ) * tnorm ), normalize( cameraPosWV - vec4(( viewPosRefr.xyz/viewPosRefr.w),1).xyz )), 0.0),40);
	return saturate(lerp(originalColor, lerp(vec4(refraction,1.0f), vec4(reflection,1.0f),  0.7*fresnel) + vec4(specular,1) + foam ,saturate(depth2 * shoreTransition)));

}