#pragma once
#include <Siv3D.hpp>
#include <memory>
#include <list>

class Game;

class Shot
{
public:
	static const double RADIUS;

	Shot(){};
	void init(Vec2 pos, Vec2 vec, double rad);
	void move(Game* game);
	void draw(Game* game);

	Vec2 getPos() { return pos; }
	bool isEnable() { return enable; }
	void disable() { enable = false; }
private:
	Vec2 pos, vec;
	double rad;
	bool enable;
};

class Player
{
public:
	enum class State {
		NORMAL,
		DAMAGE,
		REBORN,
	};

	Player(){};
	void init();
	void move(Game* game);
	void draw(Game* game);

	Vec2 getPos() { return pos; }
	const std::list<std::shared_ptr<Shot>>& getShots() const { return shots; }
private:
	int hp;
	Vec2 pos, vec;
	int cnt;
	State state;
	double rad, turretRad;

	void moveShot(Game* game);
	std::list<std::shared_ptr<Shot>> shots;
};
