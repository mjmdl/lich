#ifndef LICH_INPUT_HPP
#define LICH_INPUT_HPP

namespace lich
{

enum class Key_Action : U8
{
	None = 0,
	Up,
	Down,
	Repeat,
};

enum class Key_Code
{
	None            = -1,
	Space           = 32,
	Apostrophe      = 39,
	Comma           = 44,
	Minus           = 45,
	Period          = 46,
	Slash           = 47,
	Number_0        = 48,
	Number_1        = 49,
	Number_2        = 50,
	Number_3        = 51,
	Number_4        = 52,
	Number_5        = 53,
	Number_6        = 54,
	Number_7        = 55,
	Number_8        = 56,
	Number_9        = 57,
	Semicolon       = 59,
	Equal           = 61,
	A               = 65,
	B               = 66,
	C               = 67,
	D               = 68,
	E               = 69,
	F               = 70,
	G               = 71,
	H               = 72,
	I               = 73,
	J               = 74,
	K               = 75,
	L               = 76,
	M               = 77,
	N               = 78,
	O               = 79,
	P               = 80,
	Q               = 81,
	R               = 82,
	S               = 83,
	T               = 84,
	U               = 85,
	V               = 86,
	W               = 87,
	X               = 88,
	Y               = 89,
	Z               = 90,
	Left_Bracket    = 91,
	Back_slash      = 92,
	Right_Bracket   = 93,
	Grave_Accent    = 96,
	World_1         = 161,
	World_2         = 162,
	Escape          = 256,
	Enter           = 257,
	Tab             = 258,
	Backspace       = 259,
	Insert          = 260,
	Delete          = 261,
	Right           = 262,
	Left            = 263,
	Down            = 264,
	Up              = 265,
	Page_Up         = 266,
	Page_Down       = 267,
	Home            = 268,
	End             = 269,
	Caps_Lock       = 280,
	Scroll_Lock     = 281,
	Num_Lock        = 282,
	Print_Screen    = 283,
	Pause           = 284,
	F1              = 290,
	F2              = 291,
	F3              = 292,
	F4              = 293,
	F5              = 294,
	F6              = 295,
	F7              = 296,
	F8              = 297,
	F9              = 298,
	F10             = 299,
	F11             = 300,
	F12             = 301,
	F13             = 302,
	F14             = 303,
	F15             = 304,
	F16             = 305,
	F17             = 306,
	F18             = 307,
	F19             = 308,
	F20             = 309,
	F21             = 310,
	F22             = 311,
	F23             = 312,
	F24             = 313,
	F25             = 314,
	Keypad_0        = 320,
	Keypad_1        = 321,
	Keypad_2        = 322,
	Keypad_3        = 323,
	Keypad_4        = 324,
	Keypad_5        = 325,
	Keypad_6        = 326,
	Keypad_7        = 327,
	Keypad_8        = 328,
	Keypad_9        = 329,
	Keypad_Decimal  = 330,
	Keypad_Divide   = 331,
	Keypad_Multiply = 332,
	Keypad_Subtract = 333,
	Keypad_Add      = 334,
	Keypad_Enter    = 335,
	Keypad_Equal    = 336,
	Left_Shift      = 340,
	Left_Control    = 341,
	Left_Alt        = 342,
	Left_Super      = 343,
	Right_Shift     = 344,
	Right_Control   = 345,
	Right_Alt       = 346,
	Right_Super     = 347,
	Menu            = 348,
};

enum class Mouse_Code
{
	None          = -1,
	Button_1      = 0,
	Button_2      = 1,
	Button_3      = 2,
	Button_4      = 3,
	Button_5      = 4,
	Button_6      = 5,
	Button_7      = 6,
	Button_8      = 7,
	Button_Last   = Button_8,
	Button_Left   = Button_1,
	Button_Right  = Button_2,
	Button_Middle = Button_3,
};

class Input
{
public:
	static bool
	key_down(Key_Code code)
	{
		return singleton_->_key_down(code);
	}
	
	static bool
	mouse_down(Mouse_Code code)
	{
		return singleton_->_mouse_down(code);
	}

	static std::pair<F64, F64>
	mouse_pos()
	{
		return singleton_->_mouse_pos();
	}

protected:
	virtual bool _key_down(Key_Code code) = 0;
	virtual bool _mouse_down(Mouse_Code code) = 0;
	virtual std::pair<F64, F64> _mouse_pos() = 0;

protected:
	inline static std::unique_ptr<Input> singleton_;
};

}

#endif
