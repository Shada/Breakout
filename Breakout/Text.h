#pragma once
#include "Resource.h"
#include "Font.h"
class Text
{
private:
	Font *font;
	const char *text;
	std::vector<BBFont> *textData;
	unsigned int vbStartIndex;

	Vec2 pos, scale;
	float rotation;
	Vec4 tintAlpha;
public:
	Text(std::vector<BBFont> *fontBillboards, const char *text, Vec2 pos = Vec2(0,0), Vec2 scale = Vec2(0,0), float rotation = 0, Vec4 tintAlpha = Vec4(0,0,0,1));
	void setText( const char *text );
	unsigned int getVBStartIndex()		{ return vbStartIndex; }
	unsigned int getTextSize()			{ return strlen(text); }
	Vec2 getPosition()					{ return pos; }
	Vec2 getScale()						{ return scale; }
	float getRotation()					{ return rotation; }
	Vec4 getTintAlpha()					{ return tintAlpha; }
	void setFont( Font *font )			{ this->font = font; }
	void updateTextData();
	~Text();
};

