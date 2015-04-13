#pragma once
#include <memory>

class Tank;

class Game
{
public:
	Game(){};
	void init();
	void move();
	void draw();
private:
	std::shared_ptr<Tank> tank;
};
