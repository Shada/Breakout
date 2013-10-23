#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "Object2D.h"
class UIElement :
	public Object2D
{
private:
	std::vector<BBUI> *bbData;
	unsigned int bufferIndex;
	int texIndex;
public:
	UIElement( std::vector<BBUI> *billboards, int texIndex, Vec2 pos = Vec2(0,0), Vec2 scale = Vec2(1,1), float rotation = 0, Vec4 tintAlpha = Vec4(1,1,1,1) );
	int getTexIndex()				{ return texIndex; }
	unsigned int getBufferIndex()	{ return bufferIndex; }
	void updateBufferData();
	~UIElement();
};

#endif
