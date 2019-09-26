#include "InputEvent.h"
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>


InputEvent::InputEvent(void)
{}



InputEvent::~InputEvent(void)
{
	Destroying();
}


void InputEvent::Destroying(void)
{
	if(IsReceiversExist(PlayerControl::DESTROY_PLAYER_CONTROL_FINISH_MESSAGE))
	{ 
		SendPackage(PlayerControl::DESTROY_PLAYER_CONTROL_FINISH_MESSAGE, this);
		DisconnectAllReceivers();
	}
}
