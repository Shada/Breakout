
struct VSParticleIn
{
    float3 pos              : POSITION;         //position of the particle
    float3 vel              : NORMAL;           //velocity of the particle
    float  Timer            : TIMER;            //timer for the particle
    uint   Type             : TYPE;             //particle type
};

struct VSParticleDrawOut
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float radius : RADIUS;
};

struct PSSceneIn
{
    float4 pos : SV_Position;
    float2 tex : TEXTURE0;
    float4 color : COLOR0;
};


	Texture2D g_txDiffuse;
	Texture1D g_txRandom;
	//bool Spawned = false;
	



SamplerState g_samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};


SamplerState g_samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap;
};  //-------------------------------------------------------------

BlendState AdditiveBlending
{
    //AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    //SrcBlend = SRC_ALPHA;
    SrcBlend = ONE;
    DestBlend = ONE;
    BlendOp = ADD;
    SrcBlendAlpha = ONE;
    //SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState NoBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = FALSE;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

//
// Sample a random direction from our random texture
//
float3 RandomDir(float fOffset)
{
    float tCoord = ((g_fGlobalTime + fOffset)) / 30;
    return g_txRandom.SampleLevel( g_samPoint, tCoord, 0 );
}








//
// Main particle system handler... handler particles and streams them out to a vertex buffer
//




//
// RenderParticles - renders particles on the screen
//
technique10 RenderParticles
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSScenemain() ) );
        SetGeometryShader( CompileShader( gs_4_0, GSScenemain() ) );
        SetPixelShader( CompileShader( ps_4_0, PSScenemain() ) );
        
        SetBlendState( AdditiveBlending, float4( 1.0f, 1.0f, 1.0f, 1.0f ), 0xFFFFFFFF );

		SetRasterizerState( NoCulling  );
        SetDepthStencilState(EnableDepth, 0xff);
    }  
}

//
// AdvanceParticles - advances the particle system one time step
//

GeometryShader gsStreamOut = ConstructGSWithSO( CompileShader( gs_4_0, GSAdvanceParticlesMain() ), "POSITION.xyz; NORMAL.xyz; TIMER.x; TYPE.x;" );
technique10 AdvanceParticles
{
    pass p0
    {
        SetVertexShader( CompileShader( vs_4_0, VSPassThroughmain() ) );
        SetGeometryShader( gsStreamOut );
        SetPixelShader( NULL );
        
		SetRasterizerState( NoCulling  );
        SetDepthStencilState(DisableDepth, 0xff);
    }  
}
