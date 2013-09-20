#include "MenuElement.h"
#include "GraphicsInterface.h"

MenuElement::MenuElement()
{
}

void MenuElement::updateGraphicsArray(std::vector<BBUI> *bbData)
{
	for(unsigned int i = bufferStartPos; i < bufferStartPos + bufferAmount; i++)
		bbData->at(i) = billboards.at(i);
}

void MenuElement::draw(double dt)
{
	//TODO: call graphical interface's draw().
	
}

MenuElement::~MenuElement()
{
}
