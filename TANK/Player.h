#pragma once
#include <Siv3D.hpp>
#include <memory>
#include <vector>

class Game;

class Player
{
public:
	enum class State {
		Normal,
		Damage,
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
	int cnt;
	State state;
	double radian, turretRad;
	Color color;

	void fire(Game* game);
	bool checkEnemyShotHit(Game* game);
};
