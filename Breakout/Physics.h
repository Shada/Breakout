#ifndef _PHYSICS_H_
#define _PHYSICS_H_

//#include "linearalgebra.h"
//#include "Object.h"
#include "Ball.h"
#include <vector>
//#include "Timer.h"
#include "ObjectCore.h"

namespace Logic
{
#pragma region Collission
	static int borderMaxX = 300;
	static int borderMaxY = 200;


	inline bool Intersects(Ball* _ball, Brick* _brick)
	{
		Vec3 tBallPos = _ball->getPosition();
		Vec3 tBrickPos = _brick->getPosition();
		float tRadius = _ball->getRadius();

		//Formula: Dist = sqrt( (Ball.x - Object.x)^2 + (Ball.y - Object.y)^2 )
		float tDistance = sqrt( ((tBallPos.x - tBrickPos.x)*(tBallPos.x - tBrickPos.x))
								+ ((tBallPos.y - tBrickPos.y)*(tBallPos.y - tBrickPos.y)) );

		//If distance is lower than:
		//		ballradius + objectheight/2 AND ballradius + objectlength/2
		// that means they intersect.

		if(tDistance <= tRadius + _brick->getHeight()/2 && tDistance <= tRadius + _brick->getHeight()/2)
			return true;

		//If position will be withing bounds next frame, assuming same deltaTime
		tBallPos = _ball->getPosition();
		tDistance = sqrt( ((tBallPos.x - tBrickPos.x)*(tBallPos.x - tBrickPos.x))
								+ ((tBallPos.y - tBrickPos.y)*(tBallPos.y - tBrickPos.y)) );

		if(tDistance <= tRadius + _brick->getHeight()/2 && tDistance <= tRadius + _brick->getHeight()/2)
			return true; //Might need other type of return to clarify next frame will hit

		if(tDistance <= tRadius + (float)_brick->getHeight()/2 && tDistance <= tRadius + (float)_brick->getWidth()/2)
			return true;

		return false;
	}

	inline void edgeCollision(Ball *_ball, Brick* _object)
	{
		Vec3 ballPos = _ball->getPosition();
		Vec3 lastBallPos = _ball->getLastFrame();
		Vec3 objPos = _object->getPosition();
		float radius = _ball->getRadius();
		
		float LENGTH = 15, HEIGHT = 7.5f;

		float width = _object->getWidth();
		float height = _object->getHeight();

		if(abs(objPos.x + width / 2 - ballPos.x) < abs(objPos.x - width / 2 - ballPos.x))
			objPos.x += width / 2;
		else
			objPos.x -= width / 2;

		if(abs(objPos.y + height / 2 - ballPos.y) < abs(objPos.y - height / 2 - ballPos.y))
			objPos.y -= height / 2;
		else
			objPos.y += height / 2;

		Vec3 dir = _ball->getDirection();

		float speed = _ball->getSpeed();
		/*float a = speed * dir.x * speed * dir.x + speed * dir.y * speed * dir.y;
		float b = 2 * (dir.x * speed * (lastBallPos.x - objPos.x));
		float c = (lastBallPos.x - objPos.x) * (lastBallPos.x - objPos.x) + (lastBallPos.y - objPos.y) * (lastBallPos.y - objPos.y);

		float d = 4 * a * (c - radius * radius);
		float delta = abs(_min(-b + sqrt(abs(2 * b * b - d)), -b - sqrt(abs(2 * b * b - d))) / (2 * a));*/
		float bx = (ballPos.x - lastBallPos.x) / (dir.x * speed);
		float by = (ballPos.y - lastBallPos.y) / (dir.y * speed);
		float t = sqrt(bx * bx + by * by);

		float dx = speed * dir.x;
		float dy = speed * dir.y;

		float a = dx * dx + dy * dy;
		float b = 2 * (lastBallPos.x - objPos.x) * dx + 2 * (lastBallPos.y - objPos.y) * dy;
		float c = (lastBallPos.x - objPos.x) * (lastBallPos.x - objPos.x) + (lastBallPos.y - objPos.y) * (lastBallPos.y - objPos.y) - radius * radius;
		float d = b * b - 4 * a * c;

		//float t1 = (-b - sqrt(abs(d))) / (2 * a), t2 = (-b + sqrt(abs(d))) / (2 * a);
		float delta = (-b - sqrt(abs(d))) / (2 * a);

		while(delta > t)
			delta -= t;
		
		Vec3 collidePos = lastBallPos + dir * speed * delta;

		Matrix mRot;
		rotationAxis(mRot, normalize(-objPos + collidePos), acos(normalize(-objPos + collidePos).dot(dir)));
		Vec3 _newDir = mRot * normalize(-objPos + collidePos);
		_newDir.z = 0;
		_newDir.normalize();


		// Don't want the new direction to be at an angle that is too low
		if(abs(_newDir.x) > 0.9 || abs(_newDir.y) > 0.9)
		{
			abs(_newDir.x) > 0.9 ? _newDir.y += _newDir.y > 0 ? 0.25f : - 0.25f : _newDir.x += _newDir.y > 0 ? 0.25f : -0.25f;
			_newDir.normalize();
		}

		_ball->setDirection(_newDir.x, _newDir.y, NULL);
		_ball->setPosition(Vec3(collidePos.x + (t - delta) * _newDir.x, collidePos.y + (t - delta) * _newDir.y, 0));
	}


