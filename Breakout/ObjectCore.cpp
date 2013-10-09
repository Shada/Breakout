#include "ObjectCore.h"
#include "Resource.h"

namespace Logic
{
	ObjectCore::ObjectCore()
	{
		pad = new Pad();
		ball = new Ball();
		testFont = new Font();
		testText = new Text(&fontBillboards, "FPS:", Vec2(1,1),Vec2(0.3f,0.3f),0.1f,Vec4(1,1,1,1));
	}


	ObjectCore::~ObjectCore()
	{
		SAFE_DELETE(pad);
		SAFE_DELETE(ball);
		for(unsigned int i = 0; i < bricks.size(); i++)
			SAFE_DELETE(bricks.at(i));
		SAFE_DELETE(testFont);
		SAFE_DELETE(testText);
	}
}
