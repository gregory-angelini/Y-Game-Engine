#ifndef POINT_H
#define POINT_H
#include <Engine/Core/Object/TranslatableObject/TranslatableObject.h>
class Point
{
	public: Vector3 point;
	public: Point(void);
	public: virtual ~Point(void);
	public: void SetPosition(Vector3 _point);
	public: void DisplacePosition(Vector3 _offset);
	public: void MoveTo(Vector3 _direction, float _units);
};



#endif 