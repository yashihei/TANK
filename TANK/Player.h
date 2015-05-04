#pragma once
#include <Siv3D.hpp>
#include <memory>
#include <list>

class Shot
{
public:
	const double Radius = 10.0;

	Shot(){};
	void init(Vec2 pos, Vec2 vec, double rad);
	void move();
	void draw();

	Vec2 getPos() { return pos; }
	bool isEnable() { return enable; }
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
	void move();
	void draw();

	Vec2 getPos() { return pos; }
	std::list<std::shared_ptr<Shot>> getShots() { return shots; }//‚±‚ê‚â‚Î‚¢
private:
	int hp;
	Vec2 pos, vec;
	int cnt;
	State state;
	double rad;

	void moveShot();
	std::list<std::shared_ptr<Shot>> shots;
};
