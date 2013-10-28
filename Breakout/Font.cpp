#include "Font.h"
#include <string.h>

Font::Font()
{
	spacing = 5;
	characters = std::vector<Character>();
}

void Font::loadFontSettings(std::string filePath)
{
	std::ifstream myfile (filePath);
	if (myfile.is_open())
	{
		unsigned int number;
		while(myfile >> number)
		{
			characters.push_back(Character());
			//posx
			characters.at(characters.size()-1).posx = number;
			//posy
			myfile >> number;
			characters.at(characters.size()-1).posy = number;
			//width
			myfile >> number;
			characters.at(characters.size()-1).width = number;
		}
	}
	myfile.close();
}

unsigned int Font::loadText(std::vector<BBFont> *outData, std::string text)
{
	unsigned int usedBufferAmount = 0;
	unsigned int size = text.size();
	BBFont current;
	int posx = 0;
	if(characters.size() == 0)
	{
		std::cout << "Error: No font loaded for text ( " << text.c_str() << " )";
		return 0;
	}
	for(unsigned int i = 0; i < size; i++)
	{
		if(text[i] >= 33 && text[i] < 127)
		{
			current.x			= (float)posx;
			current.texCoords.x = (float)characters.at(text[i] - 33).posx;		// x1
			current.texCoords.y = (float)characters.at(text[i] - 33).posy;		// y1
			current.texCoords.z = (float)characters.at(text[i] - 33).width;		// width
			current.texCoords.w = 75;											// height

			outData->push_back(current);

			posx += characters.at(text[i] - 33).width + 5;
			usedBufferAmount++;
		}
		else if(text[i] == 32) //space
		{
			posx += 25;
		}
		else
		{
			std::cout << "Error: unrecognizable character ( " << text[i] << " )";
		}
	}
	return usedBufferAmount;
}

Font::~Font()
{

}
