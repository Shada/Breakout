#ifndef _BALL_H_
#define _BALL_H_

#include "Object3D.h"
#include <cstdlib>
#include <time.h>

namespace Logic
{
	class Ball : public Object3D
	{
	public:
		Ball();
		~Ball();

		void update(double dt);
		void startWind();					//Wind effect
		void draw();

		float getRadius()		{ return radius; };
		Vec3 getDirection()		{ return direction; };

		/* Returns position for where ball would be next frame, assuming same deltaTime. */
		Vec3 getNextFrame()		{ return nextFrame; };

#ifdef _WIN32
		/* Sets the direction-variables. Use NULL if you want them to remain unchanged (also the default value). */
		void setDirection(float x = NULL, float y = NULL, float z = NULL);
#else // _WIN32
		/* Sets the direction-variables. Use NULL if you want them to remain unchanged (also the default value). */
		void setDirection(float x, float y, float z);
#endif // _WIN32
	private:
		float speed;
		Vec3 direction, nextFrame;
		float radius;

		int shaderTechniqueID;

		int activeEffect; //0 = nothing, 1 = Wind
		Vec3 effectDirection;
		float effectTimer, effectAcceleration, effectSpeed;
	};
}
#endif // ! _BALL_H_