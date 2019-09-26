#include "ScalableObject.h"

ScalableObject::ScalableObject(void)
{
	AddClassProperty(SCALABLE_OBJECT_CLASS);
}

	

ScalableObject::~ScalableObject(void)
{
	Destroying();
}

