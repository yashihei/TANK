#include "Enemy.h"

#include "Game.h"
#include "Player.h"
#include "Bullet.h"

Enemy::Enemy() {
	cnt = 0;
	radian = 0.0;
	enable = true;
	state = State::Normal;
}

//FIXME:������������������������͂�
void Enemy::defaultMove(Game* game) {
	cnt++;

	if (checkShotHit(game)) {
		hp--;
		if (hp == 0) {
			enable = false;
			return;
		}
		cnt = 0;
		state = State::Damage;
	}
	if (cnt == 5) {
		state = State::Normal;
	}
}

bool Enemy::checkShotHit(Game* game) {
	auto& bullets = game->getBulletManager()->getBullets();
	for (auto& bullet : bullets) {
		if (bullet->getType() != Bullet::Type::PLAYER) continue;
		if (Geometry2D::Intersect(Circle(bullet->getPos(), bullet->getRadius()), Circle(pos, radius))) {
			bullet->disable();
			return true;
		}
	}
	return false;
}

Noob::Noob() {
	hp = 20;
	radius = 15.0;
}

void Noob::move(Game* game) {
	const Vec2 playerPos = game->getPlayer()->getPos();
	double sp = 1.0;
	
	//����ɐ��� FIXME:
	//double tRad;
	//double limitRad = Radians(2.0);
	//tRad = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	//double difRad = rad - tRad;
	//if (Abs(difRad) > limitRad) {
	//	rad += (difRad > 0) ? -limitRad : limitRad;
	//} else {
	//	rad = tRad;
	//}
	//Println(L"EnemyRad:", rad);
	
	radian = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	turretRadian = radian;

	if (!Geometry2D::Intersect(Circle(playerPos, 100.0), Circle(pos, radius))) {
		vec.x = Cos(radian) * sp;
		vec.y = Sin(radian) * sp;
		pos += vec;
	}

	if (state == State::Damage) {
		color = Palette::White;
	} else {
		color = Palette::Red;
	}

	if (cnt % 10 == 0) {
		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<Bullet>();
		bullet->init(pos, { Cos(radian) * 8.0, Sin(radian) * 8.0 }, radian, Bullet::Type::ENEMY);
		bulletManager->add(bullet);
	}

	defaultMove(game);
}

void Noob::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);

	if (state == State::Damage) Graphics2D::SetBlendState(BlendState::Additive);
	TextureAsset(L"playerTank").rotate(radian + Radians(90)).drawAt(screenPos, color);
	TextureAsset(L"turret").rotate(turretRadian + Radians(90)).drawAt(screenPos, color);
	Graphics2D::SetBlendState(BlendState::Default);
}

TankDestroyer::TankDestroyer() {
	hp = 30;
	radius = 20.0;
}

void TankDestroyer::move(Game* game) {
	const Vec2 playerPos = game->getPlayer()->getPos();

	radian = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	defaultMove(game);
}

void TankDestroyer::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	auto& tex = TextureAsset(L"SU-152").scale(1.0).rotate(radian + Radians(90));
	if (state == State::Normal) {
		tex.drawAt(screenPos);
	} else {
		tex.drawAt(screenPos, { 255, 0, 0 });//TODO:���t���b�V��
	}
	//Circle(screenPos, radius).draw(Palette::Yellow);//�����蔻��
}

void EnemyManager::init() {
	enemies.clear();
}

void EnemyManager::move(Game* game) {
	for (auto& enemy : enemies) {
		enemy->move(game);
	}
	Erase_if(enemies, [](std::shared_ptr<Enemy> enemy) { return !enemy->isEnable(); });
	Println(L"EnemyNum:", enemies.size());
}

void EnemyManager::draw(Game* game) {
	for (auto& enemy : enemies) {
		enemy->draw(game);
	}
}

void EnemyManager::add(std::shared_ptr<Enemy> e) {
	enemies.push_back(e);
}