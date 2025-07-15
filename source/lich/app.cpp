#include "app.hpp"
#include "log.hpp"
#include "platform.hpp"
#include "render.hpp"

namespace lich {

App::App(const App_Spec &app_spec, const Console_Args &console_args) :
	_window{nullptr}, _app_spec{app_spec}, _console_args{console_args},
	_last_frame_time{0.0f}, _success{false}, _running{false}
{
	if (_app_spec.name != "Lich Engine") {
		Logger::client_logger = Logger{_app_spec.name};
	}
	
	_window = Window::create(Window_Spec{
		app_spec.name, app_spec.width, app_spec.height});
	if (not _window->success()) return;
	
	_window->set_event_callback(std::bind(&App::_on_window_event, this,
		std::placeholders::_1, std::placeholders::_2));
	_window->move_to_center();
	_window->set_visible(true);
	
	_success = true;
}

int App::run() {
	if (not _success) return EXIT_FAILURE;

	_running = true;
	while (_running) {
		float time = Platform::get_time();
		Timestep timestep{time - _last_frame_time};
		_last_frame_time = time;
		
		Render_Command::set_clear_color(glm::vec4{0.5f, 0.2f, 0.5f, 1.0f});
		Render_Command::clear();

		Renderer::begin_scene();

		_layer_stack.update(timestep);
		
		Renderer::end_scene();
		
		_window->update();
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

const App_Spec &App::app_spec() const { return _app_spec; }
const Console_Args &App::console_args() const { return _console_args; }
bool App::success() const { return _success; }
bool App::running() const { return _running; }

bool App:: _on_window_event([[maybe_unused]] Window &window, Event &event) {
	_layer_stack.handle(event);

	Event_Dispatcher dispatcher{event};
	return dispatcher.handle<Window_Close_Event>([this] (const auto &) -> bool {
		_running = false;
		return true;
	});
}

}
