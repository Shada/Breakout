#pragma once
#include "Resource.h"
#include "Font.h"
class Text
{
private:
	Font *font;
	const char *text;
	std::vector<BBFont> textData;
public:
	Text(const char *text);
	void setText( const char *text );
	void updateTextData();
	~Text();
};

