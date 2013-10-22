#include "LoadHandler.h"
#include <iostream>
#include <fstream>
#include "Skybox.h"
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
		myfile.open("Textures/textures.txt");
#else
        myfile.open("/home/torrebjorne/Documents/GitHub/Breakout/Breakout/Textures/textures.txt");
#endif // _WIN32

		if (myfile.is_open())
		{
			while(!myfile.eof())
			{
				myfile.getline(file,sizeof(myfile));
				if(file[0] != NULL)
				{
					textures.push_back(new Texture());
					loader->loadTexture(file,textures.back());
				}
			}
		}
		myfile.close();

		// add the skybox model
		// might be exchanged with having a obj file
		// instead of generating the vertices
		Model* tModel = new Model();
		tModel->addData(Skybox::generateVertices());
		models.push_back(tModel);
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
