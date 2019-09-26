#ifndef LERPINGOBJECT_H
#define LERPINGOBJECT_H
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
class LerpingObject: public virtual AbstractObject
{
	public: enum: int64 
	{
		LERPING_OBJECT_CLASS = 8192
	};
	public: enum
	{
		CHANGE_FLOAT_LERP_MESSAGE = 40
	};
	protected: MagicPointer<FloatLerp> floatLerp;
	public: LerpingObject(void);
	public: virtual ~LerpingObject(void);
	public: FloatLerp* GetFloatLerp(void)const;
	public: virtual void BindFloatLerp(FloatLerp* _floatLerp) = 0;
};




#endif
