#include "Pad.h"
#include "GraphicsDX11.h"
#include "Physics.h"

namespace Logic
{
	Vec3 Pad::posKey = Vec3(0, 0, 0);
	Vec3 Pad::posMouse = Vec3(0, 0, 0);
	Vec3 Pad::rotMouse = Vec3(0, 0, 0);
	Vec3 Pad::rotKey = Vec3(0, 0, 0);
	bool Pad::releaseBall = false;
	Pad::Pad()
	{

		position	= Vec3(0, 0, 0);
		prevPos		= Vec3(0, 0, 0);
		rotation	= Vec3(0, 0, 0);
		scale		= Vec3(2.0f, 5.0f, 2.0f);
		movementSpeed = 1.0f;
		angle2D = 0.0f;
		angle3D = 0.0f;
		radius = 3.09544396f;
		
		width = radius * scale.y;

		rotation = rotMouse;

		rotationAxis(orientation, Vec3(0, 0, 1.0f), rotation.z);

		activeEffect = 0;
	}

	Pad::~Pad()
	{

	}

	void Pad::setPosition(Vec3 _pos)
	{
		position = _pos;
		posMouse = _pos;
	}

	void Pad::update(float _dt)
	{
		prevPos = position;
		if(posKey.x != 0)
		{
			posMouse.x = position.x += posKey.x * (float)_dt * movementSpeed;
		}
		else if(posMouse.x != position.x)
		{
			if (posMouse.x > position.x + 1)
				 position.x += 150  * (float)_dt * movementSpeed;
			else if(posMouse.x < position.x - 1)
				 position.x += -150  * (float)_dt * movementSpeed;
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
			direction = Vec3(cos(rotation.z), sin(rotation.z), 0);
			direction.normalize();
		}

		if(position.x > Logic::borderMaxX || position.x < 0)
		{
			position.x = position.x > Logic::borderMaxX ? Logic::borderMaxX : 0.f;
			posMouse.x = posKey.x = position.x;
		}

		if(!releaseBall)
		{
			ballPos = Vec3(0, 10, 0);
			Matrix mRot;
			rotationAxis(mRot, Vec3(0, 0, 1), rotation.z - (float)PI / 2);
			ballPos = mRot * ballPos;
			ballPos += position;
		}

		posKey.x = 0;

		//effect calculations
		if (activeEffect == 1)//stun
		{
			effectTimer -= _dt;
			if (effectTimer < 0)
			{
				movementSpeed += (effectAcceleration * _dt);
				effectAcceleration = effectAcceleration * 1.2f;
			}

			if(movementSpeed > 1.0f)
			{
				movementSpeed = 1.0f;
				effectRotation = 0.4f;
				activeEffect = 0;
			}
		} 
		else if (activeEffect == 2)//slow
		{
			movementSpeed += effectAcceleration;
			effectTimer -= _dt;

			if (movementSpeed <= 0.6f)
				effectAcceleration = 0;
			if (effectTimer < 0)
				effectAcceleration = 0.005f;

			if(movementSpeed > 1.0f)
			{
				movementSpeed = 1.0f;
				activeEffect = 0;
			}
		} 
		else if (activeEffect == 3) //speed
		{
			movementSpeed += effectAcceleration;
			effectTimer -= _dt;

			if (movementSpeed >= 1.3f)
				effectAcceleration = 0;
			if (effectTimer < 0)
				effectAcceleration = -0.005f;

			if(movementSpeed < 1.0f)
			{
				movementSpeed = 1.0f;
				activeEffect = 0;
			}
		} 

	}

	void Pad::startStun()
	{
		if (activeEffect == 0)
		{
			effectTimer			= 0.8f;
			activeEffect		= 1;
			movementSpeed		= 0.3f;
			effectAcceleration	= 0.1f;
			effectRotation		= 0.4f;
		}
	}
	
	void Pad::startSlow()
	{
		if (activeEffect == 0)
		{
			effectTimer			= 5;
			activeEffect		= 2;
			effectAcceleration	= -0.005f;
		}
	}
	
	void Pad::startSpeed()
	{
		if (activeEffect == 0)
		{
			effectTimer			= 5;
			activeEffect		= 3;
			effectAcceleration	= 0.005f;
		}
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