#pragma once

#include <GLFW/glfw3.h>
#include <string>

enum class EStateInput : unsigned char
{
	UP = 0,
	DOWN = 1,
};

enum class EMouseButton : unsigned char
{
	B1 = GLFW_MOUSE_BUTTON_1,                //
	B2 = GLFW_MOUSE_BUTTON_2,                //
	B3 = GLFW_MOUSE_BUTTON_3,                //
	B4 = GLFW_MOUSE_BUTTON_4,                //
	B5 = GLFW_MOUSE_BUTTON_5,                //
	B6 = GLFW_MOUSE_BUTTON_6,                //
	B7 = GLFW_MOUSE_BUTTON_7,                //
	B8 = GLFW_MOUSE_BUTTON_8,                //
	LEFT = GLFW_MOUSE_BUTTON_LEFT,           //
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT,         //
	MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,       //
	LAST = GLFW_MOUSE_BUTTON_8,              //
};

enum class EKey : int
{
	UNKNOWN = GLFW_KEY_UNKNOWN,              //
	SPACE = GLFW_KEY_SPACE,                  //
	APOSTROPHE = GLFW_KEY_APOSTROPHE,        //
	COMMA = GLFW_KEY_COMMA,                  //
	MINUS = GLFW_KEY_MINUS,                  //
	PERIOD = GLFW_KEY_PERIOD,                //
	SLASH = GLFW_KEY_SLASH,                  //
	K0 = GLFW_KEY_0,                         //
	K1 = GLFW_KEY_1,                         //
	K2 = GLFW_KEY_2,                         //
	K3 = GLFW_KEY_3,                         //
	K4 = GLFW_KEY_4,                         //
	K5 = GLFW_KEY_5,                         //
	K6 = GLFW_KEY_6,                         //
	K7 = GLFW_KEY_7,                         //
	K8 = GLFW_KEY_8,                         //
	K9 = GLFW_KEY_9,                         //
	SEMICOLON = GLFW_KEY_SEMICOLON,          //
	EQUAL = GLFW_KEY_EQUAL,                  //
	A = GLFW_KEY_A,                          //
	B = GLFW_KEY_B,                          //
	C = GLFW_KEY_C,                          //
	D = GLFW_KEY_D,                          //
	E = GLFW_KEY_E,                          //
	F = GLFW_KEY_F,                          //
	G = GLFW_KEY_G,                          //
	H = GLFW_KEY_H,                          //
	I = GLFW_KEY_I,                          //
	J = GLFW_KEY_J,                          //
	K = GLFW_KEY_K,                          //
	L = GLFW_KEY_L,                          //
	M = GLFW_KEY_M,                          //
	N = GLFW_KEY_N,                          //
	O = GLFW_KEY_O,                          //
	P = GLFW_KEY_P,                          //
	Q = GLFW_KEY_Q,                          //
	R = GLFW_KEY_R,                          //
	S = GLFW_KEY_S,                          //
	T = GLFW_KEY_T,                          //
	U = GLFW_KEY_U,                          //
	V = GLFW_KEY_V,                          //
	W = GLFW_KEY_W,                          //
	X = GLFW_KEY_X,                          //
	Y = GLFW_KEY_Y,                          //
	Z = GLFW_KEY_Z,                          //
	LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,    //
	BACKSLASH = GLFW_KEY_BACKSLASH,          //
	RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,  //
	GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,    //
	WORLD_1 = GLFW_KEY_WORLD_1,              //
	WORLD_2 = GLFW_KEY_WORLD_2,              //
	ESCAPE = GLFW_KEY_ESCAPE,                //
	ENTER = GLFW_KEY_ENTER,                  //
	TAB = GLFW_KEY_TAB,                      //
	BACKSPACE = GLFW_KEY_BACKSPACE,          //
	INSERT = GLFW_KEY_INSERT,                //
	DELETE = GLFW_KEY_DELETE,                //
	RIGHT = GLFW_KEY_RIGHT,                  //
	LEFT = GLFW_KEY_LEFT,                    //
	DOWN = GLFW_KEY_DOWN,                    //
	UP = GLFW_KEY_UP,                        //
	PAGE_UP = GLFW_KEY_PAGE_UP,              //
	PAGE_DOWN = GLFW_KEY_PAGE_DOWN,          //
	HOME = GLFW_KEY_HOME,                    //
	END = GLFW_KEY_END,                      //
	CAPS_LOCK = GLFW_KEY_CAPS_LOCK,          //
	SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,      //
	NUM_LOCK = GLFW_KEY_NUM_LOCK,            //
	PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,    //
	PAUSE = GLFW_KEY_PAUSE,                  //
	F1 = GLFW_KEY_F1,                        //
	F2 = GLFW_KEY_F2,                        //
	F3 = GLFW_KEY_F3,                        //
	F4 = GLFW_KEY_F4,                        //
	F5 = GLFW_KEY_F5,                        //
	F6 = GLFW_KEY_F6,                        //
	F7 = GLFW_KEY_F7,                        //
	F8 = GLFW_KEY_F8,                        //
	F9 = GLFW_KEY_F9,                        //
	F10 = GLFW_KEY_F10,                      //
	F11 = GLFW_KEY_F11,                      //
	F12 = GLFW_KEY_F12,                      //
	F13 = GLFW_KEY_F13,                      //
	F14 = GLFW_KEY_F14,                      //
	F15 = GLFW_KEY_F15,                      //
	F16 = GLFW_KEY_F16,                      //
	F17 = GLFW_KEY_F17,                      //
	F18 = GLFW_KEY_F18,                      //
	F19 = GLFW_KEY_F19,                      //
	F20 = GLFW_KEY_F20,                      //
	F21 = GLFW_KEY_F21,                      //
	F22 = GLFW_KEY_F22,                      //
	F23 = GLFW_KEY_F23,                      //
	F24 = GLFW_KEY_F24,                      //
	F25 = GLFW_KEY_F25,                      //
	KP_0 = GLFW_KEY_KP_0,                    //
	KP_1 = GLFW_KEY_KP_1,                    //
	KP_2 = GLFW_KEY_KP_2,                    //
	KP_3 = GLFW_KEY_KP_3,                    //
	KP_4 = GLFW_KEY_KP_4,                    //
	KP_5 = GLFW_KEY_KP_5,                    //
	KP_6 = GLFW_KEY_KP_6,                    //
	KP_7 = GLFW_KEY_KP_7,                    //
	KP_8 = GLFW_KEY_KP_8,                    //
	KP_9 = GLFW_KEY_KP_9,                    //
	KP_DECIMAL = GLFW_KEY_KP_DECIMAL,        //
	KP_DIVIDE = GLFW_KEY_KP_DIVIDE,          //
	KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,      //
	KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,      //
	KP_ADD = GLFW_KEY_KP_ADD,                //
	KP_ENTER = GLFW_KEY_KP_ENTER,            //
	KP_EQUAL = GLFW_KEY_KP_EQUAL,            //
	LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,        //
	LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,    //
	LEFT_ALT = GLFW_KEY_LEFT_ALT,            //
	LEFT_SUPER = GLFW_KEY_LEFT_SUPER,        //
	RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,      //
	RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,  //
	RIGHT_ALT = GLFW_KEY_RIGHT_ALT,          //
	RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,      //
	MENU = GLFW_KEY_MENU
};

