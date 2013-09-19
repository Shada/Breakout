#include "Map.h"
#include "LoadHandler.h"

namespace Logic
{

	void Map::loadMap(unsigned int _mapID,std::vector<Object3D*> *_bricks,Ball *_ball,Pad *_pad)
	{
		FIBITMAP *pHeightMap = Resources::LoadHandler::getInstance()->getMap(_mapID)->getDib();

		if ( pHeightMap )
		{
			//get height and width & allocate memory
			unsigned int hmHeight = FreeImage_GetHeight(pHeightMap);
			unsigned int hmWidth = FreeImage_GetWidth(pHeightMap);
		

			RGBQUAD color;	
			//save map info
			FreeImage_GetPixelColor(pHeightMap,0,0,&color);
			int lvlnum = color.rgbRed;
			int maptype = color.rgbGreen;
			int difficulty = color.rgbBlue;

			//Load the rest of the pixel items
			for (unsigned int r=0; r < hmHeight; r++)
			{
				for(unsigned int c=1; c < hmWidth; c++)
				{
					FreeImage_GetPixelColor(pHeightMap,c,r,&color);

					//Creating objects
					if(color.rgbRed == 1)
					{
						//Set pad start pos here
						_pad->setPosition(Vec3((float)c,(float)r,0.0f));
						_pad->setModelID(color.rgbBlue);
						_pad->setTextureID(color.rgbGreen);
					}
					else if(color.rgbRed == 2)
					{
						//Set ball start pos here
						_ball->setPosition(Vec3((float)c,(float)r,0.0f));
						_ball->setModelID(color.rgbBlue);
						_ball->setTextureID(color.rgbGreen);

					}
					else if(color.rgbRed != 0)
					{
						//Set brick property here
						Brick tBrick(Vec3(0,0,0));
						tBrick.setPosition(Vec3((float)c,(float)r,0.0f));
						//tBrick.setType(color.rgbRed);
						tBrick.setTextureID(color.rgbGreen);
						tBrick.setModelID(color.rgbBlue);
						_bricks->push_back(&tBrick);
					}
					
				}
			}
		}
	}

}