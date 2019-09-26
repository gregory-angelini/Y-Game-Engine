#ifndef SCALABLEOBJECT_H
#define SCALABLEOBJECT_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class ScalableObject: public virtual AbstractObject
{
	friend class ModelView;
	public: enum: int64
	{
		SCALABLE_OBJECT_CLASS = 131072
	};
	public: enum
	{
		REPLACE_SCALE_MESSAGE = 42,
		DISPLACE_SCALE_MESSAGE = 43,
		GET_SCALE_MESSAGE = 75
	};
	public: ScalableObject(void);
	public: virtual ~ScalableObject(void);
	public: virtual void SetScale(Vector3 _scale) = 0;
	public: virtual void DisplaceScale(Vector3 _scale) = 0;
	public: virtual Vector3 GetScale(void) = 0;
	public: virtual Vector3 GetAbsoluteScale(void) = 0;

};


#endif 