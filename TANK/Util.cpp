#include "Util.h"

#include "Game.h"
#include "Player.h"

void Camera2D::shake(int num) {
	offsetPos.x += Random(-num, num);
	offsetPos.y += Random(-num, num);
}

Vec2 Camera2D::convertToScreenPos(Vec2 pos) {
	return pos + offsetPos;
}

void Camera2D::posUpdate(Game* game) {
	auto player = game->getPlayer();
	const Rect stageSize = game->getStageSize();

	offsetPos.set(Window::Width() / 2 - player->getPos().x, Window::Height() / 2 - player->getPos().y);
	offsetPos.x = Clamp(offsetPos.x, static_cast<double>(Window::Width() - stageSize.x), 0.0);
	offsetPos.y = Clamp(offsetPos.y, static_cast<double>(Window::Height() - stageSize.y), 0.0);
}

void Animation::init(String name, int sepNum, int intervalCnt) {
	this->assetName = name;
	this->sepNum = sepNum;
	this->intervalCnt = intervalCnt;
	trimSize = Point(TextureAsset(assetName).width / sepNum, TextureAsset(assetName).height);
	trimRect = Rect(0, 0, trimSize);
}

void Animation::move() {
	cnt++;
	if (cnt == intervalCnt * sepNum) cnt = 0;
	trimRect = Rect(trimSize.x * (cnt / intervalCnt), 0, trimSize);
}

void Animation::draw(Vec2 pos) {
	TextureAsset(assetName)(trimRect).drawAt(pos);
}

void Animation::draw(Vec2 pos, double radian) {
	TextureAsset(assetName)(trimRect).rotate(radian + Radians(90)).drawAt(pos);
}

void Animation::draw(Vec2 pos, double radian, int alpha) {
	TextureAsset(assetName)(trimRect).rotate(radian + Radians(90)).drawAt(pos, Alpha(alpha));
}
