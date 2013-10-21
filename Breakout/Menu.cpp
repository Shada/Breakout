#include "Menu.h"
#include "ObjectCore.h"

namespace Logic
{
	Menu::Menu(ObjectCore *objectCore)
	{
		this->objectCore = objectCore;
		selection				= 0;
		optionAmount			= 0;
	}

	void Menu::addOption(std::string text)
	{
		objectCore->optionList.push_back( Text( &objectCore->fontBillboards, text.c_str() ) );
		objectCore->optionList.back().setFont(objectCore->testFont);
		objectCore->optionList.back().updateTextData();
		objectCore->optionList.back().updateCB();
		objectCore->gui.push_back( UIElement( &objectCore->uiBillboards, 41 ) );
		optionAmount++;
	}
	
	void Menu::open()
	{
		if(optionAmount > 0)
		{
			Vec2 desu = Vec2(0, ( SCRHEIGHT - 200 ) / optionAmount);
			Vec4 color = Vec4(1,1,1,0.5f);
			for(unsigned int i = 0; i < optionAmount; i++)
			{
				if(selection >= i - 1 && selection <= i + 1)
				{
					if(selection == i)
					{
						color = Vec4(0.5f,1,1,1);
					}
					else
					{
						color = Vec4(1,1,1,0.8f);
					}
				}

				Action2D action(	Vec2( 200, 100 ) + desu * i,
									Vec2(1,1),
									1.0f,
									color,
									1.0f,
									3 );
				objectCore->optionList.at(i).addAction(action);
			}
		}
		else
			std::cout << "Failed to open menu. (There are no options)";
	}

	void Menu::close()
	{
		for(unsigned int i = 0; i < optionAmount; i++)
		{
			Action2D action(	Vec2(200, SCRHEIGHT * 0.5f),
								Vec2(1,1),
								1.0f,
								Vec4(1,1,1,0.0f),
								1.0f,
								3 );
			objectCore->optionList.at(i).addAction(action);
		}
	}

	void Menu::update(double dt)
	{
		for(unsigned int i = 0; i < optionAmount; i++)
		{
			objectCore->optionList.at(i).update(dt);
		}
	}

	void Menu::moveDown()
	{
		if(selection < 0)
			selection = optionAmount;
		else
			selection--;

		open();
	}

	void Menu::moveUp()
	{
		if(selection > optionAmount)
			selection = 0;
		else
			selection++;

		open();
	}

	Menu::~Menu()
	{

	}
}