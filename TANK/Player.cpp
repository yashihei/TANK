#include "Player.h"

#include "Game.h"
#include "Bullet.h"
#include "Enemy.h"

void Player::init() {
	hp = 10;
	pos = Vec2(100.0, 100.0);
	cnt = 0;
	state = State::Normal;
	radian = turretRad = 0.0;
	color = Palette::White;
}

void Player::move(Game* game) {
	cnt++;

	if (checkEnemyShotHit(game)) {
		hp--;
		SoundAsset(L"damage").playMulti();
		state = State::Damage;
		cnt = 0;
	}
	if (cnt == 5) state = State::Normal;
	if (state == State::Normal && cnt % 30 == 0 && hp < 10) {
		hp++;
	}

	if (state == State::Damage) {
		color = Palette::Red;
	} else {
		color = Palette::White;
	}

	//‘OiŒã‘Þ
	const double speed = 7.0;
	vec = Vec2(speed * Cos(radian), speed * Sin(radian));
	if (Input::KeyW.pressed) {
		pos += vec;
	} else if (Input::KeyS.pressed) {
		pos -= vec;
	}

	pos.x = Clamp(pos.x, 0.0, static_cast<double>(game->getStageSize().x));
	pos.y = Clamp(pos.y, 0.0, static_cast<double>(game->getStageSize().y));

	//‰ñ“]
	double turnSpeed = 4.0;
	if (Input::KeyS.pressed) {
		turnSpeed *= -1;
	}
	if (Input::KeyD.pressed) {
		radian += Radians(turnSpeed);
	} else if (Input::KeyA.pressed) {
		radian -= Radians(turnSpeed);
	}

	fire(game);
}

void Player::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	TextureAsset(L"playerTank").rotate(radian + Pi/2).drawAt(screenPos, color);
	TextureAsset(L"turret").rotate(turretRad + Pi/2).drawAt(screenPos, color);

	Circle(Mouse::Pos(), 5).draw(Palette::Yellow);
	//Line(screenPos, Mouse::Pos()).draw(Palette::Darkgray);
}

void Player::fire(Game* game) {
	const Point mousePos = Mouse::Pos();
	const Vec2 offsetPos = game->getCamera2D()->getoffsetPos();
	turretRad = Atan2(mousePos.y - pos.y - offsetPos.y, mousePos.x - pos.x - offsetPos.x);

	if (Input::MouseL.pressed && System::FrameCount() % 3 == 0) {
		double shotRad = Atan2(mousePos.y - offsetPos.y - pos.y, mousePos.x - offsetPos.x - pos.x) + Radians(Random(-5.0, 5.0));
		double shotSpeed = 15.0;

		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<Bullet>();
		bullet->init(pos, { Cos(shotRad) * shotSpeed, Sin(shotRad) * shotSpeed }, shotRad, Bullet::Type::PLAYER);
		bulletManager->add(bullet);
		SoundAsset(L"shoot").playMulti();
	}
}

bool Player::checkEnemyShotHit(Game* game) {
	auto& bullets = game->getBulletManager()->getBullets();
	for (auto& bullet : bullets) {
		if (bullet->getType() == Bullet::Type::PLAYER) continue;
		if (Geometry2D::Intersect(Circle(bullet->getPos(), bullet->getRadius()), Circle(pos, 10.0))) {
			bullet->disable();
			return true;
		}
	}
	return false;
}
