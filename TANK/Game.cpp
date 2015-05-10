#include "Game.h"

#include "Player.h"
#include "Enemy.h"

void Game::init() {
	TextureAsset::Register(L"playerTank", L"dat/panther.png", TextureDesc::Mipped);
	TextureAsset::Register(L"turret", L"dat/turret.png", TextureDesc::Mipped);
	TextureAsset::Register(L"SU-152", L"dat/SU-152.png", TextureDesc::Mipped);
	TextureAsset::Register(L"backGround", L"dat/background.png");

	player = std::make_shared<Player>();
	enemyManager = std::make_shared<EnemyManager>();
	camera2D = std::make_shared<Camera2D>();

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
	camera2D->posUpdate(this);

	if (Input::Key1.pressed) {
		auto e = std::make_shared<Noob>();
		e->setPos(Mouse::Pos());//TODO:À•W
		enemyManager->add(e);
	}
	if (Input::Key2.pressed) {
		auto e = std::make_shared<TankDestroyer>();
		e->setPos(Mouse::Pos());
		enemyManager->add(e);
	}

	if (Input::KeyP.pressed) camera2D->shake();
}

void Game::draw() {
	TextureAsset(L"backGround").draw(camera2D->convertToScreenPos({ 0, 0 }));
	player->draw(this);
	enemyManager->draw(this);
}

void Camera2D::shake() {
	offsetPos.x += Random(-30, 30);
	offsetPos.y += Random(-30, 30);
}

Vec2 Camera2D::convertToScreenPos(Vec2 pos) {
	return pos + offsetPos;
}

void Camera2D::posUpdate(Game* game) {
	auto player = game->getPlayer();
	Point stageSize = game->getStageSize();

	offsetPos.set(Window::Width() / 2 - player->getPos().x, Window::Height() / 2 - player->getPos().y);
	offsetPos.x = Clamp(offsetPos.x, static_cast<double>(Window::Width() - stageSize.x), 0.0);
	offsetPos.y = Clamp(offsetPos.y, static_cast<double>(Window::Height() - stageSize.y), 0.0);
}

