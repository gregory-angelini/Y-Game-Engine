
#include "PlayerControl.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
StringANSI PlayerControl::playerControlsSuffix = "p-control";
StringANSI PlayerControl::playerControlsDir = "controls/p-controls/";



PlayerControl::PlayerControl(void): activationMode(SINGLE), timing(0.1f)
{
	events.playerControl = this;
	AddClassProperty(PLAYER_CONTROL_CLASS);
}


PlayerControl::~PlayerControl(void)
{
	Destroy();
	Destroying();
}



void PlayerControl::AddEvent(InputEvent* _event)
{
	events.AddToTail(_event);
}


bool PlayerControl::IsEquals(const Keyboard::Event& _event1, const Keyboard::Event& _event2)const
{
	if(_event1.GetEventType() == _event2.GetEventType())
	{
		if(_event1.GetKey() == _event2.GetKey())
		{ 
			return true; 
		}
	}
	return false;
}

bool PlayerControl::IsEquals(const Mouse::Event& _event1, const Mouse::Event& _event2)const
{
	if(_event1.GetEventType() == _event2.GetEventType())
	{
		switch(_event2.GetEventType())
		{
			case Mouse::KEY_DOWN:
			case Mouse::KEY_UP:
			case Mouse::DBL_CLICK:
			{
				if(_event1.GetKey() == _event2.GetKey())
				{ return true; }
				break;
			}
				
			case Mouse::WHEEL:
			{
				return true; 
			}

			case Mouse::MOVE:
			{
				return true;
			}
		}
	}
	return false;
}

  

void PlayerControl::KeyboardEventFilter(Keyboard::Event _event)
{ 
	if(events.IsNotEmpty() && !IsActivated())
	{
		if(activationMode == SINGLE)
		{
			events.FindSimilar(_event.GetEventType(), &InputEvent::GetEventType, &checkingEvents, MagicContainer::SHARE);
			while(checkingEvents.GetHead())
			{ 
				Keyboard::Event* itEvent = dynamic_cast<Keyboard::Event*>(checkingEvents.GetHead());
	 
				if(IsEquals(*itEvent, _event))
				{
					(*itEvent) = _event;
					activatedEvents.AddToTail(itEvent);
					checkingEvents.Clear();
					Activate();
					return;
				}
				delete checkingEvents.GetHeadAddress();
			}
		}
		else if(events.GetSize() > 1 && activationMode == COMBO)
		{ 

			if(timer.Check())
			{
				activatedEvents.Clear();
			}
 
			events.FindSimilar(_event.GetEventType(), &InputEvent::GetEventType, &checkingEvents, MagicContainer::SHARE);
			while(checkingEvents.GetHead())
			{ 
				Keyboard::Event* itEvent = dynamic_cast<Keyboard::Event*>(checkingEvents.GetHead());
	 
				if(IsEquals(*itEvent, _event))
				{
					(*itEvent) = _event;
					checkingEvents.Clear();
					ComboUpdate(itEvent);
					return;
				}  
				delete checkingEvents.GetHeadAddress();
			}
			activatedEvents.Clear();
			timer.Reset();
		}
		else if(events.GetSize() > 1 && activationMode == SEQUENCE)
		{ 
								
			if(timer.Check())
			{
				activatedEvents.Clear();
			} 
 
			Keyboard::Event* itEvent = dynamic_cast<Keyboard::Event*>(events[activatedEvents.GetSize()]);
	 
			if(itEvent && IsEquals(*itEvent, _event))
			{
				(*itEvent) = _event;
				SequenceUpdate(itEvent);
			}
			else
			{
				activatedEvents.Clear();
				timer.Reset();
			}
		}
	}
}




