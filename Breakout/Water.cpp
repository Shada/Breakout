#include "Water.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif // _WIN32

Water::Water(float _waterLevel)
{
	waterLevel = _waterLevel;
	windDirection = Vec2(-0.2f, 0.8f);
	paused = false;
	maxSpeed = 1.f;
	acceleration = 0.1f;
	timer = 0.0;
	speed = 0.4f;
	speedModifier = 1.0f;
	pausedDuration = 0;
	pauseFlow(5.f);

	CBWaterOnce cb;
	cb.waterFade			= .15f;
	cb.normalScaling		= 1.0f;
	cb.maxAmplitude			= 1.0f;
	cb.shoreTransition		= 0.5f;
	cb.refractionStrength	= .0f;
	cb.displacementStrength	= 0.7f;
	cb.shininess			= .0f;
	cb.specularIntensity	= .32f;
	cb.transparency			= 4.0f;
	cb.refractionScale		= .0008f;
	
	cb.normalModifier		= Vec4(1.0f,2.0f,4.0f,8.0f);	
	cb.foamOptions			= Vec3(0.75f, 1.32f, 0.5f);
	cb.waterSurfaceColor	= Vec3(0.0078f, 0.517f, 0.7f);
	cb.waterDepthColor		= Vec3(0,0,0.4);//Vec3(0.0039f, 0.0196f, 0.145f );
	cb.extinction			= Vec3(28.0f, 120.0f, 160.0f );
	cb.scale				= Vec2( 0.0005f, 0.0005f );
	cb.temp					= Vec3(0,0,0);
	GraphicsDX11::getInstance()->updateCBWaterOnce(cb);
}

void Water::update(double _dt)
{
	timer += _dt*5000.0;
	// if paused only tick the timer and dont move the waterlevel
	if(paused)
	{
		pausedDuration -= _dt;
		if(pausedDuration < 0)
		{
			paused = false;
			pausedDuration = 0;
		}
	}
	else
	{
		if(speedDuration > 0)
		{
			speedDuration -= _dt;
			if(speedDuration < 0)
			{
				speedModifier = 1;
				speedDuration = 0;
			}
		}
		speed += acceleration * (float)_dt;
		if(speed > maxSpeed)
		{
			speed = maxSpeed;
		}
		waterLevel += speed * speedModifier * (float)_dt;
	}
	CBWater cb;
	cb.timer = timer;
	cb.waterLevel = waterLevel;
	cb.windDirection = windDirection;
#ifdef _WIN32
	GraphicsDX11::getInstance()->updateCBWater(cb);
#else
#endif // _WIN32
}
void Water::setSpeedModifier(float _modifier, double _duration)
{
	speedModifier = _modifier;
	speedDuration = _duration;
}
void Water::reverseFlow()
{
	speed = maxSpeed;
	speed *= -1;
}
void Water::pauseFlow(double _duration)
{
	paused = true;
	pausedDuration += _duration;
}
Water::~Water()
{
}