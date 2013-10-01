#ifndef _PHYSICS_H_
#define _PHYSICS_H_


//These are only used temporary, should be screen size and width.
#define MAX_WIDTH 210
#define MAX_HEIGHT 150

//Temporary, should be a hitbox of sorts for the bricks.
#define LENGTH 5.03898811
#define HEIGHT 5.03898811

//#include "linearalgebra.h"
//#include "Object.h"
#include "Ball.h"
#include <vector>
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
		if(tBallPos.x + LENGTH/2 < tObjPos.x || tBallPos.x - LENGTH/2 > tObjPos.x )
			tBallDir.x *= -1;

		//Compare Y positions
		if(tBallPos.y + HEIGHT/2 < tObjPos.y || tBallPos.y - HEIGHT/2 > tObjPos.y)
			tBallDir.y *= -1;
#ifdef _WIN32
		_ball->setDirection(tBallDir.x, tBallDir.y);
#else
		_ball->setDirection(tBallDir.x, tBallDir.y, NULL);
#endif
	}

	inline bool BorderCollide(Ball* _ball)
	{
		Vec3 tBallPos = _ball->getPosition();
		Vec3 tBallDir = _ball->getDirection();
		float tRadius = _ball->getRadius();

		bool collides = false;

		//Compare X
		if(tBallPos.x - tRadius < 0 || tBallPos.x + tRadius > MAX_WIDTH)
		{
			if((tBallPos.x - tRadius < 0 && tBallDir.x < 0) || (tBallPos.x + tRadius > MAX_HEIGHT && tBallDir.x > 0))
				tBallDir.x *= -1;
#ifdef _WIN32
			_ball->setDirection(tBallDir.x);
#else
			_ball->setDirection(tBallDir.x, tBallDir.y, NULL);
#endif
			collides = true;
		}

		//Compare Y
		if(tBallPos.y - tRadius < 0 || tBallPos.y + tRadius > MAX_HEIGHT)
		{
			if((tBallPos.y - tRadius < 0 && tBallDir.y < 0) || (tBallPos.y + tRadius > MAX_HEIGHT && tBallDir.y > 0))
				tBallDir.y *= -1;
			_ball->setDirection(NULL, tBallDir.y, NULL);
			collides = true;
		}

		return collides;
	}

	/*Check if ball collides with a list of objects. Calculates any collissions. */
	inline int Check2DCollissions(Ball* _ball, std::vector<Object3D*> _listOfObjects)
	{
		//Function could be bool-based if we want effects when colliding.
		// Should probably return false on bordercollide then.

		if(BorderCollide(_ball))
		{
			//Ball collides with border, calculate new direction and return.
			return -1;
		}

		for(unsigned int i = 0; i < _listOfObjects.size(); i++)
		{
			if(Intersects(_ball, _listOfObjects[i]))
			{
				//Ball collides with object, calculate new direction and return.
				CalculateCollission(_ball, _listOfObjects[i]);
				//"Attack" Object[i]. Destroy? Damage? AoEAttack? Whatever, do it here.
				return i;
			}

		}
		return -1;
	}

	inline bool ballCollision(Ball *_ball, Pad *_pad, float currentRotation)
	{
		Vec3 tBallPos = _ball->getNextFrame();
		Vec3 ballDir = _ball->getDirection();
		Vec3 tObjPos = _pad->getPosition();
		Vec3 prevPadPos = _pad->getPrevPos();
		float tRadius = _ball->getRadius();

		Vec3 padScale = _pad->getScale() * _pad->getRadius();

		float zrot = _pad->getOrientation();
		Vec3 p1 = Vec3(-padScale.y, 0, 0), p2 = Vec3(padScale.y, 0, 0);
		
		if(min(tObjPos.x, prevPadPos.x) < tBallPos.x && max(tObjPos.x, prevPadPos.x) > tBallPos.x)
			tObjPos.x = tBallPos.x;

		//rotate p1 and p2
		Matrix rot; rotationAxis(rot, Vec3(0, 0, 1), zrot);
		p1 = rot * p1;
		p2 = rot * p2;

		p1 += tObjPos; p2 += tObjPos;
		p1.y += padScale.x / 2; p2.y += padScale.x / 2;

		if(max(p1.x, p2.x) - min(p1.x, p2.x) < tRadius * 5)
		{
			bool collide = false;
			float dx = (max(p1.x, p2.x) - min(p1.x, p2.x)) / 10, x = min(p1.x, p2.x);
			float dy = (max(p1.y, p2.y) - min(p1.y, p2.y)) / 10, y = min(p1.y, p2.y);
			for(int c = 0; c < 11 && !collide; c++)
			{
				collide = (x - tBallPos.x) * (x - tBallPos.x) + (y - tBallPos.y) * (y - tBallPos.y) <= tRadius * tRadius;
				x += dx;
				y += dy;
			}

			if(collide)
			{
				Vec3 padRot = Vec3((float)cos(zrot + PI / 2), (float)sin(zrot + PI /2), 0);
				Vec3 newDir = planeReflection(_ball->getDirection(), padRot);
				newDir.normalize();
				_ball->setDirection(newDir.x, newDir.y, 0);
				return true;
			}
		}
		else
		{
			if(tBallPos.x + tRadius * ballDir.x > min(p1.x, p2.x) && tBallPos.x - tRadius * ballDir.x < max(p1.x, p2.x))
			{
				float ratio = (p1.x - tBallPos.x) / (p1.x - p2.x);
				float yIntersect = min(p1.y, p2.y) + (max(p1.y, p2.y) - min(p1.y, p2.y)) * (p2.y < p1.y ? 1 - ratio : ratio);
			
				if(tBallPos.y - tRadius <= yIntersect)
				{
					Vec3 padRot = Vec3(cos(zrot + (float)(PI / 2)), sin(zrot + (float)(PI / 2)), 0);
					Vec3 newDir = planeReflection(_ball->getDirection(), padRot);
					newDir.normalize();
					_ball->setDirection(newDir.x, newDir.y, 0);
					return true;
				}
			}
		}
		// collision ball vs ball
		return false;

	}

