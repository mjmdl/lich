#include "app.hpp"

namespace lich {

App::App(const App_Spec &app_spec, const Console_Args &console_args):
	_app_spec{app_spec}, _console_args{console_args},
	_success{false}, _running{false} {
	
	_success = true;
}

int App::run(void) {
	if (!_success) {
		return EXIT_FAILURE;
	}

	_running = true;
	while (_running) {
		break;
	}
	
	return EXIT_SUCCESS;
}

const App_Spec &App::app_spec(void) const {
	return _app_spec;
}

const Console_Args &App::console_args(void) const {
	return _console_args;
}

bool App::success(void) const {
	return _success;
}

bool App::running(void) const {
	return _running;
}

}
