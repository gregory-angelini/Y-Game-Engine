#ifndef ABSTRACTSURFACE_H
#define ABSTRACTSURFACE_H
#include <Engine/Core/Shape/Line/Line.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
class AbstractSurface: public virtual AbstractObject
{
	public: enum: int64 
	{
		ABSTRACT_SURFACE_CLASS = 536870912
	};
	public: enum
	{
		CHECK_LINE_IN_SURFACE_MESSAGE = 37,
		CHANGE_INTERSECTION_MODE_MESSAGE = 118
	};
	public: enum 
	{
		CHECK_ONLY_FRONT_FACE = 1,
		APPLY_TRANSFORMATION = 2
	};
	protected: int32 intersectionMode;
	public: AbstractSurface(void);
	public: virtual ~AbstractSurface(void);
    public: AbstractSurface& operator = (const AbstractSurface& _object);
	public: void SetIntersectionMode(int32 _mode);
	public: virtual void SetIntersectionMode(int32 _mode, bool _enable);
	public: int32 GetIntersectionMode(void)const;
	public: virtual int32 IsIntersected(Line _line, Vector3* _intersectionPoint = NIL) = 0;
};


#endif 