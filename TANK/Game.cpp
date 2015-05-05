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
	cnt++;

	player->move(this);
	enemyManager->move(this);

	if (cnt % 60 == 0) {
		auto e = std::make_shared<Noob>();
		e->setPos(Vec2(Random(0, stageSize.x),Random(0, stageSize.y)));
		enemyManager->add(e);
	}
}

void Game::draw() {
	offsetPos.set(Window::Width() / 2 - player->getPos().x, Window::Height() / 2 - player->getPos().y);
	offsetPos.x = Min({ offsetPos.x, 0.0 });
	offsetPos.x = Max({ offsetPos.x, (double)Window::Width() - stageSize.x });
	offsetPos.y = Min({ offsetPos.y, 0.0 });
	offsetPos.y = Max({ offsetPos.y, (double)Window::Height() - stageSize.y });

	if (Input::KeyP.pressed) {
		offsetPos.x += Random(-30, 30);
		offsetPos.y += Random(-30, 30);
	}

	TextureAsset(L"backGround").draw(offsetPos.x, offsetPos.y);
	player->draw(this);
	enemyManager->draw(this);
}
