#ifndef _BALL_H_
#define _BALL_H_

#include "Object3D.h"

namespace Logic
{
	class Ball : public Object3D
	{
	public:
		Ball();
		~Ball();
		void Update(double _dt);
	private:
		float speed;
		Vec3 direction;
		float radius;
	};
}
#endif