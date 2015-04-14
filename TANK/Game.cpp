#include "Game.h"

#include "Player.h"

void Game::init() {
	player = std::make_shared<Player>();
	player->init();
}

void Game::move() {
	player->move();
}

void Game::draw() {
	player->draw();
}
