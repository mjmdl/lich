#include <glad/glad.h>

#include "glfw_input.hpp"
#include "glfw_window.hpp"

namespace lich {

static Key_Code to_our_key_code_(int glfw_key_code) {
	return static_cast<Key_Code>(glfw_key_code);
}

static Mouse_Code to_our_mouse_code_(int glfw_mouse_code) {
	return static_cast<Mouse_Code>(glfw_mouse_code);
}

std::unique_ptr<Window> Window::create(const Window_Spec &window_spec) {
	return std::make_unique<Glfw_Window>(window_spec);
}

void Glfw_Window::glfw_error_callback_(int error, const char *description) {
	logger_.error("GLFW error #{}: {}", error, description);
}

Glfw_Window::Glfw_Window(const Window_Spec &window_spec) :
	_window{NULL},
	_title{window_spec.title},
	_event_callback{},
	_success{false}
{
	if (not glfw_init_) {
		glfwSetErrorCallback(glfw_error_callback_);
		
		if (not glfwInit()) {
			logger_.fatal("Failed to initialize GLFW!");
			return;
		}
		
		glfw_init_ = true;
	}

	glfwDefaultWindowHints();

	if (not window_spec.visible) glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	
	_window = glfwCreateWindow(
		window_spec.width,
		window_spec.height,
		_title.c_str(),
		NULL,
		NULL
	);
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
	Glfw_Input::init(_window);

	_success = true;
}

Glfw_Window::~Glfw_Window() {
	if (not glfw_init_ or _window == NULL) return;
	
	glfwDestroyWindow(_window);
	--window_count_;

	if (window_count_ == 0) {
		glfwTerminate();
		glfw_init_ = false;
	}
}

void Glfw_Window::present() {
	glfwSwapBuffers(_window);
}

void Glfw_Window::clear() {
	glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Glfw_Window::update() {
	glfwPollEvents();
}

bool Glfw_Window::success() const {
	return _success;
}

bool Glfw_Window::should_close() const {
	return glfwWindowShouldClose(_window);
}

bool Glfw_Window::visible() const {
	return glfwGetWindowAttrib(_window, GLFW_VISIBLE) == GLFW_TRUE;
}

void Glfw_Window::set_visible(bool visible) {
	if (visible) {
		glfwShowWindow(_window);
	} else {
		glfwHideWindow(_window);
	}
}

bool Glfw_Window::focused() const {
	return glfwGetWindowAttrib(_window, GLFW_FOCUSED) == GLFW_TRUE;
}

void Glfw_Window::set_focused(bool focused) {
	if (focused) glfwFocusWindow(_window);
	// TODO: Implement platform specific window blur/unfocus functionality.
}

void *Glfw_Window::handle() const {
	return _window;
}

const std::string &Glfw_Window::title() const {
	return _title;
}

void Glfw_Window::set_title(const std::string &title) {
	_title = title;
	glfwSetWindowTitle(_window, title.c_str());
}

std::pair<I32, I32> Glfw_Window::pos() const {
	int x, y;
	glfwGetWindowPos(_window, &x, &y);
	return {static_cast<I32>(x), static_cast<I32>(y)};
}

void Glfw_Window::set_pos(I32 x, I32 y) {
	glfwSetWindowPos(_window, x, y);
}

std::pair<U32, U32> Glfw_Window::size() const {
	int width, height;
	glfwGetWindowSize(_window, &width, &height);
	return {static_cast<U32>(width), static_cast<U32>(height)};
}

void Glfw_Window::set_size(U32 width, U32 height) {
	glfwSetWindowSize(_window, static_cast<int>(width), static_cast<int>(height));
}

std::pair<U32, U32> Glfw_Window::screen_size() const {
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	LICH_ASSERT(monitor != NULL, "Failed to retrieve the primary monitor.");
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	LICH_ASSERT(mode != NULL, "Failed to retrieve the primary monitor video information");
	return {static_cast<U32>(mode->width), static_cast<U32>(mode->height)};
}

Glfw_Window *Glfw_Window::window_self_(GLFWwindow *handle) {
	void *user_pointer = glfwGetWindowUserPointer(handle);
	LICH_ASSERT(user_pointer != NULL, "GLFW Window user pointer is not set.");
	auto self = reinterpret_cast<Glfw_Window *>(user_pointer);
	LICH_ASSERT(self->_event_callback != nullptr, "The event callback is not set.");
	return self;
}

void Glfw_Window:: set_event_callback(const Event_Callback &callback){
	_event_callback = callback;
	
	glfwSetWindowCloseCallback(_window, glfw_close_callback_);
	glfwSetWindowFocusCallback(_window, glfw_focus_callback_);
	glfwSetWindowPosCallback(_window, glfw_pos_callback_);
	glfwSetWindowSizeCallback(_window, glfw_size_callback_);
	glfwSetKeyCallback(_window, glfw_key_callback_);
	glfwSetMouseButtonCallback(_window, glfw_mouse_button_callback_);
	glfwSetCursorPosCallback(_window, glfw_cursor_pos_callback_);
	glfwSetScrollCallback(_window, glfw_scroll_callback_);
}

void Glfw_Window::glfw_close_callback_(GLFWwindow *window) {
	auto self = window_self_(window);
	Window_Close_Event event;
	self->_event_callback(*self, event);
}

void Glfw_Window::glfw_focus_callback_(GLFWwindow *window, int focused) {
	auto self = window_self_(window);
	if (focused == GLFW_TRUE) {
		Glfw_Input::init(window);
		
		Window_Focus_Event event;
		self->_event_callback(*self, event);
	} else {
		Window_Blur_Event event;
		self->_event_callback(*self, event);
	}
}

void Glfw_Window::glfw_pos_callback_(GLFWwindow *window, int xpos, int ypos) {
	auto self = window_self_(window);
	Window_Move_Event event{xpos, ypos};
	self->_event_callback(*self, event);
}

void Glfw_Window::glfw_size_callback_(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	
	auto self = window_self_(window);
	Window_Size_Event event{width, height};
	self->_event_callback(*self, event);
}

void Glfw_Window::glfw_key_callback_(
	GLFWwindow *window,
	int key,
	[[maybe_unused]] int scancode,
	int action,
	[[maybe_unused]] int mods
) {
	auto self = window_self_(window);
	if (action == GLFW_RELEASE) {
		Key_Release_Event event{to_our_key_code_(key)};
		self->_event_callback(*self, event);
	} else if (action == GLFW_PRESS) {
		Key_Press_Event event{to_our_key_code_(key), 0};
		self->_event_callback(*self, event);
	} else if (action == GLFW_REPEAT) {
		Key_Press_Event event{to_our_key_code_(key), 1};
		self->_event_callback(*self, event);
	} else {
		log_warn("Unknown GLFW window key action: {}", action);
	}
}

void Glfw_Window::glfw_mouse_button_callback_(
	GLFWwindow *window,
	int button,
	int action,
	[[maybe_unused]] int mods
) {
	auto self = window_self_(window);
	if (action == GLFW_RELEASE) {
		Mouse_Release_Event event{to_our_mouse_code_(button)};
		self->_event_callback(*self, event);
	} else if (action == GLFW_PRESS) {
		Mouse_Press_Event event{to_our_mouse_code_(button), 0};
		self->_event_callback(*self, event);
	} else if (action == GLFW_REPEAT) {
		Mouse_Press_Event event{to_our_mouse_code_(button), 1};
		self->_event_callback(*self, event);
	} else {
		log_warn("Unknown GLFW window key action: {}", action);
	}
}

void Glfw_Window::glfw_cursor_pos_callback_(
	GLFWwindow *window,
	double xpos,
	double ypos
) {
	auto self = window_self_(window);
	Mouse_Move_Event event{xpos, ypos};
	self->_event_callback(*self, event);
}

void Glfw_Window::glfw_scroll_callback_(
	GLFWwindow *window,
	double xoffset,
	double yoffset
) {
	auto self = window_self_(window);
	Mouse_Scroll_Event event{xoffset, yoffset};
	self->_event_callback(*self, event);
}

void Glfw_Window::move_to_center() {
	const auto [width, height] = this->size();
	const auto [screen_width, screen_height] = this->screen_size();
	set_pos((screen_width - width) / 2, (screen_height - height) / 2);
}

}
