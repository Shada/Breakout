#pragma once

#include "Object.h"

namespace Logic
{
	class Pad: public Object
	{
	public:
		Pad() : Object();
		~Pad();
		void update();
	private:
	}
}