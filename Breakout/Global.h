#ifndef _GLOBAL_H_
#define _GLOBAL_H_

enum GameState
{
	GAME_MENU = 0,
	GAME_PLAY = 1,
	GAME_PLAYING = 2,
};

class Global
{

private:
	static Global			*instance;
	Global();
public:
	static Global			*getInstance()
	{
		if(!instance)
			instance = new Global();
		return instance;
	}
	~Global();
	GameState	gameState;
};

#endif // ! _GLOBAL_H_
