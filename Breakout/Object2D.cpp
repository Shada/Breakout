#include "Object2D.h"


Object2D::Object2D(Vec2 pos, Vec2 scale, float rotation, Vec4 tintAlpha)
{
	this->pos		= oldPos		= pos;
	this->scale		= oldScale		= scale;
	this->rotation	= oldRotation	= rotation;
	this->tintAlpha	= oldTintAlpha	= tintAlpha;
}

void Object2D::update(double dt)
{
	if(actionList.size() > 0)
	{
		linProgress = 1;
		actionList.front().timePassed += dt;
		if(actionList.front().time > 0)
		{
			linProgress = actionList.front().timePassed / actionList.front().time;
			switch(actionList.front().expFactor)
			{
			case 2: //exponential speed
				{
					progress = pow( linProgress, 2 );
					break;
				}
			case 3: //soft (smooth, but more expensive than linear)
				{
					progress = 3*pow( linProgress, 2 ) - 2*pow( linProgress ,3);
					break;
				}
			case 5: //more soft (most smooth, most expensive)
				{
					progress = 6*pow( linProgress, 5 ) - 15*pow( linProgress, 4 ) + 10*pow( linProgress, 3 );
					break;
				}
			default: //linear (cheapest, but not smooth at all)
				{
					progress = linProgress;
					break;
				}
			}
			
		}
		if(linProgress >= 1)
		{
			pos			= oldPos		= actionList.front().newPos;
			scale		= oldScale		= actionList.front().newScale;
			rotation	= oldRotation	= actionList.front().newRotation;
			tintAlpha	= oldTintAlpha	= actionList.front().newTintAlpha;
			actionList.pop_front();
		}
		else
		{
			pos			= oldPos		+ (actionList.front().newPos - oldPos) * progress;
			scale		= oldScale		+ (actionList.front().newScale - oldScale) * progress;
			rotation	= oldRotation	+ (actionList.front().newRotation - oldRotation) * progress;
			tintAlpha	= oldTintAlpha	+ (actionList.front().newTintAlpha - oldTintAlpha) * progress;
		}
	}
}

Object2D::~Object2D()
{

}
