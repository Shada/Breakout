#include "Model.h"
#ifdef BAJSAPA
#include "GraphicsDX11.h"
#endif // BAJSAPA

Model::Model()
{
	startIndex = 0;
	vertexAmount = 0;
}

Model::~Model()
{
}

void Model::addData(Vertex iData)
{
	data.push_back(iData);
	vertexAmount++;
}
void Model::addData(std::vector<Vertex> _data)
{
	data.insert(data.end(),_data.begin(),_data.end());
	vertexAmount+=_data.size();
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