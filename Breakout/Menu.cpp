#include "Menu.h"
#include "ObjectCore.h"


Menu::Menu()
{
	selection = 0;
}

void Menu::addOption(std::string text)
{

}

void Menu::update()
{

}

void Menu::moveDown()
{
	if(selection < 0)
		selection = optionAmount;
	else
		selection--;
}

void Menu::moveUp()
{
	if(selection > optionAmount)
		selection = 0;
	else
		selection++;
}

Menu::~Menu()
{

}
