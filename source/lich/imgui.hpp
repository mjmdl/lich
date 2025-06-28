#ifndef LICH_IMGUI_HPP
#define LICH_IMGUI_HPP

#include "layer.hpp"

namespace lich {

class Imgui_Layer : public Layer {
public:
	Imgui_Layer(void *window_handle);

	void init(void) override;
	void quit(void) override;
	void update(void) override;
	void handle(Event &event) override;

private:
	void *_window_handle;
	bool _show_demo_window;
};

}

#endif
