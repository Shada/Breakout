#pragma once
#include "Resource.h"
#include "Font.h"
#include "Object2D.h"
#include <iostream>

class Text : public Object2D
{
private:
	Font *font;
	std::string text;
	std::vector<BBFont> *textData;
	unsigned int vbStartIndex;
	unsigned int allocatedSize;
	unsigned int actualSize;

public:
	Text(std::vector<BBFont> *fontBillboards, std::string text, unsigned int startIndex = 0, Vec2 pos = Vec2(0,0), Vec2 scale = Vec2(1,1), float rotation = 0, Vec4 tintAlpha = Vec4(1,1,1,1));
	void setText( std::string text );
	unsigned int getVBStartIndex()		{ return vbStartIndex; }
	std::string	getText()				{ return text; }
	unsigned int getTextSize()			{ return text.size(); }
	Vec2 getTextDimensions()			{ return Vec2( textData->back().x + textData->back().texCoords.z , 75); }
	unsigned int getAllocatedSize()		{ return allocatedSize; }
	unsigned int getActualSize()		{ return actualSize; }
	void setFont( Font *font )			{ this->font = font; }
	void updateCB();
	void updateTextData();
	void appendTextToData();
	void setTextData(unsigned int bufferStartIndex, unsigned int allocatedSize);
	~Text();
};

