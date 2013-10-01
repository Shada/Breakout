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

	float2 pos =  float2( input[0].pos.x, 0 );
	float2 size = float2( input[0].tex.x, input[0].tex.y );
	
	output.pos = float4( pos.x/resolution.x * 2 - 1, pos.y/resolution.y * 2 - 1, 0, 1 );//output.pos = float4( pos, 0, 1 );
	output.tex = float2( 0, 0 );
	triStream.Append(output);

	output.pos = float4( pos.x/resolution.x * 2 - 1, (pos.y + size.y)/resolution.y * 2 - 1, 0, 1 );//output.pos = float4( pos.x, pos.y + input[0].size.y, 0, 1 );
	output.tex = float2( 0, 1 );
	triStream.Append(output);

	output.pos = float4( (pos.x + size.x)/resolution.x * 2 - 1, pos.y/resolution.y * 2 - 1, 0, 1);//output.pos = float4( pos.x + input[0].size.x, pos.y, 0, 1);
	output.tex = float2( 1, 0 );
	triStream.Append(output);

	output.pos = float4( (pos.x + size.x)/resolution.x * 2 - 1, (pos.y + size.y)/resolution.y * 2 - 1, 0, 1 );//output.pos = float4( pos.x + input[0].size.x, pos.y + input[0].size.y, 0, 1 );
	output.tex = float2( 1, 1 );
	triStream.Append(output);
}