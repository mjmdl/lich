#include "glfw_input.hpp"

namespace lich {

static int to_glfw_key_code_(Key_Code code) {
	return static_cast<int>(code);
}

static int to_glfw_mouse_code_(Mouse_Code code) {
	return static_cast<int>(code);
}

static Key_Action from_glfw_key_action_(int action) {
	switch (action) {
	case GLFW_RELEASE: return Key_Action::Up;
	case GLFW_PRESS:   return Key_Action::Down;
	case GLFW_REPEAT:  return Key_Action::Repeat;
	default:           return Key_Action::None;
	}
}

void Glfw_Input::init(GLFWwindow *window) {
	if (not singleton_) {
		singleton_ = std::make_unique<Glfw_Input>(window);
	} else {
		auto instance = static_cast<Glfw_Input *>(singleton_.get());
		instance->_window = window;
	}
}

Glfw_Input::Glfw_Input(GLFWwindow *window) :
	_window{window} {}

bool Glfw_Input::_key_down(Key_Code code) {
	Key_Action action = _key(code);
	return action == Key_Action::Down or action == Key_Action::Repeat;
}

bool Glfw_Input::_mouse_down(Mouse_Code code) {
	Key_Action action = _mouse(code);
	return action == Key_Action::Down or action == Key_Action::Repeat;
}

std::pair<F64, F64> Glfw_Input::_mouse_pos() {
	F64 xpos, ypos;
	glfwGetCursorPos(_window, &xpos, &ypos);
	return {xpos, ypos};
}

Key_Action Glfw_Input::_key(Key_Code code) {
	return from_glfw_key_action_(glfwGetKey(_window, to_glfw_key_code_(code)));
}

Key_Action Glfw_Input::_mouse(Mouse_Code code) {
	return from_glfw_key_action_(glfwGetKey(_window, to_glfw_mouse_code_(code)));
}

}
