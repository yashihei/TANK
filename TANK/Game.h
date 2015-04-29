#pragma once
#include <memory>

class Player;
class EnemyManager;

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
	
	std::shared_ptr<Player> getPlayer() { return player; }
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<EnemyManager> enemyManager;
};
