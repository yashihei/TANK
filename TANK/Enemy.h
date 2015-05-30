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
	Vec2 getPos() { return pos; }
	double getRadius() { return radius; }
	State getState() { return state; }
	bool isEnable() { return enable; }
protected:
	State state;
	Vec2 pos, vec;
	int hp, cnt;
	double radian, radius;
	bool enable;
	Color color;
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
};
