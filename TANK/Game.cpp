#include "Game.h"

#include "Player.h"
#include "Enemy.h"

void Game::init() {
	player = std::make_shared<Player>();
	enemyManager = std::make_shared<EnemyManager>();

	player->init();
	enemyManager->init();
}

void Game::move() {
	player->move();
	enemyManager->move(this);

	if (Input::KeyZ.clicked) {
		auto e = std::make_shared<Noob>();
		e->setPos(Vec2(0,0));
		enemyManager->add(e);
	}
}

void Game::draw() {
	player->draw();
	enemyManager->draw();
}
