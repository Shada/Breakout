#pragma once

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

		void update(double dt);
		static void move(int pixels);
	};
}