#include "Enemy.h"

#include "Game.h"
#include "Player.h"
#include "Bullet.h"

Enemy::Enemy() {
	cnt = 0;
	radian = 0.0;
	enable = true;
	state = State::Normal;
	explosionAnimation = std::make_shared<Animation>();
}

//FIXME:‚à‚¤­‚µ‚¢‚¢‘‚«•û‚ª‚ ‚é‚Í‚¸
void Enemy::defaultMove(Game* game) {
	cnt++;

	if (state == State::Burn) {
		if (cnt == explosionAnimation->getCycleCnt()) enable = false;
		game->getCamera2D()->shake(15);
		explosionAnimation->move();
		return;
	}
	if (checkShotHit(game)) {
		if (hp <= 0) {
			state = State::Burn;
			explosionAnimation->init(L"explosion", 7, 5);
			SoundAsset(L"burn").playMulti();
			cnt = 0;
			game->addScore(100);
			return;
		}
	}
	if (cnt == 5) {
		state = State::Normal;
	}

	if (state == State::Damage) {
		color = Palette::Red;
	} else {
		color = Palette::White;
	}
}

void Enemy::addDamage(int damage) {
	hp -= damage;
	SoundAsset(L"hit").playMulti();
	cnt = 0;
	state = State::Damage;
}

bool Enemy::checkShotHit(Game* game) {
	auto& bullets = game->getBulletManager()->getBullets();
	for (auto& bullet : bullets) {
		if (bullet->getTarget() == Bullet::Target::PLAYER) continue;
		if (Circle(pos, radius).intersects(Circle(bullet->getPos(), bullet->getRadius()))) {
			bullet->disable();
			addDamage(bullet->getDamage());
			return true;
		}
	}
	return false;
}

Technyan::Technyan() {
	hp = 10;
	radius = 35.0;
}

void Technyan::move(Game* game) {
	defaultMove(game);
	if (state == State::Burn) return;

	const Vec2 playerPos = game->getPlayer()->getPos();
	double sp = 2.0;
	
	//ù‰ñ‚É§ŒÀ FIXME:
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

	if (!Geometry2D::Intersect(Circle(playerPos, 100.0), Circle(pos, radius))) {
		vec.x = Cos(radian) * sp;
		vec.y = Sin(radian) * sp;
		pos += vec;
	}

	if (System::FrameCount() % 7 == 0) {
		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<NormalBullet>();
		const double bulletSp = 15.0;

		bullet->setParam(pos, { Cos(radian) * bulletSp, Sin(radian) * bulletSp }, radian, Bullet::Target::PLAYER);
		bulletManager->add(bullet);
	}
}

void Technyan::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	if (state == State::Burn) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"technyan").scale(0.5).rotate(radian + Pi/2).drawAt(screenPos, color);
}

MissileLauncher::MissileLauncher() {
	hp = 30;
	radius = 30.0;
}

void MissileLauncher::move(Game* game) {
	defaultMove(game);
	if (state == State::Burn) return;
	const Vec2 playerPos = game->getPlayer()->getPos();
	radian = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	if (System::FrameCount() % 80 == 0) {
		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<Missile>();
		const double bulletSp = -5.0;

		bullet->setParam(pos, { Cos(radian) * bulletSp, Sin(radian) * bulletSp }, radian, Bullet::Target::PLAYER);
		bulletManager->add(bullet);
		SoundAsset(L"missile_shoot").playMulti();
	}
}

void MissileLauncher::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	if (state == State::Burn) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"missile_lancher").scale(2.0).rotate(radian + Pi / 2).drawAt(screenPos, color);
}

void EnemyManager::init() {
	enemies.clear();
}

void EnemyManager::move(Game* game) {
	for (auto& enemy : enemies) {
		enemy->move(game);
	}
	Erase_if(enemies, [](std::shared_ptr<Enemy> enemy) { return !enemy->isEnable(); });
}

void EnemyManager::draw(Game* game) {
	for (auto& enemy : enemies) {
		enemy->draw(game);
	}
}

void EnemyManager::add(std::shared_ptr<Enemy> e) {
	enemies.push_back(e);
}
