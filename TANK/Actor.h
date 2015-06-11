#pragma once
#include <Siv3D.hpp>
#include <memory>
#include <vector>

class Game;

class Actor : private Uncopyable {
public:
	Actor() = default;
	virtual ~Actor() = default;
	virtual void move(Game* game) = 0;
	virtual void draw(Game* game) = 0;

	void kill() { enable = false; }
	bool isEnable() const { return enable; }
protected:
	bool enable = true;
};

template<typename Type>
class ActorManager {
public:
	ActorManager() = default;
	virtual ~ActorManager() = default;

	void add(std::shared_ptr<Type> actor) {
		actors.push_back(actor);
	}
	void clear() {
		actors.clear();
	}
	virtual void move(Game* game) {
		for (auto& actor : actors) {
			actor->move(game);
		}
		Erase_if(actors, [](std::shared_ptr<Actor> actor) { return !actor->isEnable(); });
	}
	virtual void draw(Game* game) {
		for (auto& actor : actors) {
			actor->draw(game);
		}
	}

	typename std::vector<std::shared_ptr<Type>>::const_iterator begin() { return actors.begin(); }
	typename std::vector<std::shared_ptr<Type>>::const_iterator end() { return actors.end(); }
private:
	std::vector<std::shared_ptr<Type>> actors;
};
