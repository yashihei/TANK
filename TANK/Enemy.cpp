#include "Enemy.h"

#include "Game.h"
#include "Player.h"

Enemy::Enemy() {
	cnt = 0;
	rad = 0.0;
	enable = true;
}

bool Enemy::checkShotHit(Game* game) {
	auto shots = game->getPlayer()->getShots();
	for (auto& shot : shots) {
		if (Geometry2D::Intersect(Circle(shot->getPos(), shot->RADIUS), Circle(pos, 15))) {
			return true;
		}
	}
	return false;
}

Noob::Noob() {
	hp = 10;
}

void Noob::move(Game* game) {
	const Vec2 playerPos = game->getPlayer()->getPos();
	double sp = 1.0;
	double tRad;
	double limitRad = Radians(2.0);
	
	//����ɐ��� FIXME:
	tRad = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	//double difRad = rad - tRad;
	//if (Abs(difRad) > limitRad) {
	//	rad += (difRad > 0) ? -limitRad : limitRad;
	//} else {
	//	rad = tRad;
	//}
	//Println(L"EnemyRad:", rad);
	
	rad = tRad;

	vec.x = Cos(rad) * sp;
	vec.y = Sin(rad) * sp;
	pos.moveBy(vec);

	if (checkShotHit(game)) enable = false;
}

void Noob::draw(Game* game) {
	int w = 30, h = 60;
	Vec2 screenPos = game->getOffsetPos() + pos;
	RectF(screenPos.x - w / 2, screenPos.y - h / 2, w, h).rotated(rad + Radians(90)).draw(Palette::Red).drawFrame( 1.5 , Palette::White);
}

TankDestroyer::TankDestroyer() {
}

void TankDestroyer::move(Game* game) {
}

void TankDestroyer::draw(Game* game) {
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