	inline void CalculateCollission(Ball* _ball, Brick* _brick)
	{

		Vec3 ballPos = _ball->getPosition();
		Vec3 ballDir = _ball->getDirection();
		Vec3 objPos = _brick->getPosition();
		float radius = _ball->getRadius();
		
		bool alreadyCollided = false;

		float width = _brick->getWidth();
		float height = _brick->getHeight();

		//Compare X positions
		if(ballPos.x + width/2 < objPos.x || ballPos.x - width/2 > objPos.x)
		{
			ballDir.x *= -1;
			alreadyCollided = true;
		}

		//Compare Y positions
		if(ballPos.y + height/2 < objPos.y || ballPos.y - height/2 > objPos.y)
		{
			if(alreadyCollided)
			{
				ballDir.x *= -1;
				edgeCollision(_ball, _brick);
				return;
			}

			ballDir.y *= -1;
		}

		if(ballDir.x != _ball->getDirection().x || ballDir.y != _ball->getDirection().y)
		{
			Vec3 lastBallPos = _ball->getLastFrame();
			Vec3 deltaPos = ballPos - lastBallPos;

			float collidePos, ratio;
			if(ballDir.x != _ball->getDirection().x)
			{
				collidePos = ballPos.x < objPos.x ? objPos.x - radius - width / 2 : objPos.x + radius + width/ 2;
				ratio = (ballPos.x - collidePos) / (ballPos.x - lastBallPos.x);
				_ball->setPosition(Vec3(collidePos - deltaPos.x * ratio, ballPos.y, ballPos.z));
			}
			else
			{
				collidePos = ballPos.y < objPos.y ? objPos.y - radius - height/2 : objPos.y + radius + height/ 2;
				ratio = (ballPos.y - collidePos) / (ballPos.y - lastBallPos.y);
				_ball->setPosition(Vec3(ballPos.x, collidePos - deltaPos.y * ratio, ballPos.z));
			}
		}

		_ball->setDirection(ballDir.x, ballDir.y, NULL);

	}

	inline bool BorderCollide(Ball* _ball, bool _isCylinder)
	{
		Vec3 ballPos = _ball->getPosition();
		Vec3 tBallDir = _ball->getDirection();
		float tRadius = _ball->getRadius();

		bool collides = false;

		//Compare X

		if(!_isCylinder && ballPos.x - tRadius < 0 || !_isCylinder && ballPos.x + tRadius > borderMaxX)
		{
			if((ballPos.x - tRadius < 0 && tBallDir.x < 0) || (ballPos.x + tRadius > borderMaxX && tBallDir.x > 0))
				tBallDir.x *= -1;

			Vec3 newPos = Vec3(ballPos.x - tRadius < 0 ? tRadius : borderMaxX - tRadius, ballPos.y, ballPos.z);
			_ball->setPosition(newPos);
			_ball->setDirection(tBallDir.x, tBallDir.y, NULL);

			collides = true;
		}

		//Compare Y
		if(ballPos.y + tRadius > borderMaxY)
		{
			if((ballPos.y - tRadius < 0 && tBallDir.y < 0) || (ballPos.y + tRadius > borderMaxY && tBallDir.y > 0))
				tBallDir.y *= -1;

			Vec3 newPos = Vec3(ballPos.x, ballPos.y - tRadius < 0 ? tRadius : borderMaxY - tRadius, ballPos.z);
			_ball->setPosition(newPos);
			_ball->setDirection(NULL, tBallDir.y, NULL);

			collides = true;
		}

		return collides;
	}

