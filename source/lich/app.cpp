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
	_window->set_event_callback(std::bind(&App::_on_window_event, this,
		std::placeholders::_1, // Window &self
		std::placeholders::_2 // Event &event
		));
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
		_layer_stack.update();

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

Usize App::push_layer(std::unique_ptr<Layer> layer) {
	return _layer_stack.push(std::move(layer));
}

Usize App::push_overlay(std::unique_ptr<Layer> overlay) {
	return _layer_stack.push_over(std::move(overlay));
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

bool App::_on_window_event(Window &window, Event &event) {
	(void)window;

	_layer_stack.handle(event);

	Event_Dispatcher dispatcher{event};
	return dispatcher.handle<Window_Close_Event>([this] (const auto &) -> bool {
		_running = false;
		return true;
	});
}

}
