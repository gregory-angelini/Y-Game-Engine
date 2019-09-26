#include "ActivatableObject.h"



ActivatableObject::ActivatableObject(void): activationLimit(-1), deactivationMode(NEVER_SWITCH_OFF)
{
	AddClassProperty(ACTIVATABLE_OBJECT_CLASS);
   }


ActivatableObject::~ActivatableObject(void)
{
	Destroying();
}


int32 ActivatableObject::GetDeactivationMode(void)const
{
	return deactivationMode;
}


void ActivatableObject::SetDeactivationMode(int32 _mode)
{
	deactivationMode = _mode;
}


int32 ActivatableObject::GetActivationLimit(void)const
{
	return activationLimit;
}


void ActivatableObject::SetActivationLimit(int32 _activationLimit)
{
	activationLimit = _activationLimit;
}
