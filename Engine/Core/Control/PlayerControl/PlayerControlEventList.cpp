#include "PlayerControl.h"


PlayerControl::EventList::EventList(void)
{}


PlayerControl::EventList::~EventList(void)
{
	Clear();
}


void PlayerControl::EventList::RemoveNodeEvent(MagicNode* _node)
{
	DisconnectFrom(dynamic_cast<InputEvent*>(_node));
}
	


void PlayerControl::EventList::DestroyInputEvent(InputEvent* _event)
{
	delete _event->GetAddress(&playerControl->events);
	DisconnectFrom(_event);
}


void PlayerControl::EventList::DisconnectFrom(InputEvent* _event)
{ 
	
	FindSimilar(_event->GetEventType(), &InputEvent::GetEventType, &playerControl->checkingEvents, MagicContainer::SHARE);

	if(playerControl->checkingEvents.GetSize() == 0)
	{
		if(_event->GetDeviceType() == Mouse::MOUSE_DEVICE)
		{
			for(int32 i = 0; i < Window::_GetWindowList().GetSize(); ++i)
			{
				Window::_GetWindowList()[i]->Disconnect(_event->GetMessageId(), this);
			}
		}
		else if(_event->GetDeviceType() == Keyboard::KEYBOARD_DEVICE)
		{
			for(int32 i = 0; i < Window::_GetWindowList().GetSize(); ++i)
			{
				Window::_GetWindowList()[i]->Disconnect(_event->GetMessageId(), this);
			}
		}
	}

	playerControl->checkingEvents.Clear();
}


void PlayerControl::EventList::AddNodeEvent(MagicNode* _node)
{
	playerControl->activatedEvents.Clear();
	playerControl->timer.Reset();
	
	InputEvent* event = dynamic_cast<InputEvent*>(_node);
	event->Connect(DESTROY_PLAYER_CONTROL_FINISH_MESSAGE, this, Caller<>(this, &EventList::DestroyInputEvent));

	if(event->GetDeviceType() == Mouse::MOUSE_DEVICE)
	{
		for(int32 i = 0; i < Window::_GetWindowList().GetSize(); ++i)
		{
			Window::_GetWindowList()[i]->Connect(event->GetMessageId(), this, Caller<>(playerControl, &PlayerControl::MouseEventFilter));
		}
	}
	else if(event->GetDeviceType() == Keyboard::KEYBOARD_DEVICE)
	{
		for(int32 i = 0; i < Window::_GetWindowList().GetSize(); ++i)
		{
			Window::_GetWindowList()[i]->Connect(event->GetMessageId(), this, Caller<>(playerControl, &PlayerControl::KeyboardEventFilter));
		}
	}
}