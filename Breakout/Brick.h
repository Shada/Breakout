#pragma once

#include "Object3D.h"

namespace Logic
{
	class Brick: public Object3D
	{
	public:
		Brick();
		~Brick();
		void update(double dt);
	private:
	};
}