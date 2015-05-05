#include <Siv3D.hpp>
#include "Game.h"

void Main()
{
	Window::SetFullscreen(true, { 800, 600 });
	Game game;

	game.init();
	while (System::Update())
{	
		game.move();
		game.draw();
	}
}
