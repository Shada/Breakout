#version 430 core

uniform sampler2D texScene;
uniform sampler2D texDepth;
uniform sampler2D reflectionMap;
uniform sampler2D heightMap;
uniform sampler2D normalMap;
uniform sampler2D foamMap;

layout(std140, binding = 5) uniform cbWater
{
	float waterLevel; //height of sea level
	float timer;
	vec2 windDirection;
};

layout(std140, binding = 1) uniform CameraMove
{
	mat4 view;
	mat4 viewInv;
	mat4 viewRefl;
	vec3 cameraPos;
	vec3 cameraDir;
};

layout(std140, binding = 0) uniform CameraOnce
{
	mat4 projection;
	mat4 projInv;
	vec4 lightPos;
	vec2 resolution;
};

layout(std140, binding = 6) uniform cbWaterOnce
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
	vec2 scale;

	vec4 normalModifier; //multiplier for the different normals. first one is for small waves.

	vec4 foamOptions; //depth of which foam starts to fade out, depth of which foam is invisible, height of which foam appears for waves.

	vec4 waterSurfaceColor;

	vec4 waterDepthColor;

	vec4 extinction;
};
float saturate(float infloat)
{
	if(infloat > 1)
	{
		infloat = 1;
	}
	else if(infloat < 0)
	{
		infloat = 0;
	}
	return infloat;
}

vec3 saturate(vec3 invector)
{
	vec3 outvector = invector;
	if(invector.x > 1)
	{
		outvector.x = 1;
	}
	else if(invector.x < 0)
	{
		outvector.x = 0;
	}
	if(invector.y > 1)
	{
		outvector.y = 1;
	}
	else if(invector.y < 0)
	{
		outvector.y = 0;
	}
	if(invector.z > 1)
	{
		outvector.z = 1;
	}
	else if(invector.z < 0)
	{
		outvector.z = 0;
	}
	return outvector;
}

vec4 saturate(vec4 invector)
{
	vec4 outvector = invector;
	if(invector.x > 1)
	{
		outvector.x = 1;
	}
	else if(invector.x < 0)
	{
		outvector.x = 0;
	}
	if(invector.y > 1)
	{
		outvector.y = 1;
	}
	else if(invector.y < 0)
	{
		outvector.y = 0;
	}
	if(invector.z > 1)
	{
		outvector.z = 1;
	}
	else if(invector.z < 0)
	{
		outvector.z = 0;
	}
	if(invector.w > 1)
	{
		outvector.w = 1;
	}
	else if(invector.w < 0)
	{
		outvector.w = 0;
	}
	return outvector;
}
mat3 computeTangentMatrix(vec3 normal, vec3 position, vec2 texCoord)
{
	vec3 dp1	= dFdx(position);
	vec3 dp2	= dFdy(position);
	vec2 duv1	= dFdx(texCoord);
	vec2 duv2	= dFdy(texCoord);
	
	mat3 m			= mat3( dp1, dp2, cross( dp1, dp2 ) );
	mat2x3 inverseM	= mat2x3( cross( m[1], m[2] ), cross( m[2], m[0] ) );
	vec3 tangent	= inverseM * vec2( duv1.x, duv2.x );
	vec3 biTangent	= inverseM * vec2( duv1.y, duv2.y );
	
	return mat3(normalize(tangent), normalize(biTangent), normal);
}
float fresnelTerm(vec3 normal, vec3 eyeDir)
{
		float angle		= 1.0f - saturate(dot( normal, eyeDir ));
		float fresnel	= angle * angle;
		fresnel			= fresnel * fresnel;
		fresnel			= fresnel * angle;
		return saturate(fresnel * ( 1.0f - .5f ) + .5f - refractionStrength);
}


in vec2 UV;
out vec4 color;

