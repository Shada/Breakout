#pragma once
#include "Resource.h"
#include "TechniqueHLSL.h"


#define MAX_PARTICLES 300000

struct PARTICLE_VERTEX
{
    Vec3 pos;
    Vec3 vel;
    float Timer;
    UINT Type;
};

class Particles
{
public:
	Particles(ID3D11Device *_device, char _shader[256] , Vec3 _startpos, Vec3 _startspeed, float _radie, Vec3 _radieDirection, int _particleTexID, Vec4 _frameGravity, float _lifeTime, float _stages, int _particlesPerStage, Vec4 _color,Vec4 _fadeColor,int _randomDir);
	~Particles(void);
	void update(float dt);
	void draw();

private:
	ID3D11Device *device;


	CBParticles effectVariables;

	bool First, Finished;
	float count;
	int texID;

	void Initialize(char _shader[256] , int _particleTexID);
	void createBuffer( Vec3 pos);
	void createRandomTexture();

	ID3D11ShaderResourceView*           RandomTexRV;

	ID3D11Buffer*                       ParticleStart;
	ID3D11Buffer*                       ParticleStreamTo;
	ID3D11Buffer*                       ParticleDrawFrom;
	ID3D11InputLayout*					ParticleVertexLayout;

};

