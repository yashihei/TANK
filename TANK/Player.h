#pragma once
#include <Siv3D.hpp>
#include <memory>
#include <list>

//ショットに関する処理が増えたらクラス化
struct Shot
{
	Vec2 pos, vec;
	double rad;
};

class Player
{
public:
	Player(){};
	void init();
	void move();
	void draw();

	enum class State {
		NORMAL,
		DAMAGE,
		REBORN,
	};

private:
	int hp;
	Vec2 pos;
	//Vec2 vec;
	int cnt;
	State state;
	double rad;

	std::list<std::shared_ptr<Shot>> shots;
};
