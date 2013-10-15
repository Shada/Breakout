#include "ParticleSystemHandler.h"


ParticleSystemHandler::ParticleSystemHandler(void)
{
}


ParticleSystemHandler::~ParticleSystemHandler(void)
{
}

void ParticleSystemHandler::addEffect(char TShader[256] , Vec3 TStartpos, Vec3 TStartspeed, float Tradie, Vec3 TradieDirection, char TParticleTexRV[256], Vec4 Tg_vFrameGravity, float TPT_Lifetime, float TStages, int TparticlesPerStage, Vec4 TColor,Vec4 TFadeColor,int TRandomDir)
{
	ParticleEffects.push_back(new Particles(TShader,TStartpos,TStartspeed,Tradie,TradieDirection,TParticleTexRV,Tg_vFrameGravity,TPT_Lifetime,TStages,TparticlesPerStage,TColor,TFadeColor,TRandomDir));
}

void ParticleSystemHandler::Update(float dt)
{
	for(int i = 0; i < ParticleEffects.size();i++)
	{
		ParticleEffects.at(i)->Update(dt);
	}
}

void ParticleSystemHandler::Draw(Matrix *viewMatrix,Matrix *projectionMatrix)
{
	for(int i = 0; i < ParticleEffects.size();i++)
	{
		ParticleEffects.at(i)->Draw(viewMatrix,projectionMatrix);
	}
}