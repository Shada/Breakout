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

	public:
		Pad();
		~Pad();

		float getRadius() { return radius; }
		float getOrientation() { return rotation.z - PI / 2; }
		float getPadRotation() { return rotation.z; }
		float getWidth() { return width; }

		void update(double dt);
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
	};
}
#endif // ! _PAD_H_