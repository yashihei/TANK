#include "Enemy.h"

#include "Game.h"
#include "Player.h"

void Noob::init() {
	hp = 10;
}

void Noob::move(Game* game) {
	Vec2 playerPos = game->getPlayer()->getPos();
	double sp = 2.0;
	rad = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	vec.set(Cos(rad) * sp, Sin(rad) * sp);
	pos.moveBy(vec);
}

void Noob::draw() {
	Circle(pos, 30.0).draw(Palette::Orange);
}

void EnemyManager::init() {
	enemies.clear();
}

void EnemyManager::move(Game* game) {
	for (auto& enemy : enemies) {
		enemy->move(game);
	}
	//Erase_if(enemies, [](std::shared_ptr<Enemy> enemy) {return });
}

void EnemyManager::draw() {
	for (auto& enemy : enemies) {
		enemy->draw();
	}
}

void EnemyManager::add(std::shared_ptr<Enemy> e) {
	enemies.push_back(e);
}