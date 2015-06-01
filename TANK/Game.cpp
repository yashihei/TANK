#include "Game.h"

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

void Game::init() {
	TextureAsset::Register(L"playerTank", L"dat/panther.png");
	TextureAsset::Register(L"turret", L"dat/turret.png");
	TextureAsset::Register(L"bullet", L"dat/bullet.png");
	TextureAsset::Register(L"backGround", L"dat/background.png");
	TextureAsset::Register(L"technyan", L"dat/technyan.png");
	TextureAsset::Register(L"explosion", L"dat/explosion.png");
	TextureAsset::Register(L"missile", L"dat/missile.png");
	TextureAsset::Register(L"missile_lancher", L"dat/missile_launcher.png");
	TextureAsset::Register(L"marker", L"dat/marker.png");
	TextureAsset::Register(L"title", L"dat/title.png");
	TextureAsset::Register(L"gameOver", L"dat/gameover.png");
	SoundAsset::Register(L"shoot", L"dat/shoot.wav");
	SoundAsset::Register(L"missile_shoot", L"dat/missile_shoot.wav");
	SoundAsset::Register(L"hit", L"dat/hit.wav");
	SoundAsset::Register(L"damage", L"dat/damage.wav");
	SoundAsset::Register(L"burn", L"dat/burn.wav");
	SoundAsset::Register(L"bgm", L"dat/bgm.mp3");
	FontAsset::Register(L"score", 20, Typeface::Black);

	player = std::make_shared<Player>();
	enemyManager = std::make_shared<EnemyManager>();
	bulletManager = std::make_shared<BulletManager>();
	camera2D = std::make_shared<Camera2D>();

	player->init();
	enemyManager->init();
	bulletManager->init();

	SoundAsset(L"bgm").setLoop(true);
	SoundAsset(L"missile_shoot").setSpeed(1.5);
	Graphics2D::SetSamplerState(SamplerState::WrapPoint);
	stageSize.x = TextureAsset(L"backGround").width;
	stageSize.y = TextureAsset(L"backGround").height;
}

void Game::move() {
	if (Input::KeySpace.pressed) return;
	cnt++;

	switch (state) {
	case State::TITLE:
		if (Input::MouseL.clicked) gameStart();
		enemyManager->move(this);
		bulletManager->move(this);
		break;
	case State::PLAY:
		camera2D->posUpdate(this);
		player->move(this);
		enemyManager->move(this);
		bulletManager->move(this);
		makeEnemies();
		break;
	case State::GAME_OVER:
		enemyManager->move(this);
		bulletManager->move(this);
		if (Input::MouseL.clicked || cnt > 300) {
			SoundAsset(L"bgm").stop();
			state = State::TITLE;
		}
		break;
	}

	if (Input::KeyR.clicked) enemyManager->init();
	if (Input::KeyP.pressed) camera2D->shake(30);
}

void Game::gameStart() {
	player->init();
	enemyManager->init();
	bulletManager->init();
	SoundAsset(L"bgm").play();
	state = State::PLAY;
	score = 0;
}

void Game::gameOver() {
	state = State::GAME_OVER;
	camera2D->posUpdate(this);
	cnt = 0;
}

Vec2 Game::makeRandomPos() {
	Vec2 randomPos;
	while (true) {
		randomPos = Vec2(Random(0, stageSize.x), Random(0, stageSize.y));
		if (!Geometry2D::Intersect(player->getPos().asPoint(), Circle(randomPos, 150.0)))
			break;
	}
	return randomPos;
}

void Game::makeEnemies() {
	if (System::FrameCount() % 90 == 0) {
		auto e = std::make_shared<Technyan>();
		e->setPos(makeRandomPos());
		enemyManager->add(e);
	}
	if (System::FrameCount() % 400 == 0) {
		auto e = std::make_shared<MissileLauncher>();
		e->setPos(makeRandomPos());
		enemyManager->add(e);
	}
}

void Game::draw() {
	switch (state) {
	case State::TITLE:
		TextureAsset(L"backGround").draw(camera2D->convertToScreenPos({ 0, 0 }));
		enemyManager->draw(this);
		bulletManager->draw(this);
		TextureAsset(L"title").drawAt(Window::Center());
		break;
	case State::PLAY:
		TextureAsset(L"backGround").draw(camera2D->convertToScreenPos({ 0, 0 }));
		player->draw(this);
		enemyManager->draw(this);
		bulletManager->draw(this);
		drawHUD();
		break;
	case State::GAME_OVER:
		TextureAsset(L"backGround").draw(camera2D->convertToScreenPos({ 0, 0 }));
		enemyManager->draw(this);
		bulletManager->draw(this);
		TextureAsset(L"gameOver").drawAt(Window::Center());
		break;
	}
}

void Game::drawHUD() {
	if (Input::MouseL.pressed) {
		TextureAsset(L"marker")(31, 0, 31, 31).scale(1.3).drawAt(Mouse::Pos());
	} else {
		TextureAsset(L"marker")(0, 0, 31, 31).scale(1.3).drawAt(Mouse::Pos());
	}
	drawMinimap();
	drawHpCircle();
	FontAsset(L"score").draw(Pad(score, { 6, L'0' }), { 11.5, 11.5 }, Palette::Black);
	FontAsset(L"score").draw(Pad(score, { 6, L'0' }), { 10.0, 10.0 }, Palette::White);
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
		if (enemy->getState() == Enemy::State::Burn) continue;

		Color color(Palette::Red);
		if (typeid(*enemy.get()) == typeid(MissileLauncher)) {
			color = Palette::Greenyellow;
		}
		if (enemy->getState() == Enemy::State::Damage) {
			color = Palette::White;
		}
		Circle(enemy->getPos() / scale + offset, 2.0).draw(color);
	}
}

void Game::drawHpCircle() {
	const int playerHp = player->getHp();
	const Vec2 screenPos = camera2D->convertToScreenPos(player->getPos());
	if (playerHp < Player::HP_MAX)
		Circle(screenPos, (500 / Player::HP_MAX) * playerHp).drawFrame(0.0, 1000.0, Color(255, 30, 30).setAlpha(70));
}

void Camera2D::shake(int num) {
	offsetPos.x += Random(-num, num);
	offsetPos.y += Random(-num, num);
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

void Animation::init(String name, int sepNum, int intervalCnt) {
	this->assetName = name;
	this->sepNum = sepNum;
	this->intervalCnt = intervalCnt;
	trimSize = Point(TextureAsset(assetName).width / sepNum, TextureAsset(assetName).height);
	trimRect = Rect(0, 0, trimSize);
}

void Animation::move() {
	cnt++;
	if (cnt == intervalCnt * sepNum) cnt = 0;
	trimRect = Rect(trimSize.x * (cnt / intervalCnt), 0, trimSize);
}

void Animation::draw(Vec2 pos) {
	TextureAsset(assetName)(trimRect).drawAt(pos);
}

void Animation::draw(Vec2 pos, double radian) {
	TextureAsset(assetName)(trimRect).rotate(radian + Radians(90)).drawAt(pos);
}
