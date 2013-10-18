#include "Object2D.h"


Object2D::Object2D(Vec2 pos, Vec2 scale, float rotation, Vec4 tintAlpha)
{
	this->pos		= pos;
	this->scale		= scale;
	this->rotation	= rotation;
	this->tintAlpha	= tintAlpha;
}

void Object2D::update(double dt)
{
	float progress = 1;

	actionList.at(0).time += dt;
	if(actionList.at(0).time != 0)
		progress = actionList.at(0).timePassed / actionList.at(0).time;

	if(progress >= 1)
	{
		pos			= actionList.at(0).newPos;
		scale		= actionList.at(0).newScale;
		rotation	= actionList.at(0).newRotation;
		tintAlpha	= actionList.at(0).newTintAlpha;
	}
	else
	{
		pos			= oldPos		+ (actionList.at(0).newPos - oldPos) * progress;
		scale		= oldScale		+ (actionList.at(0).newScale - oldScale) * progress;
		rotation	= oldRotation	+ (actionList.at(0).newRotation - oldRotation) * progress;
		tintAlpha	= oldTintAlpha	+ (actionList.at(0).newTintAlpha - oldTintAlpha) * progress;
	}
}

Object2D::~Object2D()
{

}
