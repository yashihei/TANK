#pragma once
#include <Siv3D.hpp>
#include <list>
#include <memory>

class Game;

class Enemy
{
public:
	enum class State {
	};

	Enemy();
	virtual void move(Game* game) = 0;
	virtual void draw(Game* game) = 0;

	void setPos(Vec2 pos) { this->pos = pos; }
	Vec2 getPos(Vec2 pos) { return pos; }
	bool isEnable() { return enable; }
protected:
	Vec2 pos, vec;
	int hp, cnt;
	double rad;
	bool enable;

	bool checkShotHit(Game* game);
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
private:
	std::list<std::shared_ptr<Enemy>> enemies;
};
