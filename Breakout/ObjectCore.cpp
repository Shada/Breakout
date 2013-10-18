#include "ObjectCore.h"
#include "Resource.h"

namespace Logic
{
	ObjectCore::ObjectCore()
	{
		water	= NULL;
		pad		= new Pad();
		ball	= new Ball();
		skybox	= new Skybox();

		//temporary prob, should be done in loadMap() in map.cpp
		//so that different skyboxes in different maps
		//also these indeces are very likely to change
		//especially textureID
		skybox->setModelID(3); 
		//skybox->setTextureID(0);
		testFont = new Font();
		testText = new Text(&fontBillboards, "FPS:", Vec2(1,1),Vec2(0.3f,0.3f),0.1f,Vec4(1,1,1,1));
		
		skybox->setTextureID(7); //Utkommenterad i merge
	}


	ObjectCore::~ObjectCore()
	{
		SAFE_DELETE(pad);
		SAFE_DELETE(ball);
		SAFE_DELETE(water);
		SAFE_DELETE(skybox);
		for(unsigned int i = 0; i < bricks.size(); i++)
			SAFE_DELETE(bricks.at(i));
		SAFE_DELETE(testFont);
		SAFE_DELETE(testText);
	}
}
