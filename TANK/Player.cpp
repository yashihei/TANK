#include "Player.h"

#include "Game.h"

const double Shot::RADIUS = 10.0;

void Shot::init(Vec2 pos, Vec2 vec, double rad) {
	this->pos = pos;
	this->vec = vec;
	this->radian = rad;
	enable = true;
}

void Shot::move(Game* game) {
	pos.moveBy(vec);
	Point stageSize = game->getStageSize();

	if (pos.x > stageSize.x || pos.x < 0 || pos.y > stageSize.y || pos.y < 0) {
		enable = false;
	}
}

void Shot::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	TextureAsset(L"bullet").rotate(radian + Radians(90)).drawAt(screenPos);
	//Circle(screenPos, RADIUS).draw(Palette::White);
}

void Player::init() {
	hp = 100;
	pos = Vec2(100.0, 100.0);
	cnt = 0;
	state = State::REBORN;
	radian = turretRad = 0.0;
}

void Player::move(Game* game) {
	cnt++;

	//前進後退
	double speed = 7.0;
	vec = Vec2(speed * Cos(radian), speed * Sin(radian));
	if (Input::KeyW.pressed) {
		pos.moveBy(vec);
	} else if (Input::KeyS.pressed) {
		pos.moveBy(-vec);
	}

	pos.x = Clamp(pos.x, 0.0, static_cast<double>(game->getStageSize().x));
	pos.y = Clamp(pos.y, 0.0, static_cast<double>(game->getStageSize().y));

	//回転
	double turnSpeed = 4.0;
	if (Input::KeyS.pressed) {
		turnSpeed *= -1;
	}
	if (Input::KeyD.pressed) {
		radian += Radians(turnSpeed);
	} else if (Input::KeyA.pressed) {
		radian -= Radians(turnSpeed);
	}

	const Point mousePos = Mouse::Pos();
	Vec2 offsetPos = game->getCamera2D()->getoffsetPos();
	turretRad = Atan2(mousePos.y - pos.y - offsetPos.y, mousePos.x - pos.x - offsetPos.x);

	moveShot(game);
}

void Player::moveShot(Game* game) {
	if (Input::MouseL.pressed && cnt % 3 == 0) {
		const Point mousePos = Mouse::Pos();
		Vec2 offsetPos = game->getCamera2D()->getoffsetPos();
		double shotRad = Atan2(mousePos.y - offsetPos.y - pos.y, mousePos.x - offsetPos.x - pos.x) + Radians(Random(-5.0, 5.0));
		double shotSpeed = 15.0;

		//ポインタで持つ必要性無いかも
		auto shot = std::make_shared<Shot>();
		shot->init(pos, { Cos(shotRad) * shotSpeed, Sin(shotRad) * shotSpeed }, shotRad);
		shots.push_back(shot);
	}

	for (auto& shot : shots) {
		shot->move(game);
	}
	Erase_if(shots, [](std::shared_ptr<Shot> shot) { return !shot->isEnable(); });
}

void Player::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	TextureAsset(L"playerTank").rotate(radian + Radians(90)).drawAt(screenPos);
	TextureAsset(L"turret").rotate(turretRad + Radians(90)).drawAt(screenPos);

	for (auto& shot : shots) {
		shot->draw(game);
	}

	Circle(Mouse::Pos(), 5).draw(Palette::Yellow);
	//Line(screenPos, Mouse::Pos()).draw(Palette::Darkgray);
}
