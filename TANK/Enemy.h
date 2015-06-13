#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <memory>
#include "Actor.h"

class Game;
class Animation;

class Enemy : public Actor
{
public:
	enum class State {
		NORMAL,
		DAMAGE,
		BURN,
	};

	Enemy();
	virtual void move(Game* game);
	virtual void draw(Game* game) = 0;

	void setPos(Vec2 pos) { this->pos = pos; }
	Vec2 getPos() const { return pos; }
	double getRadius() const { return radius; }
	State getState() const { return state; }
	Color getMinimapColor() const { return minimapColor; }
protected:
	State state;
	Vec2 pos, vec;
	int hp, stateCnt, fireCnt;
	double radian, radius;
	Color color, minimapColor;
	std::shared_ptr<Animation> explosionAnimation;

	bool checkShotHit(Game* game);
	void addDamage(int damage);
};

class T3485 : public Enemy
{
public:
	using Super = Enemy;
	T3485();
	void move(Game* game) override;
	void draw(Game* game) override;
private:
	double turretRad;
	double turnRad;
};

class MissileLauncher : public Enemy
{
public:
	using Super = Enemy;
	MissileLauncher();
	void move(Game* game) override;
	void draw(Game* game) override;
};

class EnemyManager : public ActorManager<Enemy>
{
public:
	using Super = ActorManager<Enemy>;
	void move(Game* game) override;
	void draw(Game* game) override;
};
