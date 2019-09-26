
#include "Mouse.h"
#include <Engine/Core/Window/Window.h>



Mouse::Event::Event(void)
{
	time = 0; 
	key = NO_KEY;
	type = NO_EVENT_TYPE;
	wheel = 0;
	messageId = 0;
}


Mouse::Event::Event(const Mouse::Event& _event)
{

	*this = _event;
}


Mouse::Event::~Event(void)
{
	Destroying();
}



Vector2 Mouse::Event::GetPosition(void)const
{
	return position;
}


	
Vector2 Mouse::Event::GetOffset(void)const
{
	return offset;
}



int32 Mouse::Event::GetWheel(void)const
{
	return wheel;
}


void Mouse::Event::SetKey(Key _key)
{
	key = _key;
}
	

void Mouse::Event::SetEventType(EventType _type)
{
	type = _type;
	messageId = _GetMessageOfEventType(_type);
}



Mouse::Event& Mouse::Event::operator = (const Mouse::Event& _event)
{  
	time = _event.time;
	type = _event.type;
	key = _event.key;
	position = _event.position;
	offset = _event.offset;
	wheel = _event.wheel;
	messageId = _event.messageId;
	return *this;
}


int32 Mouse::Event::GetMessageId(void)const
{
	return messageId;
}


int32 Mouse::Event::_GetMessageOfEventType(int32 _type)
{
	switch(_type)
	{
		case Mouse::KEY_DOWN:
		{
			return Window::MOUSE_KEY_DOWN_MESSAGE;
		}
		case Mouse::KEY_UP:
		{
			return Window::MOUSE_KEY_UP_MESSAGE;
		}
		case Mouse::WHEEL:
		{
			return Window::MOUSE_WHEEL_MESSAGE;
		}
		case Mouse::MOVE:
		{
			return Window::MOUSE_MOVE_MESSAGE;
		}
		case Mouse::DBL_CLICK:
		{
			return Window::MOUSE_DOUBLE_CLICK_MESSAGE;
		}
	}
	return 0;
}


int64 Mouse::Event::GetTime(void)const
{
	return time; 
}

Mouse::Key Mouse::Event::GetKey(void)const
{
	return key;
}

int32 Mouse::Event::GetEventType(void)const
{
	return type;
}

int32 Mouse::Event::GetDeviceType(void)const
{
	return Mouse::MOUSE_DEVICE;
}


bool Mouse::Event::IsExactEquals(const Mouse::Event& _event)const
{
	if(type == _event.type)
	{
		switch(_event.type)
		{	
			case KEY_DOWN:
			case KEY_UP:
			case DBL_CLICK:
			{
				if(key == _event.key)
				{ return true; }
				break;
			}
			
			case WHEEL:
			{
				if(wheel == _event.wheel)
				{ return true; }
				break;
			}

			case MOVE:
			{
				if(position == _event.position && offset == _event.offset)
				{ return true; }
				break;
			}
		}
	}
	return false;
}

bool Mouse::Event::IsInexactEquals(const Mouse::Event& _event)const
{
	if(type == _event.type)
	{
		return true;
	}
	return false;
}


bool Mouse::Event::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(time);
	    _file.Write(type);
		 _file.Write(key);
		_file.Write(position);
		_file.Write(offset);
		_file.Write(wheel);
		return true;
	}
	return false;
}


				
bool Mouse::Event::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		_file.Read(time);
	    _file.Read(type);
		messageId = _GetMessageOfEventType(type);
		 _file.Read(key);
		_file.Read(position);
		_file.Read(offset);
		_file.Read(wheel);
		return true;
	}
	return false;
}