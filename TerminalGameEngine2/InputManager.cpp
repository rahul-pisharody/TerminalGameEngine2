#include "InputManager.hpp"

InputManager& InputManager::getInstance()
{
	static InputManager *instance = new InputManager();
	return *instance;
}

InputManager::InputManager()
{
	std::memset(m_keyNewState, 0, 256 * sizeof(short));
	std::memset(m_keyOldState, 0, 256 * sizeof(short));
	std::memset(m_keys, 0, 256 * sizeof(KeyState));
}
//InputManager::~InputManager()
//{
//	instance = nullptr;
//}

bool InputManager::isKeyPressed(int key_ID)
{
	return (m_keys[key_ID].pressed || m_keys[key_ID].held);
}
bool InputManager::isKeyReleased(int key_ID)
{
	return (m_keys[key_ID].released);
}
bool InputManager::isKeyJustPressed(int key_ID)
{
	return (m_keys[key_ID].pressed);
}


//https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
void InputManager::updateInput()
{
	for (int i = 0; i < 256; i++)
	{
		m_keyNewState[i] = GetAsyncKeyState(i);

		m_keys[i].pressed = false;
		m_keys[i].released = false;

		if (m_keyNewState[i] != m_keyOldState[i])
		{
			if (m_keyNewState[i] & 0x8000)
			{
				m_keys[i].pressed = !m_keys[i].held;
				m_keys[i].held = true;
			}
			else
			{
				m_keys[i].released = true;
				m_keys[i].held = false;
			}
		}

		m_keyOldState[i] = m_keyNewState[i];
	}
}

//void InputManager::_startUp()
//{
//}

//void InputManager::_shutDown()
//{
//}
