#include "Menu.h"
#include "ObjectCore.h"
#include <iostream>

namespace Logic
{
	Menu::Menu(ObjectCore *objectCore)
	{
		this->objectCore = objectCore;
		selection				= 0;
		optionAmount			= 0;
		menuIsActive			= true;
		objectCore->selector = new UIElement( &objectCore->uiBillboards, 1 );
		objectCore->circle = new UIElement( &objectCore->uiBillboards, 4, Vec2(SCRWIDTH*0.5f - 300, SCRHEIGHT*0.5f - 300),Vec2(600,600),0.0f, Vec4(1,1,1,0) );
		objectCore->logo = new UIElement( &objectCore->uiBillboards, 2, Vec2(SCRWIDTH-700, 0),Vec2(547, 181), 0.0f, Vec4(1,1,1,0) );
		objectCore->loli = new UIElement( &objectCore->uiBillboards, 3, Vec2(SCRWIDTH-500, SCRHEIGHT-700),Vec2(544,676),0.0f, Vec4(1,1,1,0) );

		Action2D action(	Vec2( SCRWIDTH-700, 100 ),
							Vec2(547, 181),
							0.0f,
							Vec4(1,1,1,1),
							2,
							3 );
		objectCore->logo->addAction(action);
		action = Action2D(	Vec2( SCRWIDTH-600, SCRHEIGHT-700 ),
							Vec2(544,676),
							0.0f,
							Vec4(1,1,1,1),
							2,
							5 );
		objectCore->loli->addAction(action);

		action = Action2D(	Vec2(SCRWIDTH*0.5f - 500, SCRHEIGHT*0.5f - 500),
							Vec2(1000,1000),
							0.0f,
							Vec4(1,1,1,1),
							3,
							3 );
		objectCore->circle->addAction(action);
	}

	void Menu::addOption(std::string text)
	{
		objectCore->optionList.push_back( Text( &objectCore->fontBillboards, text.c_str() ) );
		objectCore->optionList.back().setFont(objectCore->testFont);
		
		objectCore->gui.push_back( UIElement( &objectCore->uiBillboards, 0 ) );
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
				color = Vec4(1,1,1,1);
				action = Action2D(	Vec2( 150, 25 ) + desu * i,
									scale,
									1.0f,
									color,
									0.5f,
									3 );
				objectCore->gui.at(i).addAction(action);
			}
			action = Action2D(	Vec2( 150, 50 ) + desu * selection,
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

		objectCore->logo->update(dt);
		objectCore->logo->updateBufferData();

		objectCore->loli->update(dt);
		objectCore->loli->updateBufferData();

		objectCore->circle->update(dt);
		objectCore->circle->updateBufferData();

		if(!menuIsActive && objectCore->selector->getActionsLeft() == 0 && objectCore->optionList.at(0).getActionsLeft() == 0)
			Global::getInstance()->gameState = GAME_PLAY;
	}

	void Menu::moveUp(Menu *_menu)
	{
		_menu->_moveUp();
	}

	void Menu::moveDown(Menu *_menu)
	{
		_menu->_moveDown();
	}

	void Menu::confirm(Menu *_menu)
	{
		_menu->_confirm();
	}

	void Menu::_moveUp()
	{
		if(selection <= 0)
			selection = optionAmount - 1;
		else
			selection--;
		open();
	}

	void Menu::_moveDown()
	{
		if(selection >= optionAmount - 1)
			selection = 0;
		else
			selection++;

		open();
	}

	void Menu::_confirm()
	{
		Vec2 moveDistance = Vec2(0, ( SCRHEIGHT - 200 ) / optionAmount);
		Action2D action(	Vec2( 200, 100 ) + moveDistance * selection,
									Vec2(1.3f,1.3f),
									1.0f,
									Vec4(1,1,1,1),
									0.1f,
									3 );
		objectCore->optionList.at(selection).addAction(action);
		action = Action2D(	Vec2( 200, 100 ) + moveDistance * selection,
									Vec2(1.1f,1.1f),
									1.0f,
									Vec4(1,1,0,0.5),
									0.05f,
									3 );
		objectCore->optionList.at(selection).addAction(action);
		action = Action2D(	Vec2( 200, 100 ) + moveDistance * selection,
									Vec2(1.3f,1.3f),
									1.0f,
									Vec4(1,1,1,1),
									0.05f,
									3 );
		objectCore->optionList.at(selection).addAction(action);

		if(selection == 0)
		{
			close();
			menuIsActive = false;
		}
		if(selection == objectCore->optionList.size() - 1)
		{
			exit(0);
		}
	}

	Menu::~Menu()
	{

	}
}