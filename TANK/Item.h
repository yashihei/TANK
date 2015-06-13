#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <memory>
#include "Actor.h"

class Game;

enum class ItemType {
	INCREASE_SHOT,
	SEPARATE_SHOT,
	NONE,
};

class Item : public Actor {
public:
	static const double RADIUS;
	Item(Vec2, ItemType);
	void move(Game* game) override;
	void draw(Game* game) override;

	Vec2 getPos() const { return pos; }
	ItemType getItemType() const { return type; }
protected:
	Vec2 pos;
	ItemType type;
	int cnt;
};

class ItemManager : public ActorManager<Item> {
public:
	using Super = ActorManager<Item>;
	ItemManager() = default;
	void move(Game* game) override;
	void draw(Game* game) override;
};
