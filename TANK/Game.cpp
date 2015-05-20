#include "Game.h"

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

void Game::init() {
	TextureAsset::Register(L"playerTank", L"dat/panther.png");
	TextureAsset::Register(L"turret", L"dat/turret.png");
	TextureAsset::Register(L"SU-152", L"dat/SU-152.png");
	TextureAsset::Register(L"bullet", L"dat/bullet.png");
	TextureAsset::Register(L"backGround", L"dat/background.png");
	TextureAsset::Register(L"technyan", L"dat/technyan.png");
	TextureAsset::Register(L"explosion", L"dat/explosion.png");
	SoundAsset::Register(L"shoot", L"dat/shoot.wav");
	SoundAsset::Register(L"hit", L"dat/hit.wav");
	SoundAsset::Register(L"damage", L"dat/damage.wav");
	SoundAsset::Register(L"burn", L"dat/burn.wav");
	SoundAsset::Register(L"bgm", L"dat/bgm.mp3");

	player = std::make_shared<Player>();
	enemyManager = std::make_shared<EnemyManager>();
	bulletManager = std::make_shared<BulletManager>();
	camera2D = std::make_shared<Camera2D>();

	player->init();
	enemyManager->init();
	bulletManager->init();

	cnt = 0;
	stageSize.x = TextureAsset(L"backGround").width;
	stageSize.y = TextureAsset(L"backGround").height;

	SoundAsset(L"bgm").play();
	Graphics2D::SetSamplerState(SamplerState::WrapPoint);
}

void Game::move() {
	if (Input::KeySpace.pressed) return;
	ClearPrint();
	Println(L"FPS:", Profiler::FPS());
	Profiler::Graphics().print();
	cnt++;

	camera2D->posUpdate(this);
	player->move(this);
	enemyManager->move(this);
	bulletManager->move(this);

	if (cnt % 180 == 0) {
		auto e = std::make_shared<Technyan>();
		e->setPos(Vec2(Random(0, stageSize.x), Random(0, stageSize.y)));
		enemyManager->add(e);
	}

	if (Input::KeyR.clicked) enemyManager->init();
	if (Input::KeyP.pressed) camera2D->shake(30);
}

void Game::draw() {
	TextureAsset(L"backGround").draw(camera2D->convertToScreenPos({ 0, 0 }));
	player->draw(this);
	enemyManager->draw(this);
	bulletManager->draw(this);
	drawMinimap();
}

void Game::drawMinimap() {
	auto& enemies = enemyManager->getEnemies();
	const Vec2 offset(Window::Width() - 150, 30);
	const Rect mapRect(offset.asPoint(), {120, 120});
	const double scale = static_cast<double>(stageSize.x) / mapRect.w;
	const Vec2 playerPos = player->getPos();

	mapRect.draw({ 0, 122, 255, 122 });
	Circle(playerPos / scale + offset, 2.0).draw(Palette::Yellow);
	for (auto& enemy : enemies) {
		Circle(enemy->getPos() / scale + offset, 2.0).draw(Palette::Red);
		if (enemy->getState() == Enemy::State::Damage)
			Circle(enemy->getPos() / scale + offset, 2.0).draw(Color(255).setAlpha(200));
	}
}

void Camera2D::shake(int num) {
	offsetPos.x += Random(-num, num);
	offsetPos.y += Random(-num, num);
	Println(L"Shake!");
}

Vec2 Camera2D::convertToScreenPos(Vec2 pos) {
	return pos + offsetPos;
}

void Camera2D::posUpdate(Game* game) {
	auto player = game->getPlayer();
	const Rect stageSize = game->getStageSize();

	offsetPos.set(Window::Width() / 2 - player->getPos().x, Window::Height() / 2 - player->getPos().y);
	offsetPos.x = Clamp(offsetPos.x, static_cast<double>(Window::Width() - stageSize.x), 0.0);
	offsetPos.y = Clamp(offsetPos.y, static_cast<double>(Window::Height() - stageSize.y), 0.0);
}

void Animation::init(String name, int sepNum, int sepTime) {
	this->assetName = name;
	this->sepNum = sepNum;
	this->sepTime = sepTime;
	trimSize = Point(TextureAsset(assetName).width / sepNum, TextureAsset(assetName).height);
	cnt = 0;
}

void Animation::move() {
	cnt++;
	if (cnt == sepTime * sepNum) cnt = 0;
}

void Animation::draw(Vec2 pos) {
	int animeNum = cnt / sepTime;
	Rect trimRect(trimSize.x * animeNum, 0, trimSize);
	TextureAsset(assetName)(trimRect).drawAt(pos);
}
