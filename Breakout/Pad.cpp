#include "Pad.h"
#include "GraphicsDX11.h"

namespace Logic
{
	Vec3 Pad::posKey = Vec3(0, 0, 0);
	Vec3 Pad::posMouse = Vec3(0, 0, 0);
	Vec3 Pad::rotMouse = Vec3(0, 0, (float)PI / 2);
	Vec3 Pad::rotKey = Vec3(0, 0, (float)PI / 2);
	bool Pad::releaseBall = false;
	Pad::Pad()
	{
		position	= Vec3(0, 0, 0);
		rotation	= Vec3(0, 0, 0);
		scale		= Vec3(2.0f, 5.0f, 2.0f);
		movementSpeed = 1.0f;
		angle2D = 0.0f;
		angle3D = 0.0f;
		radius = 3.09544396f;

		width = radius * scale.y;

		rotation = rotMouse;
		rotationAxis(orientation, Vec3(0, 0, 1.0f), rotation.z);

#ifdef _WIN32
		shaderTechniqueID = GraphicsDX11::getInstance()->getTechIDByName("techSimple");
#endif
	}

	Pad::~Pad()
	{

	}

	void Pad::update(double _dt)
	{
		if(posMouse.x != position.x)
		{
			position.x = posMouse.x;
			posKey.x = 0;
		}
		else if(posKey.x != 0)
		{
			posMouse.x = position.x += posKey.x * (float)_dt * movementSpeed;
		}

		if(rotation.z != rotMouse.z || rotation.z != rotKey.z)
		{
			if(rotation.z != rotMouse.z)
			{
				rotation.z = rotMouse.z;
				rotKey.z = rotation.z;
			}
			else
			{
				rotation.z = rotKey.z;
				rotMouse.z = rotation.z;
			}

			rotationAxis(orientation, Vec3(0, 0, 1), rotation.z);
		}

		if(releaseBall)
		{
			//direction = Vec3(1, 1, 0);
			direction = Vec3(cos(rotation.z), sin(rotation.z), 0);
			direction.normalize();
			/*Matrix mRot;
			rotationAxis(mRot, Vec3(0, 0, 1), rotation.z - PI / 2);
			direction = Vec3(1, 1, 0);
			direction = mRot * direction;
			direction.normalize();*/
			
		}
		
		
		if(!releaseBall)
		{
			ballPos = Vec3(0, 10, 0);
			Matrix mRot;
			rotationAxis(mRot, Vec3(0, 0, 1), rotation.z - (float)PI / 2);
			ballPos = mRot * ballPos;
			ballPos += position;
		}

		if(position.x > 200 || position.x < 0)
		{
			position.x = position.x > 200.0f ? 200.0f : 0.0f;
			posMouse.x = posKey.x = position.x;
		}

		posKey.x = 0;
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
		rotMouse.z += (float)(12 * PI / 180) * _direction;
		if(rotMouse.z > 2 * PI)
			rotMouse.z -= (float)(2 * PI);
	}
	
	void Pad::rotateRight()
	{
		// Theoretically, the mouse wheel cannot be rotated more than 1 tick during 1 frame
		// This means that the input will always be 120 from delta z, which in our program will mean 12 degrees
		rotKey.z += (float)(2 * PI / 180);
		if(rotKey.z > 2 * PI)
			rotKey.z -= (float)(2 * PI);
	}

	void Pad::rotateLeft()
	{
		// Theoretically, the mouse wheel cannot be rotated more than 1 tick during 1 frame
		// This means that the input will always be 120 from delta z, which in our program will mean 12 degrees
		rotKey.z -= (float)(2 * PI / 180);
		if(rotKey.z > 2 * PI)
			rotKey.z -= (float)(2 * PI);
	}

	void Pad::ejectBall()
	{
		releaseBall = true;
	}
}