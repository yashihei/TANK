#include "Item.h"

#include "Game.h"

const double Item::RADIUS = 10.0;

Item::Item(Vec2 pos, Type type) {
	this->pos = pos;
	this->type = type;
}

void Item::move(Game* game) {
	cnt++;
	if (cnt > 300) kill();
}

void Item::draw(Game* game) {
	const Vec2 screenPos = game->getCamera2D()->convertToScreenPos(pos);
	Color color;
	if (type == Type::INCREASE_SHOT) {
		color = Palette::Red;
	} else if (type == Type::SEPARATE_SHOT) {
		color = Palette::Green;
	}
	Circle(screenPos, RADIUS).draw(color);
}

void ItemManager::move(Game* game) {
	Super::move(game);
}

void ItemManager::draw(Game* game) {
	Super::draw(game);
}
