#include "Player.h"

#include "Game.h"
#include "Bullet.h"

void Player::init() {
	hp = 100;
	pos = Vec2(100.0, 100.0);
	cnt = 0;
	state = State::REBORN;
	radian = turretRad = 0.0;
}

void Player::move(Game* game) {
	cnt++;

	//ëOêiå„ëﬁ
	double speed = 7.0;
	vec = Vec2(speed * Cos(radian), speed * Sin(radian));
	if (Input::KeyW.pressed) {
		pos += vec;
	} else if (Input::KeyS.pressed) {
		pos -= vec;
	}

	pos.x = Clamp(pos.x, 0.0, static_cast<double>(game->getStageSize().x));
	pos.y = Clamp(pos.y, 0.0, static_cast<double>(game->getStageSize().y));

	//âÒì]
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
	const Vec2 offsetPos = game->getCamera2D()->getoffsetPos();
	turretRad = Atan2(mousePos.y - pos.y - offsetPos.y, mousePos.x - pos.x - offsetPos.x);

	if (Input::MouseL.pressed && cnt % 3 == 0) {
		double shotRad = Atan2(mousePos.y - offsetPos.y - pos.y, mousePos.x - offsetPos.x - pos.x) + Radians(Random(-5.0, 5.0));
		double shotSpeed = 15.0;

		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<Bullet>();
		bullet->init(pos, { Cos(shotRad) * shotSpeed, Sin(shotRad) * shotSpeed }, shotRad, Bullet::Type::PLAYER);
		bulletManager->add(bullet);
	}
}

void Player::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	TextureAsset(L"playerTank").rotate(radian + Radians(90)).drawAt(screenPos);
	TextureAsset(L"turret").rotate(turretRad + Radians(90)).drawAt(screenPos);

	Circle(Mouse::Pos(), 5).draw(Palette::Yellow);
	//Line(screenPos, Mouse::Pos()).draw(Palette::Darkgray);
}
