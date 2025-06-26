#ifndef LICH_GLFW_WINDOW_HPP
#define LICH_GLFW_WINDOW_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "log.hpp"
#include "window.hpp"

namespace lich {

class Glfw_Window final : public Window {
public:
	Glfw_Window(const Window_Spec &window_spec = {});
	~Glfw_Window(void) override;

	void update(void) override;
	void clear(void) override;
	void present(void) override;

	bool success(void) const override;
	bool should_close(void) const override;
	bool visible(void) const override;
	void set_visible(bool visible) override;
	bool focused(void) const override;
	void set_focused(bool focused) override;

	void *handle(void) const override;
	const std::string &title(void) const override;
	void set_title(const std::string &title) override;
	std::pair<I32, I32> pos(void) const override;
	void set_pos(I32 x, I32 y) override;
	std::pair<U32, U32> size(void) const override;
	void set_size(U32 width, U32 height) override;
	std::pair<U32, U32> screen_size(void) const override;

	void set_close_callback(const Close_Callback &callback) override;
	void move_to_center(void) override;

private:
	static void glfw_error_callback_(int error, const char *description);
	
private:
	inline static bool glfw_init_ = false;
	inline static U32 window_count_ = 0;
	inline static Logger logger_{"lich::Glfw_Window"};
	
	GLFWwindow *_window;
	std::string _title;
	Close_Callback _close_callback;
	bool _success;
};

}

#endif
