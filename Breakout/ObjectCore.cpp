#include "ObjectCore.h"
#include "Resource.h"

namespace Logic
{
	ObjectCore::ObjectCore()
	{
		water = NULL;
		pad = new Pad();
		ball = new Ball();
	}


	ObjectCore::~ObjectCore()
	{
		SAFE_DELETE(pad);
		SAFE_DELETE(ball);
		SAFE_DELETE(water);
		for(unsigned int i = 0; i < bricks.size(); i++)
			SAFE_DELETE(bricks.at(i));
	}
}
