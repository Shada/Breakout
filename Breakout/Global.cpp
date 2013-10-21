#include "Global.h"
#include "Resource.h"

Global	*Global::instance = NULL;
Global::Global()
{
	gameState = GameState::GAME_MENU;
}


Global::~Global()
{
}
