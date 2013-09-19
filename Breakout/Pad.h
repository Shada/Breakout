#ifndef _PAD_H_
#define _PAD_H_

#include "Object3D.h"

namespace Logic
{
	class Pad: public Object3D
	{
	private:
		static Vec3 posKey, posMouse, rot;
		float movementSpeed, angle2D, angle3D;

	public:
		Pad();
		~Pad();

		void update(double dt);
		void move2D(double dt, float x);
		void move3D(double dt, float x);
		void setAngle(float a)				{ angle2D = a;			}

		static void move(int pixels)		{ posMouse.x += pixels; }
		static void moveLeft()				{ posKey.x -= 5;		}
		static void moveRight()				{ posKey.x += 5;		}

		static void rotateRight();
		static void rotateLeft();
	};
}
#endif // ! _PAD_H_