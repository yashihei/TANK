#include "Enemy.h"

#include "Game.h"
#include "Player.h"
#include "Bullet.h"
#include "Util.h"

Enemy::Enemy() :
state(State::NORMAL),
pos(0.0, 0.0), vec(Vec2::Zero),
hp(0), stateCnt(0), fireCnt(0),
radian(0.0), radius(0.0),
color(Palette::White), minimapColor(Palette::White)
{
	explosionAnimation = std::make_shared<Animation>();
}

//FIXME:‚à‚¤­‚µ‚¢‚¢‘‚«•û‚ª‚ ‚é‚Í‚¸
void Enemy::move(Game* game) {
	stateCnt++;
	fireCnt++;

	if (state == State::BURN) {
		if (stateCnt == explosionAnimation->getCycleCnt()) kill();
		game->getCamera2D()->shake(15);
		explosionAnimation->move();
		return;
	}
	if (checkShotHit(game)) {
		if (hp <= 0) {
			state = State::BURN;
			explosionAnimation->init(L"explosion", 7, 5);
			SoundAsset(L"burn").playMulti();
			stateCnt = 0;
			game->addScore(100);
			return;
		}
	}
	if (stateCnt == 5) {
		state = State::NORMAL;
	}

	if (state == State::DAMAGE) {
		color = Palette::Red;
	} else {
		color = Palette::White;
	}
}

void Enemy::addDamage(int damage) {
	hp -= damage;
	SoundAsset(L"hit").playMulti();
	stateCnt = 0;
	state = State::DAMAGE;
}

bool Enemy::checkShotHit(Game* game) {
	auto bulletManager = game->getBulletManager();
	for (auto bullet : *bulletManager) {
		if (bullet->getTarget() != Bullet::Target::ENEMY) continue;
		if (Circle(pos, radius).intersects(Circle(bullet->getPos(), bullet->getRadius()))) {
			bullet->kill();
			addDamage(bullet->getDamage());
			return true;
		}
	}
	return false;
}

T3485::T3485() : turretRad(0.0) {
	hp = 10;
	radius = 30.0;
	minimapColor = Palette::Red;
	turnRad = Radians(3.0);
}

void T3485::move(Game* game) {
	Super::move(game);
	if (state == State::BURN) return;

	const Vec2 playerPos = game->getPlayer()->getPos();
	const double sp = 3.0;
	
	turretRad = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	if (Random(0, 60) == 0) turnRad *= -1;
	radian += turnRad;

	vec.x = Cos(radian) * sp;
	vec.y = Sin(radian) * sp;
	pos += vec;
	pos.x = Clamp(pos.x, 0.0, static_cast<double>(game->getStageSize().x));
	pos.y = Clamp(pos.y, 0.0, static_cast<double>(game->getStageSize().y));

	if (fireCnt % 7 == 0 && fireCnt % 100 > 50 && Circle(pos, 500).intersects(playerPos)) {
		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<NormalBullet>();
		const double bulletSp = 15.0;
		const double shotRad = turretRad;

		bullet->setParam(pos, { Cos(shotRad) * bulletSp, Sin(shotRad) * bulletSp }, shotRad, Bullet::Target::PLAYER);
		bulletManager->add(bullet);
	}
}

void T3485::draw(Game* game) {
	const Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	if (state == State::BURN) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"T-34-85").rotate(radian + Pi/2).drawAt(screenPos, color);
	TextureAsset(L"T-34-85_turret").rotate(turretRad + Pi/2).drawAt(screenPos, color);
}

MissileLauncher::MissileLauncher() {
	hp = 15;
	radius = 30.0;
	minimapColor = Palette::Orange;
}

void MissileLauncher::move(Game* game) {
	Super::move(game);
	if (state == State::BURN) return;
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
	if (state == State::BURN) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"missile_lancher").scale(1.4).rotate(radian + Pi / 2).drawAt(screenPos, color);
}

void EnemyManager::move(Game* game) {
	Super::move(game);
}

void EnemyManager::draw(Game* game) {
	Super::draw(game);
}
