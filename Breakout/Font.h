#pragma once
#include "Resource.h"
#include <iostream>
#include <fstream>
class Font
{
private:
	unsigned int imageIndex;
	unsigned int imageWidth;
	unsigned int imageHeight;
	int spacing;
	struct Character
	{
		unsigned int posx;  //pixel position.x in font image
		unsigned int posy;  //pixel position.y in font image
		unsigned int width; //pixel width in font image
	};
	std::vector<Character> characters;
public:
	Font();
	void setImageIndex(unsigned int index) { imageIndex = index; }
	void loadFontSettings(const char *filePath);
	void loadText(std::vector<BBFont> *outData, const char *text);
	~Font();
};

