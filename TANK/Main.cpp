#include <Siv3D.hpp>
#include "Game.h"

void Main()
{
	auto command = MessageBox::Show(L"フルスクリーンで起動しますか?", MessageBoxStyle::YesNoCancel);
	if (command == MessageBoxCommand::Yes) {
		Window::SetFullscreen(true, { 800, 600 });
	} else if (command == MessageBoxCommand::No) {
		Window::Resize(800, 600);
	} else {
		System::Exit();
	}

	Game game;
	game.init();
	while (System::Update())
	{	
		game.move();
		game.draw();
	}
}
