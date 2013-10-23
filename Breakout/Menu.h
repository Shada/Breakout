#pragma once
#include "Resource.h"
#include "Text.h"
#include "UIElement.h"
#include "ObjectCore.h"

namespace Logic
{
	class Menu
	{
	private:
		ObjectCore					*objectCore;
		unsigned int				selection;
		unsigned int				optionAmount;
		bool						menuIsActive;

		void						addActions();
		void			_moveUp();
		void			_moveDown();
		void			_confirm();

	public:
		
		Menu( ObjectCore *objectCore );
		void addOption(std::string text);
		unsigned int	getSelectedOptionInt()		{ return selection; }
		std::string		getSelectedOptionString()	{ return objectCore->optionList.at(selection).getText(); }
		void			update( double dt );
		void			open();
		void			close();

		bool			isOpen() { return menuIsActive; }

		static void		moveUp(Menu* menu);
		static void		moveDown(Menu* menu);
		static void		confirm(Menu* menu);

		~Menu();
	};
}