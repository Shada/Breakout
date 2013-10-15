#pragma once
//#include "hrTimer.h"
//#include "stdafx.h"
//#include "Camera.h"
#include "Resource.h"
//#include "TechniqueHLSL.h"
#include "GraphicsDX11.h"

#define MAX_PARTICLES 30000

class ParticleSystem
{
public:
	ParticleSystem(char TShader[256] , Vec3 TStartpos, Vec3 TStartspeed, float Tradie, Vec3 TradieDirection, char TParticleTexRV[256], Vec4 Tg_vFrameGravity, float TPT_Lifetime, float TStages, int TparticlesPerStage, Vec4 TColor,Vec4 TFadeColor,int TRandomDir);
	~ParticleSystem();

	void Initialize(char TShader[256] ,Vec3 TStartpos, Vec3 TStartspeed, float Tradie, Vec3 TradieDirection, char TParticleTexRV[256], Vec4 Tg_vFrameGravity, float TPT_Lifetime, float TStages, int TparticlesPerStage, Vec4 TColor,Vec4 TFadeColor,int TRandomDir);
	bool getFinished() { return Finished; }
	void setFinished(bool f) { Finished = f; }
private:
	//timer explosionTimer;
	float global,temp;
	bool First, Finished;
	float count;
	void createBuffer( Vec3 pos);

	ID3D10Effect*                       Effect10;


	ID3D10ShaderResourceView*           ParticleTexRV;
	ID3D10Texture1D*                    RandomTexture;
	ID3D10ShaderResourceView*           RandomTexRV;

	ID3D10EffectTechnique*              RenderParticles;
	ID3D10EffectTechnique*              AdvanceParticles;
	ID3D10EffectMatrixVariable*         mWorldViewProj;
	ID3D10EffectMatrixVariable*         mInvView;
	ID3D10EffectScalarVariable*         fGlobalTime;
	ID3D10EffectScalarVariable*         fElapsedTime;
	ID3D10EffectShaderResourceVariable* DiffuseTex;
	ID3D10EffectShaderResourceVariable* RandomTex;
	ID3D10EffectScalarVariable*         SecondsPerFirework;
	ID3D10EffectVectorVariable*         vFrameGravity;
	ID3D10EffectVectorVariable*         Color;
	ID3D10EffectVectorVariable*         FadeColor;


	ID3D10EffectVectorVariable*         Startpos;
	ID3D10EffectScalarVariable*         particlesPerStage;
	ID3D10EffectScalarVariable*         Stages;
	ID3D10EffectVectorVariable*         Startspeed;
	ID3D10EffectScalarVariable*         radie;
	ID3D10EffectVectorVariable*         radieDirection;

	ID3D10EffectScalarVariable*         PT_Lifetime;

	ID3D10EffectScalarVariable*			RandomDir;
};