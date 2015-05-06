#include "Game.h"

#include "Player.h"
#include "Enemy.h"

void Game::init() {
	TextureAsset::Register(L"playerTank", L"dat/panther.png", TextureDesc::Mipped);
	TextureAsset::Register(L"backGround", L"dat/background.png");

	player = std::make_shared<Player>();
	enemyManager = std::make_shared<EnemyManager>();

	player->init();
	enemyManager->init();

	cnt = 0;
	stageSize.x = TextureAsset(L"backGround").width;
	stageSize.y = TextureAsset(L"backGround").height;

	Graphics2D::SetSamplerState(SamplerState::WrapPoint);
}

void Game::move() {
	ClearPrint();
	Println(L"FPS:", Profiler::FPS());
	Profiler::Graphics().print();
	cnt++;

	player->move(this);
	enemyManager->move(this);

	if (cnt % 60 == 0) {
		auto e = std::make_shared<Noob>();
		e->setPos(Vec2(Random(0, stageSize.x),Random(0, stageSize.y)));
		enemyManager->add(e);
	}
	if (cnt % 300 == 0) {
		auto e = std::make_shared<TankDestroyer>();
		e->setPos(Vec2(Random(0, stageSize.x),Random(0, stageSize.y)));
		enemyManager->add(e);
	}

	offsetPos.set(Window::Width() / 2 - player->getPos().x, Window::Height() / 2 - player->getPos().y);
	offsetPos.x = Clamp(offsetPos.x, static_cast<double>(Window::Width() - stageSize.x), 0.0);
	offsetPos.y = Clamp(offsetPos.y, static_cast<double>(Window::Height() - stageSize.y), 0.0);

	if (Input::KeyP.pressed) {
		offsetPos.x += Random(-30, 30);
		offsetPos.y += Random(-30, 30);
	}
}

void Game::draw() {
	TextureAsset(L"backGround").draw(offsetPos.x, offsetPos.y);
	player->draw(this);
	enemyManager->draw(this);
}
