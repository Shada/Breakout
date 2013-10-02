#include "Text.h"


Text::Text(std::vector<BBFont> *fontBillboards, const char *text, Vec2 pos, Vec2 scale, float rotation, Vec4 tintAlpha)
{
	textData = fontBillboards;

	this->text		= text;
	this->pos		= pos;
	this->scale		= scale;
	this->rotation	= rotation;
	this->tintAlpha	= tintAlpha;
	font = NULL;
	vbStartIndex = 0;
}

void Text::setText(const char *text)
{
	this->text = text;
}

void Text::updateTextData()
{
	if(!font)
	{
		throw std::exception("No font selected.");
		return;
	}
	vbStartIndex = textData->size();
	font->loadText(textData, text);
}

Text::~Text()
{
}
