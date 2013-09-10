#ifdef _BALL_H_
#define _BALL_H_

#include "Object.h"

namespace Logic
{
	class Ball : public Object
	{
	public:
		Ball() : Object();
		~Ball();
		void update(double _dt);
	private:
		float speed;
		Vec3 direction;
		float radius;
	}
}
#endif