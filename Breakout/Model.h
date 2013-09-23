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
	
	/* draw the model */
	void draw();

	//L�gger till ett nytt v�rde i slutet p� data
	void addData(Vertex iData);
	//L�gger till den sista vertex facet till Data
	void lastFace();
	//Returnerar Data
	std::vector<Vertex>* getData()	{return &data;}
	void setStartIndex(int index);
};

