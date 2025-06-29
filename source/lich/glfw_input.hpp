#ifndef LICH_GLFW_INPUT_HPP
#define LICH_GLFW_INPUT_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input.hpp"

namespace lich {

class GlfwInput final: public Input {
public:
	static void init(GLFWwindow *window);

	GlfwInput(GLFWwindow *window);
	
protected:
	bool _key_down(KeyCode code) override;
	bool _mouse_down(MouseCode code) override;
	std::pair<F64, F64> _mouse_pos() override;

private:
	KeyAction _key(KeyCode code);
	KeyAction _mouse(MouseCode code);
	
private:
	GLFWwindow *_window;
};

}

#endif
