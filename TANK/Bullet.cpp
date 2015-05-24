#include "Bullet.h"

#include "Game.h"

void Bullet::setParam(Vec2 pos, Vec2 vec, double radian, Target target) {
	this->pos = pos;
	this->vec = vec;
	this->radian = radian;
	this->target = target;
}

void Bullet::checkOutStage(Game* game) {
	Rect stageSize = game->getStageSize();

	if (pos.x > stageSize.x || pos.x < 0 || pos.y > stageSize.y || pos.y < 0) {
		enable = false;
	}
}

NormalBullet::NormalBullet() {
	damage = 2;
	radius = 5.0;
}

void NormalBullet::move(Game* game) {
	cnt++;
	if (cnt > 10) damage = 1;
	pos += vec;
	checkOutStage(game);
}

void NormalBullet::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	TextureAsset(L"bullet").rotate(radian + Pi/2).drawAt(screenPos);
}

Missile::Missile() {
	damage = 10;
	radius = 10.0;
}

void Missile::move(Game* game) {
	const double sp = 0.50;
	vec += Vec2(Cos(radian) * sp, Sin(radian) * sp);
	pos += vec;
	checkOutStage(game);
}

void Missile::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	TextureAsset(L"missile").scale(2.0).rotate(radian + Pi/2).drawAt(screenPos);
}

void BulletManager::init() {
	bullets.clear();
}

void BulletManager::add(std::shared_ptr<Bullet> bullet) {
	bullets.push_back(bullet);
}

void BulletManager::move(Game* game) {
	for (auto& bullet : bullets) {
		bullet->move(game);
	}
	Erase_if(bullets, [](std::shared_ptr<Bullet> bullet) { return !bullet->isEnabled(); });
	Println(L"BulletNum:", bullets.size());
}

void BulletManager::draw(Game* game) {
	for (auto& bullet : bullets) {
		bullet->draw(game);
	}
}