#pragma endregion

#pragma region Gravitation
	//TODO
#pragma endregion

	/* Calculates where the frustum has its border points at a given distance.
		Doesn't return anything at this point, check numbers with breakpoints. */
	inline void calculateCameraBorders(Vec3 _camPos, float _distance, float _aspectRatio)
	{
		/* All vectors need to be normalized. */
		Vec3 up = Vec3(0,1,0);
		Vec3 right = Vec3(1,0,0);
		Vec3 view = Vec3(0,0,1);

		float fov = (float)(PI / 2); // 90 degrees in radians.

		float height = 2 * tan(fov / 2) * _distance;
		float width = height * _aspectRatio;
		Vec3 center = _camPos + view * _distance;

		Vec3 topLeft		= center + (up * (height/2)) - (right * (width/2));
		Vec3 topRight		= center + (up * (height/2)) + (right * (width/2));
		Vec3 bottomLeft		= center - (up * (height/2)) - (right * (width/2));
		Vec3 bottomRight	= center - (up * (height/2)) + (right * (width/2));		
	}

	inline Vec3 calculateCenter(Vec3 _topLeft, Vec3 _topRight, Vec3 _bottomLeft, Vec3 _bottomRight)
	{
		Vec3 center;
		
		center.x = (_topLeft.x + _topRight.x + _bottomLeft.x + _bottomRight.x) * 0.25f;
		center.y = (_topLeft.y + _topRight.y + _bottomLeft.y + _bottomRight.y) * 0.25f;
		center.z = (_topLeft.z + _topRight.z + _bottomLeft.z + _bottomRight.z) * 0.25f;

		return center;
	}

	/* This assumes you get a flat, rectangular plane. */
	inline Vec3 fitToScreen(Vec3 _targetTopLeft, Vec3 _targetTopRight, Vec3 _targetBottomLeft, Vec3 _targetBottomRight)
	{
		Vec3 pCenter = calculateCenter(_targetTopLeft, _targetTopRight, _targetBottomLeft, _targetBottomRight);

		//Since the triangle will be a right triangle, the distance should be equal to half the height of the picture.
		float distance = fabs(_targetTopRight.y - _targetBottomRight.y) * 0.5f;

		Vec3 planeNormal = cross((_targetTopLeft - _targetTopRight), (_targetBottomLeft - _targetTopRight));
		planeNormal = normalize(planeNormal);

		Vec3 pos = pCenter - (planeNormal * distance);
		
		return pos;
	}


}

#endif // ! _PHYSICS_H_
