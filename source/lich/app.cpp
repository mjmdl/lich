#include <imgui/imgui.h>

#include "app.hpp"
#include "debug_render.hpp"
#include "log.hpp"

namespace lich {

App::App(const App_Spec &app_spec, const Console_Args &console_args):
	_app_spec{app_spec}, _console_args{console_args},
	_window{nullptr}, _success{false}, _running{false} {

	if (_app_spec.name != "Lich Engine") {
		Logger::client_logger = Logger{_app_spec.name};
	}

	_window = Window::create(Window_Spec{
		app_spec.name, app_spec.width, app_spec.height});
	if (not _window->success()) {
		return;
	}
	_window->set_close_callback(std::bind(&App::_on_window_close, this, std::placeholders::_1));
	_window->move_to_center();
	_window->set_visible(true);
	
	_success = true;
}

int App::run(void) {
	if (not _success) {
		return EXIT_FAILURE;
	}

	debug_render::bootstrap(_window->handle());
	bool show_demo_window = true;
	
	_running = true;
	while (_running) {
		_window->update();

		_window->clear();

		debug_render::begin();
		if (show_demo_window) {
			ImGui::ShowDemoWindow(&show_demo_window);
		}
		debug_render::end();
		
		_window->present();
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

void App::_on_window_close(Window &window) {
	(void)window;
	_running = false;
}

}
