#include "Bullet.h"

#include "Game.h"

void Bullet::init(Vec2 pos, Vec2 vec, double radian, Type type) {
	this->pos = pos;
	this->vec = vec;
	this->radian = radian;
	this->type = type;
	enable = true;
	cnt = 0;
}

void Bullet::move(Game* game) {
	pos += vec;

	Rect stageSize = game->getStageSize();

	if (pos.x > stageSize.x || pos.x < 0 || pos.y > stageSize.y || pos.y < 0) {
		enable = false;
	}
}

void Bullet::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	TextureAsset(L"bullet").rotate(radian + Radians(90)).drawAt(screenPos);
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
}

void BulletManager::draw(Game* game) {
	for (auto& bullet : bullets) {
		bullet->draw(game);
	}
}
