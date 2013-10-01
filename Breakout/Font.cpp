#include "Font.h"


Font::Font()
{
	spacing = 5;
}

void Font::loadFontSettings(const char *filePath)
{
	std::ifstream myfile (filePath);
	if (myfile.is_open())
	{
		float number;
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

void Font::loadText(std::vector<BBFont> *outData, const char *text)
{
	unsigned int size = strlen(text);
	BBFont current;
	int posx = 0;

	for(unsigned int i = 0; i < size; i++)
	{
		if(text[i] >= 33 && text[i] < 127)
		{
			current.x = posx;
			current.texCoords.x = characters.at(text[i] - 33).posx;		// x1
			current.texCoords.y = characters.at(text[i] - 33).posy;		// y1
			current.texCoords.z = characters.at(text[i] - 33).posx + characters.at(text[i] - 33).width; // x2
			current.texCoords.w = characters.at(text[i] - 33).posy + 100; // y2
			outData->push_back(current);

			posx += characters.at(text[i] - 33).width;
		}
		else if(text[i] == 32) //space
		{
			posx += 15;
		}
		else
		{
			std::cout << "Error: unrecognizable character ( " << text[i] << " )";
		}
	}
}

Font::~Font()
{

}
