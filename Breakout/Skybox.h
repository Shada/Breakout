#pragma once
#include "Resource.h"
class Skybox
{
private:
	int modelID;
	int textureID;
public:
	Skybox();
	~Skybox();

	static std::vector<Vertex> generateVertices();

	int getModelID()		{ return modelID;	}
	int getTextureID()		{ return textureID;	}

	void setModelID(int _modelID) { modelID = _modelID; }
	void setTextureID(int _textureID) { textureID = _textureID; }
};

