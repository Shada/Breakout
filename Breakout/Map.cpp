#include "Map.h"
#include "LoadHandler.h"



namespace Logic
{

	void Map::loadMap(unsigned int _mapID,std::vector<Brick*> *_bricks,Ball *_ball,Pad *_pad)
	{
		//clear the brick vector, not sure if this should be done here
		for(unsigned int i = 0; i < _bricks->size(); i++)
			SAFE_DELETE(_bricks->at(i) );
		_bricks->clear();
		FIBITMAP *pHeightMap = Resources::LoadHandler::getInstance()->getMap(_mapID)->getDib();

		if ( pHeightMap )
		{
			//get height and width & allocate memory
			unsigned int hmHeight = FreeImage_GetHeight(pHeightMap);
			unsigned int hmWidth = FreeImage_GetWidth(pHeightMap);
		
			unsigned int displacementX = 7;
			unsigned int displacementY = 5;


			RGBQUAD color;	
			//save map info
			FreeImage_GetPixelColor(pHeightMap,0,0,&color);
			int lvlnum = color.rgbRed;
			mapType = color.rgbGreen;
			int difficulty = color.rgbBlue;

			//Load the rest of the pixel items
			for (unsigned int r=0; r < hmHeight; r++)
			{
				for(unsigned int c=0; c < hmWidth; c++)
				{
					FreeImage_GetPixelColor(pHeightMap,c,r,&color);

					//Creating objects
					if(c == 0 && r == 0)
					{
					}
					else if(color.rgbRed == 1)
					{
						//if no pad is desired or the pad from the previously loaded map is wanted
						//this will be null
						if(_pad != NULL)
						{
							//Set pad start pos here
							_pad->setPosition(Vec3((float)c*displacementX,(float)r*displacementY,0.0f));
							_pad->updateWorld();
							_pad->setModelID(color.rgbBlue);
							_pad->setTextureID(color.rgbGreen);
						}
						_pad->setReleaseBall(false);
					}
					else if(color.rgbRed == 2)
					{
						//if no balls are desired or the pad from the previously loaded map is wanted
						//this will be null
						if( _ball != NULL )
						{
							//Set ball start pos here
							_ball->setPosition(Vec3((float)c*displacementX,(float)r*displacementY,0.0f));
							_ball->updateWorld();
							_ball->setModelID(color.rgbBlue);
							_ball->setTextureID(color.rgbGreen);
							_ball->setModelID(2);
						}

					}
					else if(color.rgbRed != 0 && color.rgbRed != 255 )
					{
						//Set brick property here
						Vec3 brickPos = Vec3((float)c*displacementX,(float)r*displacementY,0.0f);
						Brick *tBrick = new Brick(brickPos);
						tBrick->setTextureID(color.rgbGreen);
						tBrick->setModelID(color.rgbBlue);

						mapType == 3 ? tBrick->transformToCyl() : tBrick->updateWorld();

						_bricks->push_back(tBrick);
					}
					
				}
			}
		}
	}

}