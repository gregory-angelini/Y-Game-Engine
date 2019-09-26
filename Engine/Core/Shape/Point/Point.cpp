#include "Point.h"



Point::Point(void)
{}


Point::~Point(void)
{}


void Point::SetPosition(Vector3 _point)
{
	point = _point;
}


void Point::DisplacePosition(Vector3 _offset)
{
	SetPosition(point + _offset);
}



				
void Point::MoveTo(Vector3 _direction, float _units)
{
	if(_units != 0.0f) { SetPosition(point + _direction * _units); }
}
