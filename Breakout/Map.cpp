#include "Map.h"
#include "LoadHandler.h"



namespace Logic
{

	void Map::loadMap(unsigned int _mapID,std::vector<Object3D*> *_bricks,Ball *_ball,Pad *_pad)
	{
		FIBITMAP *pHeightMap = Resources::LoadHandler::getInstance()->getMap(7)->getDib();

		if ( pHeightMap )
		{
			//get height and width & allocate memory
			unsigned int hmHeight = FreeImage_GetHeight(pHeightMap);
			unsigned int hmWidth = FreeImage_GetWidth(pHeightMap);
		
			unsigned int displacementX = 7;
			unsigned int displacementY = 5;

			float circleRadie = 100;

			RGBQUAD color;	
			//save map info
			FreeImage_GetPixelColor(pHeightMap,0,0,&color);
			int lvlnum = color.rgbRed;
			int maptype = 4;//color.rgbGreen;
			int difficulty = color.rgbBlue;



			float radWJump = (2*PI)/hmWidth;
			float radHJump = (2*PI)/hmHeight;


			//Load the rest of the pixel items
			for (unsigned int y=0; y < hmHeight; y++)
			{
				for(unsigned int x=0; x < hmWidth; x++)
				{
					FreeImage_GetPixelColor(pHeightMap,x,y,&color);

					Vec3 displace;
					/*float radWIt = radWJump*(x - (hmWidth/2));
					float radHIt = radWJump*(y - (hmHeight/2));*/

					float radWIt = radWJump*x;
					float radHIt = radWJump*y;

					if(maptype == 0)//test
					{
						displace.x = x*displacementX;
						displace.y = y*displacementY;
						displace.z = 0;
					}
					else if(maptype == 1)//wind
					{
						displace.x = x*displacementX;
						displace.y = y*displacementY;
						displace.z = 0;
					}
					else if(maptype == 2)//water
					{
						displace.x = x*displacementX;
						displace.y = y*displacementY;
						displace.z = 0;
					}
					else if(maptype == 3)//fire
					{
						displace.x = sin(radWIt) * circleRadie;
						displace.y = y * displacementY;
						displace.z = cos(radWIt) * circleRadie;
					}
					else if(maptype == 4)//earth
					{
						// x = r * sin(theta) * cos(phi)
						// y = r * sin(theta) * sin(phi)
						// z = r * cos(theta) * 
						
					/*	displace.x = circleRadie * sin(radWIt) * cos(radHIt);
						displace.y = circleRadie * sin(radWIt) * sin(radHIt);  //Orienterat runt z
						displace.z = circleRadie * cos(radWIt);
*/
						displace.x = circleRadie * sin(radWIt) * cos(radHIt);
						displace.z = circleRadie * sin(radWIt) * sin(radHIt);  //Orienterat runt y
						displace.y = circleRadie * cos(radWIt);

						
					}


				
					//Creating objects
					if(x == 0 && y == 0)
					{
					}
					else if(color.rgbRed == 12)
					{
						//Set pad start pos here
						_pad->setPosition(Vec3((float)x*displacementX,(float)y*displacementY,0.0f));
						_pad->updateWorld();
						_pad->setModelID(color.rgbBlue);
						_pad->setTextureID(color.rgbGreen);
					}
					else if(color.rgbRed == 24)
					{
						//Set ball start pos here
						_ball->setPosition(Vec3((float)x*displacementX,(float)y*displacementY,0.0f));
						_ball->updateWorld();
						_ball->setModelID(color.rgbBlue);
						_ball->setTextureID(color.rgbGreen);
						_ball->setModelID(2);

					}
					else if(color.rgbRed != 0 && color.rgbRed != 255 )
					{
						//Set brick property here
						Brick *tBrick = new Brick(Vec3(0,0,0));

						tBrick->setPosition(Vec3(displace.x,(float)displace.y,displace.z));
						
						//tBrick->setPosition(Vec3((float)x*displacementX,(float)y*displacementY,0.0f));
						tBrick->updateWorld();

						//tBrick.setType(color.rgbRed);
						tBrick->setTextureID(color.rgbGreen);
						tBrick->setModelID(color.rgbBlue);
						_bricks->push_back(tBrick);
					}
				}
			}
		}
	}

}

