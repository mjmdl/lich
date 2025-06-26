#include <glad/glad.h>

#include "glfw_window.hpp"

namespace lich {

std::unique_ptr<Window> Window::create(const Window_Spec &window_spec) {
	return std::make_unique<Glfw_Window>(window_spec);
}

void Glfw_Window::glfw_error_callback_(int error, const char *description) {
	logger_.error("GLFW error #{}: {}", error, description);
}

Glfw_Window::Glfw_Window(const Window_Spec &window_spec):
	_window{NULL}, _title{window_spec.title}, _close_callback{},
	_success{false} {

	if (not glfw_init_) {
		glfwSetErrorCallback(glfw_error_callback_);
		
		if (not glfwInit()) {
			logger_.fatal("Failed to initialize GLFW!");
			return;
		}
		
		glfw_init_ = true;
	}

	glfwDefaultWindowHints();

	if (not window_spec.visible) {
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	}

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	
	_window = glfwCreateWindow(window_spec.width, window_spec.height, _title.c_str(),
		NULL, NULL);
	if (_window == NULL) {
		logger_.fatal("Failed to create a GLFW window!");		
		return;
	}
	++window_count_;

	glfwMakeContextCurrent(_window);
	if (not gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logger_.fatal("Failed to load OpenGL using GLAD!");
		return;
	}
	
	glfwSetWindowUserPointer(_window, this);

	_success = true;
}

Glfw_Window::~Glfw_Window(void) {
	if (not glfw_init_ or _window == NULL) {
		return;
	}
	
	glfwDestroyWindow(_window);
	--window_count_;

	if (window_count_ == 0) {
		glfwTerminate();
		glfw_init_ = false;
	}
}

void Glfw_Window::present(void) {
	glfwSwapBuffers(_window);
}

void Glfw_Window::clear(void) {
	glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Glfw_Window::update(void) {
	glfwPollEvents();
}

bool Glfw_Window::success(void) const {
	return _success;
}

bool Glfw_Window::should_close(void) const {
	return glfwWindowShouldClose(_window);
}

bool Glfw_Window::visible(void) const {
	return glfwGetWindowAttrib(_window, GLFW_VISIBLE) == GLFW_TRUE;
}

void Glfw_Window::set_visible(bool visible) {
	if (visible) {
		glfwShowWindow(_window);
	} else {
		glfwHideWindow(_window);
	}
}

bool Glfw_Window::focused(void) const {
	return glfwGetWindowAttrib(_window, GLFW_FOCUSED) == GLFW_TRUE;
}

void Glfw_Window::set_focused(bool focused) {
	if (focused) {
		glfwFocusWindow(_window);
	} else {
		// TODO: Implement platform specific window blur/unfocus functionality.
	}
}

const std::string &Glfw_Window::title(void) const {
	return _title;
}

void Glfw_Window::set_title(const std::string &title) {
	_title = title;
	glfwSetWindowTitle(_window, title.c_str());
}

std::pair<I32, I32> Glfw_Window::pos(void) const {
	int x;
	int y;
	glfwGetWindowPos(_window, &x, &y);
	return {static_cast<I32>(x), static_cast<I32>(y)};
}

void Glfw_Window::set_pos(I32 x, I32 y) {
	glfwSetWindowPos(_window, x, y);
}

std::pair<U32, U32> Glfw_Window::size(void) const {
	int width;
	int height;
	glfwGetWindowSize(_window, &width, &height);
	return {static_cast<U32>(width), static_cast<U32>(height)};
}

void Glfw_Window::set_size(U32 width, U32 height) {
	glfwSetWindowSize(_window,
		static_cast<int>(width), static_cast<int>(height));
}

std::pair<U32, U32> Glfw_Window::screen_size(void) const {
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	assert(monitor != NULL);
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	assert(mode != NULL);
	return {static_cast<U32>(mode->width), static_cast<U32>(mode->height)};
}

void Glfw_Window::set_close_callback(const Close_Callback &callback) {
	_close_callback = callback;
	glfwSetWindowCloseCallback(_window, [](GLFWwindow *window) -> void {
		void *user_pointer = glfwGetWindowUserPointer(window);
		assert(user_pointer != NULL);
		auto self = reinterpret_cast<Glfw_Window *>(user_pointer);
		assert(self->_close_callback != nullptr);
		self->_close_callback(*self);
	});
}

void Glfw_Window::move_to_center(void) {
	const auto [width, height] = this->size();
	const auto [screen_width, screen_height] = this->screen_size();
	set_pos((screen_width - width) / 2, (screen_height - height) / 2);
}

}
