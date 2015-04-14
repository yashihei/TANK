#include "Player.h"

void Player::init() {
	hp = 100;
	pos = Vec2(100.0, 100.0);
	cnt = 0;
	state = State::REBORN;
	rad = 0;
}

void Player::move() {
	cnt++;

	double speed = 5.0;
	if (Input::KeyW.pressed) {
		pos.x += speed * Cos(rad);
		pos.y += speed * Sin(rad);
	} else if (Input::KeyS.pressed) {
		pos.x -= speed * Cos(rad);
		pos.y -= speed * Sin(rad);
	}

	double turnSpeed = 3.5;
	if (Input::KeyS.pressed) {
		turnSpeed *= -1;
	}
	if (Input::KeyD.pressed) {
		rad += Radians(turnSpeed);
	} else if (Input::KeyA.pressed) {
		rad -= Radians(turnSpeed);
	}

	//以下shot関連
	if (Input::MouseL.clicked) {
		const Point mousePos = Mouse::Pos();
		const double rad = Atan2(mousePos.x - pos.x, mousePos.y - pos.y);
		double shotSpeed = 10.0;

		//ポインタで持つ必要性無いかも
		auto shot = std::make_shared<Shot>();
		shot->pos.set(pos);
		shot->vec.set(Sin(rad * shotSpeed), Cos(rad * shotSpeed));//FIXME
		shots.push_back(shot);
	}

	for (auto& shot : shots) {
		shot->pos.moveBy(shot->vec);
	}
}

void Player::draw() {
	int w = 30, h = 60;
	RectF(pos, w, h).rotated(rad + Radians(90)).draw(Palette::Red);

	for (auto& shot : shots) {
		Circle(shot->pos, 30).draw(Palette::White);
	}
}
