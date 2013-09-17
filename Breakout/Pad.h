#ifndef _PAD_H_
#define _PAD_H_

#include "Object3D.h"

namespace Logic
{
	class Pad: public Object3D
	{
	private:
		static Vec3 pos;
		float movementSpeed, angle2D, angle3D;

	public:
		Pad();
		~Pad();

		static void move(int pixels);
		void update(double _dt);
		void move2D(double _dt, float _x);
		void move3D(double _dt, float _x);
		void setAngle(float _a)	{ angle2D = _a; };

	};
}
#endif
