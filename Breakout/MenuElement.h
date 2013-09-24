#pragma once
#include "Resource.h"
class MenuElement
{
protected:
	/*billboards to be drawn and managed by the menu element. (needs to be inserted into the vector in graphics!)*/
	std::vector<BBUI>	billboards;
	/*the start position in the vertex buffer inside graphics.*/
	unsigned int		bufferStartPos;
	/*the vertex amount used in the vertex buffer inside graphics.*/
	unsigned int		bufferAmount;
public:
	MenuElement();

	virtual void draw(double dt);

	void updateGraphicsArray( std::vector<BBUI> *bbData );

	~MenuElement();
};

