#include "Game.h"

#include "Player.h"
#include "Enemy.h"

void Game::init() {
	player = std::make_shared<Player>();
	enemyManager = std::make_shared<EnemyManager>();

	player->init();
	enemyManager->init();

	cnt = 0;
}

void Game::move() {
	ClearPrint();
	Println(L"FPS:", Profiler::FPS());
	cnt++;
	player->move();
	enemyManager->move(this);

	if (cnt % 20 == 0) {
		auto e = std::make_shared<Noob>();
		e->init();
		e->setPos(Vec2(Random(0, Window::Width()),Random(0, Window::Height())));
		enemyManager->add(e);
	}
}

void Game::draw() {
	player->draw();
	enemyManager->draw();
}
