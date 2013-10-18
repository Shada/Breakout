#include "ObjectCore.h"
#include "Resource.h"

namespace Logic
{
	ObjectCore::ObjectCore()
	{
		water	= NULL;
		pad		= new Pad();
		ball	= std::vector<Ball*>();
		ball.push_back(new Ball());
		skybox	= new Skybox();

		//temporary prob, should be done in loadMap() in map.cpp
		//so that different skyboxes in different maps
		//also these indeces are very likely to change
		//especially textureID
		skybox->setModelID(3); 
		skybox->setTextureID(7);
	}

	void ObjectCore::setMapType(int type)
	{
		switch(type)
		{
			case eEarth :	mapType = eEarth;	break;
			case eWater :	mapType = eWater;	break;
			case eWind :	mapType = eWind;	break;
			case eFire :	mapType = eFire;	break;
		}
	}

	ObjectCore::~ObjectCore()
	{
		SAFE_DELETE(pad);
		for(unsigned int i = 0; i < ball.size(); i++)
			SAFE_DELETE(ball.at(i));
		SAFE_DELETE(water);
		SAFE_DELETE(skybox);
		for(unsigned int i = 0; i < bricks.size(); i++)
			SAFE_DELETE(bricks.at(i));
	}
}
