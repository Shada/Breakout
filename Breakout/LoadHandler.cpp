#include "LoadHandler.h"

namespace Resources
{
	LoadHandler::LoadHandler(void)
	{
		loader = 0;
		loader = new Loader();

		Model *tModel = 0;
		tModel = new Model();

		loader->LoadObject("Objects/Cube.obj",1,tModel,1,1,1);

		models.push_back(tModel);

		Texture *tTexture = 0;
		tTexture = new Texture();

		loader->loadTexture("",tTexture);

		textures.push_back(tTexture);

		Texture *tMap = 0;
		tMap = new Texture();

		loader->loadTexture("",tMap);

		maps.push_back(tMap);

	}


	LoadHandler::~LoadHandler(void)
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