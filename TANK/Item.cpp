#include "Item.h"

#include "Game.h"
#include "Util.h"

const double Item::RADIUS = 30.0;

Item::Item(Vec2 pos, ItemType type) : cnt(0) {
	this->pos = pos;
	this->type = type;
}

void Item::move(Game* game) {
	cnt++;
	if (cnt > 300) kill();
}

void Item::draw(Game* game) {
	const Vec2 screenPos = game->getCamera2D()->toScreenPos(pos);
	String assetName;
	Color color(Palette::White);

	if (type == ItemType::INCREASE_SHOT) {
		assetName = L"item1";
	}
	if (cnt > 250 && cnt % 10 < 5) {
		color = Palette::Red;
	}
	TextureAsset(assetName).scale(0.5 + Sin(Radians(System::FrameCount()) * 5) / 10).drawAt(screenPos, color);
}

void ItemManager::move(Game* game) {
	Super::move(game);
}

void ItemManager::draw(Game* game) {
	Super::draw(game);
}
