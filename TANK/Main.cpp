#include <Siv3D.hpp>
#include <memory>

#include "Game.h"

void Main()
{
	auto command = MessageBox::Show(L"フルスクリーンで起動しますか?", MessageBoxStyle::YesNo);
	if (command == MessageBoxCommand::Yes) {
		Window::SetFullscreen(true, { 800, 600 });
	} else if (command == MessageBoxCommand::No) {
		Window::Resize(800, 600);
	}
	Window::SetTitle(L"たんくうぉー！");

	std::unique_ptr<Game> game = std::make_unique<Game>();
	while (System::Update())
	{
		game->move();
		game->draw();
	}
}
