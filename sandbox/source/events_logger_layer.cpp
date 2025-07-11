#include <lich/input.hpp>

#include "events_logger_layer.hpp"

namespace sand {

Events_Logger_Layer:: Events_Logger_Layer() :
	Layer{"Events_Logger_Layer"}, _logger{"sand::Events_Logger_Layer"} {}

void Events_Logger_Layer::update() {
	const auto [x, y] = lich::Input::mouse_pos();
	if (std::fabs(x - _mouse_x) > 100 or std::fabs(y - _mouse_y) > 100) {
		_logger.debug("Mouse position: {}:{}", x, y);
		_mouse_x = x;
		_mouse_y = y;
	}
}

void Events_Logger_Layer::handle(lich::Event &event) {
	if (event.flags() & (int)lich::Event_Flag::Window) {
		_logger.debug("Window event received: {}", event.string());
	}
}

}
