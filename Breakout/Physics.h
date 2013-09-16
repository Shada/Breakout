#ifndef _PHYSICS_H_
#define _PHYSICS_H_


//These are only used temporary, should be screen size and width.
#define MAX_WIDTH 1024
#define MAX_HEIGHT 768

//Temporary, should be a hitbox of sorts for the bricks.
#define LENGTH 70
#define HEIGHT 30

//#include "linearalgebra.h"
//#include "Object.h"
#include "Ball.h"
//#include "Timer.h"

namespace Logic
{
#pragma region Collission

	inline bool Intersects(Ball* _ball, Object3D* _object)
	{
		Vec3 tBallPos = _ball->getPosition();
		Vec3 tObjPos = _object->getPosition();
		float tRadius = _ball->getRadius();

		//Formula: Dist = sqrt( (Ball.x - Object.x)^2 + (Ball.y - Object.y)^2 )
		float tDistance = sqrt( ((tBallPos.x - tObjPos.x)*(tBallPos.x - tObjPos.x)) 
								+ ((tBallPos.y - tObjPos.y)*(tBallPos.y - tObjPos.y)) );

		//If distance is lower than:
		//		ballradius + objectheight/2 AND ballradius + objectlength/2
		// that means they intersect.
		if(tDistance <= tRadius + HEIGHT/2 && tDistance <= tRadius + LENGTH/2)
			return true;
		
		//If position will be withing bounds next frame, assuming same deltaTime
		tBallPos = _ball->getNextFrame();
		tDistance = sqrt( ((tBallPos.x - tObjPos.x)*(tBallPos.x - tObjPos.x)) 
								+ ((tBallPos.y - tObjPos.y)*(tBallPos.y - tObjPos.y)) );

		if(tDistance <= tRadius + HEIGHT/2 && tDistance <= tRadius + LENGTH/2)
			return true; //Might need other type of return to clarify next frame will hit

		return false;
	}

	inline void CalculateCollission(Ball* _ball, Object3D* _object)
	{
		Vec3 tBallPos = _ball->getPosition();
		Vec3 tBallDir = _ball->getDirection();
		Vec3 tObjPos = _object->getPosition();
		float tRadius = _ball->getRadius();

		//Compare X positions
		if(tBallPos.x < tObjPos.x || tBallPos.x > tObjPos.x )
		{
			tBallDir.x *= -1;
		}

		//Compare Y positions
		if(tBallPos.y < tObjPos.y || tBallPos.y < tObjPos.y)
		{
			tBallDir.y *= -1;
		}

		_ball->setDirection(tBallDir.x, tBallDir.y, tBallDir.z);

	}

	inline bool BorderCollide(Ball* _ball)
	{
		Vec3 tBallPos = _ball->getPosition();
		Vec3 tBallDir = _ball->getDirection();

		//Compare X
		if(tBallPos.x < 0 || tBallPos.x > MAX_WIDTH)
		{
			tBallDir.x *= -1;
			_ball->setDirection(tBallDir.x);
			return true;
		}

		//Compare Y
		if(tBallPos.y < 0 || tBallPos.y > MAX_HEIGHT)
		{
			tBallDir.y *= -1;
			_ball->setDirection(NULL, tBallDir.y);
			return true;
		}

		return false;
	}

	/*Check if ball collides with a list of objects. Calculates any collissions. */
	inline void Check2DCollissions(Ball* _ball, Object3D** _listOfObjects, int _nrOfObjects)
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
			if(Intersects(_ball, _listOfObjects[i]) )
			{
				//Ball collides with object, calculate new direction and return.
				CalculateCollission(_ball, _listOfObjects[i]);
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