#ifndef TRANSLATABLEOBJECT_H
#define TRANSLATABLEOBJECT_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class TranslatableObject: public virtual AbstractObject
{
	public: enum
	{
		REPLACE_POSITION_MESSAGE = 30,
		DISPLACE_POSITION_MESSAGE = 31,
		GET_POSITION_MESSAGE = 13,
		MOVE_AROUND_PIVOT_MESSAGE = 167
	};
	public: enum: int64
	{
		TRANSLATABLE_OBJECT_CLASS = 262144 
	};
	public: TranslatableObject(void);
	public: virtual ~TranslatableObject(void);
	public: virtual void SetPosition(Vector3 _position) = 0;
	public: virtual void DisplacePosition(Vector3 _offset) = 0;
	public: virtual void MoveTo(Vector3 _direction, float _units) = 0;
	public: virtual Vector3 GetPosition(void) = 0;
	public: virtual Vector3 GetAbsolutePosition(void) = 0;
};


#endif 