#include "RotatableObject.h"


RotatableObject::RotatableObject(void)
{
	AddClassProperty(ROTATABLE_OBJECT_CLASS);
}

	

RotatableObject::~RotatableObject(void)
{
	Destroying();
}


