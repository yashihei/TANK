#pragma once
#include <Siv3D.hpp>
#include <memory>
#include <vector>

class Game;
class Animation;

class Player
{
public:
	static const int HP_MAX;
	static const double RADUIS;
	enum class State {
		Normal,
		Damage,
		Burn,
	};

	Player(){};
	void init();
	void move(Game* game);
	void draw(Game* game);

	Vec2 getPos() { return pos; }
	int getHp() { return hp; }
private:
	int hp;
	Vec2 pos, vec;
	int cnt, missileCnt;
	State state;
	double radian, turretRad;
	Color color;
	std::shared_ptr<Animation> explosionAnimation;

	void fire(Game* game);
	bool checkEnemyShotHit(Game* game);
};
