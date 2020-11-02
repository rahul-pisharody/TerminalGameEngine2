/*
Input Related code
*/
#pragma once

#include <list>
#include <windows.h>

namespace Inputs
{
	enum KEY
	{
		KEY_SHIFT = 0x10,
		KEY_CTRL = 0x11,
		KEY_LEFT = 0x25,
		KEY_UP = 0x26,
		KEY_RIGHT = 0x27,
		KEY_DOWN = 0x28
	};
};

/*
Updates Keyboard input and provides functions
to check for key presses
*/
class InputManager
{
private:
	struct KeyState
	{
		bool pressed;
		bool released;
		bool held;
	} m_keys[256];

	short m_keyOldState[256];
	short m_keyNewState[256];

public:
	static InputManager& getInstance();
	InputManager();
	//~InputSystem();

	bool isKeyPressed(int key_ID);
	bool isKeyReleased(int key_ID);
	bool isKeyJustPressed(int key_ID);

	void updateInput();

	//void _startUp();
	//void _shutDown();
public:
};

