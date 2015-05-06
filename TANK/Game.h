#pragma once
#include <Siv3D.hpp>
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
	std::shared_ptr<EnemyManager> getEnemyManager() { return enemyManager; }
	Vec2 getOffsetPos() { return offsetPos; }
	Point getStageSize() { return stageSize; }
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<EnemyManager> enemyManager;

	int cnt;
	Vec2 offsetPos;
	Point stageSize;
};
