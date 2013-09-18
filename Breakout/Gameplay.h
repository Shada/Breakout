#pragma once
#include "Resource.h"
namespace Logic
{
	class Gameplay
	{
	public:
		Gameplay(/*Map*/);
		~Gameplay();


	private:
		std::vector<Vertex>			verticesDynamic;
		std::vector<PerInstance>	verticesPerInstance;

		//Brick bricks[];
		//Pad pad;
		//Ball balls[]
		
		//TODO:
		//All logik för spelandet av en bana
		
		int Update(/*tid*/);


	};

}