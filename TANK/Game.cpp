#include "Game.h"

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Item.h"
#include "Util.h"

Game::Game() :
state(State::TITLE),
stateCnt(0), inGameCnt(0),
score(0), hiScore(0)
{
	TextureAsset::Register(L"playerTank", L"dat/panther.png");
	TextureAsset::Register(L"turret", L"dat/turret.png");
	TextureAsset::Register(L"bullet", L"dat/bullet.png");
	TextureAsset::Register(L"backGround", L"dat/background.png");
	TextureAsset::Register(L"explosion", L"dat/explosion.png");
	TextureAsset::Register(L"missile", L"dat/missile.png");
	TextureAsset::Register(L"missile_lancher", L"dat/missile_launcher.png");
	TextureAsset::Register(L"ring", L"dat/ring.png");
	TextureAsset::Register(L"marker", L"dat/marker.png");
	TextureAsset::Register(L"title", L"dat/title.png");
	TextureAsset::Register(L"gameOver", L"dat/gameover.png");
	TextureAsset::Register(L"T-34-85", L"dat/T-34-85.png");
	TextureAsset::Register(L"T-34-85_turret", L"dat/T-34-85_turret.png");
	TextureAsset::Register(L"T-34-85_r", L"dat/T-34-85_r.png");
	TextureAsset::Register(L"T-34-85_turret_r", L"dat/T-34-85_turret_r.png");
	TextureAsset::Register(L"rut", L"dat/rut.png");
	TextureAsset::Register(L"item1", L"dat/item1.png");
	SoundAsset::Register(L"shoot", L"dat/shoot.wav");
	SoundAsset::Register(L"missile_shoot", L"dat/missile_shoot.wav");
	SoundAsset::Register(L"hit", L"dat/hit.wav");
	SoundAsset::Register(L"damage", L"dat/damage.wav");
	SoundAsset::Register(L"burn", L"dat/burn.wav");
	SoundAsset::Register(L"power_up", L"dat/power_up.wav");
	SoundAsset::Register(L"bgm", L"dat/bgm.mp3");
	FontAsset::Register(L"score", 20, Typeface::Black);
	FontAsset::Register(L"gameover", 40, Typeface::Black);

	player = std::make_shared<Player>();
	enemyManager = std::make_shared<EnemyManager>();
	bulletManager = std::make_shared<BulletManager>();
	itemManager = std::make_shared<ItemManager>();
	camera2D = std::make_shared<Camera2D>();

	SoundAsset(L"bgm").setLoop(true);
	SoundAsset(L"missile_shoot").setSpeed(1.5);
	SoundAsset(L"burn").setVolume(0.5);
	MasterVoice::SetVolume(0.5);
	Graphics2D::SetSamplerState(SamplerState::WrapPoint);
	stageSize.x = TextureAsset(L"backGround").width;
	stageSize.y = TextureAsset(L"backGround").height;
}

void Game::move() {
	if (Input::KeySpace.pressed) return;
	ClearPrint();
	stateCnt++;

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
		itemManager->move(this);
		createActors();
		break;
	case State::GAME_OVER:
		enemyManager->move(this);
		bulletManager->move(this);
		if (Input::MouseL.clicked || stateCnt > 300) {
			SoundAsset(L"bgm").stop();
			state = State::TITLE;
		}
		break;
	}

	if (Input::KeyR.clicked) enemyManager->clear();
	if (Input::KeyP.pressed) camera2D->shake(30);
}

Vec2 Game::makeRandomPos() {
	Vec2 randomPos;
	while (true) {
		randomPos = RandomVec2(getStageSize().x, getStageSize().y);
		if (!Geometry2D::Intersect(getPlayer()->getPos().asPoint(), Circle(randomPos, 200.0)))
			break;
	}
	return randomPos;
}

void Game::createActors() {
	//Println(200 - Min(inGameCnt / 30, 150));
	//Println(600 - Min(inGameCnt / 15, 300));
	//Println(1000 - Min(inGameCnt / 10, 600));
	//if (inGameCnt % (200 - Min(inGameCnt / 30, 150)) == 0) {
	//}
	//if (inGameCnt % (600 - Min(inGameCnt / 15, 300)) == 0) {
	//}
	//if (inGameCnt % (1000 - Min(inGameCnt / 10, 600)) == 0) {
	//}
	inGameCnt++;
	if (inGameCnt % 60 == 0) {
		auto e = std::make_shared<T3485>();
		e->setPos(makeRandomPos());
		enemyManager->add(e);
	}
	if (inGameCnt % 400 == 0) {
		auto e = std::make_shared<MissileLauncher>();
		e->setPos(makeRandomPos());
		enemyManager->add(e);
	}
	if (inGameCnt % 500 == 0) {
		ItemType t = RandomSelect({ ItemType::INCREASE_SHOT });
		auto i = std::make_shared<Item>(makeRandomPos(), t);
		itemManager->add(i);
	}
}

