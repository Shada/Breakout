#include "resource.fx"

struct VS_Input
{
	float pos	: POSITION;
	float4 tex	: TEXCOORD0;
};

struct PS_Input
{
	float4	pos			: SV_POSITION;
	float2	tex			: TEXCOORD;
};

[maxvertexcount(4)]
void gs( point VS_Input input[1], inout TriangleStream<PS_Input> triStream )
{
	PS_Input output = (PS_Input)0;

	float2 pos =  textPos + float2( input[0].pos, 0 ) * textSize;
	float4 texCoords = float4(input[0].tex.x/imageSize.x, input[0].tex.y/250, (input[0].tex.x + input[0].tex.z)/1420, (input[0].tex.y + input[0].tex.w)/250);

	output.pos = float4(	( pos.x / resolution.x ) * 2 - 1,
							( ( resolution.y - pos.y - ( input[0].tex.w * textSize.y ) ) / resolution.y ) * 2 - 1,
							0,
							1 );//output.pos = float4( pos.x, pos.y + input[0].size.y, 0, 1 );
	output.tex = float2( texCoords.x, texCoords.w );
	triStream.Append(output);

	output.pos = float4(	( pos.x / resolution.x ) * 2 - 1,
							( ( resolution.y - pos.y ) / resolution.y ) * 2 - 1,
							0,
							1 );//output.pos = float4( pos, 0, 1 );
	output.tex = float2( texCoords.x, texCoords.y );
	triStream.Append(output);

	output.pos = float4(	( (pos.x + ( input[0].tex.z * textSize.x ) ) / resolution.x) * 2 - 1,
							( (resolution.y - pos.y - ( input[0].tex.w * textSize.y ) ) / resolution.y ) * 2 - 1,
							0,
							1 );//output.pos = float4( pos.x + input[0].size.x, pos.y + input[0].size.y, 0, 1 );
	output.tex = float2( texCoords.z, texCoords.w );
	triStream.Append(output);

	output.pos = float4(	( ( pos.x + ( input[0].tex.z * textSize.x ) ) / resolution.x ) * 2 - 1,
							( ( resolution.y - pos.y ) / resolution.y ) * 2 - 1,
							0,
							1 );//output.pos = float4( pos.x + input[0].size.x, pos.y, 0, 1);
	output.tex = float2( texCoords.z, texCoords.y );
	triStream.Append(output);
}