#include "ObjectCore.h"
#include "Resource.h"

namespace Logic
{
	ObjectCore::ObjectCore()
	{
		pad = new Pad();
		ball = new Ball();
	}


	ObjectCore::~ObjectCore()
	{
		SAFE_DELETE(pad);
		SAFE_DELETE(ball);
		for(unsigned int i = 0; i < bricks.size(); i++)
			SAFE_DELETE(bricks.at(i));
	}
}
