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

			float circleRadie = 50;

			RGBQUAD color;	
			//save map info
			FreeImage_GetPixelColor(pHeightMap,0,0,&color);
			int lvlnum = color.rgbRed;
			int maptype = color.rgbGreen;
			int difficulty = color.rgbBlue;
			float radWJump = (2*PI)/hmWidth;
			float radHJump = (2*PI)/hmHeight;


			//Load the rest of the pixel items
			for (unsigned int r=0; r < hmHeight; r++)
			{
				for(unsigned int c=0; c < hmWidth; c++)
				{
					FreeImage_GetPixelColor(pHeightMap,c,r,&color);

					Vec3 cylinderDisplace;
					/*float radWIt = radWJump*(c - (hmWidth/2));
					float radHIt = radWJump*(r - (hmHeight/2));*/

					float radWIt = radWJump*c;
					float radHIt = radWJump*r;

					//--------Cylinder
					//cylinderDisplace.x = sin(radWIt) * circleRadie;
					//cylinderDisplace.y = r * displacementX;
					//cylinderDisplace.z = cos(radWIt) * circleRadie;
					//--------


					cylinderDisplace.x = cos(radHIt) * circleRadie + sin(radWIt) * circleRadie;
					cylinderDisplace.y = sin(radHIt) * circleRadie + sin(radHIt) * circleRadie;
					cylinderDisplace.z = cos(radHIt) * circleRadie + cos(radWIt) * circleRadie;



					cylinderDisplace.x = cos(radHIt) * circleRadie;
					cylinderDisplace.y = sin(radHIt) * circleRadie;   //  X Y led
					cylinderDisplace.z = 0;

					//cylinderDisplace.x = c*displacementX;
					//cylinderDisplace.y = sin(radHIt) * circleRadie;   // Y Z led
					//cylinderDisplace.z = cos(radHIt) * circleRadie;

					cylinderDisplace.x = cos(radHIt+radWIt) * circleRadie + sin(radWIt) * circleRadie;
					cylinderDisplace.y = sin(radHIt) * circleRadie + sin(radHIt) * circleRadie;
					cylinderDisplace.z = cos(radHIt+radWIt) * circleRadie + cos(radWIt) * circleRadie;

				
					//Creating objects
					if(c == 0 && r == 0)
					{
					}
					else if(color.rgbRed == 12)
					{
						//Set pad start pos here
						_pad->setPosition(Vec3((float)c*displacementX,(float)r*displacementY,0.0f));
						_pad->setModelID(color.rgbBlue);
						_pad->setTextureID(color.rgbGreen);
					}
					else if(color.rgbRed == 24)
					{
						//Set ball start pos here
						_ball->setPosition(Vec3((float)c*displacementX,(float)r*displacementY,0.0f));
						_ball->setModelID(color.rgbBlue);
						_ball->setTextureID(color.rgbGreen);
						_ball->setModelID(2);

					}
					else if(color.rgbRed != 0 && color.rgbRed != 255 )
					{
						//Set brick property here
						Brick *tBrick = new Brick(Vec3(0,0,0));
						tBrick->setPosition(Vec3(cylinderDisplace.x,(float)cylinderDisplace.y,cylinderDisplace.z));
						
						//tBrick->setPosition(Vec3((float)c*displacementX,(float)r*displacementY,0.0f));
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