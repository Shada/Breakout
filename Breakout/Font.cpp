#include "Font.h"


Font::Font()
{

}

void Font::loadFontSettings(std::string filePath)
{
	std::ifstream myfile (filePath);
	if (myfile.is_open())
	{
		while(!myfile.eof())
		{
			
		}
	}
	myfile.close();
}

void Font::loadText(std::string text)
{

}

Font::~Font()
{

}