inline std::string ToString(const EMouseButton b) noexcept
{
	switch (b)
	{
	case EMouseButton::B1:
		return "EMouseButton::LEFT";

	case EMouseButton::B2:
		return "EMouseButton::RIGHT";

	case EMouseButton::B3:
		return "EMouseButton::MIDDLE";

	case EMouseButton::B4:
		return "EMouseButton::B4";

	case EMouseButton::B5:
		return "EMouseButton::B5";

	case EMouseButton::B6:
		return "EMouseButton::B6";

	case EMouseButton::B7:
		return "EMouseButton::B7";

	case EMouseButton::B8:
		return "EMouseButton::B8";

	default:
		return "Unknow mouse button";
	}
}

inline std::string ToString(const EKey key) noexcept
{
	switch (key)
	{
	case EKey::UNKNOWN:
		return "EKey::UNKNOWN";

	case EKey::SPACE:
		return "EKey::SPACE";

	case EKey::APOSTROPHE:
		return "EKey::APOSTROPHE";

	case EKey::COMMA:
		return "EKey::COMMA";

	case EKey::MINUS:
		return "EKey::MINUS";

	case EKey::PERIOD:
		return "EKey::PERIOD";

	case EKey::SLASH:
		return "EKey::SLASH";

	case EKey::K0:
		return "EKey::K0";

	case EKey::K1:
		return "EKey::K1";

	case EKey::K2:
		return "EKey::K2";

	case EKey::K3:
		return "EKey::K3";

	case EKey::K4:
		return "EKey::K4";

	case EKey::K5:
		return "EKey::K5";

	case EKey::K6:
		return "EKey::K6";

	case EKey::K7:
		return "EKey::K7";

	case EKey::K8:
		return "EKey::K8";

	case EKey::K9:
		return "EKey::K9";

	case EKey::SEMICOLON:
		return "EKey::SEMICOLON";

	case EKey::EQUAL:
		return "EKey::EQUAL";

	case EKey::A:
		return "EKey::A";

	case EKey::B:
		return "EKey::B";

	case EKey::C:
		return "EKey::C";

	case EKey::D:
		return "EKey::D";

	case EKey::E:
		return "EKey::E";

	case EKey::F:
		return "EKey::F";

	case EKey::G:
		return "EKey::G";

	case EKey::H:
		return "EKey::H";

	case EKey::I:
		return "EKey::I";

	case EKey::J:
		return "EKey::J";

	case EKey::K:
		return "EKey::K";

	case EKey::L:
		return "EKey::L";

	case EKey::M:
		return "EKey::M";

	case EKey::N:
		return "EKey::N";

	case EKey::O:
		return "EKey::O";

	case EKey::P:
		return "EKey::P";

	case EKey::Q:
		return "EKey::Q";

	case EKey::R:
		return "EKey::R";

	case EKey::S:
		return "EKey::S";

	case EKey::T:
		return "EKey::T";

	case EKey::U:
		return "EKey::U";

	case EKey::V:
		return "EKey::V";

	case EKey::W:
		return "EKey::W";

	case EKey::X:
		return "EKey::X";

	case EKey::Y:
		return "EKey::Y";

	case EKey::Z:
		return "EKey::Z";

	case EKey::LEFT_BRACKET:
		return "EKey::LEFT_BRACKET";

	case EKey::BACKSLASH:
		return "EKey::BACKSLASH";

	case EKey::RIGHT_BRACKET:
		return "EKey::RIGHT_BRACKET";

	case EKey::GRAVE_ACCENT:
		return "EKey::GRAVE_ACCENT";

	case EKey::WORLD_1:
		return "EKey::WORLD_1";

	case EKey::WORLD_2:
		return "EKey::WORLD_2";

	case EKey::ESCAPE:
		return "EKey::ESCAPE";

	case EKey::ENTER:
		return "EKey::ENTER";

	case EKey::TAB:
		return "EKey::TAB";

	case EKey::BACKSPACE:
		return "EKey::BACKSPACE";

	case EKey::INSERT:
		return "EKey::INSERT";

	case EKey::DELETE:
		return "EKey::DELETE";

	case EKey::RIGHT:
		return "EKey::RIGHT";

	case EKey::LEFT:
		return "EKey::LEFT";

	case EKey::DOWN:
		return "EKey::DOWN";

	case EKey::UP:
		return "EKey::UP";

	case EKey::PAGE_UP:
		return "EKey::PAGE_UP";

	case EKey::PAGE_DOWN:
		return "EKey::PAGE_DOWN";

	case EKey::HOME:
		return "EKey::HOME";

	case EKey::END:
		return "EKey::END";

	case EKey::CAPS_LOCK:
		return "EKey::CAPS_LOCK";

	case EKey::SCROLL_LOCK:
		return "EKey::SCROLL_LOCK";

	case EKey::NUM_LOCK:
		return "EKey::NUM_LOCK";

	case EKey::PRINT_SCREEN:
		return "EKey::PRINT_SCREEN";

	case EKey::PAUSE:
		return "EKey::PAUSE";

	case EKey::F1:
		return "EKey::F1";

	case EKey::F2:
		return "EKey::F2";

	case EKey::F3:
		return "EKey::F3";

	case EKey::F4:
		return "EKey::F4";

	case EKey::F5:
		return "EKey::F5";

	case EKey::F6:
		return "EKey::F6";

	case EKey::F7:
		return "EKey::F7";

	case EKey::F8:
		return "EKey::F8";

	case EKey::F9:
		return "EKey::F9";

	case EKey::F10:
		return "EKey::F10";

	case EKey::F11:
		return "EKey::F11";

	case EKey::F12:
		return "EKey::F12";

	case EKey::F13:
		return "EKey::F13";

	case EKey::F14:
		return "EKey::F14";

	case EKey::F15:
		return "EKey::F15";

	case EKey::F16:
		return "EKey::F16";

	case EKey::F17:
		return "EKey::F17";

	case EKey::F18:
		return "EKey::F18";

	case EKey::F19:
		return "EKey::F19";

	case EKey::F20:
		return "EKey::F20";

	case EKey::F21:
		return "EKey::F21";

	case EKey::F22:
		return "EKey::F22";

	case EKey::F23:
		return "EKey::F23";

	case EKey::F24:
		return "EKey::F24";

	case EKey::F25:
		return "EKey::F25";

	case EKey::KP_0:
		return "EKey::KP_0";

	case EKey::KP_1:
		return "EKey::KP_1";

	case EKey::KP_2:
		return "EKey::KP_2";

	case EKey::KP_3:
		return "EKey::KP_3";

	case EKey::KP_4:
		return "EKey::KP_4";

	case EKey::KP_5:
		return "EKey::KP_5";

	case EKey::KP_6:
		return "EKey::KP_6";

	case EKey::KP_7:
		return "EKey::KP_7";

	case EKey::KP_8:
		return "EKey::KP_8";

	case EKey::KP_9:
		return "EKey::KP_9";

	case EKey::KP_DECIMAL:
		return "EKey::KP_DECIMAL";

	case EKey::KP_DIVIDE:
		return "EKey::KP_DIVIDE";

	case EKey::KP_MULTIPLY:
		return "EKey::KP_MULTIPLY";

	case EKey::KP_SUBTRACT:
		return "EKey::KP_SUBTRACT";

	case EKey::KP_ADD:
		return "EKey::KP_ADD";

	case EKey::KP_ENTER:
		return "EKey::KP_ENTER";

	case EKey::KP_EQUAL:
		return "EKey::KP_EQUAL";

	case EKey::LEFT_SHIFT:
		return "EKey::LEFT_SHIFT";

	case EKey::LEFT_CONTROL:
		return "EKey::LEFT_CONTROL";

	case EKey::LEFT_ALT:
		return "EKey::LEFT_ALT";

	case EKey::LEFT_SUPER:
		return "EKey::LEFT_SUPER";

	case EKey::RIGHT_SHIFT:
		return "EKey::RIGHT_SHIFT";

	case EKey::RIGHT_CONTROL:
		return "EKey::RIGHT_CONTROL";

	case EKey::RIGHT_ALT:
		return "EKey::RIGHT_ALT";

	case EKey::RIGHT_SUPER:
		return "EKey::RIGHT_SUPER";

	case EKey::MENU:
		return "EKey::MENU";

	default:
		return "Unknow key";
	}
}