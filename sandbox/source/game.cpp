#include <lich/imgui.hpp>

#include "events_logger_layer.hpp"
#include "game.hpp"

namespace sand {

Game::Game(const lich::ConsoleArgs &console_args):
	App{lich::AppSpec{"Sandbox", 800, 600}, console_args},
	_logger{"sand::Game"}
{
	push_layer<EventsLoggerLayer>();
	push_overlay<lich::ImguiLayer>(_window->handle());
		
	_logger.trace("Hello!");
}

Game::~Game()
{
	_logger.trace("Goodbye!");
}

}
