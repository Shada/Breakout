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

		void						addActions();

	public:
		
		Menu( ObjectCore *objectCore );
		void addOption(std::string text);
		unsigned int	getSelectedOptionInt()		{ return selection; }
		std::string		getSelectedOptionString()	{ return objectCore->optionList.at(selection).getText(); }
		void			update( double dt );
		void			moveUp();
		void			moveDown();
		void			open();
		void			close();
		~Menu();
	};
}