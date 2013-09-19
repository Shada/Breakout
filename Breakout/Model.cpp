#include "Model.h"
#ifdef _WIN32
#include "GraphicsDX11.h"
#endif // _WIN32

Model::Model()
{
	startIndex = 0;
	vertexAmount = 0;
}

Model::~Model()
{
}

void Model::draw()
{
#ifdef _WIN32
	GraphicsDX11::getInstance()->draw(startIndex, vertexAmount);
#else
	//linux::draw();
#endif // _WIN32
}

void Model::addData(Vertex iData)
{
	data.push_back(iData);
	vertexAmount++;
}

void Model::setStartIndex(int _index)
{
	startIndex = _index;
}

void Model::lastFace()
{
	data.push_back(data.at(data.size() - 3));								
	data.push_back(data.at(data.size() - 2));
}