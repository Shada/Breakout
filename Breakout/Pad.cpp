#include "Pad.h"
#include "Resource.h"
#include "GraphicsDX11.h"

namespace Logic
{
	Vec3 Pad::posKey = Vec3(0, 0, 0);
	Vec3 Pad::posMouse = Vec3(0, 0, 0);
	Vec3 Pad::rot = Vec3(0, 0, 0);
	Pad::Pad()
	{
		movementSpeed = 1.0f;
		angle2D = 0.0f;
		angle3D = 0.0f;
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
		rotation.z = rot.z;

		posKey.x = 0;
	}

	void Pad::draw()
	{
		CBWorld cb;
		cb.world = scalingMatrix(Vec3(2, 5, 2)) * yawPitchRoll(rotation) * translationMatrix(position);
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
	
	void Pad::rotateRight()
	{
		// Theoretically, the mouse wheel cannot be rotated more than 1 tick during 1 frame
		// This means that the input will always be 120 from delta z, which in our program will mean 12 degrees
		rot.z += (float)(12 * PI / 180);
		if(rot.z > 2 * PI)
			rot.z -= (float)(2 * PI);
	}

	void Pad::rotateLeft()
	{
		// Theoretically, the mouse wheel cannot be rotated more than 1 tick during 1 frame
		// This means that the input will always be 120 from delta z, which in our program will mean 12 degrees
		rot.z -= (float)(12 * PI / 180);
		if(rot.z > 2 * PI)
			rot.z -= (float)(2 * PI);
	}
}
