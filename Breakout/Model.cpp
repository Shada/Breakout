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
	Data.push_back(iData);
}

void Model::setStartIndex(int _index)
{
	startIndex = _index;
}

void Model::lastFace()
{
	Data.push_back(Data.at(Data.size()-3));								
	Data.push_back(Data.at(Data.size()-2));
}