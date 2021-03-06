#include "Player.h"

#include "Game.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Item.h"
#include "Util.h"

const int Player::HP_MAX = 7;
const double Player::RADUIS = 10.0;

Player::Player() :
hp(0),
pos(0.0, 0.0), vec(Vec2::Zero),
stateCnt(0), missileCnt(0), shotCnt(0), itemCnt(0),
state(State::NORMAL),
radian(0.0), turretRad(0.0),
color(Palette::White),
itemType(ItemType::NONE)
{
	explosionAnimation = std::make_shared<Animation>();
}

void Player::start() {
	hp = HP_MAX;
	pos = Vec2(540.0, 540.0);
	vec = Vec2(0.0, 0.0);
	stateCnt = missileCnt = shotCnt = itemCnt = 0;
	state = State::NORMAL;
	radian = turretRad = 0.0;
	color = Palette::White;
	itemType = ItemType::NONE;
	ruts.clear();
}

void Player::move(Game* game) {
	stateCnt++;

	if (state == State::BURN) {
		game->getCamera2D()->shake(30);
		explosionAnimation->move();
		if (stateCnt == explosionAnimation->getCycleCnt()) {
			game->gameOver();
		}
		return;
	}

	itemCnt++;
	auto itemManager = game->getItemManager();
	for (auto& item : *itemManager) {
		if (Circle(pos, RADUIS * 3).intersects(Circle(item->getPos(), item->RADIUS))) {
			item->kill();
			itemType = item->getItemType();
			itemCnt = 0;
			SoundAsset(L"power_up").playMulti();
		}
	}
	if (itemCnt > 300) {
		itemType = ItemType::NONE;
	}

	if (checkHitEnemyShot(game)) {
		SoundAsset(L"damage").playMulti();
		state = State::DAMAGE;
		stateCnt = 0;
		if (hp <= 0) {
			state = State::BURN;
			explosionAnimation->init(L"explosion", 7, 4);
			SoundAsset(L"burn").playMulti();
		}
	} else if (stateCnt == 5) {
		state = State::NORMAL;
	} else if (state == State::NORMAL && stateCnt % 30 == 0) {
		hp++;
	}
	hp = Clamp(hp, 0, HP_MAX);

	if (state == State::DAMAGE) {
		color = Palette::Red;
	} else {
		color = Palette::White;
	}

	//�O�i���
	const double speed = 7.0;
	vec = Vec2(speed * Cos(radian), speed * Sin(radian));
	if (Input::KeyW.pressed) {
		pos += vec;
	} else if (Input::KeyS.pressed) {
		pos -= vec;
	}

	pos.x = Clamp(pos.x, 0.0, game->getStageRect().x);
	pos.y = Clamp(pos.y, 0.0, game->getStageRect().y);

	//��]
	double turnSpeed = 4.0;
	if (Input::KeyS.pressed) {
		turnSpeed *= -1;
	}
	if (Input::KeyD.pressed) {
		radian += Radians(turnSpeed);
	} else if (Input::KeyA.pressed) {
		radian -= Radians(turnSpeed);
	}

	auto rut = std::make_shared<Rut>();
	rut->pos = pos;
	rut->radian = radian;
	ruts.push_back(rut);

	for (auto& rut : ruts) {
		rut->cnt++;
	}
	Erase_if(ruts, [](std::shared_ptr<Rut> rut) { return rut->cnt > 30; });

	fire(game);
}

void Player::draw(Game* game) {
	for (auto& rut : ruts) {
		TextureAsset(L"rut").rotate(rut->radian + Pi / 2).drawAt(game->getCamera2D()->toScreenPos(rut->pos), Alpha(255 - 255 / 30 * rut->cnt));
	}
	const Vec2 screenPos = game->getCamera2D()->toScreenPos(pos);
	if (state == State::BURN) {
		explosionAnimation->draw(screenPos);
		return;
	}
	TextureAsset(L"playerTank").rotate(radian + Pi/2).drawAt(screenPos, color);
	TextureAsset(L"turret").rotate(turretRad + Pi/2).drawAt(screenPos, color);
}

void Player::fire(Game* game) {
	const Point mousePos = Mouse::Pos();
	const Vec2 offsetPos = game->getCamera2D()->getoffsetPos();
	turretRad = Atan2(mousePos.y - pos.y - offsetPos.y, mousePos.x - pos.x - offsetPos.x);

	shotCnt++;
	if (Input::MouseL.pressed && shotCnt % 3 == 0) {
		auto bulletManager = game->getBulletManager();
		const double shotSpeed = 15.0;

		int num = 1;
		if (itemType == ItemType::INCREASE_SHOT) {
			num = 3;
		} else if (itemType == ItemType::SEPARATE_SHOT) {
			num = 3;
		}

		for (int i = 0; i < num; i++) {
			double shotRad = Atan2(mousePos.y - offsetPos.y - pos.y, mousePos.x - offsetPos.x - pos.x) + Radians(Random(-8.0, 8.0));
			if (itemType == ItemType::SEPARATE_SHOT) {
				shotRad += Radians(-30.0 + 30.0 * i);
			}
			auto bullet = std::make_shared<NormalBullet>();
			bullet->setParam(pos, { Cos(shotRad) * shotSpeed, Sin(shotRad) * shotSpeed }, shotRad, Bullet::Target::ENEMY);
			bulletManager->add(bullet);
		}
		double volume = 0.5;
		if (itemType != ItemType::NONE) {
			volume = 1.0;
		}
		SoundAsset(L"shoot").playMulti(volume);
	}
}

bool Player::checkHitEnemyShot(Game* game) {
	auto bulletManager = game->getBulletManager();
	for (auto& bullet : *bulletManager) {
		if (bullet->getTarget() != Bullet::Target::PLAYER) continue;
		if (Circle(pos, RADUIS).intersects(Circle(bullet->getPos(), bullet->getRadius()))) {
			bullet->kill();
			hp -= bullet->getDamage();
			return true;
		}
	}
	return false;
}
