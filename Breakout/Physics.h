#ifndef _PHYSICS_H_
#define _PHYSICS_H_


//These are only used temporary, should be screen size and width.
#define MAX_WIDTH 1024
#define MAX_HEIGHT 768

//Temporary, should be a hitbox of sorts for the bricks.
#define LENGTH 70
#define HEIGHT 30

//#include "linearalgebra.h"
#include "Object.h"
#include "Timer.h"

namespace Logic
{
#pragma region Collission

	bool Intersects(Ball* _ball, Object3D* _object)
	{
		//Formula: Dist = sqrt( (Ball.x - Object.x)^2 + (Ball.y - Object.y)^2 )
		Vec3 tDistance = sqrt( ((_ball->position.x - _object->position.x)*(_ball->position.x - _object->position.x)) 
								+ ((_ball->position.y - _object->position.y)*(_ball->position.y - _object->position.y)) );

		//If distance is lower than:
		//		ballradius + objectheight/2 AND ballradius + objectlength/2
		// that means they intersect.
		if(tDistance <= _ball->radius + HEIGHT/2 && tDistance <= _ball->radius + LENGTH/2)
			return true;
		
		//If position will be withing bounds next frame, assuming same deltaTime
		//return true;

		return false;
	}

	void CalculateCollission(Ball* _ball, Object3D* _object)
	{
		//Compare X positions
		if(_ball->position.x < _object->position.x || _ball->position.x > _object->position.x )
		{
			_ball->direction.x *= -1;
		}


		//Compare Y positions
		if(_ball->position.y < _object->position.y || _ball->position.y < _object->position.y)
		{
			_ball->direction.y *= -1;
		}

	}

	bool BorderCollide(Ball* _ball)
	{
		//Compare X
		if(_ball->position.x < 0 || _ball->position.x > MAX_WIDTH)
		{
			_ball->direction.x *= -1;
			return true;
		}

		//Compare Y
		if(_ball->position.y < 0 || _ball->position.y > MAX_HEIGHT)
		{
			_ball->direction.y *= -1;
			return true;
		}

		return false;
	}

	/*Check if ball collides with a list of objects. Calculates any collissions. */
	void Check2DCollissions(Ball* _ball, Object3D* _listOfObjects, int _nrOfObjects)
	{
		//Function could be bool-based if we want effects when colliding. 
		// Should probably return false on bordercollide then.

		if(BorderCollide(_ball))
		{
			//Ball collides with border, calculate new direction and return.
			return;
		}

		for(int i = 0; i < _nrOfObjects; i++)
		{
			if(Intersects(_ball, Object[i])
			{
				//Ball collides with object, calculate new direction and return.
				CalculateCollission(_ball, Object[i]);
				//"Attack" Object[i]. Destroy? Damage? AoEAttack? Whatever, do it here.
				return;
			}
				
		}
	}
#pragma endregion

#pragma region Gravitation
	//TODO
#pragma endregion
}

#endif