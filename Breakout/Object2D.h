#pragma once
#include "Resource.h"
#include <list>

class Object2D
{
private:
	float	progress;
	float	linProgress;
	std::list<Action2D>	actionList;
	Vec2	oldPos, oldScale;
	float	oldRotation;
	Vec4	oldTintAlpha;
protected:
	Vec2 pos, scale;
	float rotation;
	Vec4 tintAlpha;
public:
	Object2D(Vec2 pos, Vec2 scale, float rotation, Vec4 tintAlpha);

	Vec2	getPosition()	{ return pos; }
	Vec2	getScale()		{ return scale; }
	float	getRotation()	{ return rotation; }
	Vec4	getTintAlpha()	{ return tintAlpha; }
	int		getActionsLeft(){ return actionList.size(); }

	void	addAction(Action2D action)	{ actionList.push_back(action); }

	void	update(double dt);

	~Object2D();
};

