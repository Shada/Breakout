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

	//Lägger till ett nytt värde i slutet på data
	void addData(Vertex iData);
	//Lägger till den sista vertex facet till Data
	void lastFace();
	//Returnerar Data
	std::vector<Vertex>* getData(){return &Data;}
	//Inverts the objects vector positions
};

