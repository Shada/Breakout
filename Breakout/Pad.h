#pragma once

#include "Object3D.h"

namespace Logic
{
	class Pad: public Object3D
	{
	public:
		Pad();
		~Pad();
		void update(double dt);
	private:
	};
}