#include "Physics.h"


namespace Logic
{
	void Object3D::transformToCyl()
	{
		//Set appropriate rotation
		rotation.y = (-position.x / borderMaxX) * 2 * (float)PI - PI/2; 

		//Save away pad position and modify the real one for the matrix
		Vec3 temp = position;
		position = Logic::from2DToCylinder(position, 100, Vec3(borderMaxX/2, 0, 0)); //Cylcenter is right now (mapwidth/2, 0, 0);

		//Update world matrix
		updateWorld();

		//Reset to original value
		position = temp;
	}

}