#pragma once
#include "Resource.h"
#include <vector>


class Model
{
private:
	std::vector<Vertex> data;
	int startIndex;
	int vertexAmount;

public:
	Model();
	~Model();

	//L�gger till ett nytt v�rde i slutet p� data
	void addData(Vertex iData);
	// add a chunk of data
	void addData(std::vector<Vertex> _data);
	//L�gger till den sista vertex facet till Data
	void lastFace();

	//Returnerar Data
	std::vector<Vertex>* getData() { return &data; }

	void setStartIndex(int index);
	int getStartIndex() { return startIndex; }
	int getVertexAmount() { return vertexAmount; }
};

