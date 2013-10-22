#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "Object3D.h"

namespace Logic
{
	class Effect : public Object3D
	{
	private:
		float fallSpeed;
	public:
		Effect(Vec3 startPos, int ballDirY);
		~Effect();

		void update(double dt);
	};
}
#endif