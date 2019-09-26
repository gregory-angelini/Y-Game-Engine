#include "TriggerFinishAction.h"
#include <Engine/Core/Trigger/Trigger.h>


#ifdef SendMessage
   #undef SendMessage
#endif



TriggerFinishAction::TriggerFinishAction(void)
{}


TriggerFinishAction::TriggerFinishAction(int32 _type, Trigger* _trigger): AbstractAction(_type)
{
	targetObject.Attach(_trigger);
}



TriggerFinishAction::~TriggerFinishAction(void)
{
	Destroying();
	targetObject.Clear(MagicContainer::REMOVE_NODE);
}



void TriggerFinishAction::operator () (void)
{
	Trigger* trigger = targetObject.Get();

	if(trigger)
	{
		if(Trigger::_IsEventLogEnabled())
		{
			Trigger::_GetEventLogList().AddToTail(Trigger::EventInfo("", Trigger::EventInfo::ACTION_CATEGORY, type, Renderer::_GetPicked()->GetWorkingTimeInMs()));
		}

		if(trigger->IsReceiversExist(Trigger::TRIGGER_FINISH_MESSAGE)) { trigger->SendMessage(Trigger::TRIGGER_FINISH_MESSAGE); }
	}
}
