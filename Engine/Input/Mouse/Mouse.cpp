
#include "Mouse.h"
#include <Engine/Core/Shape/ScreenQuad/ScreenQuad.h>
#include <Engine/Core/Window/Window.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h>
#if !defined(WHEEL_DELTA)
#define WHEEL_DELTA 120
#endif
 

#if !defined(WM_MOUSEWHEEL)
#define WM_MOUSEWHEEL 0x020A
#endif
bool Mouse::cursorVisible = true;
Mouse::Mouse(void)
{
	Mouse::_SetShowCursor(Mouse::cursorVisible);
	state[LEFT_BUTTON] = false; 
	state[MIDDLE_BUTTON] = false;
	state[RIGHT_BUTTON] = false;
}



const MagicList<Mouse::Event>& Mouse::GetEventList(void)
{
	return events;
}


void Mouse::ClearEventList(void)
{
	events.Clear();
}



const SimpleList<Mouse::Key>& Mouse::GetPressedKeysList(void)
{
	return pressedKeysList;
}


Mouse::Event* Mouse::EventFilter(Window* _window, UINT _msg, WPARAM _wParam, LPARAM _lParam) 
{
	switch(_msg)
	{
		case WM_LBUTTONDOWN:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = KEY_DOWN;
			mouseEvent->key = LEFT_BUTTON;
			mouseEvent->messageId = Window::MOUSE_KEY_DOWN_MESSAGE;
			if(!(state[mouseEvent->key])) { pressedKeysList.AddToTail(mouseEvent->key); }
			state[mouseEvent->key] = true;
			
			return mouseEvent;
		}
		case WM_MBUTTONDOWN:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = KEY_DOWN;
			mouseEvent->key = MIDDLE_BUTTON;
			mouseEvent->messageId = Window::MOUSE_KEY_DOWN_MESSAGE;
			if(!(state[mouseEvent->key])) { pressedKeysList.AddToTail(mouseEvent->key); }
			state[mouseEvent->key] = true;

			return mouseEvent;
		}
		case WM_RBUTTONDOWN:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = KEY_DOWN;
			mouseEvent->key = RIGHT_BUTTON;
			mouseEvent->messageId = Window::MOUSE_KEY_DOWN_MESSAGE;
			if(!(state[mouseEvent->key])) { pressedKeysList.AddToTail(mouseEvent->key); }
			state[mouseEvent->key] = true;

			return mouseEvent;
		}
		case WM_LBUTTONDBLCLK:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = DBL_CLICK;
			mouseEvent->key = LEFT_BUTTON;
			mouseEvent->messageId = Window::MOUSE_DOUBLE_CLICK_MESSAGE;
			if(!(state[mouseEvent->key])) { pressedKeysList.AddToTail(mouseEvent->key); }
			state[mouseEvent->key] = true;

			return mouseEvent;
		}
		case WM_MBUTTONDBLCLK:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = DBL_CLICK;
			mouseEvent->key = MIDDLE_BUTTON;
			mouseEvent->messageId = Window::MOUSE_DOUBLE_CLICK_MESSAGE;
			if(!(state[mouseEvent->key])) { pressedKeysList.AddToTail(mouseEvent->key); }
			state[mouseEvent->key] = true;

			return mouseEvent;
		}
		case WM_RBUTTONDBLCLK:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = DBL_CLICK;
			mouseEvent->key = RIGHT_BUTTON;
			mouseEvent->messageId = Window::MOUSE_DOUBLE_CLICK_MESSAGE;
			if(!(state[mouseEvent->key])) { pressedKeysList.AddToTail(mouseEvent->key); }
			state[mouseEvent->key] = true;

			return mouseEvent;
		}
		case WM_LBUTTONUP:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = KEY_UP;
			mouseEvent->key = LEFT_BUTTON;
			mouseEvent->messageId = Window::MOUSE_KEY_UP_MESSAGE;
			state[mouseEvent->key] = false;
			pressedKeysList.Remove(mouseEvent->key);

			return mouseEvent;
		}
		case WM_MBUTTONUP:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = KEY_UP;
			mouseEvent->key = MIDDLE_BUTTON;
			mouseEvent->messageId = Window::MOUSE_KEY_UP_MESSAGE;
			state[mouseEvent->key] = false;
			pressedKeysList.Remove(mouseEvent->key);

			return mouseEvent;
		}
		case WM_RBUTTONUP:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = KEY_UP;
			mouseEvent->key = RIGHT_BUTTON;
			mouseEvent->messageId = Window::MOUSE_KEY_UP_MESSAGE;
			state[mouseEvent->key] = false;
			pressedKeysList.Remove(mouseEvent->key);

			return mouseEvent;
		}
		case WM_MOUSEMOVE:
		{
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();

			mouseEvent->type = MOVE;
			mouseEvent->messageId = Window::MOUSE_MOVE_MESSAGE;
			const Vector2 newPos((float)LOWORD(_lParam), (float)HIWORD(_lParam));
			mouseEvent->position = newPos;
			mouseEvent->offset = newPos - previousPos;
			previousPos = newPos;

			return mouseEvent;
		}
        case WM_MOUSEWHEEL: 
		{ 
			Event* mouseEvent = new Event();

			mouseEvent->time = SystemTime::_GetSystemTimeInMs();
			
			mouseEvent->type = WHEEL;
			mouseEvent->messageId = Window::MOUSE_WHEEL_MESSAGE;
			mouseEvent->wheel = short(HIWORD(_wParam)) / WHEEL_DELTA;

			return mouseEvent;
		} 
	}
	return NIL;
}



