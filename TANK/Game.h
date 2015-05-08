#pragma once
#include <Siv3D.hpp>
#include <memory>

class Player;
class EnemyManager;
class Camera2D;

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
	std::shared_ptr<Camera2D> getCamera2D() { return camera2D; }
	Point getStageSize() { return stageSize; }
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<EnemyManager> enemyManager;
	std::shared_ptr<Camera2D> camera2D;

	int cnt;
	Point stageSize;
};

class Camera2D
{
public:
	Camera2D(){};
	void posUpdate(Game* game);
	void shake();
	Vec2 convertToScreenPos(Vec2 pos);

	Vec2 getoffsetPos() { return offsetPos; }
private:
	Vec2 offsetPos;
};
