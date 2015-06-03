#pragma once
#include <Siv3D.hpp>
#include <vector>
#include <memory>

class Game;
class Animation;

class Enemy
{
public:
	enum class State {
		Spawn,
		Normal,
		Damage,
		Burn,
	};

	Enemy();
	virtual void move(Game* game) = 0;
	virtual void draw(Game* game) = 0;

	void setPos(Vec2 pos) { this->pos = pos; }
	Vec2 getPos() const { return pos; }
	double getRadius() const { return radius; }
	State getState() const { return state; }
	bool isEnabled() const { return enabled; }
	Color getMinimapColor() const { return minimapColor; }
protected:
	State state = State::Normal;
	Vec2 pos, vec = { 0.0, 0.0 };
	int hp, stateCnt = 0, fireCnt = 0;
	double radian = 0.0, radius;
	bool enabled = true;
	Color color = Palette::White, minimapColor;
	std::shared_ptr<Animation> explosionAnimation;

	bool checkShotHit(Game* game);
	void defaultMove(Game* game);
	void addDamage(int damage);
};

class Technyan : public Enemy
{
public:
	Technyan();
	void move(Game* game) override;
	void draw(Game* game) override;
};

class MissileLauncher : public Enemy
{
public:
	MissileLauncher();
	void move(Game* game) override;
	void draw(Game* game) override;
};

class EnemyManager
{
public:
	void init();
	void move(Game* game);
	void draw(Game* game);
	void add(std::shared_ptr<Enemy>);

	const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return enemies; }
private:
	std::vector<std::shared_ptr<Enemy>> enemies;
	Vec2 makeRandomPos(Game* game);
	int cnt;
};
