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
	void loadFontSettings(std::string filePath);
	void loadText(std::vector<BBFont> *outData, std::string text);

	unsigned int getImageIndex() { return imageIndex; }
	~Font();
};

