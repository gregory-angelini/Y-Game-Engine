#ifndef ACTIVATABLEOBJECT_H
#define ACTIVATABLEOBJECT_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
class ActivatableObject: public virtual AbstractObject
{
	public: enum: int64 
	{
		ACTIVATABLE_OBJECT_CLASS = 2251799813685248
	};
	public: enum 
	{
		NEVER_SWITCH_OFF,
		AUTO_SWITCH_OFF,
		OVERLAY_SWITCH_OFF
	};
	public: enum
	{
		CHANGE_ACTIVATION_LIMIT_MESSAGE = 314,
		CHANGE_DEACTIVATION_MODE_MESSAGE = 315
	};
	protected: int32 activationLimit;
	protected: int32 deactivationMode;
	public: ActivatableObject(void);
	public: virtual ~ActivatableObject(void);
	public: void SetDeactivationMode(int32 _mode);
	public: int32 GetDeactivationMode(void)const;
	public: int32 GetActivationLimit(void)const;
	public: void SetActivationLimit(int32 _activationLimit);
};




#endif
