#include "resource.fx"

//cbuffer cbImmutable
//{
//	float3 g_positions[4] =
//    {
//        float3( -1, 1, 0 ),
//        float3( 1, 1, 0 ),
//        float3( -1, -1, 0 ),
//        float3( 1, -1, 0 ),
//    };
//    float2 g_texcoords[4] = 
//    { 
//        float2(0,1), 
//        float2(1,1),
//        float2(0,0),
//        float2(1,0),
//    };
//};



struct PSSceneIn
{
    float4 pos : SV_Position;
    float2 tex : TEXTURE0;
    float4 color : COLOR0;
};


struct VSParticleDrawOut
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float radius : RADIUS;
};

//
// GS for rendering point sprite particles.  Takes a point and turns it into 2 tris.
//
[maxvertexcount(4)]
void GSScenemain(point VSParticleDrawOut input[1], inout TriangleStream<PSSceneIn> SpriteStream)
{
    PSSceneIn output;
    
	float3 g_positions[4];
    
	g_positions[0] =	float3( -1, 1, 0 );
    g_positions[1] =	float3( 1, 1, 0 );
	g_positions[2] =	float3( -1, -1, 0 );
	g_positions[3] =	float3( 1, -1, 0 );
    
    float2 g_texcoords[4];
    
    g_texcoords[0]  =	float2(0,1);
    g_texcoords[1]  =   float2(1,1);
    g_texcoords[2]  =   float2(0,0);
    g_texcoords[3]  =   float2(1,0);
    

    //
    // Emit two new triangles
    //

	//float4x4 tWorldViewProj = mul(mul(world,view),projection);
	float4x4 tWorldViewProj = mul(view,projection);

    for(int i=0; i<4; i++)
    {
        float3 position = g_positions[i]*input[0].radius;
        position = mul( (float3x3)viewInv, position ) + input[0].pos;
        output.pos = mul(tWorldViewProj, float4(position.xyz,1.0));
        
        output.color = input[0].color;
        output.tex = g_texcoords[i];
        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}