void main()
{
	vec4 originalColor	= texture2D(texScene, UV);
	
	if(cameraPos.y < waterLevel) //save work if submerged
	{
		color = texture2D(texScene, vec2( UV.x + 2*sin(timer * 0.002f + 3.0f * abs(1-UV.y)) * (refractionScale * min(1-UV.y, 1.0f)), UV.y ) ) + vec4(0,0,0.3f,1);
		return;
	}
	float depth		= texture2D(texDepth, UV).r;
	
	vec4 clipPos = vec4(UV.x * 2.0 - 1.0,
						(UV.y) * 2.0 - 1.0,
						depth, 1);
	
	vec4 viewPos = projInv * clipPos;
	vec4 worldPos = viewInv * (viewPos / viewPos.w);

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
	
		float bias = texture2D(heightMap, UV).r;
	
		bias *= 0.1f;
		level += bias * maxAmplitude;
		t = (level - cameraPos.y) / eyeDir.y;
		surfacePos = cameraPos + eyeDir * t;
	}
	
	
	depth = length(worldPos.xyz - surfacePos);
	float depth2 = surfacePos.y - worldPos.y;
	eyeDir = normalize(cameraPos - surfacePos);
	//calculate normal
	float normal1 = texture2D(heightMap, (texCoord + vec2(-1,0)) / 256).r;
	float normal2 = texture2D(heightMap, (texCoord + vec2(1,0)) / 256).r;
	float normal3 = texture2D(heightMap, (texCoord + vec2(0,-1)) / 256).r;
	float normal4 = texture2D(heightMap, (texCoord + vec2(0,1)) / 256).r;
	
	
	
	vec3 surfaceNormal = normalize( vec3( (normal1 - normal2) * maxAmplitude,
												normalScaling,
												(normal3 - normal4) * maxAmplitude) );
	
							
	//calculate tangent
	texCoord			= (surfacePos.xz * 0.016 + windDirection * timer * 0.00016);
	mat3 mTangent		= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	vec3 normal0a		= normalize( mTangent * (2.0f * texture2D(normalMap, texCoord ).xyz - 1.0f ) );
	
	texCoord			= (surfacePos.xz * 0.008 + windDirection * timer * 0.00008);
	mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	vec3 normal1a		= normalize( mTangent * (2.0f * texture2D(normalMap, texCoord ).xyz - 1.0f ) );
	
	texCoord			= (surfacePos.xz * 0.004 + windDirection * timer * 0.00004);
	mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	vec3 normal2a		= normalize( mTangent * (2.0f * texture2D(normalMap, texCoord ).xyz - 1.0f ) );
	
	texCoord			= (surfacePos.xz * 0.001 + windDirection * timer * 0.00002);
	mTangent			= computeTangentMatrix(surfaceNormal, eyeDir, texCoord);
	vec3 normal3a		= normalize( mTangent * (2.0f * texture2D(normalMap, texCoord ).xyz - 1.0f ) );
	
	vec3 normal			= normalize(normal0a * normalModifier.x + normal1a * normalModifier.y + normal2a * normalModifier.z + normal3a * normalModifier.w);
			
	
	//calculate refraction
	texCoord	= UV;
	texCoord.x	+= sin(timer * 0.002f + 1.0f * abs(worldPos.y)) * (refractionScale * min(depth2, 1.0f));
	
	vec3 refraction		= texture2D(texScene, texCoord ).rgb;
	float depthRefr		= texture2D(texDepth, texCoord ).r;
	
	vec4 clipPosRefr	= vec4(texCoord.x * 2.0 - 1.0,
						(texCoord.y) * 2.0 - 1.0,
						depthRefr, 1);
									
	vec4 viewPosRefr	= projInv * clipPosRefr;
	vec4 worldPosRefr	= viewInv * vec4( (viewPosRefr / clipPosRefr.w).xyz, 1);
								
	
	if( worldPosRefr.y < level)
		refraction = originalColor.xyz;
	
	
	//vec3 reflection		= texture2D(reflectionMap, texCoordProj);
	vec3 reflection		= texture2D(reflectionMap, vec2(1-texCoord.x, texCoord.y)).rgb;
	
	float fresnel = fresnelTerm(normal, eyeDir);
	float depthN = depth * waterFade;
	
	float llll = length(vec3(1.0f,1.0f,1.0f));
	vec3 waterColor = saturate(vec3(llll,llll,llll) / 3.0f);
	refraction = mix(mix(refraction, waterSurfaceColor.xyz * waterColor, 1-saturate(depthN / transparency)),
						waterDepthColor.xyz * waterColor, saturate(depth2 / extinction.xyz));
	
	vec4 foam = vec4( 0.0f,0.0f,0.0f,0.0f);
	
	texCoord			= (surfacePos.xz + eyeDir.xz * 0.1) * 0.005 + timer * 0.00001f * windDirection + sin(timer * 0.001 + worldPos.x) * 0.000005;
	vec2 texCoord2	= (surfacePos.xz + eyeDir.xz * 0.1) * 0.005 + timer * 0.00002f * windDirection + sin(timer * 0.001 + worldPos.z) * 0.000005;
	
	//texCoord = vec2(texCoord.x, 1 - texCoord.y);
	//texCoord2 = vec2(texCoord2.x, 1 - texCoord2.y);
	if(depth2 < foamOptions.x)
	{
		foam = ( texture2D(foamMap, texCoord) + texture2D(foamMap, texCoord2) ) * 0.5f;
	}
	else if(depth2 < foamOptions.y)
	{
		foam = mix((texture2D(foamMap, texCoord) + texture2D(foamMap, texCoord2)) * 0.5f, vec4(0.0f, 0, 0, 0), (depth2 - foamOptions.x) / (foamOptions.y - foamOptions.x));
	}
	if(maxAmplitude - foamOptions.z > 0.0001f)
	{
		foam += (texture2D(foamMap, texCoord) + texture2D(foamMap, texCoord2)) * 0.05f * saturate((level - (waterLevel + foamOptions.z)) / (maxAmplitude - foamOptions.z));
	}
	vec3 tnorm = (view * vec4(normal, 0)).xyz;
	vec3 lightDist = (view * vec4(-10,2000,4800,1)).xyz - vec4(( viewPosRefr.xyz/viewPosRefr.w),1).xyz;
	vec3 s = normalize( lightDist );
	vec3 cameraPosWV	= ( view * vec4(cameraPos,1.0) ).xyz;
	float spec = pow(max(dot(normalize(-s + 2 *dot(s, tnorm ) * tnorm ), normalize( cameraPosWV - vec3( viewPosRefr.xyz/viewPosRefr.w))), 0.0), 40);
	vec3 specular = vec3(spec, spec, spec);
	
	color = saturate(mix(originalColor, mix(vec4(refraction,1.0f), vec4(reflection,1.0f),  0.7*fresnel) + vec4(specular,1) + foam, saturate(depth2 * shoreTransition)));
}