#include "Tank.h"

void Tank::init() {
	hp = 100;
	pos = Vec2(100.0, 100.0);
	cnt = 0;
	state = State::REBORN;
	radian = 0;
}

void Tank::move() {
	cnt++;

	//FIXME:OŠpŠÖ”–Y‚ê‚½“®‚«‚ª•Ï
	double speed = 3.0;
	if (Input::KeyW.pressed) {
		pos.x += speed * Cos(radian);
		pos.y += speed * Sin(radian);
	} else if (Input::KeyS.pressed) {
		pos.x -= speed * Cos(radian);
		pos.y -= speed * Sin(radian);
	}

	double turnSpeed = 2.0;
	//turn
	if (Input::KeyD.pressed) {
		radian += Radians(turnSpeed);
	} else if (Input::KeyA.pressed) {
		radian -= Radians(turnSpeed);
	}
}

void Tank::draw() {
	int w = 30, h = 60;

	RectF(pos, w, h).rotated(radian).draw(Palette::Red);
}
