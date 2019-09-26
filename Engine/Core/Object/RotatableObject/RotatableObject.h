#ifndef ROTATABLEOBJECT_H
#define ROTATABLEOBJECT_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Math/Quaternion/Quaternion.h>
class RotatableObject: public virtual AbstractObject
{
	public: enum
	{
		REPLACE_ROTATION_MESSAGE = 35,
		DISPLACE_ROTATION_MESSAGE = 36,
		GET_ROTATION_MESSAGE = 74,
		CREATE_ROTATION_MESSAGE = 196,
		CREATE_ROTATION_FROM_VECTOR_3_TO_VECTOR_3_MESSAGE = 266,
		CREATE_ROTATION_FROM_QUATERNION_TO_QUATERNION_MESSAGE = 267,
		ROTATE_AROUND_PIVOT_MESSAGE = 309
	};
	public: enum: int64
	{
		ROTATABLE_OBJECT_CLASS = 524288
	};
	public: RotatableObject(void);
	public: virtual ~RotatableObject(void);
	public: virtual void SetRotation(Quaternion _rotation) = 0;
	public: virtual void DisplaceRotation(Quaternion _rotation) = 0;
	public: virtual Quaternion GetRotation(void) = 0;
	public: virtual Quaternion GetAbsoluteRotation(void) = 0;
};





#endif 