	/*Check if ball collides with a list of objects. Calculates any collissions. */
	inline int Check2DCollissions(Ball* _ball, std::vector<Brick*> _listOfBricks, bool _isCylinder)
	{
		//Function could be bool-based if we want effects when colliding.
		// Should probably return false on bordercollide then.

		if(BorderCollide(_ball, _isCylinder))
		{
			//Ball collides with border, calculate new direction and return.
			return -1;
		}

		for(unsigned int i = 0; i < _listOfBricks.size(); i++)
		{
			if(Intersects(_ball, _listOfBricks[i]))
			{
				//Ball collides with object, calculate new direction and return.
				CalculateCollission(_ball, _listOfBricks[i]);
				//"Attack" Object[i]. Destroy? Damage? AoEAttack? Whatever, do it here.
				return i;
			}

		}
		return -1;
	}

	inline bool ballCollision(Ball *_ball, Pad *_pad, float currentRotation)
	{
		Vec3 ballPos = _ball->getPosition();
		Vec3 ballDir = _ball->getDirection();
		Vec3 padPos = _pad->getPosition();
		Vec3 prevPadPos = _pad->getPrevPos();
		Vec3 lastBallPos = _ball->getLastFrame();
		float radius = _ball->getRadius();
		float speed = _ball->getSpeed();

		Vec3 padScale = _pad->getScale() * _pad->getRadius();

		float zrot = _pad->getOrientation();
		Vec3 p1 = Vec3(-padScale.y, 0, 0), p2 = Vec3(padScale.y, 0, 0);
		

		if(_min(padPos.x, prevPadPos.x) < ballPos.x && _max(padPos.x, prevPadPos.x) > ballPos.x)
			padPos.x = padPos.x > ballPos.x ? ballPos.x - radius / 10 : ballPos.x + radius / 10;

		//rotate p1 and p2
		Matrix rot; rotationAxis(rot, Vec3(0, 0, 1), zrot);
		p1 = rot * p1;
		p2 = rot * p2;

		p1 += padPos; p2 += padPos;
		p1.y += padScale.x / 2; p2.y += padScale.x / 2;

		float bx = (ballPos.x - lastBallPos.x) / (ballDir.x * speed);
		float by = (ballPos.y - lastBallPos.y) / (ballDir.y * speed);
		float t = sqrt(bx * bx + by * by);
		float px = padPos.x - prevPadPos.x;

		if(ballPos.x + radius * ballDir.x > _min(p1.x, p2.x) && ballPos.x - radius * ballDir.x < _max(p1.x, p2.x))
		{
			float ratio = (p1.x - ballPos.x) / (p1.x - p2.x);
			float yIntersect = _min(p1.y, p2.y) + (_max(p1.y, p2.y) - _min(p1.y, p2.y)) * (p2.y < p1.y ? 1 - ratio : ratio);
			
			if(ballPos.y - radius <= yIntersect && ballPos.y - radius >= yIntersect - 5)
			{
				float collidePosY = yIntersect + radius;
				float length = abs(collidePosY - ballPos.y) / cos(ballDir.x);
				Vec3 lastBallPos = _ball->getLastFrame();
				Vec3 outPos = Vec3(lastBallPos.x + ballDir.x * ((lastBallPos - ballPos).length() - length), collidePosY, 0);

				Vec3 padRot = Vec3(cos(zrot + (float)(PI / 2)), sin(zrot + (float)(PI / 2)), 0);
				Vec3 newDir = normalize(planeReflection(_ball->getDirection(), padRot));

				if(ballPos.y - lastBallPos. y > 0 && (padPos.x - prevPadPos.x < 0 && ballPos.x - lastBallPos.x < 0 || padPos.x - prevPadPos.x > 0 && ballPos.x - lastBallPos.x > 0))
				{
					newDir = planeReflection(_ball->getDirection(), Vec3((float)cos(zrot), (float)sin(zrot), 0));
					t *= ratio;
					outPos = Vec3(outPos.x + length * newDir.x, outPos.y - length * newDir.y, 0);
					outPos.x += padPos.x - ballPos.x > 0 ? - newDir.x * t * speed : + newDir.x * t * speed;
					outPos.y -= newDir.y * t * speed;
				}
				else
					outPos = Vec3(outPos.x + length * newDir.x, outPos.y - length * newDir.y, 0);

				_ball->setPosition(outPos);
				_ball->setDirection(newDir.x, newDir.y, 0);
				return true;
			}
		}

		if(_max(p1.x, p2.x) - _min(p1.x, p2.x) < 3 * radius)
		{
			bool collide = false;
			float dx = (_max(p1.x, p2.x) - _min(p1.x, p2.x)) / 10, x = _min(p1.x, p2.x);
			float dy = (_max(p1.y, p2.y) - _min(p1.y, p2.y)) / 10, y = _min(p1.y, p2.y);
			
			for(int c = 0; c < 11 && !collide; c++)
			{
				collide = (x - ballPos.x) * (x - ballPos.x) + (y - ballPos.y) * (y - ballPos.y) <= radius * radius;
				x += dx;
				y += dy;
			}

			if(collide)
			{
				float collidePosY = y - radius;
				float length = abs(collidePosY - ballPos.y) / cos(ballDir.x);
				Vec3 padRot = Vec3((float)cos(zrot + PI / 2), (float)sin(zrot + PI / 2), 0);
				Vec3 newDir = planeReflection(_ball->getDirection(), padRot);

				Vec3 lastBallPos = _ball->getLastFrame();
				Vec3 outPos = Vec3(lastBallPos.x + ballDir.x * ((lastBallPos - ballPos).length() - length), collidePosY, 0);
				newDir.normalize();
				
				if(ballPos.y - lastBallPos. y > 0 && (padPos.x - prevPadPos.x < 0 && ballPos.x - lastBallPos.x < 0 || padPos.x - prevPadPos.x > 0 && ballPos.x - lastBallPos.x > 0))
				{
					newDir = planeReflection(_ball->getDirection(), Vec3((float)cos(zrot), (float)sin(zrot), 0));
					outPos = Vec3(outPos.x + length * newDir.x, outPos.y - length * newDir.y, 0);
					outPos.x += padPos.x - ballPos.x > 0 ? - newDir.x * t * speed : + newDir.x * t * speed;
					outPos.y -= newDir.y * t * speed;
				}
				else
					outPos = Vec3(outPos.x + length * newDir.x, outPos.y - length * newDir.y, 0);
				
				_ball->setPosition(outPos);
				_ball->setDirection(newDir.x, newDir.y, 0);
				return true;
			}
		}
		else
		{
			if(ballPos.x + radius * ballDir.x > _min(p1.x, p2.x) && ballPos.x - radius * ballDir.x < _max(p1.x, p2.x))
			{
				float ratio = (p1.x - ballPos.x) / (p1.x - p2.x);
				float yIntersect = _min(p1.y, p2.y) + (_max(p1.y, p2.y) - _min(p1.y, p2.y)) * (p2.y < p1.y ? 1 - ratio : ratio);
			
				if(ballPos.y - radius <= yIntersect && ballPos.y - radius >= yIntersect - 5)
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

		float distance = fabs(_distance);

		float fov = (float)(PI / 2); // 90 degrees in radians.

		/*float height = 2 * tan(fov / 2) * distance;
		float width = height * _aspectRatio;*/
		float width = 2 * tan(fov / 2) * distance * _aspectRatio;
		float height = width / _aspectRatio;
		Vec3 center = _camPos + view * distance;

		Vec3 topLeft		= center + (up * (height/2)) - (right * (width/2));
		Vec3 topRight		= center + (up * (height/2)) + (right * (width/2));
		Vec3 bottomLeft		= center - (up * (height/2)) - (right * (width/2));
		Vec3 bottomRight	= center - (up * (height/2)) + (right * (width/2));

		//borderMaxX = (int)topRight.x;
		//borderMaxY = (int)topRight.y;
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
	inline Vec3 fitToScreen(Vec3 _targetTopLeft, Vec3 _targetTopRight, Vec3 _targetBottomLeft, Vec3 _targetBottomRight, float _aspectRatio = (4.f/3.f))
	{
		Vec3 pCenter = calculateCenter(_targetTopLeft, _targetTopRight, _targetBottomLeft, _targetBottomRight);

		//Since the triangle will be a right triangle, the distance should be equal to half the height of the picture.
		float distance = fabs(_targetTopLeft.x - _targetBottomRight.x) * 0.5f;
		distance /= _aspectRatio;

		Vec3 planeNormal = cross((_targetTopLeft - _targetTopRight), (_targetBottomLeft - _targetTopRight));
		planeNormal = normalize(planeNormal);

		Vec3 pos = pCenter - (planeNormal * distance);
		
		return pos;
	}

#pragma region Coordinate Transforms

	/* Converts a position from 2D to a fitting Cylinder position. 
		Default assumes the Cylinder is centered at (0,0,0). */
	inline Vec3 from2DToCylinder(Vec3 _pos , float _radius, Vec3 _cylCenter = Vec3(0,0,0))
	{
		Vec3 result;

		float diff = _pos.x/(float)borderMaxX;
		float par = diff * 2 * PI;
		float sin = sinf(par);
		float cos = cosf(par);
		
		result.x = _cylCenter.x + _radius * cosf( diff * 2 * (float)PI);
		result.y = _cylCenter.y + _pos.y;
		result.z = _cylCenter.z + _radius * sinf( diff * 2 * (float)PI);

		return result;
	}

	inline Vec3 from2DToCylinder(float _angle , float _radius, Vec3 _cylCenter = Vec3(0,0,0))
	{
		Vec3 result;
		
		result.x = _cylCenter.x + _radius * sinf( _angle);
		result.y = _cylCenter.y;
		result.z = _cylCenter.z + _radius * cosf( _angle);

		return result;
	}

	/* Transform a cartesian (X,Y,Z) coordinate to a spherical (Theta, Phi, R).
		x = Azimuth Theta, y = elevation Phi and z = radius R. */
	inline Vec3 cart2Sph(Vec3 _pos)
	{
		Vec3 result;

		double temp = sqrt(_pos.x * _pos.x + _pos.y * _pos.y );

		result.x = (float)atan2(temp, _pos.z);											// Theta = atan2(sqrt(x^2 + y^2), z );
		result.y = (float)atan2(_pos.y, _pos.x);										// Phi = atan2(y,x);
		result.z = sqrt(_pos.x * _pos.x + _pos.y * _pos.y + _pos.z * _pos.z);	// R = sqrt(x^2 + y^2 + z^2);

		return result;
	}

	/* Transform a cartesian (X,Y,Z) coordinate to a cylindrical (Theta, y, Rho) 
		x = Theta, z = Rho. */
	inline Vec3 cart2Cyl(Vec3 _pos)
	{
		Vec3 result;

		result.x = atan2(_pos.y, _pos.x);						// Theta = atan2(y,x);
		result.y = _pos.y;										// y = y
		result.z = sqrt(_pos.x * _pos.x + _pos.y * _pos.y );	// Rho = sqrt(x^2 + y^2);
																

		return result;
	}

	/* Transforms a spherical coordinate (Theta, Phi, R) to a cartesian (X, Y, Z). */
	inline Vec3 sph2Cart(Vec3 _pos)
	{
		Vec3 result;

		result.x = _pos.z * sin(_pos.x) * cos(_pos.y);	// x = r * sin(Theta) * cos(Phi)
		result.y = _pos.z * sin(_pos.x) * sin(_pos.y);	// y = r * sin(Theta) * sin(Phi)
		result.z = _pos.z * cos(_pos.x);				// z = r * cos(Theta)

		return result;
	}

	/* Transforms a cylindrical coordinate (Theta, Rho, z) to a cartesian (X, Y, Z). */
	inline Vec3 cyl2Cart(Vec3 _pos)
	{
		Vec3 result;

		result.x = _pos.y * cos(_pos.x);	//x = Rho * cos(Theta)
		result.y = _pos.y * sin(_pos.x);	//y = Rho * sin(Theta)
		result.z = _pos.z;					//z = z

		return result;
	}


#pragma endregion
}

#endif // ! _PHYSICS_H_
