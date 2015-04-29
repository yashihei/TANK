#pragma once
#include <Siv3D.hpp>
#include <list>
#include <memory>

class Game;

class Enemy
{
public:
	Enemy(){};
	virtual void init() = 0;
	virtual void move(Game* game) = 0;
	virtual void draw() = 0;

	Vec2 getPos(Vec2 pos) { return pos; }
	void setPos(Vec2 pos) { this->pos = pos; }
protected:
	Vec2 pos, vec;
	int hp, cnt;
	double rad;
	bool enable;
};

class Noob : public Enemy
{
public:
	void init() override;
	void move(Game* game) override;
	void draw() override;
};

class EnemyManager
{
public:
	void init();
	void move(Game* game);
	void draw();
	void add(std::shared_ptr<Enemy>);
private:
	std::list<std::shared_ptr<Enemy>> enemies;
};
