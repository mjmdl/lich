#ifndef LICH_EVENT_HPP
#define LICH_EVENT_HPP

#include "input.hpp"

namespace lich {

enum class Event_Variant {
	None = 0,
	Window_Close,
	Window_Focus,
	Window_Blur,
	Window_Size,
	Window_Move,
	Key_Press,
	Key_Release,
	Mouse_Press,
	Mouse_Release,
	Mouse_Move,
	Mouse_Scroll,
};

using Event_Flags = U8;

enum class Event_Flag : Event_Flags {
	None = 0,
	App = 0x1,
	Window = 0x2,
	Input = 0x4,
	Key = 0x8,
	Mouse = 0x10,
	Mouse_Button = 0x20,
};

struct Event {
	bool handled;

	virtual ~Event(void) = default;
	
	virtual const char *name(void) const = 0;
	virtual Event_Variant variant(void) const = 0;
	virtual Event_Flags flags(void) const = 0;
	
	virtual std::string string(void) const { return name(); }
};

template<typename Type>
concept Event_Derived = std::is_base_of_v<Event, Type> &&
	requires {
		{ Type::static_variant() } -> std::same_as<Event_Variant>;
	};

struct Event_Dispatcher {
	template<Event_Derived Type>
	using Callback = std::function<bool(Type &event)>;
	
	Event &event;

	Event_Dispatcher(Event &event): event{event} {}

	template<Event_Derived Type>
	bool handle(Callback<Type> callback) {
		if (event.variant() == Type::static_variant()) {
			event.handled = callback(static_cast<Type &>(event));
			return true;
		}
		return false;
	}
};

#define GEN_EVENT_FUNCTIONS(NAME, VARIANT, FLAGS) \
	static Event_Variant static_variant(void) { return VARIANT; } \
	const char *name(void) const override { return NAME; } \
	Event_Variant variant(void) const override { return VARIANT; } \
	Event_Flags flags(void) const override { return FLAGS; }
#define GEN_EVENT_STRING(...) \
	std::string string(void) const { \
		std::stringstream stream; \
		stream << __VA_ARGS__; \
		return stream.str(); \
	}

struct Window_Close_Event : public Event {
	Window_Close_Event(void) = default;

	GEN_EVENT_FUNCTIONS("Window_Close_Event", Event_Variant::Window_Close,
		(U8)Event_Flag::Window)
};

struct Window_Focus_Event : public Event {
	Window_Focus_Event(void) = default;

	GEN_EVENT_FUNCTIONS("Window_Focus_Event", Event_Variant::Window_Focus,
		(U8)Event_Flag::Window)
};

struct Window_Blur_Event : public Event {
	Window_Blur_Event(void) = default;

	GEN_EVENT_FUNCTIONS("Window_Blur_Event", Event_Variant::Window_Blur,
		(U8)Event_Flag::Window)
};

struct Window_Size_Event : public Event {
	U32 width;
	U32 height;

	Window_Size_Event(U32 width, U32 height): width{width}, height{height} {}
	Window_Size_Event(int width, int height):
		Window_Size_Event{static_cast<U32>(width), static_cast<U32>(height)} {}

	GEN_EVENT_FUNCTIONS("Window_Size_Event", Event_Variant::Window_Size,
		(U8)Event_Flag::Window)
	GEN_EVENT_STRING(name() <<
		"{.width = " << width << ", .height = " << height << "}")
};

struct Window_Move_Event : public Event {
	I32 x;
	I32 y;
	
	Window_Move_Event(I32 x, I32 y): x{x}, y{y} {}

	GEN_EVENT_FUNCTIONS("Window_Move_Event", Event_Variant::Window_Move,
		(U8)Event_Flag::Window)
	GEN_EVENT_STRING(name() << "{.x = " << x << ", .y = " << y << "}")
};

struct Key_Press_Event : public Event {
	Key_Code code;
	U32 repeat;

	Key_Press_Event(Key_Code code, U32 repeat = 0): code{code}, repeat{repeat} {}

	GEN_EVENT_FUNCTIONS("Key_Press_Event", Event_Variant::Key_Press,
		(U8)Event_Flag::Input | (U8)Event_Flag::Key)
	GEN_EVENT_STRING(name() <<
		"{.code = " << (int)code << ", .repeat = " << repeat << "}")
};

struct Key_Release_Event : public Event {
	Key_Code code;

	Key_Release_Event(Key_Code code): code{code} {}

	GEN_EVENT_FUNCTIONS("Key_Release_Event", Event_Variant::Key_Release,
		(U8)Event_Flag::Input | (U8)Event_Flag::Key)
	GEN_EVENT_STRING(name() << "{.code = " << (int)code << "}")
};

struct Mouse_Press_Event : public Event {
	Mouse_Code code;
	U32 repeat;

	Mouse_Press_Event(Mouse_Code code, U32 repeat = 0):
		code{code}, repeat{repeat} {}

	GEN_EVENT_FUNCTIONS("Mouse_Press_Event", Event_Variant::Mouse_Press,
		(U8)Event_Flag::Input | (U8)Event_Flag::Mouse | (U8)Event_Flag::Mouse_Button)
	GEN_EVENT_STRING(name() <<
		"{.code = " << (int)code << ", .repeat = " << repeat << "}")
};

struct Mouse_Release_Event : public Event {
	Mouse_Code code;

	Mouse_Release_Event(Mouse_Code code): code{code} {}

	GEN_EVENT_FUNCTIONS("Mouse_Release_Event", Event_Variant::Mouse_Release,
		(U8)Event_Flag::Input | (U8)Event_Flag::Mouse | (U8)Event_Flag::Mouse_Button)
	GEN_EVENT_STRING(name() <<
		"{.code = " << (int)code << "}")
};

struct Mouse_Move_Event : public Event {
	F64 x;
	F64 y;

	Mouse_Move_Event(F64 x, F64 y) : x{x}, y{y} {}

	GEN_EVENT_FUNCTIONS("Mouse_Move_Event", Event_Variant::Mouse_Move,
		(U8)Event_Flag::Input | (U8)Event_Flag::Mouse)
	GEN_EVENT_STRING(name() << "{.x = " << x << ", .y = " << y << "}")
};

struct Mouse_Scroll_Event : public Event {
	F64 x;
	F64 y;

	Mouse_Scroll_Event(F64 x, F64 y) : x{x}, y{y} {}

	GEN_EVENT_FUNCTIONS("Mouse_Scroll_Event", Event_Variant::Mouse_Scroll,
		(U8)Event_Flag::Input | (U8)Event_Flag::Mouse)
	GEN_EVENT_STRING(name() << "{.x = " << x << ", .y = " << y << "}")
};

}

#undef GEN_EVENT_FUNCTIONS
#undef GEN_EVENT_STRING

#endif
