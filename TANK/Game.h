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
private:
	std::shared_ptr<Player> player;
};
