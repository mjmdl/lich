#ifndef SAND_EVENTS_LOGGER_LAYER_HPP
#define SAND_EVENTS_LOGGER_LAYER_HPP

#include <lich/layer.hpp>
#include <lich/log.hpp>

namespace sand {

class Events_Logger_Layer final : public lich::Layer {
public:
	Events_Logger_Layer(void);
	void update(void) override;
	void handle(lich::Event &event) override;
	
private:
	lich::Logger _logger;
	lich::F64 _mouse_x;
	lich::F64 _mouse_y;
};

}

#endif
