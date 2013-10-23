#include "Physics.h"


namespace Logic
{
	void Object3D::transformToCyl()
	{
		int borderMaxX = Logic::Physics::getInstance()->getBorderX();

		//Set appropriate rotation
		rotation.y = (-position.x / borderMaxX) * 2 * (float)PI - (float)(PI / 2); 

		//Save away pad position and modify the real one for the matrix
		Vec3 temp = position;
		position = Logic::Physics::getInstance()->from2DToCylinder(position, (float)Physics::getInstance()->getCylRadius(), Vec3((float)borderMaxX / 2, 0, 0)); //Cylcenter is right now (mapwidth/2, 0, 0);

		//Update world matrix
		updateWorld();

		//Reset to original value
		position = temp;
	}

}