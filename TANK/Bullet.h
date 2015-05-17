#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <memory>

class Game;

class Bullet {
public:
	enum class Type {
		ENEMY,
		PLAYER
	};

	Bullet() {}
	void init(Vec2 pos, Vec2 vec, double radian, Type type);
	void move(Game* game);
	void draw(Game* game);

	Vec2 getPos() { return pos; }
	Type getType() { return type; }
	bool isEnabled() { return enable; }
private:
	Vec2 pos, vec;
	double radian;
	int cnt;
	bool enable;
	Type type;
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
