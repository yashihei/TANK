#include "Enemy.h"

#include "Game.h"
#include "Player.h"

Enemy::Enemy() {
	cnt = 0;
	rad = 0.0;
	enable = true;
	state = State::Normal;
}

//FIXME:‚à‚¤­‚µ‚¢‚¢‘‚«•û‚ª‚ ‚é‚Í‚¸
void Enemy::defaultMove(Game* game) {
	cnt++;

	if (checkShotHit(game)) {
		hp--;
		if (hp == 0) {
			enable = false;
			return;
		}
		cnt = 0;
		state = State::Damage;
	}
	if (state == State::Damage && cnt == 5) {
		state = State::Normal;
	}
}

bool Enemy::checkShotHit(Game* game) {
	auto shots = game->getPlayer()->getShots();//FIXME:‚È‚ñ‚©‚±‚±•‰’S‚©‚©‚Á‚Ä‚¢‚é
	for (auto& shot : shots) {
		if (Geometry2D::Intersect(Circle(shot->getPos(), shot->RADIUS), Circle(pos, radius))) {
			shot->disable();
			return true;
		}
	}
	return false;
}

Noob::Noob() {
	hp = 5;
	radius = 15.0;
}

void Noob::move(Game* game) {
	const Vec2 playerPos = game->getPlayer()->getPos();
	double sp = 1.0;
	double tRad;
	double limitRad = Radians(2.0);
	
	//ù‰ñ‚É§ŒÀ FIXME:
	tRad = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	//double difRad = rad - tRad;
	//if (Abs(difRad) > limitRad) {
	//	rad += (difRad > 0) ? -limitRad : limitRad;
	//} else {
	//	rad = tRad;
	//}
	//Println(L"EnemyRad:", rad);
	
	rad = tRad;

	vec.x = Cos(rad) * sp;
	vec.y = Sin(rad) * sp;
	pos.moveBy(vec);

	if (state == State::Damage) {
		color = Palette::White;
	} else {
		color = Palette::Red;
	}

	defaultMove(game);
}

void Noob::draw(Game* game) {
	int w = 30, h = 60;
	Vec2 screenPos = game->getOffsetPos() + pos;
	RectF(screenPos.x - w / 2, screenPos.y - h / 2, w, h).rotated(rad + Radians(90)).draw(color).drawFrame( 1.5 , Palette::White);
}

TankDestroyer::TankDestroyer() {
	hp = 10;
	radius = 20.0;
}

void TankDestroyer::move(Game* game) {
	const Vec2 playerPos = game->getPlayer()->getPos();

	rad = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);
	if (state == State::Damage) {
		color = Palette::White;
	} else {
		color = Palette::Blue;
	}

	defaultMove(game);
}

void TankDestroyer::draw(Game* game) {
	Vec2 screenPos = game->getOffsetPos() + pos;
	Triangle(screenPos, 60.0).rotated(rad + Radians(90)).draw(color).drawFrame(1.5, Palette::White);
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