#ifndef AXISANGLE_H
#define AXISANGLE_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class AxisAngle
{
	public: Vector3 axis;
	public: float angle;
	public: AxisAngle(void);
	public: AxisAngle(Vector3 _axis, float _angle);
	public: bool operator == (const AxisAngle& _axisAngle)const;
	public: bool operator != (const AxisAngle& _axisAngle)const;
};



#endif 
