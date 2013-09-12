#include "Model.h"


Model::Model(void)
{
}


Model::~Model(void)
{
}


void Model::addData(Vertex iData)
{
	Data.push_back(iData);
}

void Model::lastFace()
{
	Data.push_back(Data.at(Data.size()-3));								
	Data.push_back(Data.at(Data.size()-2));
}