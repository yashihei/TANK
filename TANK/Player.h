#pragma once
#include <Siv3D.hpp>
#include <memory>
#include <list>

//ショットに関する処理が増えたらクラス化
struct Shot
{
	Vec2 pos, vec;
	double rad;
	bool enable;
};

class Player
{
public:
	Player(){};
	void init();
	void move();
	void draw();

	Vec2 getPos() { return pos; }

	enum class State {
		NORMAL,
		DAMAGE,
		REBORN,
	};
private:
	int hp;
	Vec2 pos, vec;
	int cnt;
	State state;
	double rad;

	void moveShot();
	std::list<std::shared_ptr<Shot>> shots;
};
