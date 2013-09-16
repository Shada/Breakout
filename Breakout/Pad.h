#ifndef _PAD_H_
#define _PAD_H_

#include "Object3D.h"

namespace Logic
{
	class Pad: public Object3D
	{
	private:
		static Vec3 pos;
	public:
		Pad();
		~Pad();


		static void Move(int pixels);
		void Update(double _dt);
		void Move2D(double _dt, float _x);
		void Move3D(double _dt, float _x);
		void setAngle(float _a)	{ angle2D = _a; };

	private:
		float movementSpeed, angle2D, angle3D;
	};
}
#endif
