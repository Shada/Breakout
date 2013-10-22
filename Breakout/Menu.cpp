#include "Menu.h"
#include "ObjectCore.h"

namespace Logic
{
	Menu::Menu(ObjectCore *objectCore)
	{
		this->objectCore = objectCore;
		selection				= 0;
		optionAmount			= 0;
		objectCore->selector = new UIElement( &objectCore->uiBillboards, 1 );

	}

	void Menu::addOption(std::string text)
	{
		objectCore->optionList.push_back( Text( &objectCore->fontBillboards, text.c_str() ) );
		objectCore->optionList.back().setFont(objectCore->testFont);
		
		objectCore->gui.push_back( UIElement( &objectCore->uiBillboards, 41 ) );
		optionAmount++;
	}
	
	void Menu::open()
	{
		if(optionAmount > 0)
		{
			Action2D action;
			Vec2 desu = Vec2(0, ( SCRHEIGHT - 200 ) / optionAmount);
			Vec4 color;
			Vec2 scale;
			for(int i = 0; i < optionAmount; i++)
			{
				color = Vec4(1,1,1,0.5f);
				scale = Vec2(0.8f,0.8f);
				if(selection + 1 >= i && selection <= i + 1)
				{
					if(selection == i)
					{
						color = Vec4(0.5f,1,1,1);
						scale = Vec2(1.2f,1.2f);
					}
					else
					{
						color = Vec4(1,1,1,0.8f);
					}
				}

				action = Action2D(	Vec2( 200, 100 ) + desu * i,
									scale,
									1.0f,
									color,
									0.5f,
									3 );
				objectCore->optionList.at(i).addAction(action);
				scale = Vec2(500,200) * Vec2(scale.x, 1);
				action = Action2D(	Vec2( 150, 25 ) + desu * i,
									scale,
									1.0f,
									color,
									0.5f,
									3 );
				objectCore->gui.at(i).addAction(action);
			}
			action = Action2D(	Vec2( 200, 100 ) + desu * selection,
									Vec2(500,200),
									1.0f,
									Vec4(1,1,1,0.2f),
									0.5f,
									3 );
			objectCore->selector->addAction(action);
		}
		else
			std::cout << "Failed to open menu. (There are no options)";

	}

	void Menu::close()
	{
		if(optionAmount > 0)
		{
			Vec2 desu = Vec2(0, ( SCRHEIGHT - 200 ) / optionAmount);
			Vec4 color;
			Vec2 scale;
			for(int i = 0; i < optionAmount; i++)
			{
				color = Vec4(1,1,1,0.0f);
				scale = Vec2(0.8f,0.8f);

				Action2D action(	Vec2( 100, 100 ) + desu * i,
									scale,
									1.0f,
									color,
									1.0f,
									3 );
				objectCore->optionList.at(i).addAction(action);
			}
		}
	}

	void Menu::update(double dt)
	{
		for(unsigned int i = 0; i < optionAmount; i++)
		{
			objectCore->optionList.at(i).update(dt);
			objectCore->gui.at(i).update(dt);
			objectCore->gui.at(i).updateBufferData();
		}
		objectCore->selector->update(dt);
		objectCore->selector->updateBufferData();
	}

	void Menu::moveUp()
	{
		if(selection < 0)
			selection = optionAmount;
		else
			selection--;
		open();
	}

	void Menu::moveDown()
	{
		if(selection > optionAmount)
			selection = 0;
		else
			selection++;

		open();
	}

	void Menu::confirm()
	{
		Vec2 desu = Vec2(0, ( SCRHEIGHT - 200 ) / optionAmount);
		Action2D action(	Vec2( 200, 100 ) + desu * selection,
									Vec2(1.3f,1.3f),
									1.0f,
									Vec4(1,1,1,1),
									0.1f,
									3 );
		objectCore->optionList.at(selection).addAction(action);
		action = Action2D(	Vec2( 200, 100 ) + desu * selection,
									Vec2(1.1f,1.1f),
									1.0f,
									Vec4(1,1,0,0.5),
									0.05f,
									3 );
		objectCore->optionList.at(selection).addAction(action);
		action = Action2D(	Vec2( 200, 100 ) + desu * selection,
									Vec2(1.3f,1.3f),
									1.0f,
									Vec4(1,1,1,1),
									0.05f,
									3 );
		objectCore->optionList.at(selection).addAction(action);

		open();
	}

	Menu::~Menu()
	{

	}
}