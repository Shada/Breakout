#include "UIElement.h"


UIElement::UIElement( std::vector<BBUI> *billboards, int texIndex, Vec2 pos, Vec2 scale, float rotation, Vec4 tintAlpha )
	:Object2D( pos,scale,rotation,tintAlpha )
{
	this->texIndex = texIndex;
	this->bbData = billboards;
	bufferIndex = billboards->size();
	bbData->push_back( BBUI( pos,scale,rotation,tintAlpha,texIndex ) );
}

void UIElement::updateBufferData()
{
	bbData->at(bufferIndex).pos			= pos;
	bbData->at(bufferIndex).rotation	= rotation;
	bbData->at(bufferIndex).size		= scale;
	bbData->at(bufferIndex).texIndex	= texIndex;
	bbData->at(bufferIndex).tintAlpha	= tintAlpha;
}

UIElement::~UIElement()
{

}
