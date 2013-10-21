#pragma once

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
	enum GameState	gameState;
	~Global();
};

