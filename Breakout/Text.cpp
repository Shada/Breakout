#include "Text.h"

#ifdef _WIN32
#include "GraphicsDX11.h"
#else
#include "GraphicsOGL4.h"
#endif


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

void Text::updateCB()
{
	CBFont cb;
	cb.pos			= pos;
	cb.scale		= scale;
	cb.rotation		= rotation;
	cb.tintAlpha	= tintAlpha;
	cb.imageSize	= Vec2(1420,250);

	#ifdef _WIN32
	GraphicsDX11::getInstance()->updateCBFont(cb);
	#else
	//GraphicsOGL4::getInstance()->updateCBFont(cb);
	#endif
}

Text::~Text()
{
}
