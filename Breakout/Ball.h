#ifndef _BALL_H_
#define _BALL_H_

#include "Object3D.h"

namespace Logic
{
	class Ball : public Object3D
	{
	private:
		float speed;
		Vec3 direction, nextFrame;
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
		Vec3 getNextFrame()		{ return nextFrame; };

		/* Sets the direction-variables. Use NULL if you want them to remain unchanged. */
		void setDirection(float x, float y, float z);
	};
}
#endif // ! _BALL_H_