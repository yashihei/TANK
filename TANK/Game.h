#pragma once
#include <memory>

class Player;

class Game
{
public:
	Game(){};
	void init();
	void move();
	void draw();

	enum class State {
		TITLE,
		PLAY,
	};
private:
	std::shared_ptr<Player> player;
};