void PlayerControl::MouseEventFilter(Mouse::Event _event)
{   
	if(events.IsNotEmpty() && !IsActivated())
	{
		if(activationMode == SINGLE)
		{
			events.FindSimilar(_event.GetEventType(), &InputEvent::GetEventType, &checkingEvents, MagicContainer::SHARE);
			while(checkingEvents.GetHead())
			{ 
				Mouse::Event* itEvent = dynamic_cast<Mouse::Event*>(checkingEvents.GetHead());
	 
				if(IsEquals(*itEvent, _event))
				{
					(*itEvent) = _event;
					activatedEvents.AddToTail(itEvent);
					checkingEvents.Clear();
					Activate();
					return;
				}
				delete checkingEvents.GetHeadAddress();
			}
		}
		else if(events.GetSize() > 1 && activationMode == COMBO)
		{ 

			if(timer.Check())
			{
				activatedEvents.Clear();
			}
 
			events.FindSimilar(_event.GetEventType(), &InputEvent::GetEventType, &checkingEvents, MagicContainer::SHARE);
			while(checkingEvents.GetHead())
			{ 
				Mouse::Event* itEvent = dynamic_cast<Mouse::Event*>(checkingEvents.GetHead());
	 
				if(IsEquals(*itEvent, _event))
				{
					(*itEvent) = _event;
					checkingEvents.Clear();
					ComboUpdate(itEvent);
					return;
				}
				delete checkingEvents.GetHeadAddress();
			}
			activatedEvents.Clear();
			timer.Reset();
		}
		else if(events.GetSize() > 1 && activationMode == SEQUENCE)
		{ 
								
			if(timer.Check())
			{
				activatedEvents.Clear();
			} 
 
			Mouse::Event* itEvent = dynamic_cast<Mouse::Event*>(events[activatedEvents.GetSize()]);
	 
			if(itEvent && IsEquals(*itEvent, _event))
			{
				(*itEvent) = _event;
				SequenceUpdate(itEvent);
			}
			else
			{
				activatedEvents.Clear();
				timer.Reset();
			}
		}
	}
}


float PlayerControl::GetTimingInSec(void)const
{
	return timing;
}

void PlayerControl::SetTimingInSec(float _timing)
{
	if(_timing > 0.0f)
	{
		timing = _timing;
	}
}

float PlayerControl::GetRemainedTimeInSec(void)
{
	if(activationMode == SINGLE) { return 0.0f; }

	if(timer.Check())
	{
		activatedEvents.Clear();
		return 0.0f;
	}
	return timer.RemainedTimeInSec();
}



MagicList<InputEvent>& PlayerControl::GetActivatedEvents(void)
{	
	if(activationMode != SINGLE)
	{
		if(timer.Check())
		{
			activatedEvents.Clear();
		}
	}

	return activatedEvents;
}


bool PlayerControl::IsActivated(void)
{
	if(events.IsNotEmpty())
	{
		if(activationMode == SINGLE) 
		{
			return activatedEvents.IsNotEmpty();
		}
		else 
		{
			return (events.GetSize() == activatedEvents.GetSize());
		}
	}
	return false;
}


bool PlayerControl::IsActivated(InputEvent* _event)
{
	return (_event->GetAddress(&activatedEvents) ? true : false);
}


void PlayerControl::Activate(void)
{
	if(IsReceiversExist(PLAYER_CONTROL_FINISH_MESSAGE)) { SendPackage(PLAYER_CONTROL_FINISH_MESSAGE, this); }

	Deactivate();
}


void PlayerControl::Deactivate(void)
{
	activatedEvents.Clear();
	timer.Reset();
}





PlayerControl::ActivationMode PlayerControl::GetActivationMode(void)const
{
	return activationMode;
}



void PlayerControl::SetActivationMode(PlayerControl::ActivationMode _mode)
{
	if(activationMode == _mode) { return; }
	activationMode = _mode;
	activatedEvents.Clear();
	timer.Reset();
}


void PlayerControl::ComboUpdate(InputEvent* _event)
{
	if(activatedEvents.IsEmpty())
	{
		activatedEvents.AddToTail(_event);
		timer.StartSec(timing);
	}
	else
	{
		if(IsActivated(_event))
		{
			activatedEvents.Clear();
			ComboUpdate(_event);
			return;
		}

		activatedEvents.AddToTail(_event);

		if(events.GetSize() == activatedEvents.GetSize())
		{
			Activate();
		}
	}
}


