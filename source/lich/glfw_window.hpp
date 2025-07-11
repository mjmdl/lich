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
	~Glfw_Window() override;

	void update() override;
	void clear() override;
	void present() override;

	bool success() const override;
	bool should_close() const override;
	bool visible() const override;
	void set_visible(bool visible) override;
	bool focused() const override;
	void set_focused(bool focused) override;

	void *handle() const override;
	const std::string &title() const override;
	void set_title(const std::string &title) override;
	std::pair<I32, I32> pos() const override;
	void set_pos(I32 x, I32 y) override;
	std::pair<U32, U32> size() const override;
	void set_size(U32 width, U32 height) override;
	std::pair<U32, U32> screen_size() const override;

	void set_event_callback(const Event_Callback &callback) override;
	void move_to_center() override;

private:
	static void glfw_error_callback_(int error, const char *description);
	static Glfw_Window *window_self_(GLFWwindow *handle);

	static void glfw_close_callback_(GLFWwindow *window);
	static void glfw_focus_callback_(GLFWwindow *window, int focused);
	static void glfw_pos_callback_(GLFWwindow *window, int xpos, int ypos);
	static void glfw_size_callback_(GLFWwindow *window, int width, int height);
	static void glfw_key_callback_(GLFWwindow *window, int key,
		int scancode, int action, int mods);
	static void glfw_mouse_button_callback_(GLFWwindow *window, int button,
		int action, int mods);
	static void glfw_cursor_pos_callback_(GLFWwindow *window,
		double xpos, double ypos);
	static void glfw_scroll_callback_(GLFWwindow *window,
		double xoffset, double yoffset);
	
private:
	inline static bool glfw_init_ = false;
	inline static U32 window_count_ = 0;
	inline static Logger logger_{"lich::Glfw_Window"};
	
	GLFWwindow *_window;
	std::string _title;
	Event_Callback _event_callback;
	bool _success;
};

}

#endif
