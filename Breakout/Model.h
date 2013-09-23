#pragma once
#include "Resource.h"
#include <vector>


class Model
{
private:
	std::vector<Vertex> Data;
	int startIndex;
	int vertexAmount;
public:
	Model(void);
	~Model(void);
	
	/* draw the model */
	void draw();
	//Lägger till ett nytt värde i slutet på data
	void addData(Vertex iData);
	//Lägger till den sista vertex facet till Data
	void lastFace();
	//Returnerar Data
	std::vector<Vertex>* getData() { return &Data; }
	void setStartIndex(int index);
	int getStartIndex() { return startIndex; }
	int getVertexAmount() { return vertexAmount; }
};

