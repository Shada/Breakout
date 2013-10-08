#ifndef _BALL_H_
#define _BALL_H_

#include "Object3D.h"

namespace Logic
{
	class Ball : public Object3D
	{
	private:
		float speed;
		Vec3 direction, lastFrame;
		float radius;

		int shaderTechniqueID;

	public:
		Ball();
		~Ball();

		void update(double dt);
		void draw();

		float getRadius()		{ return radius; };
		Vec3 getDirection()		{ return direction; };

		/* Returns position for where ball would be next frame, assuming same deltaTime. */
		Vec3 getLastFrame()		{ return lastFrame; };
		float getSpeed()		{ return speed; }

		/* Sets the direction-variables. Use NULL if you want them to remain unchanged. */
		void setDirection(float x, float y, float z);
		void setPosition(Vec3 _pos);
	};
}
#endif // ! _BALL_H_