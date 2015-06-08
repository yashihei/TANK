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
		NORMAL,
		DAMAGE,
		BURN,
	};

	Player();
	void init();
	void move(Game* game);
	void draw(Game* game);

	Vec2 getPos() const { return pos; }
	int getHp() const { return hp; }
private:
	int hp;
	Vec2 pos, vec;
	int stateCnt, missileCnt, shotCnt;
	State state;
	double radian, turretRad;
	Color color;
	std::shared_ptr<Animation> explosionAnimation;

	struct Rut
	{
		Vec2 pos;
		double radian;
		int cnt = 0;
	};
	std::vector<std::shared_ptr<Rut>> ruts;

	void fire(Game* game);
	bool checkEnemyShotHit(Game* game);
};
