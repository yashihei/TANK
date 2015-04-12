#include "Tank.h"

void Tank::init() {
	hp = 100;
	pos = Vec2(100, 100);
	cnt = 0;
	state = State::REBORN;
	radian = 0;
}

void Tank::move() {
	cnt++;
}

void Tank::draw() {
	RectF(pos, 50, 100).rotated(radian).draw(Palette::Red);
}
