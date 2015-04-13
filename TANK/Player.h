#pragma once
#include <Siv3D.hpp>

//�v���C���[�@
//������{�I�ɏ������đ����ĖŒ��ꒃ�Η͎����Ă銴����

class Player
{
public:
	Player(){};
	void init();
	void move();
	void draw();

	enum class State {
		NORMAL,
		DAMAGE,
		REBORN,
	};
private:
	int hp;
	Vec2 pos;
	int cnt;
	State state;
	double radian;
};
