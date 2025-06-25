#include "game.hpp"

namespace sand {

Game::Game(const lich::Console_Args &console_args):
	App{lich::App_Spec{"Sandbox", 800, 600}, console_args},
	_logger{"sand::Game"} {

	_logger.trace("Hello!");
}

Game::~Game(void) {
	_logger.trace("Goodbye!");
}

}
