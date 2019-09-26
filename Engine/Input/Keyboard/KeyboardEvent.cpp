
#include "Keyboard.h"
#include <Engine/Core/Window/Window.h>



Keyboard::Event::Event(void)
{
	time = 0; 
	type = NO_EVENT_TYPE;
	key = NO_KEY;
	autoRepeat = false;
	messageId = 0;
}
	

Keyboard::Event::Event(const Keyboard::Event& _event)
{

	*this = _event;
}


Keyboard::Event::~Event(void)
{
	Destroying();
}


Keyboard::Event& Keyboard::Event::operator = (const Keyboard::Event& _event)
{  
	time = _event.time;
	type = _event.type;
	key = _event.key;
	autoRepeat = _event.autoRepeat;
	keyName = _event.keyName;
	messageId = _event.messageId;
	return *this;
}


int64 Keyboard::Event::GetTime(void)const
{
	return time; 
}



void Keyboard::Event::SetKey(Key _key)
{
	key = _key;
	keyName = _GetKeyName(_key);
}
	

void Keyboard::Event::SetEventType(EventType _type)
{
	type = _type;
	messageId = _GetMessageOfEventType(_type);
}



bool Keyboard::Event::IsAutoRepeat(void)const
{
	return autoRepeat;
}


Keyboard::Key Keyboard::Event::GetKey(void)const
{
	return key;
}


StringANSI Keyboard::Event::GetKeyName(void)const
{
	return keyName;
}


int32 Keyboard::Event::GetEventType(void)const
{
	return type;
}


int32 Keyboard::Event::GetMessageId(void)const
{
	return messageId;
}
 


int32 Keyboard::Event::_GetMessageOfEventType(int32 _type)
{
	switch(_type)
	{
		case Keyboard::KEY_DOWN:
		{
			return Window::KEYBOARD_KEY_DOWN_MESSAGE;
		}
		case Keyboard::KEY_UP:
		{
			return Window::KEYBOARD_KEY_UP_MESSAGE;
		}
	}
	return 0;
}


int32 Keyboard::Event::GetDeviceType(void)const
{
	return Keyboard::KEYBOARD_DEVICE;
}



bool Keyboard::Event::IsExactEquals(const Keyboard::Event& _event)const
{
	if(type == _event.type)
	{
		if(key == _event.key)
		{ return true; }
	}
	return false;
}

bool Keyboard::Event::IsInexactEquals(const Keyboard::Event& _event)const
{
	if(type == _event.type)
	{
		return true; 
	}
	return false;
}



bool Keyboard::Event::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(time);
	    _file.Write(type);
		_file.Write(key); 
		return true;
	}
	return false;
}


				
bool Keyboard::Event::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		_file.Read(time);
	    _file.Read(type);
		messageId = _GetMessageOfEventType(type);
		_file.Read(key);
		keyName = _GetKeyName(key);
		return true;
	}
	return false;
}