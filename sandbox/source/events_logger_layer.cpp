#include "events_logger_layer.hpp"

namespace sand {

Events_Logger_Layer::Events_Logger_Layer(void):
	Layer{"Events_Logger"}, _logger{"sand::Events_Logger"} {}

void Events_Logger_Layer::handle(lich::Event &event) {
	if (event.flags() & (int)lich::Event_Flag::Window) {
		_logger.debug("Window event received: {}", event.string());
	}
}

}
