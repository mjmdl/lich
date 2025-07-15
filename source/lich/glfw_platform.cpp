#include <GLFW/glfw3.h>

#include "platform.hpp"

namespace lich {

float Platform::get_time() {
	return glfwGetTime();
}

}
