#ifndef _MAP_H_
#define _MAP_H_

#include <vector>
#include "Ball.h"
#include "Pad.h"
#include "Brick.h"
#include "FreeImage.h"
#include "LoadHandler.h"

namespace Logic
{
	class Map
	{
	public:
		Map();
		~Map();
	void Map::loadMap(unsigned int _mapID,std::vector<Object3D*> *_bricks,Ball *_ball,Pad *_pad);

		//TODO:
		//funktion f�r inladdning av bana
	private:
		//TODO:
		//f�rvaring av bana.
		//etc

	};
}

#endif