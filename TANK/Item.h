#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <memory>
#include "Actor.h"

class Game;

class Item : public Actor {
public:
	static const double RADIUS;
	enum class Type {
		INCREASE_SHOT,
		SEPARATE_SHOT,
	};

	Item(Vec2, Type);
	void move(Game* game) override;
	void draw(Game* game) override;
protected:
	Vec2 pos;
	Type type;
	int cnt = 0;
};

class ItemManager : public ActorManager<Item> {
public:
	using Super = ActorManager<Item>;
	void move(Game* game) override;
	void draw(Game* game) override;
};
