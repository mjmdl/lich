#ifndef LICH_APP_HPP
#define LICH_APP_HPP

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
	virtual ~App(void) = default;
	int run(void);

	const App_Spec &app_spec(void) const;
	const Console_Args &console_args(void) const;
	bool success(void) const;
	bool running(void) const;

private:
	void _on_window_close(Window &window);
	
private:
	App_Spec _app_spec;
	Console_Args _console_args;
	std::unique_ptr<Window> _window;
	bool _success;
	bool _running;
};

}

#endif
