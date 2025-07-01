#include "app.hpp"
#include "log.hpp"
#include "render.hpp"

namespace lich {

App::App(const AppSpec &app_spec, const ConsoleArgs &console_args):
	_window{nullptr}, _app_spec{app_spec}, _console_args{console_args},
	_success{false}, _running{false}
{
	if (_app_spec.name != "Lich Engine") {
		Logger::client_logger = Logger{_app_spec.name};
	}

	_window = Window::create(WindowSpec{
		app_spec.name, app_spec.width, app_spec.height});
	if (not _window->success()) return;
	_window->set_event_callback(std::bind(&App::_on_window_event, this,
		std::placeholders::_1, std::placeholders::_2));
	_window->move_to_center();
	_window->set_visible(true);
	
	_success = true;
}

int App::run()
{
	if (not _success) return EXIT_FAILURE;

	_running = true;
	while (_running) {
		RenderCommand::set_clear_color(glm::vec4{0.5f, 0.2f, 0.5f, 1.0f});
		RenderCommand::clear();

		Renderer::begin_scene();

		_layer_stack.update();
		
		Renderer::end_scene();
		
		_window->update();
		_window->present();
	}
	
	return EXIT_SUCCESS;
}

Usize App::push_layer(std::unique_ptr<Layer> layer)
{
	return _layer_stack.push(std::move(layer));
}

Usize App::push_overlay(std::unique_ptr<Layer> overlay)
{
	return _layer_stack.push_over(std::move(overlay));
}

const AppSpec &App::app_spec() const
{
	return _app_spec;
}

const ConsoleArgs &App::console_args() const
{
	return _console_args;
}

bool App::success() const
{
	return _success;
}

bool App::running() const
{
	return _running;
}

bool App::_on_window_event([[maybe_unused]] Window &window, Event &event)
{
	_layer_stack.handle(event);

	EventDispatcher dispatcher{event};
	return dispatcher.handle<WindowCloseEvent>([this] (const auto &) -> bool
	{
		_running = false;
		return true;
	});
}

}
