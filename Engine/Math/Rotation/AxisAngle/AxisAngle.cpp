
#include "AxisAngle.h"


AxisAngle::AxisAngle(void): angle(0.0f)
{}


AxisAngle::AxisAngle(Vector3 _axis, float _angle): axis(_axis), angle(_angle)
{}



bool AxisAngle::operator == (const AxisAngle& _axisAngle)const
{
	if(axis == _axisAngle.axis && angle == _axisAngle.angle)
	{
		return true;
	}
	return false;
}




bool AxisAngle::operator != (const AxisAngle& _axisAngle)const
{
		return !(*this == _axisAngle);
}