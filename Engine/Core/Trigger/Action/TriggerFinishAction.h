#ifndef TRIGGERFINISHACTION_H
#define TRIGGERFINISHACTION_H
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
class TriggerFinishAction: public AbstractAction
{
	friend class Trigger;
	private: MagicPointer<Trigger> targetObject;
	private: TriggerFinishAction(void);
	private: TriggerFinishAction(int32 _type, Trigger* _trigger);
	public: virtual ~TriggerFinishAction(void);
	public: virtual void operator () (void);
};


#endif 