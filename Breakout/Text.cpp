#include "Text.h"

#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif

#include <iostream>       // std::cerr
#include <typeinfo>       // operator typeid
#include <exception>      // std::exception

Text::Text(std::vector<BBFont> *fontBillboards, std::string text, unsigned int startIndex, Vec2 pos, Vec2 scale, float rotation, Vec4 tintAlpha)
	:Object2D(pos,scale,rotation,tintAlpha)
{
	textData = fontBillboards;
	this->text		= text;
	font = NULL;
	vbStartIndex = startIndex;
	actualSize = 0;
}

void Text::setText(std::string text)
{
	this->text = text;
}

void Text::updateTextData()
{
	if(!font)
	{
		///throw std::exception("No font selected.");
		return;
	}
	textData->clear();
	actualSize = font->loadText(textData, text);
}

void Text::appendTextToData()
{
	if(!font)
	{
		throw std::exception("No font selected.");
		return;
	}
	actualSize = font->loadText(textData, text);
}

void Text::setTextData(unsigned int bufferStartIndex, unsigned int allocatedSize)
{
	vbStartIndex		= bufferStartIndex;
	this->allocatedSize	= allocatedSize;
	updateTextData();
}

void Text::updateCB()
{
	CBFont cb;
	cb.pos			= pos;
	cb.scale		= scale;
	cb.rotation		= rotation;
	cb.imageSize	= Vec2(1420,250);
	cb.tintAlpha	= tintAlpha;

	#ifdef _WIN32
		GraphicsDX11::getInstance()->updateCBFont(cb);
	#else
		GraphicsOGL4::getInstance()->updateCBFont(cb);
	#endif
}

Text::~Text()
{

}