void PlayerControl::SequenceUpdate(InputEvent* _event)
{
	if(activatedEvents.IsEmpty())
	{
		activatedEvents.AddToTail(_event);
		timer.StartSec(timing);
	}
	else
	{
		activatedEvents.AddToTail(_event);

		if(events.GetSize() == activatedEvents.GetSize())
		{
			bool success = true;
			for(int32 i = 0; i < events.GetSize(); ++i)
			{
				if(events[i] != activatedEvents[i])
				{ 
					success = false;
					break;
				}
			}
								
			if(success)
			{
				Activate();
			}
			else
			{
				activatedEvents.Clear();
				timer.Reset();
			}
		}
	}
}



StringANSI PlayerControl::_GetDir(void)
{
	return playerControlsDir;
}


void PlayerControl::_SetFileSuffix(StringANSI _suffix)
{
	playerControlsSuffix = _suffix;
}


StringANSI PlayerControl::_GetFileSuffix(void)
{
	return playerControlsSuffix;
}

 

MagicList<InputEvent>& PlayerControl::GetEvents(void)
{
	return events;
}


void PlayerControl::Destroy(void)
{
	timer.Reset();
	events.Clear();
	activatedEvents.Clear();
	checkingEvents.Clear();
}


bool PlayerControl::IsExist(void)const
{
	return events.IsNotEmpty();
}


bool PlayerControl::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + playerControlsDir + GetName() + "." + playerControlsSuffix;
	}

	File file(_path, File::REWRITE);
		
	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool PlayerControl::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(activationMode);
		_file.Write(timing);
		_file.Write(events.GetSize());
		MagicList<InputEvent>::AddressDerived* it = events.GetHeadAddress();

		while(it)
		{
			_file.Write(it->GetNode()->GetDeviceType());
			it->GetNode()->SaveToFile(_file);
			it = it->GetRight();
		}
		return true;
	}
	return false;
}



bool PlayerControl::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + PlayerControl::_GetDir() + _name + "." + PlayerControl::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool PlayerControl::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(activationMode);
		_file.Write(timing);
		_file.Write(events.GetSize());
		MagicList<InputEvent>::AddressDerived* it = events.GetHeadAddress();

		while(it)
		{
			_file.Write(it->GetNode()->GetDeviceType());
			it->GetNode()->SaveToFile(_file);
			it = it->GetRight();
		}
		return true;
	}
	return false;
}




bool PlayerControl::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + playerControlsDir + _path + "." + playerControlsSuffix;
	}
	
	File file(_path, File::READ);

	return LoadFromFile(file);
}


bool PlayerControl::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Destroy();
		int32 version;
		_file.Read(version);
		_file.Read(properties);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		Rename(name);
		_file.Read(activationMode);
		_file.Read(timing);
		int32 amountOfEvents = 0;
		_file.Read(amountOfEvents);
		for(int32 i = 0; i < amountOfEvents; ++i)
		{
			int32 deviceType;
			_file.Read(deviceType);

			if(deviceType == Mouse::MOUSE_DEVICE)
			{
				Mouse::Event* event = new Mouse::Event();
				event->LoadFromFile(_file);
				AddEvent(event);
			}
			else if(deviceType == Keyboard::KEYBOARD_DEVICE)
			{
				Keyboard::Event* event = new Keyboard::Event();
				event->LoadFromFile(_file);
				AddEvent(event);
			}
		}
		return true;
	}
	return false;
}



PlayerControl* PlayerControl::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{ 
		_path = AssetLibrary::_GetDir() + playerControlsDir + _path + "." + playerControlsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


PlayerControl* PlayerControl::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		PlayerControl* playerControl = new PlayerControl();

		if(playerControl->LoadFromFile(_file))
		{
			return playerControl;
		}
		delete playerControl;
	}
	return NIL;
}
