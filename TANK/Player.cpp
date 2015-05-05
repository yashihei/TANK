#include "Player.h"

#include "Game.h"

const double Shot::RADIUS = 10.0;

void Shot::init(Vec2 pos, Vec2 vec, double rad) {
	this->pos = pos;
	this->vec = vec;
	this->rad = rad;
	enable = true;
}

void Shot::move() {
	pos.moveBy(vec);

	//if (pos.x > Window::Width() || pos.x < 0 || pos.y > Window::Height() || pos.y < 0) {
	//	enable = false;
	//}
}

void Shot::draw(Vec2 offsetPos) {
	Circle(pos + offsetPos, RADIUS).draw(Palette::White);
}

void Player::init() {
	hp = 100;
	pos = Vec2(100.0, 100.0);
	cnt = 0;
	state = State::REBORN;
	rad = turretRad = 0.0;
}

void Player::move(Game* game) {
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
	double turnSpeed = 3.0;
	if (Input::KeyS.pressed) {
		turnSpeed *= -1;
	}
	if (Input::KeyD.pressed) {
		rad += Radians(turnSpeed);
		turretRad += Radians(turnSpeed);
	} else if (Input::KeyA.pressed) {
		rad -= Radians(turnSpeed);
		turretRad -= Radians(turnSpeed);
	}

	double turretTurnSpeed = 5.0;
	if (Input::KeyE.pressed) {
		turretRad += Radians(turretTurnSpeed);
	} else if (Input::KeyQ.pressed) {
		turretRad -= Radians(turretTurnSpeed);
	}

	moveShot();
}

void Player::moveShot() {
	if (Input::KeyZ.pressed && cnt % 10 == 0) {
		double shotSpeed = 10.0;

		//ポインタで持つ必要性無いかも
		auto shot = std::make_shared<Shot>();
		shot->init(pos, { Cos(turretRad) * shotSpeed, Sin(turretRad) * shotSpeed }, rad);
		shots.push_back(shot);
	}

	for (auto& shot : shots) {
		shot->move();
	}
	Erase_if(shots, [](std::shared_ptr<Shot> shot) { return !shot->isEnable(); });
}

void Player::draw(Game* game) {
	Vec2 offsetPos = game->getOffsetPos();
	Vec2 screenPos = offsetPos + pos;
	TextureAsset(L"playerTank").scale(1.5).rotate(rad + Radians(90)).drawAt(screenPos);
	//int w = 30, h = 60;
	//RectF(pos.x - w/2, pos.y - h/2, w, h).rotated(rad + Radians(90)).draw(Palette::Blue);

	for (auto& shot : shots) {
		shot->draw(offsetPos);
	}

	Line(screenPos, screenPos + Vec2(Cos(turretRad) * 100, Sin(turretRad) * 100)).draw(Palette::Blue);
}
