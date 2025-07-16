#ifndef LICH_APP_HPP
#define LICH_APP_HPP

#include "layer.hpp"
#include "util.hpp"
#include "window.hpp"

namespace lich {

struct App_Spec {
	std::string name = "Lich Engine";
	U32 width = 960;
	U32 height = 540;
};

struct Console_Args {
	int argc = 0;
	char **argv = nullptr;
};

class App {
public:
	App(const App_Spec &app_spec = {}, const Console_Args &console_args = {});
	virtual ~App() = default;
	int run();

	Usize push_layer(std::unique_ptr<Layer> layer);
	Usize push_overlay(std::unique_ptr<Layer> overlay);

	const App_Spec &app_spec() const;
	const Console_Args &console_args() const;
	bool success() const;
	bool running() const;

	template<Layer_Derived Type, typename ...Args>
		requires std::constructible_from<Type, Args...>
	Usize push_layer(Args &&...args) {
		return push_layer(std::make_unique<Type>(std::forward<Args>(args)...));
	}
	
	template<Layer_Derived Type, typename ...Args>
		requires std::constructible_from<Type, Args...>
	Usize push_overlay(Args &&...args) {
		return push_overlay(std::make_unique<Type>(std::forward<Args>(args)...));
	}

private:
	bool _on_window_event(Window &window, Event &event);
	
protected:
	std::unique_ptr<Window> _window;

private:
	App_Spec _app_spec{};
	Console_Args _console_args{};
	Layer_Stack _layer_stack{};
	float _last_frame_time{0.0f};
	bool _success{false};
	bool _running{false};
};

}

#endif
