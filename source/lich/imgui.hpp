#ifndef LICH_IMGUI_HPP
#define LICH_IMGUI_HPP

#include "layer.hpp"

namespace lich {

class Imgui_Layer final : public Layer {
public:
	Imgui_Layer(void *window_handle);

	void init() override;
	void quit() override;
	void update() override;
	void handle(Event &event) override;

private:
	void *_window_handle;
	bool _show_demo_window;
};

}

#endif
