#pragma once
#include <Siv3D.hpp>

//プレイヤー機
//何か基本的に小さくて早くて滅茶苦茶火力持ってる感じで

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

