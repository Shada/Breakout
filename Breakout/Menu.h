#pragma once
#include "Resource.h"
#include "Text.h"
class Menu
{
private:
unsigned int				selection;
unsigned int				optionAmount;
std::vector<Text>			optionList;
public:
	Menu();
	void addOption(std::string text);
	unsigned int	getSelectedOptionInt()		{ return selection; }
	std::string		getSelectedOptionString()	{ return optionList.at(selection).getText(); }
	void			update();
	void			moveUp();
	void			moveDown();
	~Menu();
};

