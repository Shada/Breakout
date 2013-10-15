#include "ParticleSystem.h"
//#include "stdafx.h"

ParticleSystem::ParticleSystem(char TShader[256] , Vec3 TStartpos, Vec3 TStartspeed, float Tradie, Vec3 TradieDirection, char TParticleTexRV[256], Vec4 Tg_vFrameGravity, float TPT_Lifetime, float TStages, int TparticlesPerStage, Vec4 TColor,Vec4 TFadeColor,int TRandomDir) //:	Effect10(NULL),
									//ParticleVertexLayout(NULL)
{
	First = true;
	Finished = false;
	count = 0;

	Initialize(TShader,TStartpos,TStartspeed,Tradie,TradieDirection,TParticleTexRV,Tg_vFrameGravity,TPT_Lifetime,TStages,TparticlesPerStage,TColor,TFadeColor,TRandomDir);
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Initialize(char TShader[256] , Vec3 TStartpos, Vec3 TStartspeed, float Tradie, Vec3 TradieDirection, char TParticleTexRV[256], Vec4 Tg_vFrameGravity, float TPT_Lifetime, float TStages, int TparticlesPerStage, Vec4 TColor,Vec4 TFadeColor,int TRandomDir)
{
	First = true;
	Finished = false;
	count = 0;
	temp = 0.002;
	global = 0;
	

	

	d3dDevice = device;


	ID3D10Blob* errors = NULL;

	if(FAILED(D3DX10CreateEffectFromFile(	TShader,
											NULL,
											NULL,
											"fx_4_0",
											D3D10_SHADER_ENABLE_STRICTNESS,
											0,
											d3dDevice,
											NULL,
											NULL,
											&Effect10,
											&errors,
											NULL)))
	{
		MessageBox(0, (char*)errors->GetBufferPointer(), "Shader error!", 0);
	}


	RenderParticles = Effect10->GetTechniqueByName( "RenderParticles" );
    AdvanceParticles = Effect10->GetTechniqueByName( "AdvanceParticles" );

    // Obtain the parameter handles
    mWorldViewProj = Effect10->GetVariableByName( "g_mWorldViewProj" )->AsMatrix();
    mInvView = Effect10->GetVariableByName( "g_mInvView" )->AsMatrix();
    fGlobalTime = Effect10->GetVariableByName( "g_fGlobalTime" )->AsScalar();
    fElapsedTime = Effect10->GetVariableByName( "g_fElapsedTime" )->AsScalar();

    DiffuseTex = Effect10->GetVariableByName( "g_txDiffuse" )->AsShaderResource();
	RandomTex = Effect10->GetVariableByName( "g_txRandom" )->AsShaderResource();
    vFrameGravity = Effect10->GetVariableByName( "g_vFrameGravity" )->AsVector();
    Color = Effect10->GetVariableByName( "Color" )->AsVector();
	FadeColor = Effect10->GetVariableByName( "FadeColor" )->AsVector();

	Startpos = Effect10->GetVariableByName( "Startpos" )->AsVector();
    particlesPerStage = Effect10->GetVariableByName( "particlesPerStage" )->AsScalar();
    Stages = Effect10->GetVariableByName( "Stages" )->AsScalar();
    Startspeed = Effect10->GetVariableByName( "Startspeed" )->AsVector();
    radie = Effect10->GetVariableByName( "radie" )->AsScalar();
    radieDirection = Effect10->GetVariableByName( "radieDirections" )->AsVector();


    PT_Lifetime = Effect10->GetVariableByName( "P_SHELLLIFE" )->AsScalar();

	RandomDir = Effect10->GetVariableByName( "randDir" )->AsScalar();


	D3D10_PASS_DESC PassDesc;
    AdvanceParticles->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	device->CreateInputLayout( fireworkLayout, fireworksNumElements,
                                             PassDesc.pIAInputSignature,
                                             PassDesc.IAInputSignatureSize, &ParticleVertexLayout );

	D3DXCOLOR colorMtrlDiffuse( 1.0f, 1.0f, 1.0f, 1.0f );
    D3DXCOLOR colorMtrlAmbient( 0.35f, 0.35f, 0.35f, 0 );

	D3DX10CreateShaderResourceViewFromFile( device, TParticleTexRV, NULL, NULL, &ParticleTexRV, NULL );
	createBuffer( device, TStartpos );
	createRandomTexture( device );



	vFrameGravity->SetFloatVector( ( float* )&Tg_vFrameGravity );
    RandomTex->SetResource( RandomTexRV );
	DiffuseTex->SetResource( ParticleTexRV );

	Color->SetFloatVector((float*) TColor);
	FadeColor->SetFloatVector((float*)TFadeColor);

	Startpos->SetFloatVector((float*) TStartpos);
	Startspeed->SetFloatVector((float*) TStartspeed);

	radie->SetFloat(Tradie);
	radieDirection->SetFloatVector((float*)TradieDirection);
	Stages->SetFloat(TStages);
	PT_Lifetime->SetFloat(TPT_Lifetime);
	RandomDir->SetInt(TRandomDir);
	particlesPerStage->SetInt(TparticlesPerStage);
}