Vector2 Mouse::_GetCursorPosition(const Window* _window)
{
	if(_window)
	{
		if(_window->GetHandle())
		{
			POINT point; 
			
			if(::GetCursorPos(&point))
			{
				::ScreenToClient(_window->GetHandle(), &point);
				return Vector2((float)point.x, (float)point.y);
			}
		}
	}
	else
	{
		POINT point; 

		if(::GetCursorPos(&point))
		{
			return Vector2((float)point.x, (float)point.y);
		}
	}
	return Vector2(-1.0f, -1.0f);
}


Vector2 Mouse::_GetDistanceFrom(const Vector2& _point)
{
	return _point - Mouse::_GetCursorPosition();
}


Vector2 Mouse::GetPreviousCursorPosition(void)
{
	return previousPos;
}



void Mouse::_SetCursorPosition(const Vector2& _position, const Window* _window)
{
	if(_window)
	{ 
		if(_window->GetHandle())
		{
			Vector2 clientAreaPosition = _position + _window->GetClientAreaPosition();
	 
			ScreenQuad rect(_window->GetClientAreaPosition() + _window->GetClientAreaSize() / 2.0f, _window->GetClientAreaSize());

			clientAreaPosition = rect.ClampPoint(clientAreaPosition);
			::SetCursorPos((int32)clientAreaPosition.x, (int32)clientAreaPosition.y);
		}
	}
	else
	{
		::SetCursorPos((int32)_position.x, (int32)_position.y);
	}
}



bool Mouse::_IsCursorOver(const Window* _window, bool _onlyClientRect)
{
	if(_window && _window->GetHandle())
	{
		POINT point; 

		if(::GetCursorPos(&point))
		{
			if(::WindowFromPoint(point) == _window->GetHandle())
			{
				if(_onlyClientRect)
				{ 
					ScreenQuad rect(_window->GetClientAreaPosition() + _window->GetClientAreaSize() / 2.0f, _window->GetClientAreaSize());
					if(rect.IsPointInside(Vector2((float)point.x, (float)point.y)))
					{
						return true;
					}
				}
				else
				{ 
					return true;
				}	
			}
		}
	}

	return false;
}



void Mouse::_MoveCursorToCenter(const Window* _window)
{
	if(_window)
	{
		if(_window->GetHandle())
		{
			Mouse::_SetCursorPosition(_window->GetClientAreaCenter());
		}
	}
	else
	{
		Mouse::_SetCursorPosition(Vector2((float)::GetSystemMetrics(SM_CXSCREEN) / 2.0f, (float)::GetSystemMetrics(SM_CYSCREEN) / 2.0f));
	}
}



void Mouse::_SetShowCursor(bool _enable)
{
	if(_enable)
    {
		cursorVisible = true;
		while(::ShowCursor(_enable) < 0) {}
    }
    else
    {
		cursorVisible = false;

	   while(::ShowCursor(_enable) >= 0) {}
    }
}


bool Mouse::_IsShowCursor(void)
{
	return cursorVisible;
}


bool Mouse::IsKeyPressed(Mouse::Key _key)const
{
	return state[_key];
}


bool Mouse::IsKeyReleased(Mouse::Key _key)const
{
	return !(state[_key]);
}
