#ifndef LICH_EVENT_HPP
#define LICH_EVENT_HPP

#include "input.hpp"

namespace lich {

enum class EventVariant {
	None = 0,
	WindowClose,
	WindowFocus,
	WindowBlur,
	WindowSize,
	WindowMove,
	KeyPress,
	KeyRelease,
	MousePress,
	MouseRelease,
	MouseMove,
	MouseScroll,
};

using EventFlags = U8;

enum class EventFlag: EventFlags {
	None = 0,
	App = 0x1,
	Window = 0x2,
	Input = 0x4,
	Key = 0x8,
	Mouse = 0x10,
	MouseButton = 0x20,
};

struct Event {
	bool handled;

	virtual ~Event() = default;
	
	virtual const char *name() const = 0;
	virtual EventVariant variant() const = 0;
	virtual EventFlags flags() const = 0;
	
	virtual std::string string() const { return name(); }
};

template<typename Type>
concept EventDerived = std::is_base_of_v<Event, Type> && requires {
	{ Type::static_variant() } -> std::same_as<EventVariant>;
};

struct EventDispatcher {
	template<EventDerived Type>
	using Callback = std::function<bool(Type &event)>;
	
	Event &event;

	EventDispatcher(Event &event): event{event} {}

	template<EventDerived Type>
	bool handle(Callback<Type> callback)
	{
		if (event.variant() == Type::static_variant()) {
			event.handled = callback(static_cast<Type &>(event));
			return true;
		}
		return false;
	}
};

#define GEN_EVENT_FUNCTIONS(NAME, VARIANT, FLAGS) \
	static EventVariant static_variant() { return VARIANT; } \
	const char *name() const override { return NAME; } \
	EventVariant variant() const override { return VARIANT; } \
	EventFlags flags() const override { return FLAGS; }
#define GEN_EVENT_STRING(...) \
	std::string string() const \
	{ \
		std::stringstream stream; \
		stream << __VA_ARGS__; \
		return stream.str(); \
	}

struct WindowCloseEvent: public Event {
	WindowCloseEvent() = default;

	GEN_EVENT_FUNCTIONS("WindowCloseEvent", EventVariant::WindowClose,
		(U8)EventFlag::Window)
};

struct WindowFocusEvent: public Event {
	WindowFocusEvent() = default;

	GEN_EVENT_FUNCTIONS("WindowFocusEvent", EventVariant::WindowFocus,
		(U8)EventFlag::Window)
};

struct WindowBlurEvent: public Event {
	WindowBlurEvent() = default;

	GEN_EVENT_FUNCTIONS("WindowBlurEvent", EventVariant::WindowBlur,
		(U8)EventFlag::Window)
};

struct WindowSizeEvent: public Event {
	U32 width;
	U32 height;

	WindowSizeEvent(U32 width, U32 height): width{width}, height{height} {}
	WindowSizeEvent(int width, int height):
		WindowSizeEvent{static_cast<U32>(width), static_cast<U32>(height)} {}

	GEN_EVENT_FUNCTIONS("WindowSizeEvent", EventVariant::WindowSize,
		(U8)EventFlag::Window)
	GEN_EVENT_STRING(name() <<
		"{.width = " << width << ", .height = " << height << "}")
};

struct WindowMoveEvent: public Event {
	I32 x;
	I32 y;
	
	WindowMoveEvent(I32 x, I32 y): x{x}, y{y} {}

	GEN_EVENT_FUNCTIONS("WindowMoveEvent", EventVariant::WindowMove,
		(U8)EventFlag::Window)
	GEN_EVENT_STRING(name() << "{.x = " << x << ", .y = " << y << "}")
};

struct KeyPressEvent: public Event {
	KeyCode code;
	U32 repeat;

	KeyPressEvent(KeyCode code, U32 repeat = 0): code{code}, repeat{repeat} {}

	GEN_EVENT_FUNCTIONS("KeyPressEvent", EventVariant::KeyPress,
		(U8)EventFlag::Input | (U8)EventFlag::Key)
	GEN_EVENT_STRING(name() <<
		"{.code = " << (int)code << ", .repeat = " << repeat << "}")
};

struct KeyReleaseEvent: public Event {
	KeyCode code;

	KeyReleaseEvent(KeyCode code): code{code} {}

	GEN_EVENT_FUNCTIONS("KeyReleaseEvent", EventVariant::KeyRelease,
		(U8)EventFlag::Input | (U8)EventFlag::Key)
	GEN_EVENT_STRING(name() << "{.code = " << (int)code << "}")
};

struct MousePressEvent: public Event {
	MouseCode code;
	U32 repeat;

	MousePressEvent(MouseCode code, U32 repeat = 0):
		code{code}, repeat{repeat} {}

	GEN_EVENT_FUNCTIONS("MousePressEvent", EventVariant::MousePress,
		(U8)EventFlag::Input | (U8)EventFlag::Mouse | (U8)EventFlag::MouseButton)
	GEN_EVENT_STRING(name() <<
		"{.code = " << (int)code << ", .repeat = " << repeat << "}")
};

struct MouseReleaseEvent: public Event {
	MouseCode code;

	MouseReleaseEvent(MouseCode code): code{code} {}

	GEN_EVENT_FUNCTIONS("MouseReleaseEvent", EventVariant::MouseRelease,
		(U8)EventFlag::Input | (U8)EventFlag::Mouse | (U8)EventFlag::MouseButton)
	GEN_EVENT_STRING(name() <<
		"{.code = " << (int)code << "}")
};

struct MouseMoveEvent: public Event {
	F64 x;
	F64 y;

	MouseMoveEvent(F64 x, F64 y): x{x}, y{y} {}

	GEN_EVENT_FUNCTIONS("MouseMoveEvent", EventVariant::MouseMove,
		(U8)EventFlag::Input | (U8)EventFlag::Mouse)
	GEN_EVENT_STRING(name() << "{.x = " << x << ", .y = " << y << "}")
};

struct MouseScrollEvent: public Event {
	F64 x;
	F64 y;

	MouseScrollEvent(F64 x, F64 y): x{x}, y{y} {}

	GEN_EVENT_FUNCTIONS("MouseScrollEvent", EventVariant::MouseScroll,
		(U8)EventFlag::Input | (U8)EventFlag::Mouse)
	GEN_EVENT_STRING(name() << "{.x = " << x << ", .y = " << y << "}")
};

}

#undef GEN_EVENT_FUNCTIONS
#undef GEN_EVENT_STRING

#endif
