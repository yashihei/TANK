#include "Enemy.h"

#include "Game.h"
#include "Player.h"
#include "Bullet.h"

Enemy::Enemy() {
	explosionAnimation = std::make_shared<Animation>();
}

//FIXME:‚à‚¤­‚µ‚¢‚¢‘‚«•û‚ª‚ ‚é‚Í‚¸
void Enemy::defaultMove(Game* game) {
	stateCnt++;
	fireCnt++;

	if (state == State::Burn) {
		if (stateCnt == explosionAnimation->getCycleCnt()) enabled = false;
		game->getCamera2D()->shake(15);
		explosionAnimation->move();
		return;
	}
	if (checkShotHit(game)) {
		if (hp <= 0) {
			state = State::Burn;
			explosionAnimation->init(L"explosion", 7, 5);
			SoundAsset(L"burn").playMulti();
			stateCnt = 0;
			game->addScore(100);
			return;
		}
	}
	if (stateCnt == 5) {
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
	stateCnt = 0;
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
	minimapColor = Palette::Red;
}

void Technyan::move(Game* game) {
	defaultMove(game);
	if (state == State::Burn) return;

	const Vec2 playerPos = game->getPlayer()->getPos();
	const double sp = 2.0;
	
	radian = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	if (!Geometry2D::Intersect(Circle(playerPos, 100.0), Circle(pos, radius))) {
		vec.x = Cos(radian) * sp;
		vec.y = Sin(radian) * sp;
		pos += vec;
	}

	if (fireCnt % 7 == 0) {
		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<NormalBullet>();
		const double bulletSp = 15.0;
		const double shotRad = radian;

		bullet->setParam(pos, { Cos(shotRad) * bulletSp, Sin(shotRad) * bulletSp }, shotRad, Bullet::Target::PLAYER);
		bulletManager->add(bullet);
	}
}

void Technyan::draw(Game* game) {
	const Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	if (state == State::Burn) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"technyan").scale(0.5).rotate(radian + Pi/2).drawAt(screenPos, color);
}

MissileLauncher::MissileLauncher() {
	hp = 30;
	radius = 30.0;
	minimapColor = Palette::Greenyellow;
}

void MissileLauncher::move(Game* game) {
	defaultMove(game);
	if (state == State::Burn) return;
	const Vec2 playerPos = game->getPlayer()->getPos();
	radian = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	if (fireCnt % 80 == 0) {
		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<Missile>();
		const double bulletSp = -5.0;

		bullet->setParam(pos, { Cos(radian) * bulletSp, Sin(radian) * bulletSp }, radian, Bullet::Target::PLAYER);
		bulletManager->add(bullet);
		SoundAsset(L"missile_shoot").playMulti();
	}
}

void MissileLauncher::draw(Game* game) {
	const Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	if (state == State::Burn) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"missile_lancher").scale(2.0).rotate(radian + Pi / 2).drawAt(screenPos, color);
}

void EnemyManager::init() {
	enemies.clear();
	cnt = 0;
}

Vec2 EnemyManager::makeRandomPos(Game* game) {
	Vec2 randomPos;
	while (true) {
		randomPos = Vec2(Random(0, game->getStageSize().x), Random(0, game->getStageSize().y));
		if (!Geometry2D::Intersect(game->getPlayer()->getPos().asPoint(), Circle(randomPos, 150.0)))
			break;
	}
	return randomPos;
}

void EnemyManager::move(Game* game) {
	for (auto& enemy : enemies) {
		enemy->move(game);
	}
	Erase_if(enemies, [](std::shared_ptr<Enemy> enemy) { return !enemy->isEnabled(); });

	cnt++;
	if (cnt % 90 == 0) {
		auto e = std::make_shared<Technyan>();
		e->setPos(makeRandomPos(game));
		add(e);
	}
	if (cnt % 400 == 0) {
		auto e = std::make_shared<MissileLauncher>();
		e->setPos(makeRandomPos(game));
		add(e);
	}
}

void EnemyManager::draw(Game* game) {
	for (auto& enemy : enemies) {
		enemy->draw(game);
	}
}

void EnemyManager::add(std::shared_ptr<Enemy> e) {
	enemies.push_back(e);
}
