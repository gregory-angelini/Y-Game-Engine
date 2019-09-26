
#include "Keyboard.h"
#include <Engine/Core/Window/Window.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h> 



char* Keyboard::keyNames[] =
{
	"?", "?", "?", "?", "?", "?", "?", "?", 
	"Backspace", "Tab", 
	"?", "?", "?", 
	"Enter", 
	"?", "?",
	"Shift", "Ctrl", "Alt", "Pause", "Caps_Lock", 
	"?", "?", "?", "?", "?", "?", 
	"Esc", 
	"?", "?", "?", "?",
	"Space", "Page_Up", "Page_Down", "End", "Home", "Left_Arrow", "Up_Arrow", "Right_Arrow", "Down_Arrow",
	"?", "?", "?", "Prt_Sc", 
	"Insert", "Delete", 
	"?",
	"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"?", "?", "?", "?", "?", "?", "?",
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
	"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	"Left_Window", "Right_Window", "Applications", 
	"?", "?",
	"NumPad_0", "NumPad_1", "NumPad_2", "NumPad_3", "NumPad_4",
	"NumPad_5", "NumPad_6", "NumPad_7", "NumPad_8", "NumPad_9",
	"Multiply", "Add", 
	"?", 
	"Subtract", "Decimal", "Divide",
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"Num_Lock", "Scroll_Lock",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"Semicolon", "Equals", "Comma", "Minus", "Period", "Slash", "Grave",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?",
	"Left_bracket", "Backslash", "Right_bracket", "Apostrophe",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?"
};




Keyboard::Keyboard(void)
{
	::GetKeyboardState(state);
}



MagicList<Keyboard::Event>& Keyboard::GetEventList(void)
{
	return events;
}



void Keyboard::ClearEventList(void)
{
	events.Clear();
}


const SimpleList<Keyboard::Key>& Keyboard::GetPressedKeysList(void)
{
	return pressedKeysList;
}


StringANSI Keyboard::_GetFormatKeyName(Keyboard::Event* _event)
{
	if(_event)
	{
		if(_event->key >= 65 && _event->key <= 90)
		{  
			if(_IsKeyEnabled(Keyboard::CAPS_LOCK))
			{
				if(_IsKeyPressed(SHIFT))
				{
					StringANSI keyName;
					keyName = _event->keyName[0] ^ 32;
					return keyName;
				}
				return _event->keyName;
			}
			else
			{
				if(_IsKeyPressed(SHIFT))
				{
					return _event->keyName;
				}
				StringANSI keyName;
				keyName = _event->keyName[0] ^ 32;
				return keyName;
			}
		}
		else if(_event->key >= 48 && _event->key <= 57)
		{
			return _event->keyName;
		}
		else if(_event->key == SPACE)
		{
			return " ";
		}
		else if(_event->key == MINUS && _IsKeyPressed(SHIFT))
		{
			return "_";
		}
	}
	return "";
}


const uint8* Keyboard::GetKeyboardState(void)
{
	return state;
}




Keyboard::Event* Keyboard::EventFilter(Window* _window, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch(_msg)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			Event* keyboardEvent = new Event();
			
			keyboardEvent->time = SystemTime::_GetSystemTimeInMs();

			keyboardEvent->type = KEY_DOWN;
			keyboardEvent->key = (Key)_wParam;
			keyboardEvent->messageId = Window::KEYBOARD_KEY_DOWN_MESSAGE;
			keyboardEvent->autoRepeat = _lParam & 0x40000000 ? true : false;
			if(!(state[keyboardEvent->key] & 0x80)) { pressedKeysList.AddToTail(keyboardEvent->key); }
			::GetKeyboardState(state);
 
			keyboardEvent->keyName = _GetKeyName(keyboardEvent->key);
			
			return keyboardEvent;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			Event* keyboardEvent = new Event();

			keyboardEvent->time = SystemTime::_GetSystemTimeInMs();

			keyboardEvent->type = KEY_UP;
			keyboardEvent->key = (Key)_wParam;
			keyboardEvent->messageId = Window::KEYBOARD_KEY_UP_MESSAGE;
			pressedKeysList.Remove(keyboardEvent->key);
			::GetKeyboardState(state);

			keyboardEvent->keyName = _GetKeyName(keyboardEvent->key);
			
			return keyboardEvent;
		}
	}
	return NIL;
}

 
 
void Keyboard::_CreateEvent(int32 _key, int32 _type)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = _key;

	if(_type == Keyboard::KEY_DOWN)
	{
		input.ki.dwFlags = 0; 
		SendInput(1, &input, sizeof(INPUT));
	}
	else if(_type == Keyboard::KEY_UP)
	{
		input.ki.dwFlags = KEYEVENTF_KEYUP; 
		SendInput(1, &input, sizeof(INPUT));
	}		 
}



StringANSI Keyboard::_GetKeyName(Keyboard::Key _key)
{
	return StringANSI(keyNames[_key]);
}


Keyboard::Key Keyboard::_GetKeyId(StringANSI _key)
{
	for(int32 i = 0; i < AMOUNT_OF_KEYS; ++i)
	{
		if(keyNames[i] == _key)
		{
			return (Key)i;
		}
	}
	return NO_KEY;
}



bool Keyboard::IsKeyPressed(Keyboard::Key _key)const
{
	return state[_key] & 0x80;
}



bool Keyboard::IsKeyReleased(Keyboard::Key _key)const
{
	return !(state[_key] & 0x80);
}


bool Keyboard::IsKeyEnabled(Keyboard::Key _key)const
{
	return state[_key] & 0x1;
}




bool Keyboard::_IsKeyPressed(Keyboard::Key _key)
{ 
	return ((::GetKeyState(_key) & 0x8000) != 0);
}


bool Keyboard::_IsKeyEnabled(Keyboard::Key _key)
{ 
	return ((::GetKeyState(_key) & 1) != 0);
}


bool Keyboard::_IsKeyReleased(Keyboard::Key _key)
{ 
	return ((::GetKeyState(_key) & 0x8000) == 0);
}


