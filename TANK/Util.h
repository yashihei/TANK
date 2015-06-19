#include <Siv3D.hpp>

class Game;

class Camera2D
{
public:
	Camera2D();
	void posUpdate(Game* game);
	void shake(int num);
	Vec2 toScreenPos(Vec2 pos);

	Vec2 getoffsetPos() const { return offsetPos; }
private:
	Vec2 offsetPos;
};

class Animation
{
public:
	Animation() = default;
	void init(String name, int sepNum, int intervalCnt);
	void move();
	void draw(Vec2 pos);
	void draw(Vec2 pos, double radian);
	int getCycleCnt() const { return sepNum * intervalCnt; }
private:
	String assetName;
	Point trimSize;
	int intervalCnt, sepNum;
	int cnt = 0;
	Rect trimRect;
};
