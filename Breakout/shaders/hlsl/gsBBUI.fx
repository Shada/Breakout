#include "resource.fx"

struct VS_Input
{
	float2	pos			: POSITION;
	float2	size		: SIZE;
	float	rotation	: ROTATION;
	float4	tintAlpha	: TINTALPHA;
	int		texIndex	: TEXINDEX;
};

struct PS_Input
{
	float4	pos			: SV_POSITION;
	float4	tintAlpha	: TINTALPHA;
	float2	texCoords	: TEXCOORD;
	int		texIndex	: TEXINDEX;
};


[maxvertexcount(4)]
void gs( point VS_Input input[1], inout TriangleStream<PS_Input> triStream )
{
	PS_Input output = (PS_Input)0;

	float2 pos =  float2( input[0].pos.x, input[0].pos.y );
	float2 size = float2( input[0].size.x, input[0].size.y );

	output.pos = float4( pos.x/resolution.x * 2 - 1, (resolution.y - pos.y - size.y)/resolution.y * 2 - 1, 0, 1 );//output.pos = float4( pos.x, pos.y + input[0].size.y, 0, 1 );
	output.tintAlpha = input[0].tintAlpha;
	output.texCoords = float2( 0, 1 );
	output.texIndex	= input[0].texIndex;
	triStream.Append(output);

	output.pos = float4( pos.x/resolution.x * 2 - 1, (resolution.y - pos.y)/resolution.y * 2 - 1, 0, 1 );//output.pos = float4( pos, 0, 1 );
	output.tintAlpha = input[0].tintAlpha;
	output.texCoords = float2( 0, 0 );
	output.texIndex	= input[0].texIndex;
	triStream.Append(output);

	output.pos = float4( (pos.x + size.x)/resolution.x * 2 - 1, (resolution.y - pos.y - size.y)/resolution.y * 2 - 1, 0, 1 );//output.pos = float4( pos.x + input[0].size.x, pos.y + input[0].size.y, 0, 1 );
	output.tintAlpha = input[0].tintAlpha;
	output.texCoords = float2( 1, 1 );
	output.texIndex	= input[0].texIndex;
	triStream.Append(output);

	output.pos = float4( (pos.x + size.x)/resolution.x * 2 - 1, (resolution.y - pos.y)/resolution.y * 2 - 1, 0, 1);//output.pos = float4( pos.x + input[0].size.x, pos.y, 0, 1);
	output.tintAlpha = input[0].tintAlpha;
	output.texCoords = float2( 1, 0 );
	output.texIndex	= input[0].texIndex;
	triStream.Append(output);
}