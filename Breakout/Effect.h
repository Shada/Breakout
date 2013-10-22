#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "Object3D.h"

namespace Logic
{
	class Effect : public Object3D
	{
	private:
		float fallSpeed;
		int effectType;
		bool isCylinder;
	public:
		Effect(Vec3 startPos, float ballDirY, int type, bool _isCylinder);
		~Effect();

		void update(double dt);

		int getType() { return effectType; }
	};
}
#endif