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
	Game() = default;
	void init();
	void move();
	void draw();

	enum class State {
		TITLE,
		PLAY,
		PAUSE,
		GAME_OVER,
	};
	
	std::shared_ptr<Player> getPlayer() const { return player; }
	std::shared_ptr<EnemyManager> getEnemyManager() const { return enemyManager; }
	std::shared_ptr<BulletManager> getBulletManager() const { return bulletManager; }
	std::shared_ptr<Camera2D> getCamera2D() const { return camera2D; }
	Rect getStageSize() const { return stageSize; }
	void gameOver();
	void addScore(int score) { this->score += score; }
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<EnemyManager> enemyManager;
	std::shared_ptr<BulletManager> bulletManager;
	std::shared_ptr<Camera2D> camera2D;

	Rect stageSize;
	State state = State::TITLE;
	int cnt = 0;
	int score = 0;

	void gameStart();

	void drawHUD();
	void drawMinimap();
	void drawHpCircle();
};

class Camera2D
{
public:
	Camera2D() = default;
	void posUpdate(Game* game);
	void shake(int num);
	Vec2 convertToScreenPos(Vec2 pos);

	Vec2 getoffsetPos() const { return offsetPos; }
private:
	Vec2 offsetPos = { 0, 0 };
};

class Animation
{
public:
	Animation() = default;
	void init(String name, int sepNum, int intervalCnt);
	void move();
	void draw(Vec2 pos);
	void draw(Vec2 pos, double radian);
	int getCycleCnt() const { return sepNum * intervalCnt; }
private:
	String assetName;
	Point trimSize;
	int intervalCnt, sepNum;
	int cnt = 0;
	Rect trimRect;
};

class Item
{
public:
	Item() = default;
private:
};
