#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <memory>

class Game;
class Animation;

class Bullet {
public:
	enum class Target {
		ENEMY,
		PLAYER,
		NONE
	};

	Bullet() {}
	void setParam(Vec2 pos, Vec2 vec, double radian, Target target);
	virtual void move(Game* game) = 0;
	virtual void draw(Game* game) = 0;

	Vec2 getPos() const { return pos; }
	Vec2 getVec() const { return vec; }
	Target getTarget() const { return target; }
	double getRadius() const { return radius; }
	int getDamage() const { return damage; }
	bool isEnabled() const { return enable; }
	void disable() { enable = false; }
protected:
	Vec2 pos, vec;
	double radian, radius;
	int damage, cnt = 0;
	bool enable = true;
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
	std::shared_ptr<Animation> missileAnimation;
};

class Bomb : public Bullet {
public:
	Bomb();
	void move(Game* game) override;
	void draw(Game* game) override;
	std::shared_ptr<Animation> explosionAnimation;
};

class BulletManager
{
public:
	BulletManager() {}
	void init();
	void add(std::shared_ptr<Bullet> bullet);
	void move(Game* game);
	void draw(Game* game);

	const std::vector<std::shared_ptr<Bullet>>& getBullets() const { return bullets; }
private:
	std::vector<std::shared_ptr<Bullet>> bullets;
};
