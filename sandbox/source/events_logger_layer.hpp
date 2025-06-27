#ifndef SAND_EVENTS_LOGGER_LAYER_HPP
#define SAND_EVENTS_LOGGER_LAYER_HPP

#include <lich/layer.hpp>
#include <lich/log.hpp>

namespace sand {

class Events_Logger_Layer : public lich::Layer {
public:
	Events_Logger_Layer(void);
	void handle(lich::Event &event);
	
private:
	lich::Logger _logger;
};

}

#endif
