#include <Siv3D.hpp>
#include "Game.h"

void Main()
{
	Game game;

	game.init();
	while (System::Update())
	{
		game.move();
		game.draw();
	}
}
