#pragma once
#include <Siv3D.hpp>

class Tank
{
public:
	Tank(){};
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
	int cnt;
	State state;
	double radian;
};

