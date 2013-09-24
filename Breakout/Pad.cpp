#include "Pad.h"
#include "Resource.h"
#include "GraphicsDX11.h"

namespace Logic
{
	Vec3 Pad::posKey = Vec3(0, 0, 0);
	Vec3 Pad::posMouse = Vec3(0, 0, 0);
	Vec3 Pad::rot = Vec3(0, 0, PI / 2);
	Pad::Pad()
	{
		movementSpeed = 1.0f;
		angle2D = 0.0f;
		angle3D = 0.0f;
		radius = 3.09544396f;

		scale = Vec3(2, 5, 2);

		boxMax = Vec3(radius * scale.y, radius / 2, 1);
		boxMin = Vec3(-radius * scale.y, radius / 2 - 1, 1);

		rotation = rot;
		rotationAxis(orientation, Vec3(0, 0, 1), rotation.z);

#ifdef _WIN32
		shaderTechniqueID = GraphicsDX11::getInstance()->getTechIDByName("techSimple");
#endif
	}

	Pad::~Pad()
	{

	}

	void Pad::update(double _dt)
	{
		//Calculate on buffs and debuffs

		if(posMouse.x != position.x)
			position.x = posMouse.x;
		else if(posKey.x != 0)
		{
			posMouse.x = position.x += posKey.x * _dt * movementSpeed;
		}
		//position.x = pos.x;
		if(rotation.z != rot.z)
		{
			rotation.z = rot.z;
			rotationAxis(orientation, Vec3(0, 0, 1), rotation.z);
		}

		if(position.x > 200 || position.x < 0)
			position.x = position.x > 200 ? 200 : 0;

		posKey.x = 0;
	}

	void Pad::draw()
	{
		CBWorld cb;
		cb.world = scalingMatrix(scale) * orientation * translationMatrix(position);
#ifdef _WIN32
		GraphicsDX11::getInstance()->useTechnique(shaderTechniqueID);
		GraphicsDX11::getInstance()->updateCBWorld(cb);
#endif // _WIN32
		Resources::LoadHandler::getInstance()->getModel(modelID)->draw();
	}

	void Pad::move2D(double _dt, float _x)
	{
		position.x += _x * movementSpeed * (float)_dt;
	}

	void Pad::move3D(double _dt, float _x)
	{
		//Do 3D movement (in a circle) here.
		angle3D += _x * movementSpeed * (float)_dt; //Assume degrees

		if(angle3D < 0.0f) angle3D += 360.0f;
		else if(angle3D >= 360.0f) angle3D -= 360.0f;
		
		position.x = cosf(angle3D); //Some sort of radius on the circle should be added.
		position.y = sinf(angle3D);
	}

	void Pad::rotate(int _direction)
	{
		// Theoretically, the mouse wheel cannot be rotated more than 1 tick during 1 frame
		// This means that the input will always be 120 from delta z, which in our program will mean 12 degrees
		rot.z += (float)(12 * PI / 180) * _direction;
		if(rot.z > 2 * PI)
			rot.z -= (float)(2 * PI);
	}
	
	void Pad::rotateRight()
	{
		// Theoretically, the mouse wheel cannot be rotated more than 1 tick during 1 frame
		// This means that the input will always be 120 from delta z, which in our program will mean 12 degrees
		rot.z += (float)(2 * PI / 180);
		if(rot.z > 2 * PI)
			rot.z -= (float)(2 * PI);
	}

	void Pad::rotateLeft()
	{
		// Theoretically, the mouse wheel cannot be rotated more than 1 tick during 1 frame
		// This means that the input will always be 120 from delta z, which in our program will mean 12 degrees
		rot.z -= (float)(2 * PI / 180);
		if(rot.z > 2 * PI)
			rot.z -= (float)(2 * PI);
	}
}
