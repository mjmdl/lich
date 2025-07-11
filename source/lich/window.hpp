#ifndef LICH_WINDOW_HPP
#define LICH_WINDOW_HPP

#include "event.hpp"

namespace lich {

struct Window_Spec {
	std::string title = "Lich Engine";
	U32 width = 960;
	U32 height = 540;
	bool visible = false;
};

class Window {
public:
	using Event_Callback = std::function<bool(Window &self, Event &event)>;
	
	static std::unique_ptr<Window> create(const Window_Spec &window_spec = {});

	virtual ~Window() = default;

	virtual void update() = 0;
	virtual void clear() = 0;
	virtual void present() = 0;
	
	virtual bool success() const = 0;
	virtual bool should_close() const = 0;
	virtual bool visible() const = 0;
	virtual void set_visible(bool visible) = 0;
	virtual bool focused() const = 0;
	virtual void set_focused(bool focused) = 0;

	virtual void *handle() const = 0;
	virtual const std::string &title() const = 0;
	virtual void set_title(const std::string &title) = 0;
	virtual std::pair<I32, I32> pos() const = 0;
	virtual void set_pos(I32 x, I32 y) = 0;
	virtual std::pair<U32, U32> size() const = 0;
	virtual void set_size(U32 width, U32 height) = 0;
	virtual std::pair<U32, U32> screen_size() const = 0;

	virtual void set_event_callback(const Event_Callback &callback) = 0;
	virtual void move_to_center() = 0;
};

}

#endif
