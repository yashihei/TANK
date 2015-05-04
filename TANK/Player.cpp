#include "Player.h"

void Shot::init(Vec2 pos, Vec2 vec, double rad) {
	this->pos = pos;
	this->vec = vec;
	this->rad = rad;
	enable = true;
}

void Shot::move() {
	pos.moveBy(vec);

	if (pos.x > Window::Width() || pos.x < 0 || pos.y > Window::Height() || pos.y < 0) {
		enable = false;
	}
}

void Shot::draw() {
	Circle(pos, Radius).draw(Palette::White);
}

void Player::init() {
	hp = 100;
	pos = Vec2(100.0, 100.0);
	cnt = 0;
	state = State::REBORN;
	rad = 0;
}

void Player::move() {
	cnt++;

	//前進後退
	double speed = 5.0;
	vec = Vec2(speed * Cos(rad), speed * Sin(rad));
	if (Input::KeyW.pressed) {
		pos.moveBy(vec);
	} else if (Input::KeyS.pressed) {
		pos.moveBy(-vec);
	}

	//回転
	double turnSpeed = 5.5;
	if (Input::KeyS.pressed) {
		turnSpeed *= -1;
	}
	if (Input::KeyD.pressed) {
		rad += Radians(turnSpeed);
	} else if (Input::KeyA.pressed) {
		rad -= Radians(turnSpeed);
	}

	moveShot();
}

void Player::moveShot() {
	if (Input::MouseL.pressed && cnt % 10 == 0) {
		const Point mousePos = Mouse::Pos();
		const double rad = Atan2(mousePos.y - pos.y, mousePos.x - pos.x);
		double shotSpeed = 10.0;

		//ポインタで持つ必要性無いかも
		auto shot = std::make_shared<Shot>();
		shot->init(pos, { Cos(rad) * shotSpeed, Sin(rad) * shotSpeed }, rad);
		shots.push_back(shot);
	}

	for (auto& shot : shots) {
		shot->move();
	}
	Erase_if(shots, [](std::shared_ptr<Shot> shot) { return !shot->isEnable(); });
	Println(L"ShotCount:", shots.size());
}

void Player::draw() {
	int w = 30, h = 60;
	RectF(pos.x - w/2, pos.y - h/2, w, h).rotated(rad + Radians(90)).draw(Palette::Blue);

	for (auto& shot : shots) {
		shot->draw();
	}
	Circle(Mouse::Pos(), 5).draw(Palette::Yellow);
	Line(pos, Mouse::Pos()).draw(Palette::Blue);
}
