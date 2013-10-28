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

		testFont = new Font();
		testFont->loadFontSettings("Fonts/blackwhite.txt");
		std::vector<BBFont> test = std::vector<BBFont>();
		testFont->setImageIndex(7);

		//temporary prob, should be done in loadMap() in map.cpp
		//so that different skyboxes in different maps
		//also these indeces are very likely to change
		//especially textureID
		skybox->setModelID(3); 
		skybox->setTextureID(0);
		
		testText = new Text(&fontBillboards, "FPS:", 0, Vec2(1,1),Vec2(0.3f,0.3f),0.1f,Vec4(1,1,1,1));
		testText->setFont(testFont);
		testText->setTextData(0, 10);

		lives	= new Text(&livesData, "x0", fontBillboards.size(), Vec2(SCRWIDTH-150, 100) );
		lives->setFont(testFont);
		lives->setTextData(0, 3);

		
		highscore	= new Text(&scoreData, "0", fontBillboards.size(), Vec2(SCRWIDTH-250, 200), Vec2(.5f,.5f) );
		highscore->setFont(testFont);
		highscore->setTextData(0, 12);

		sideBar	= new UIElement( &uiBillboards, 0, Vec2(0,0), Vec2(400,SCRHEIGHT), 0, Vec4(1,1,1,1) );
		statusBar = new UIElement( &uiBillboards, 4, Vec2(SCRWIDTH-300, 0), Vec2(300, SCRHEIGHT),0.0f, Vec4(1,1,1,1) );
		healthSymbol = new UIElement( &uiBillboards, 0, Vec2(SCRWIDTH-280, 100), Vec2(115, 55), 0, Vec4(1,1,1,1) );

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
		for(unsigned int i = 0; i < effects.size(); i++)
			SAFE_DELETE(effects.at(i));
		SAFE_DELETE(testFont);
		SAFE_DELETE(testText);
		SAFE_DELETE(sideBar);
		SAFE_DELETE(statusBar);
		SAFE_DELETE(selector);
		SAFE_DELETE(logo);
		SAFE_DELETE(loli);
		SAFE_DELETE(circle);
		SAFE_DELETE(healthSymbol);
		SAFE_DELETE(lives);
	}
}
