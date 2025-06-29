#include "glfw_input.hpp"

namespace lich {

static int to_glfw_key_code_(KeyCode code)
{
	return static_cast<int>(code);
}

static int to_glfw_mouse_code_(MouseCode code)
{
	return static_cast<int>(code);
}

static KeyAction from_glfw_key_action_(int action)
{
	switch (action) {
	case GLFW_RELEASE: return KeyAction::Up;
	case GLFW_PRESS:   return KeyAction::Down;
	case GLFW_REPEAT:  return KeyAction::Repeat;
	default:           return KeyAction::None;
	}
}

void GlfwInput::init(GLFWwindow *window)
{
	if (not singleton_) {
		singleton_ = std::make_unique<GlfwInput>(window);
	} else {
		auto instance = static_cast<GlfwInput *>(singleton_.get());
		instance->_window = window;
	}
}

GlfwInput::GlfwInput(GLFWwindow *window):
	_window{window} {}

bool GlfwInput::_key_down(KeyCode code)
{
	KeyAction action = _key(code);
	return action == KeyAction::Down or action == KeyAction::Repeat;
}

bool GlfwInput::_mouse_down(MouseCode code)
{
	KeyAction action = _mouse(code);
	return action == KeyAction::Down or action == KeyAction::Repeat;
}

std::pair<F64, F64> GlfwInput::_mouse_pos()
{
	F64 xpos;
	F64 ypos;
	glfwGetCursorPos(_window, &xpos, &ypos);
	return {xpos, ypos};
}

KeyAction GlfwInput::_key(KeyCode code)
{
	return from_glfw_key_action_(glfwGetKey(_window, to_glfw_key_code_(code)));
}

KeyAction GlfwInput::_mouse(MouseCode code)
{
	return from_glfw_key_action_(glfwGetKey(_window, to_glfw_mouse_code_(code)));
}

}
