#ifndef LICH_GLFW_INPUT_HPP
#define LICH_GLFW_INPUT_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input.hpp"

namespace lich {

class Glfw_Input final : public Input {
public:
	static void init(GLFWwindow *window);

	Glfw_Input(GLFWwindow *window);
	
protected:
	bool _key_down(Key_Code code) override;
	bool _mouse_down(Mouse_Code code) override;
	std::pair<F64, F64> _mouse_pos(void) override;

private:
	Key_Action _key(Key_Code code);
	Key_Action _mouse(Mouse_Code code);
	
private:
	GLFWwindow *_window;
};

}

#endif
