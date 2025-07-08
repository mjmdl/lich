#include <lich/imgui.hpp>

#include "events_logger_layer.hpp"
#include "game.hpp"
#include "render_layer.hpp"

namespace sand {

Game::Game(const lich::ConsoleArgs &console_args):
	App{lich::AppSpec{"Sandbox", 800, 600}, console_args},
	_logger{"sand::Game"}
{
	push_layer<EventsLoggerLayer>();
	push_overlay<lich::ImguiLayer>(_window->handle());
	push_layer<RenderLayer>(app_spec().width / app_spec().height);
		
	_logger.trace("Hello!");
}

Game::~Game()
{
	_logger.trace("Goodbye!");
}

}
