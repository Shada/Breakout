#include "Effect.h"

namespace Logic
{
	Effect::Effect(Vec3 _startPos, float _ballDirY, int _type, bool _isCylinder) : Object3D()
	{
		isCylinder = _isCylinder;
		effectType = _type;
		position = _startPos;
		fallSpeed = _ballDirY < 0 ? -.25f : .25f;
		modelID = 0;
		textureID = 0;
		switch(_type)
		{
		case 0: textureID = 48; break;
		case 2: textureID = 49; break;
		case 3: textureID = 47; break;
			case 5:
		textureID = 46; break;
		}
		scale = Vec3(2, 1.5f, .5f);
	}

	void Effect::update(double _dt)
	{
		if(fallSpeed < 200 * _dt)
			fallSpeed += (float)pow(9.82, _dt * .2) - 1;

		position.y -= fallSpeed;

		isCylinder ? transformToCyl() : updateWorld();
	}

	Effect::~Effect()
	{
	}
}