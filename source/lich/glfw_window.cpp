#include <glad/glad.h>

#include "glfw_input.hpp"
#include "glfw_window.hpp"

namespace lich {

static KeyCode to_our_key_code_(int glfw_key_code)
{
	return static_cast<KeyCode>(glfw_key_code);
}

static MouseCode to_our_mouse_code_(int glfw_mouse_code)
{
	return static_cast<MouseCode>(glfw_mouse_code);
}

std::unique_ptr<Window> Window::create(const WindowSpec &window_spec)
{
	return std::make_unique<GlfwWindow>(window_spec);
}

void GlfwWindow::glfw_error_callback_(int error, const char *description)
{
	logger_.error("GLFW error #{}: {}", error, description);
}

GlfwWindow::GlfwWindow(const WindowSpec &window_spec):
	_window{NULL}, _title{window_spec.title}, _event_callback{},
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
	
	_window = glfwCreateWindow(window_spec.width, window_spec.height,
		_title.c_str(), NULL, NULL);
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
	GlfwInput::init(_window);

	_success = true;
}

GlfwWindow::~GlfwWindow()
{
	if (not glfw_init_ or _window == NULL) return;
	
	glfwDestroyWindow(_window);
	--window_count_;

	if (window_count_ == 0) {
		glfwTerminate();
		glfw_init_ = false;
	}
}

void GlfwWindow::present()
{
	glfwSwapBuffers(_window);
}

void GlfwWindow::clear()
{
	glClearColor(0.17f, 0.17f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GlfwWindow::update()
{
	glfwPollEvents();
}

bool GlfwWindow::success() const
{
	return _success;
}

bool GlfwWindow::should_close() const
{
	return glfwWindowShouldClose(_window);
}

bool GlfwWindow::visible() const
{
	return glfwGetWindowAttrib(_window, GLFW_VISIBLE) == GLFW_TRUE;
}

void GlfwWindow::set_visible(bool visible)
{
	if (visible) {
		glfwShowWindow(_window);
	} else {
		glfwHideWindow(_window);
	}
}

bool GlfwWindow::focused() const
{
	return glfwGetWindowAttrib(_window, GLFW_FOCUSED) == GLFW_TRUE;
}

void GlfwWindow::set_focused(bool focused)
{
	if (focused) {
		glfwFocusWindow(_window);
	} else {
		// TODO: Implement platform specific window blur/unfocus functionality.
	}
}

void *GlfwWindow::handle() const
{
	return _window;
}

const std::string &GlfwWindow::title() const
{
	return _title;
}

void GlfwWindow::set_title(const std::string &title)
{
	_title = title;
	glfwSetWindowTitle(_window, title.c_str());
}

std::pair<I32, I32> GlfwWindow::pos() const
{
	int x;
	int y;
	glfwGetWindowPos(_window, &x, &y);
	return {static_cast<I32>(x), static_cast<I32>(y)};
}

void GlfwWindow::set_pos(I32 x, I32 y)
{
	glfwSetWindowPos(_window, x, y);
}

std::pair<U32, U32> GlfwWindow::size() const
{
	int width;
	int height;
	glfwGetWindowSize(_window, &width, &height);
	return {static_cast<U32>(width), static_cast<U32>(height)};
}

void GlfwWindow::set_size(U32 width, U32 height)
{
	glfwSetWindowSize(_window, static_cast<int>(width), static_cast<int>(height));
}

std::pair<U32, U32> GlfwWindow::screen_size() const
{
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	LICH_ASSERT(monitor != NULL, "Failed to retrieve the primary monitor.");
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	LICH_ASSERT(mode != NULL, "Failed to retrieve the primary monitor video information");
	return {static_cast<U32>(mode->width), static_cast<U32>(mode->height)};
}

GlfwWindow *GlfwWindow::window_self_(GLFWwindow *handle)
{
	void *user_pointer = glfwGetWindowUserPointer(handle);
	LICH_ASSERT(user_pointer != NULL, "GLFW Window user pointer is not set.");
	auto self = reinterpret_cast<GlfwWindow *>(user_pointer);
	LICH_ASSERT(self->_event_callback != nullptr, "The event callback is not set.");
	return self;
}

void GlfwWindow::set_event_callback(const EventCallback &callback)
{
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

void GlfwWindow::glfw_close_callback_(GLFWwindow *window)
{
	auto self = window_self_(window);
	WindowCloseEvent event;
	self->_event_callback(*self, event);
}

void GlfwWindow::glfw_focus_callback_(GLFWwindow *window, int focused)
{
	auto self = window_self_(window);
	if (focused == GLFW_TRUE) {
		GlfwInput::init(window);
		
		WindowFocusEvent event;
		self->_event_callback(*self, event);
	} else {
		WindowBlurEvent event;
		self->_event_callback(*self, event);
	}
}

void GlfwWindow::glfw_pos_callback_(GLFWwindow *window, int xpos, int ypos)
{
	auto self = window_self_(window);
	WindowMoveEvent event{xpos, ypos};
	self->_event_callback(*self, event);
}

void GlfwWindow::glfw_size_callback_(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	
	auto self = window_self_(window);
	WindowSizeEvent event{width, height};
	self->_event_callback(*self, event);
}

void GlfwWindow::glfw_key_callback_(GLFWwindow *window, int key,
	[[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods)
{
	auto self = window_self_(window);
	if (action == GLFW_RELEASE) {
		KeyReleaseEvent event{to_our_key_code_(key)};
		self->_event_callback(*self, event);
	} else if (action == GLFW_PRESS) {
		KeyPressEvent event{to_our_key_code_(key), 0};
		self->_event_callback(*self, event);
	} else if (action == GLFW_REPEAT) {
		KeyPressEvent event{to_our_key_code_(key), 1};
		self->_event_callback(*self, event);
	} else {
		log_warn("Unknown GLFW window key action: {}", action);
	}
}

void GlfwWindow::glfw_mouse_button_callback_(GLFWwindow *window, int button,
	int action, [[maybe_unused]] int mods)
{
	auto self = window_self_(window);
	if (action == GLFW_RELEASE) {
		MouseReleaseEvent event{to_our_mouse_code_(button)};
		self->_event_callback(*self, event);
	} else if (action == GLFW_PRESS) {
		MousePressEvent event{to_our_mouse_code_(button), 0};
		self->_event_callback(*self, event);
	} else if (action == GLFW_REPEAT) {
		MousePressEvent event{to_our_mouse_code_(button), 1};
		self->_event_callback(*self, event);
	} else {
		log_warn("Unknown GLFW window key action: {}", action);
	}
}

void GlfwWindow::glfw_cursor_pos_callback_(GLFWwindow *window,
	double xpos, double ypos)
{
	auto self = window_self_(window);
	MouseMoveEvent event{xpos, ypos};
	self->_event_callback(*self, event);
}

void GlfwWindow::glfw_scroll_callback_(GLFWwindow *window,
	double xoffset, double yoffset)
{
	auto self = window_self_(window);
	MouseScrollEvent event{xoffset, yoffset};
	self->_event_callback(*self, event);
}

void GlfwWindow::move_to_center()
{
	const auto [width, height] = this->size();
	const auto [screen_width, screen_height] = this->screen_size();
	set_pos((screen_width - width) / 2, (screen_height - height) / 2);
}

}
