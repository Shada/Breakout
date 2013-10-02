#ifndef _PAD_H_
#define _PAD_H_

#include "Object3D.h"
#include "Resource.h"

namespace Logic
{
	class Pad: public Object3D
	{
	private:
		Matrix orientation;
		float width, height;
		float radius;
		int shaderTechniqueID;
		static Vec3 posKey, posMouse, rotMouse, rotKey;
		float movementSpeed, angle2D, angle3D;
		static bool releaseBall;
		Vec3 direction, ballPos;

		int activeEffect; //0 = nothing, 1 = stun, 2 = slow, 3 = speed
		float effectTimer, effectAcceleration, effectRotation;

	public:
		Pad();
		~Pad();

		float getRadius() { return radius; }
		float getOrientation() { return rotation.z - PI / 2; }
		float getPadRotation() { return rotation.z; }
		float getWidth() { return width; }

		void setReleaseBall(bool state)		{ releaseBall = state; }
		bool getReleaseBall()				{ return releaseBall; }
		Vec3 getDirection()					{ return direction; }
		Vec3 getBallPos()					{ return ballPos; }

		void update(double dt);
		void startStun();					//stun effect
		void startSlow();					//slow effect
		void startSpeed();					//speed effect
		void draw();
		void move2D(double dt, float x);
		void move3D(double dt, float x);
		void setAngle(float a)				{ angle2D = a;			}

		static void move(int pixels)		{ posMouse.x += pixels; }
		static void moveLeft()				{ posKey.x -= 150;		}
		static void moveRight()				{ posKey.x += 150;		}

		static void rotate(int direction);
		static void rotateRight();
		static void rotateLeft();

		static void ejectBall();
	};
}
#endif // ! _PAD_H_