#include "MenuElement.h"

MenuElement::MenuElement()
{
}

void MenuElement::updateGraphicsArray(std::vector<BBUI> *bbData)
{
	for(unsigned int i = bufferStartPos; i < bufferStartPos + bufferAmount; i++)
		bbData->at(i) = billboards.at(i);
}

MenuElement::~MenuElement()
{
}
