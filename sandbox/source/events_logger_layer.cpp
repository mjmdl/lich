#include <lich/input.hpp>

#include "events_logger_layer.hpp"

namespace sand {

EventsLoggerLayer::EventsLoggerLayer():
	Layer{"EventsLoggerLayer"}, _logger{"sand::EventsLoggerLayer"} {}

void EventsLoggerLayer::update()
{
	const auto [x, y] = lich::Input::mouse_pos();
	if (std::fabs(x - _mouse_x) > 100 or std::fabs(y - _mouse_y) > 100) {
		_logger.debug("Mouse position: {}:{}", x, y);
		_mouse_x = x;
		_mouse_y = y;
	}
}

void EventsLoggerLayer::handle(lich::Event &event)
{
	if (event.flags() & (int)lich::EventFlag::Window) {
		_logger.debug("Window event received: {}", event.string());
	}
}

}
