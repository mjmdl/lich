#ifndef SAND_EVENTS_LOGGER_LAYER_HPP
#define SAND_EVENTS_LOGGER_LAYER_HPP

#include <lich/layer.hpp>
#include <lich/log.hpp>

namespace sand {

class EventsLoggerLayer final : public lich::Layer {
public:
	EventsLoggerLayer();
	void update() override;
	void handle(lich::Event &event) override;
	
private:
	lich::Logger _logger;
	lich::F64 _mouse_x;
	lich::F64 _mouse_y;
};

}

#endif
