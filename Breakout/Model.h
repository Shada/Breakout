#pragma once
#include "Resource.h"
#include <vector>


class Model
{
private:
	std::vector<Vertex> Data;

public:
	Model(void);
	~Model(void);

	//L�gger till ett nytt v�rde i slutet p� data
	void addData(Vertex iData);
	//L�gger till den sista vertex facet till Data
	void lastFace();
	//Returnerar Data
	std::vector<Vertex>* getData(){return &Data;}
	//Inverts the objects vector positions
};

