#include "Text.h"


Text::Text(const char *text)
{
	this->text = text;
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
	font->loadText(&textData, text);
}

Text::~Text()
{
}
