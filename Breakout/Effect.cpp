#include "Effect.h"

namespace Logic
{
	Effect::Effect(Vec3 _startPos, int _ballDirY) : Object3D()
	{
		position = _startPos;
		fallSpeed = _ballDirY > 0 ? -1 : 1;
		modelID = 0;
		textureID = 0;

		scale = Vec3(2, 1.5f, 1);
	}

	void Effect::update(double _dt)
	{
		if(fallSpeed < 200 * _dt)
			fallSpeed += pow(9.82, _dt * .5) - 1;

		position.y -= fallSpeed;
		updateWorld();
	}

	Effect::~Effect()
	{
	}
}