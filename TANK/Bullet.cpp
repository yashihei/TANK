#include "Bullet.h"

#include "Game.h"
#include "Util.h"

Bullet::Bullet() :
pos(0.0, 0.0), vec(Vec2::Zero),
radian(0.0), radius(0.0),
damage(0), cnt(0),
target(Target::NONE)
{
}

void Bullet::setParam(Vec2 pos, Vec2 vec, double radian, Target target) {
	this->pos = pos;
	this->vec = vec;
	this->radian = radian;
	this->target = target;
}

void Bullet::checkOutStage(Game* game) {
	Rect stageSize = game->getStageSize();
	const int margin = 50;

	if (pos.x > stageSize.x + margin || pos.x < 0 - margin || pos.y > stageSize.y + margin || pos.y < 0 - margin) {
		enable = false;
	}
}

NormalBullet::NormalBullet() {
	damage = 1;
	radius = 5.0;
}

void NormalBullet::move(Game* game) {
	pos += vec;
	checkOutStage(game);
}

void NormalBullet::draw(Game* game) {
	const Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	TextureAsset(L"bullet").rotate(radian + Pi/2).drawAt(screenPos);
}

Missile::Missile() {
	damage = 15;
	radius = 10.0;
	missileAnimation = std::make_shared<Animation>();
	missileAnimation->init(L"missile", 5, 8);
}

void Missile::move(Game* game) {
	const double sp = 1.0;
	vec += Vec2(Cos(radian) * sp, Sin(radian) * sp);
	pos += vec;
	checkOutStage(game);
	missileAnimation->move();
}

void Missile::draw(Game* game) {
	const Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	for (int i = 20; i > 0; i--) {
		missileAnimation->draw(screenPos - (vec / 3 * i), radian, 100 - 100 / 20 * i);
	}
	missileAnimation->draw(screenPos, radian);
}

Bomb::Bomb() {
	damage = 100;
	radius = 200;
	explosionAnimation = std::make_shared<Animation>();
	explosionAnimation->init(L"explosion", 7, 5);
}

void Bomb::move(Game* game) {
	cnt++;
}

void Bomb::draw(Game* game) {
	const Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	Circle(screenPos, 10.0).draw(Color(255, 255, 0, 122));
}

void BulletManager::move(Game* game) {
	Super::move(game);
}

void BulletManager::draw(Game* game) {
	Super::draw(game);
}
