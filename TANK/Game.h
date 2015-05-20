#pragma once
#include <Siv3D.hpp>
#include <memory>

class Player;
class EnemyManager;
class BulletManager;
class Camera2D;
class Animation;

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
	std::shared_ptr<BulletManager> getBulletManager() { return bulletManager; }
	std::shared_ptr<Camera2D> getCamera2D() { return camera2D; }
	Rect getStageSize() { return stageSize; }
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<EnemyManager> enemyManager;
	std::shared_ptr<BulletManager> bulletManager;
	std::shared_ptr<Camera2D> camera2D;

	Rect stageSize;

	void drawMinimap();
	void drawHpCircle();
};

class Camera2D
{
public:
	Camera2D(){};
	void posUpdate(Game* game);
	void shake(int num);
	Vec2 convertToScreenPos(Vec2 pos);

	Vec2 getoffsetPos() { return offsetPos; }
private:
	Vec2 offsetPos;
};

class Animation
{
public:
	Animation(){};
	void init(String name, int sepNum, int sepTime);
	void move();
	void draw(Vec2 pos);
private:
	String assetName;
	Point trimSize;
	int sepTime, sepNum;
	int cnt;
};
