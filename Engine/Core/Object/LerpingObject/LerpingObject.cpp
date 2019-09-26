#include "LerpingObject.h"



LerpingObject::LerpingObject(void)
{
	AddClassProperty(LERPING_OBJECT_CLASS);
   }


LerpingObject::~LerpingObject(void)
{
	Destroying();
}



FloatLerp* LerpingObject::GetFloatLerp(void)const
{
	return floatLerp.Get();
}

