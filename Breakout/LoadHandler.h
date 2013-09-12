#ifndef _LOADHANDLER_H_
#define _LOADHANDLER_H_

#include "Loader.h"
#include "Model.h"
#include "Texture.h"

class LoadHandler
{
private:
	Loader *loader;
	std::vector<Model*> models;
	std::vector<Texture*> textures;
	std::vector<Texture*> maps;

public:
	LoadHandler(void);
	~LoadHandler(void);
};

#endif