#include "events_logger_layer.hpp"

namespace sand {

Events_Logger_Layer::Events_Logger_Layer(void):
	Layer{"Events_Logger"}, _logger{"sand::Events_Logger"} {}

void Events_Logger_Layer::handle(lich::Event &event) {
	_logger.debug("Event received: {}", event.string());
}

}