void Game::gameStart() {
	player->start();
	enemyManager->clear();
	bulletManager->clear();
	itemManager->clear();
	SoundAsset(L"bgm").play();
	state = State::PLAY;
	score = 0;
	inGameCnt = 0;
}

void Game::gameOver() {
	state = State::GAME_OVER;
	camera2D->posUpdate(this);
	stateCnt = 0;
	hiScore = Max(score, hiScore);
}

void Game::draw() {
	switch (state) {
	case State::TITLE:
		TextureAsset(L"backGround").draw(camera2D->toScreenPos({ 0, 0 }));
		enemyManager->draw(this);
		bulletManager->draw(this);
		drawMarker();
		TextureAsset(L"title").rotate(Radians(Random(-1.0, 1.0))).drawAt(Window::Center());
		break;
	case State::PLAY:
		TextureAsset(L"backGround").draw(camera2D->toScreenPos({ 0, 0 }));
		itemManager->draw(this);
		player->draw(this);
		enemyManager->draw(this);
		bulletManager->draw(this);
		drawHUD();
		break;
	case State::GAME_OVER:
		TextureAsset(L"backGround").draw(camera2D->toScreenPos({ 0, 0 }));
		enemyManager->draw(this);
		bulletManager->draw(this);
		drawHUD();
		drawCenterAddShadowFont(L"gameover", L"げーむおーばー", Window::Center());
		drawCenterAddShadowFont(L"score", Format(L"すこあ：", score), Window::Center() + Vec2(0.0, 150.0));
		drawCenterAddShadowFont(L"score", Format(L"はいすこあ：", hiScore), Window::Center() + Vec2(0.0, 180.0));
		break;
	}
}

void Game::drawHUD() {
	drawMarker();
	drawMinimap();
	drawHpCircle();
	FontAsset(L"score").draw(Pad(score, { 6, L'0' }), { 12.0, 12.0 }, Palette::Black);
	FontAsset(L"score").draw(Pad(score, { 6, L'0' }), { 10.0, 10.0 }, Palette::White);
}

void Game::drawMarker() {
	if (Input::MouseL.pressed) {
		TextureAsset(L"marker")(31, 0, 31, 31).scale(1.3).drawAt(Mouse::Pos());
	} else {
		TextureAsset(L"marker")(0, 0, 31, 31).scale(1.3).drawAt(Mouse::Pos());
	}
}

void Game::drawMinimap() {
	const Vec2 offset(Window::Width() - 150, 30);
	const Rect mapRect(offset.asPoint(), {120, 120});
	const double scale = static_cast<double>(stageSize.x) / mapRect.w;
	const Vec2 playerPos = player->getPos();

	mapRect.draw({ 0, 122, 255, 122 });
	Circle(playerPos / scale + offset, 2.0).draw(Palette::Yellow);
	for (auto& enemy : *enemyManager) {
		if (enemy->getState() == Enemy::State::BURN) continue;

		Color color = enemy->getMinimapColor();
		if (enemy->getState() == Enemy::State::DAMAGE) {
			color = Palette::White;
		}
		Circle(enemy->getPos() / scale + offset, 2.0).draw(color);
	}

	for (auto& item : *itemManager) {
		Circle(item->getPos() / scale + offset, 2.0).draw(Palette::Greenyellow);
	}
}

void Game::drawHpCircle() {
	const int playerHp = player->getHp();
	const Vec2 screenPos = camera2D->toScreenPos(player->getPos());
	if (playerHp < Player::HP_MAX)
		Circle(screenPos, (500 / Player::HP_MAX) * playerHp).drawFrame(0.0, 1000.0, Color(255, 30, 30).setAlpha(70));
}

void Game::drawCenterAddShadowFont(String assetName, String drawStr, Vec2 pos) {
	FontAsset(assetName).drawCenter(drawStr, pos + Vec2(2.0, 2.0), Palette::Black);
	FontAsset(assetName).drawCenter(drawStr, pos, Palette::White);
}
