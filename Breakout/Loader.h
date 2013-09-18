#ifndef _LOADER_H_
#define _LOADER_H_


#include <sstream>
#include <fstream>
#include "Model.h"
#include "Texture.h"
#include "FreeImage.h"

namespace Resources
{

	class Loader
	{
	private:


	public:
		Loader(void);
		~Loader(void);
		void LoadObject(char file[256],float scale,Model *_model,float invertX = 1,float invertY = 1,float invertZ = 1);
		void loadTexture(char file[256],Texture *_texture);
	};

}

#endif