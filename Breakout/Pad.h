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

		Vec3 direction, ballPos, prevPos;

		float angle2D, angle3D;
		float width, height, radius, movementSpeed;
		float effectTimer, effectAcceleration, effectRotation, invertTimer, rotationTimer;

		bool invertedControls;

		int activeEffect; //0 = nothing, 1 = stun, 2 = slow, 3 = speed

		static float angle;
		static bool releaseBall, decreasedRotation;
		static Vec3 posKey, posMouse, rotMouse, rotKey;


		void _update(double dt);
		void _setBallToPad();

	public:
		Pad();
		~Pad();

		void setPosition(Vec3 pos);

		float getRadius()					{ return radius; }
		float getOrientation()				{ return rotation.z - (float)PI / 2; }
		float getPadRotation()				{ return rotation.z; }
		float getWidth()					{ return width; }

		bool getReleaseBall()				{ return releaseBall; }
		Vec3 getDirection()					{ return direction; }
		Vec3 getBallPos()					{ return ballPos; }
		Vec3 getPrevPos()					{ return prevPos; }
		float getAngle()					{ return angle;	}

		void update(double dt);
		void updateCylinder(double dt);
		void startStun();					//stun effect
		void startSlow();					//slow effect
		void startSpeed();					//speed effect
		void checkEffects(double dt);
		void draw();
		void move2D(double dt, float x);
		void move3D(double dt, float x);

		void setAngle(float a)				{ angle2D = a;			}
		void setReleaseBall(bool state)		{ releaseBall = state;	}
		void invertControls(float time)		{ invertTimer = time; invertedControls = true;		}
		void decreaseRotation(float time)	{ rotationTimer = time; decreasedRotation = true;	}

		// Function pointers, used by the Input handler
		static void move(int pixels)		{ posMouse.x += abs(pixels) > 15 ? pixels < 0 ? -15 : 15 : pixels; }
		static void moveLeft()				{ posKey.x -= 150;		}
		static void moveRight()				{ posKey.x += 150;		}

		static void rotate(int direction);
		static void rotateRight();
		static void rotateLeft();

		static void ejectBall();
	};
}
#endif // ! _PAD_H_