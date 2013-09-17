#include "resource.fx"

struct GS_Input
{
	float2	pos			: POSITION;
	float2	size		: SIZE;
	float	rotation	: ROTATION;
	float4	tintAlpha	: TINTALPHA;
	float2	texCoords	: TEXCOORD0;
	int		texIndex	: TEXINDEX;
};

struct PS_Input
{
	float4	pos			: SV_POSITION;
	float4	tintAlpha	: TINTALPHA;
	float2	texCoords	: TEXCOORD0;
	int		texIndex	: TEXINDEX;
};


[maxvertexcount(4)]
void gs( point GS_Input input[1], inout TriangleStream<PS_Input> triStream )
{
	PS_Input output = (PS_Input)0;

	output.pos = float4( input[0].pos, 0, 1 );
	output.tex = float2( 0, 1 );
	triStream.Append(output);

	output.pos = float4( -1, 1, 0, 1 );
	output.tex = float2( 0, 0 );
	triStream.Append(output);

	output.pos = float4( 1, -1, 0, 1);
	output.tex = float2( 1, 1 );
	triStream.Append(output);

	output.pos = float4( 1, 1, 0, 1 );
	output.tex = float2( 1, 0 );
	triStream.Append(output);
}