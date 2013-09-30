#pragma once
#include "Resource.h"
class Menu
{
private:
unsigned int				selection;
unsigned int				optionAmount;
std::vector<std::string>	optionList;
public:
	Menu();
	void addOption(std::string text);
	unsigned int	getSelectedOptionInt()		{ return selection; }
	std::string		getSelectedOptionString()	{ return optionList.at(selection); }
	void			update();
	void			moveUp();
	void			moveDown();
	~Menu();
};

