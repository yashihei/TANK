#include "Player.h"

void Player::init() {
	hp = 100;
	pos = Vec2(100.0, 100.0);
	cnt = 0;
	state = State::REBORN;
	rad = 0;
}

void Player::move() {
	cnt++;

	//�O�i���
	double speed = 5.0;
	vec = Vec2(speed * Cos(rad), speed * Sin(rad));
	if (Input::KeyW.pressed) {
		pos.moveBy(vec);
	} else if (Input::KeyS.pressed) {
		pos.moveBy(-vec);
	}

	//��]
	double turnSpeed = 3.5;
	if (Input::KeyS.pressed) {
		turnSpeed *= -1;
	}
	if (Input::KeyD.pressed) {
		rad += Radians(turnSpeed);
	} else if (Input::KeyA.pressed) {
		rad -= Radians(turnSpeed);
	}

	//�ȉ�shot�֘A
	if (Input::MouseL.pressed && cnt % 10 == 0) {
		const Point mousePos = Mouse::Pos();
		const double rad = Atan2(mousePos.y - pos.y, mousePos.x - pos.x);
		double shotSpeed = 10.0;

		//�|�C���^�Ŏ��K�v����������
		auto shot = std::make_shared<Shot>();
		shot->pos.set(pos);
		shot->vec.set(Cos(rad) * shotSpeed, Sin(rad) * shotSpeed);//FIXME
		shot->enable = true;
		shots.push_back(shot);
	}

	for (auto& shot : shots) {
		shot->pos.moveBy(shot->vec);

		if (shot->pos.x > Window::Width() || shot->pos.x < 0 || shot->pos.y > Window::Height() || shot->pos.y < 0) {
			shot->enable = false;
		}
	}
	Erase_if(shots, [](std::shared_ptr<Shot> shot) { return !shot->enable; });
}

void Player::draw() {
	int w = 30, h = 60;
	RectF(pos.x - w/2, pos.y - h/2, w, h).rotated(rad + Radians(90)).draw(Palette::Red);

	for (auto& shot : shots) {
		Circle(shot->pos, 10).draw(Palette::White);
	}
	Circle(Mouse::Pos(), 5).draw(Palette::Yellow);
	Line(pos, Mouse::Pos()).draw(Palette::Blue);
}
