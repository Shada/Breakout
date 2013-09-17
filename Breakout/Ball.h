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
		void update(double _dt);
		float getRadius()	{ return radius; };
		Vec3 getDirection()	{ return direction; };

		/* Returns position for where ball would be next frame, assuming same deltaTime. */
		Vec3 getNextFrame(){ return nextFrame; };

		/* Sets the direction-variables. Use NULL if you want them to remain unchanged (also the default value). */
		void setDirection(float _x = NULL, float _y = NULL, float _z = NULL);
		
	private:
		float speed;
		Vec3 direction, nextFrame;
		float radius;
	};
}
#endif