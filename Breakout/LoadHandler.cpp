#include "LoadHandler.h"
#include <iostream>
#include <fstream>

namespace Resources
{

	LoadHandler	*LoadHandler::loadInstance = NULL;

	LoadHandler::LoadHandler()
	{
		loader = 0;
		loader = new Loader();
		char file[256];
        std::ifstream myfile ("Objects/object.txt");

#ifdef BAJSAPA
#else
        //std::ifstream myfile ("/home/torrebjorne/Documents/GitHub/Breakout/Breakout/Objects/object.txt");
#endif // BAJSAPA
		if (myfile.is_open())
		{
			while(!myfile.eof())
			{
				Model *tModel = new Model();
				myfile.getline(file,sizeof(myfile));
				loader->LoadObject(file,1,tModel,1,1,1);
				models.push_back(tModel);
			}
		}
		myfile.close();

		myfile.open("Maps/map.txt");
#ifdef BAJSAPA
#else
        //myfile.open("/home/torrebjorne/Documents/GitHub/Breakout/Breakout/Maps/map.txt");
#endif // BAJSAPA
		if (myfile.is_open())
		{
			while(!myfile.eof())
			{
				Texture *tMap = new Texture();
				myfile.getline(file,sizeof(myfile));
				loader->loadTexture(file,tMap);
				maps.push_back(tMap);
			}
		}
		myfile.close();

		myfile.open("Textures/textures.txt");
#ifdef BAJSAPA
#else
       // myfile.open("/home/torrebjorne/Documents/GitHub/Breakout/Breakout/Textures/textures.txt");
#endif // BAJSAPA

		if (myfile.is_open())
		{
			while(!myfile.eof())
			{
				textures.push_back(new Texture());
				myfile.getline(file,sizeof(myfile));
				loader->loadTexture(file,textures.back());
			}
		}
		myfile.close();
	}

	LoadHandler::~LoadHandler()
	{
		SAFE_DELETE(loader);
		for(unsigned int i = 0; i < models.size(); i++)
			SAFE_DELETE(models.at(i));
		models.clear();

		for(unsigned int i = 0; i < textures.size(); i++)
			SAFE_DELETE(textures.at(i));
		textures.clear();

		for(unsigned int i = 0; i < maps.size(); i++)
			SAFE_DELETE(maps.at(i));
		maps.clear();
	}
}
