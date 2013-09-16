#ifndef _LOADHANDLER_H_
#define _LOADHANDLER_H_

#include "Loader.h"
#include "Model.h"
#include "Texture.h"

namespace Resources
{

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
		Model* getModel(unsigned int _index){return models.at(_index);};
		Texture* getTexture(unsigned int _index){return textures.at(_index);}
		Texture* getMap(unsigned int _index){return maps.at(_index);}
	};

}

#endif
