#include "game.hpp"

namespace sand {

Game::Game(const lich::Console_Args &console_args)
	: App{lich::App_Spec{"Sandbox", 800, 600}, console_args} {

	std::cout << "Hello!\n";
}

Game::~Game(void) {
	std::cout << "Goodbye!\n";
}

}
