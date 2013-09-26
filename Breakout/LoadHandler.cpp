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

#ifdef _WIN32
        std::ifstream myfile ("Objects/object.txt");
#else
        std::ifstream myfile ("/home/torrebjorne/Documents/GitHub/Breakout/Breakout/Objects/object.txt");
#endif // _WIN32
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

#ifdef _WIN32
		myfile.open("Maps/map.txt");
#else
        myfile.open("/home/torrebjorne/Documents/GitHub/Breakout/Breakout/Maps/map.txt");
#endif // _WIN32
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

#ifdef _WIN32
		myfile.open("Textures/texture.txt");
#else
        myfile.open("/home/torrebjorne/Documents/GitHub/Breakout/Breakout/Textures/texture.txt");
#endif // _WIN32
		if (myfile.is_open())
		{
			while(!myfile.eof())
			{
				Texture *tTexture = new Texture();
				myfile.getline(file,sizeof(myfile));
				loader->loadTexture(file,tTexture);
				textures.push_back(tTexture);
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
