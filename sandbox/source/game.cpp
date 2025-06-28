#include <lich/imgui.hpp>

#include "events_logger_layer.hpp"
#include "game.hpp"

namespace sand {

Game::Game(const lich::Console_Args &console_args):
	App{lich::App_Spec{"Sandbox", 800, 600}, console_args},
	_logger{"sand::Game"} {

	push_layer<Events_Logger_Layer>();
	push_overlay<lich::Imgui_Layer>(_window->handle());
		
	_logger.trace("Hello!");
}

Game::~Game(void) {
	_logger.trace("Goodbye!");
}

}
