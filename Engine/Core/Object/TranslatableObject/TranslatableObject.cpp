#include "TranslatableObject.h"



TranslatableObject::TranslatableObject(void)
{
	AddClassProperty(TRANSLATABLE_OBJECT_CLASS);
}



TranslatableObject::~TranslatableObject(void)
{
	Destroying();
}

