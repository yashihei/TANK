#pragma once
#include <Siv3D.hpp>
#include <memory>

class Player;
class EnemyManager;
class BulletManager;
class Camera2D;
class Animation;
class ItemManager;

class Game
{
public:
	Game();
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
	std::shared_ptr<ItemManager> getItemManager() const { return itemManager; }
	std::shared_ptr<Camera2D> getCamera2D() const { return camera2D; }
	RectF getStageRect() const { return stageRect; }

	void gameOver();
	void addScore(int score) { this->score += score; }
private:
	std::shared_ptr<Player> player;
	std::shared_ptr<EnemyManager> enemyManager;
	std::shared_ptr<BulletManager> bulletManager;
	std::shared_ptr<ItemManager> itemManager;
	std::shared_ptr<Camera2D> camera2D;

	RectF stageRect;
	State state;
	int stateCnt, inGameCnt;
	int score, hiScore;

	void gameStart();
	void createActors();
	Vec2 makeRandomPos();

	void drawHUD();
	void drawMarker();
	void drawMinimap();
	void drawHpCircle();
	void drawCenterAddShadowFont(String assetName, String drawStr, Vec2 pos);
};
