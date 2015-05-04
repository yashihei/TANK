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
		if (Geometry2D::Intersect(Circle(shot->getPos(), shot->Radius), Circle(pos, 20))) {
			return true;
		}
	}
	return false;
}

void Noob::init() {
	hp = 10;
}

void Noob::move(Game* game) {
	const Vec2 playerPos = game->getPlayer()->getPos();
	double sp = 1.5;
	double tRad;
	double limitRad = Radians(2.0);
	
	//ù‰ñ‚É§ŒÀ FIXME:
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

void Noob::draw() {
	//Circle(pos, 20.0).draw(Palette::Orange);
	int w = 30, h = 60;
	RectF(pos.x - w / 2, pos.y - h / 2, w, h).rotated(rad + Radians(90)).draw(Palette::Red).drawFrame( 1.5 , Palette::White);
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

void EnemyManager::draw() {
	for (auto& enemy : enemies) {
		enemy->draw();
	}
}

void EnemyManager::add(std::shared_ptr<Enemy> e) {
	enemies.push_back(e);
}