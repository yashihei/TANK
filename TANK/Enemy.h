#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <memory>

class Game;

class Enemy
{
public:
	enum class State {
		Normal,
		Damage,
	};

	Enemy();
	virtual void move(Game* game) = 0;
	virtual void draw(Game* game) = 0;

	void setPos(Vec2 pos) { this->pos = pos; }
	Vec2 getPos() { return pos; }
	double getRadius() { return radius; }
	State getState() { return state; }
	bool isEnable() { return enable; }
protected:
	State state;
	Vec2 pos, vec;
	int hp, cnt, shotCnt;
	double radian, radius;
	bool enable;
	Color color;//âÊëúÇ…ç∑Çµë÷Ç¶ÇΩÇÁè¡Ç∑

	bool checkShotHit(Game* game);
	void defaultMove(Game* game);
};

class Noob : public Enemy
{
public:
	Noob();
	void move(Game* game) override;
	void draw(Game* game) override;
};

class TankDestroyer : public Enemy
{
public:
	TankDestroyer();
	void move(Game* game) override;
	void draw(Game* game) override;
};

class EnemyManager
{
public:
	void init();
	void move(Game* game);
	void draw(Game* game);
	void add(std::shared_ptr<Enemy>);

	const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return enemies; }
private:
	std::vector<std::shared_ptr<Enemy>> enemies;
};
