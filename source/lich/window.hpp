#ifndef LICH_WINDOW_HPP
#define LICH_WINDOW_HPP

namespace lich {

struct Window_Spec {
	std::string title = "Lich Engine";
	U32 width = 960;
	U32 height = 540;
	bool visible = false;
};

class Window {
public:
	using Close_Callback = std::function<void(Window &self)>;
	
	static std::unique_ptr<Window> create(const Window_Spec &window_spec = {});

	virtual ~Window(void) = default;

	virtual void update(void) = 0;
	virtual void clear(void) = 0;
	virtual void present(void) = 0;
	
	virtual bool success(void) const = 0;
	virtual bool should_close(void) const = 0;
	virtual bool visible(void) const = 0;
	virtual void set_visible(bool visible) = 0;
	virtual bool focused(void) const = 0;
	virtual void set_focused(bool focused) = 0;

	virtual void *handle(void) const = 0;
	virtual const std::string &title(void) const = 0;
	virtual void set_title(const std::string &title) = 0;
	virtual std::pair<I32, I32> pos(void) const = 0;
	virtual void set_pos(I32 x, I32 y) = 0;
	virtual std::pair<U32, U32> size(void) const = 0;
	virtual void set_size(U32 width, U32 height) = 0;
	virtual std::pair<U32, U32> screen_size(void) const = 0;

	virtual void set_close_callback(const Close_Callback &callback) = 0;
	virtual void move_to_center(void) = 0;
};

}

#endif
