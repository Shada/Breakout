#pragma once
//#include "ParticleSystem.h"
#include "Particles.h"

class ParticleSystemHandler
{
public:
	ParticleSystemHandler(void);
	~ParticleSystemHandler(void);
	void addEffect(char TShader[256] , Vec3 TStartpos, Vec3 TStartspeed, float Tradie, Vec3 TradieDirection, char TParticleTexRV[256], Vec4 Tg_vFrameGravity, float TPT_Lifetime, float TStages, int TparticlesPerStage, Vec4 TColor,Vec4 TFadeColor,int TRandomDir);
	void Update(float dt);
	void Draw(Matrix *viewMatrix,Matrix *projectionMatrix);
private:
	std::vector<Particles*> ParticleEffects;
};

