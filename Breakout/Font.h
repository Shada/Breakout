#pragma once
#include <iostream>
#include <vector>
#include <fstream>
class Font
{
private:
	unsigned int imageIndex;
	struct Character
	{
		float posx;
		float posy;
		float width;
	};
	std::vector<Character> characters;
public:
	Font();
	void loadFontSettings(std::string filePath);
	void loadText(std::string text);
	~Font();
};

