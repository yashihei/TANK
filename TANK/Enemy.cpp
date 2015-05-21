#include "Enemy.h"

#include "Game.h"
#include "Player.h"
#include "Bullet.h"

Enemy::Enemy() {
	cnt = 0;
	radian = 0.0;
	enable = true;
	state = State::Normal;
	explosionAnimation = std::make_shared<Animation>();
}

//FIXME:‚à‚¤­‚µ‚¢‚¢‘‚«•û‚ª‚ ‚é‚Í‚¸
void Enemy::defaultMove(Game* game) {
	cnt++;

	if (state == State::Burn) {
		if (cnt == 32) enable = false;
		game->getCamera2D()->shake(15);
		explosionAnimation->move();
		return;
	}
	if (checkShotHit(game)) {
		hp--;
		SoundAsset(L"hit").playMulti();
		if (hp == 0) {
			state = State::Burn;
			explosionAnimation->init(L"explosion", 7, 4);
			SoundAsset(L"burn").playMulti();
			cnt = 0;
			return;
		}
		cnt = 0;
		state = State::Damage;
	}
	if (cnt == 5) {
		state = State::Normal;
	}
}

bool Enemy::checkShotHit(Game* game) {
	auto& bullets = game->getBulletManager()->getBullets();
	for (auto& bullet : bullets) {
		if (bullet->getTarget() == Bullet::Target::PLAYER) continue;
		if (Geometry2D::Intersect(Circle(bullet->getPos(), bullet->getRadius()), Circle(pos, radius))) {
			bullet->disable();
			return true;
		}
	}
	return false;
}

Technyan::Technyan() {
	hp = 20;
	radius = 35.0;
}

void Technyan::move(Game* game) {
	defaultMove(game);

	const Vec2 playerPos = game->getPlayer()->getPos();
	double sp = 2.0;
	
	//ù‰ñ‚É§ŒÀ FIXME:
	//double tRad;
	//double limitRad = Radians(2.0);
	//tRad = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	//double difRad = rad - tRad;
	//if (Abs(difRad) > limitRad) {
	//	rad += (difRad > 0) ? -limitRad : limitRad;
	//} else {
	//	rad = tRad;
	//}
	//Println(L"EnemyRad:", rad);
	
	radian = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	if (!Geometry2D::Intersect(Circle(playerPos, 100.0), Circle(pos, radius))) {
		vec.x = Cos(radian) * sp;
		vec.y = Sin(radian) * sp;
		pos += vec;
	}

	if (state == State::Damage) {
		color = Palette::Red;
	} else {
		color = Palette::White;
	}

	if (System::FrameCount() % 10 == 0) {
		auto bulletManager = game->getBulletManager();
		auto bullet = std::make_shared<NormalBullet>();
		const double bulletSp = 8.0;

		bullet->init(pos, { Cos(radian) * bulletSp, Sin(radian) * bulletSp }, radian, Bullet::Target::PLAYER);
		bulletManager->add(bullet);
	}
}

void Technyan::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	if (state == State::Burn) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"technyan").scale(0.5).rotate(radian + Pi/2).drawAt(screenPos, color);
	//Circle(screenPos, radius).draw();
}

TankDestroyer::TankDestroyer() {
	hp = 30;
	radius = 20.0;
}

void TankDestroyer::move(Game* game) {
	const Vec2 playerPos = game->getPlayer()->getPos();

	radian = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	defaultMove(game);
}

void TankDestroyer::draw(Game* game) {
	Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	auto& tex = TextureAsset(L"SU-152").scale(1.0).rotate(radian + Pi/2);
	if (state == State::Normal) {
		tex.drawAt(screenPos);
	} else {
		tex.drawAt(screenPos, { 255, 0, 0 });//TODO:”’ƒtƒ‰ƒbƒVƒ…
	}
	//Circle(screenPos, radius).draw(Palette::Yellow);//“–‚½‚è”»’è
}

void EnemyManager::init() {
	enemies.clear();
}

void EnemyManager::move(Game* game) {
	for (auto& enemy : enemies) {
		enemy->move(game);
	}
	Erase_if(enemies, [](std::shared_ptr<Enemy> enemy) { return !enemy->isEnable(); });
	Println(L"EnemyNum:", enemies.size());
}

void EnemyManager::draw(Game* game) {
	for (auto& enemy : enemies) {
		enemy->draw(game);
	}
}

void EnemyManager::add(std::shared_ptr<Enemy> e) {
	enemies.push_back(e);
}