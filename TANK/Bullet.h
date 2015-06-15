#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <memory>
#include "Actor.h"

class Game;
class Animation;

class Bullet : public Actor {
public:
	enum class Target {
		ENEMY,
		PLAYER,
		NONE
	};

	Bullet();
	void setParam(Vec2 pos, Vec2 vec, double radian, Target target);

	Vec2 getPos() const { return pos; }
	Vec2 getVec() const { return vec; }
	Target getTarget() const { return target; }
	double getRadius() const { return radius; }
	int getDamage() const { return damage; }
protected:
	Vec2 pos, vec;
	double radian, radius;
	int damage, cnt;
	Target target;

	void checkOutStage(Game* game);
};

class NormalBullet : public Bullet {
public:
	NormalBullet();
	void move(Game* game) override;
	void draw(Game* game) override;
};

class Missile : public Bullet {
public:
	Missile();
	void move(Game* game) override;
	void draw(Game* game) override;
private:
	std::shared_ptr<Animation> missileAnimation;
};

class Bomb : public Bullet {
public:
	Bomb();
	void move(Game* game) override;
	void draw(Game* game) override;
private:
	std::shared_ptr<Animation> explosionAnimation;
};

class BulletManager : public ActorManager<Bullet>
{
public:
	using Super = ActorManager<Bullet>;
	BulletManager() = default;
	void move(Game* game) override;
	void draw(Game* game) override;
};
