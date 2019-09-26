#include "ColorableObject.h"



ColorableObject::ColorableObject(void)
{
	AddClassProperty(COLORABLE_OBJECT_CLASS);
}


ColorableObject::~ColorableObject(void)
{
	Destroying();
}



