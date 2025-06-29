#ifndef LICH_APP_HPP
#define LICH_APP_HPP

#include "layer.hpp"
#include "window.hpp"

namespace lich {

struct AppSpec {
	std::string name = "Lich Engine";
	U32 width = 960;
	U32 height = 540;
};

struct ConsoleArgs {
	int argc = 0;
	char **argv = nullptr;
};

class App {
public:
	App(const AppSpec &app_spec = {}, const ConsoleArgs &console_args = {});
	virtual ~App() = default;
	int run();

	Usize push_layer(std::unique_ptr<Layer> layer);
	Usize push_overlay(std::unique_ptr<Layer> overlay);

	const AppSpec &app_spec() const;
	const ConsoleArgs &console_args() const;
	bool success() const;
	bool running() const;

	template<LayerDerived Type, typename ...Args>
		requires std::constructible_from<Type, Args...>
	Usize push_layer(Args &&...args)
	{
		return push_layer(std::make_unique<Type>(std::forward<Args>(args)...));
	}
	
	template<LayerDerived Type, typename ...Args>
		requires std::constructible_from<Type, Args...>
	Usize push_overlay(Args &&...args)
	{
		return push_overlay(std::make_unique<Type>(std::forward<Args>(args)...));
	}

private:
	bool _on_window_event(Window &window, Event &event);
	
protected:
	std::unique_ptr<Window> _window;

private:
	AppSpec _app_spec;
	ConsoleArgs _console_args;
	LayerStack _layer_stack;
	bool _success;
	bool _running;
};

}

#endif
