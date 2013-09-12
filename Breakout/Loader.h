#pragma once
#include <sstream>
#include <fstream>
#include "Model.h"
#include "Texture.h"
#include "FreeImage.h"

class Loader
{
private:


public:
	Loader(void);
	~Loader(void);
	void LoadObject(char file[256],float scale,Model *_model,float invertX,float invertY,float invertZ);
	void loadTexture(char file[256],Texture *_texture);
};

