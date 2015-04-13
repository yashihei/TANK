#include "Game.h"

#include "Tank.h"

void Game::init() {
	tank = std::make_shared<Player>();
	tank->init();
}

void Game::move() {
	tank->move();
}

void Game::draw() {
	tank->draw();
}
