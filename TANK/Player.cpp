#include "Player.h"

#include "Game.h"
#include "Bullet.h"
#include "Enemy.h"

const int Player::HP_MAX = 5;
const double Player::RADUIS = 10.0;

void Player::init() {
	hp = HP_MAX;
	pos = Vec2(540.0, 540.0);
	cnt = 0;
	state = State::Normal;
	radian = turretRad = 0.0;
	color = Palette::White;
	explosionAnimation = std::make_shared<Animation>();
}

void Player::move(Game* game) {
	cnt++;

	if (state == State::Burn) {
		game->getCamera2D()->shake(30);
		explosionAnimation->move();
		if (cnt == 32) {
			game->gameOver();
		}
		return;
	}

	if (checkEnemyShotHit(game)) {
		SoundAsset(L"damage").playMulti();
		state = State::Damage;
		cnt = 0;
		if (hp <= 0) {
			state = State::Burn;
			explosionAnimation->init(L"explosion", 7, 4);
			SoundAsset(L"burn").playMulti();
		}
	} else if (cnt == 5) {
		state = State::Normal;
	} else if (state == State::Normal && cnt % 30 == 0) {
		hp++;
	}
	hp = Clamp(hp, 0, HP_MAX);

	if (state == State::Damage) {
		color = Palette::Red;
	} else {
		color = Palette::White;
	}

	//ëOêiå„ëﬁ
	const double speed = 7.0;
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

	fire(game);
}

void Player::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	if (state == State::Burn) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"playerTank").rotate(radian + Pi/2).drawAt(screenPos, color);
	TextureAsset(L"turret").rotate(turretRad + Pi/2).drawAt(screenPos, color);
}

void Player::fire(Game* game) {
	const Point mousePos = Mouse::Pos();
	const Vec2 offsetPos = game->getCamera2D()->getoffsetPos();
	turretRad = Atan2(mousePos.y - pos.y - offsetPos.y, mousePos.x - pos.x - offsetPos.x);

	if (Input::MouseL.pressed && System::FrameCount() % 3 == 0) {
		double shotRad = Atan2(mousePos.y - offsetPos.y - pos.y, mousePos.x - offsetPos.x - pos.x) + Radians(Random(-8.0, 8.0));
		double shotSpeed = 15.0;

		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<NormalBullet>();
		bullet->setParam(pos, { Cos(shotRad) * shotSpeed, Sin(shotRad) * shotSpeed }, shotRad, Bullet::Target::ENEMY);
		bulletManager->add(bullet);
		SoundAsset(L"shoot").playMulti();
	}

	if (Input::MouseR.clicked) {
		double shotRad = Atan2(mousePos.y - offsetPos.y - pos.y, mousePos.x - offsetPos.x - pos.x);
		double shotSpeed = -5.0;

		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<Missile>();
		bullet->setParam(pos, { Cos(shotRad) * shotSpeed, Sin(shotRad) * shotSpeed }, shotRad, Bullet::Target::ENEMY);
		bulletManager->add(bullet);
	}
}

bool Player::checkEnemyShotHit(Game* game) {
	auto& bullets = game->getBulletManager()->getBullets();
	for (auto& bullet : bullets) {
		if (bullet->getTarget() == Bullet::Target::ENEMY) continue;
		if (Geometry2D::Intersect(Circle(bullet->getPos(), bullet->getRadius()), Circle(pos, RADUIS))) {
			bullet->disable();
			hp -= bullet->getDamage();
			return true;
		}
	}
	return false;
}
