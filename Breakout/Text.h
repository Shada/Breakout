#pragma once
#include "Resource.h"
#include "Font.h"
#include "Object2D.h"
class Text : public Object2D
{
private:
	Font *font;
	const char *text;
	std::vector<BBFont> *textData;
	unsigned int vbStartIndex;
	unsigned int allocatedSize;

public:
	Text(std::vector<BBFont> *fontBillboards, const char *text, Vec2 pos = Vec2(0,0), Vec2 scale = Vec2(1,1), float rotation = 0, Vec4 tintAlpha = Vec4(1,1,1,1));
	void setText( const char *text );
	unsigned int getVBStartIndex()		{ return vbStartIndex; }
	const char	*getText()				{ return text; }
	unsigned int getTextSize()			{ return textData->size(); }
	unsigned int getAllocatedSize()		{ return allocatedSize; }
	void setFont( Font *font )			{ this->font = font; }
	void updateCB();
	void updateTextData();
	void setTextData(unsigned int bufferStartIndex, unsigned int allocatedSize);
	~Text();
};

