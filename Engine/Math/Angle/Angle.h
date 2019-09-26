#ifndef ANGLE_H
#define ANGLE_H
#include <Engine/Core/Numerical/Numerical.h>
class Angle
{
	public: static double PI;
	public: static double HALF_PI;
	public: static double DOUBLE_PI;
	public: static double DEGREES_TO_RADIANS;
	public: static double RADIANS_TO_DEGREES;
	public: static double DEGREES_IN_CIRCLE;
	public: static double RADIANS_IN_CIRCLE;
	public: static float _ArcCos(float _radians);
	public: static float _ArcSin(float _radians);
	public: static float _DegreesToRadians(float _degrees);
	public: static float _RadiansToDegrees(float _radians);
	public: static float _ClampRadians(float _radians);
	public: static float _ClampDegrees(float _degrees);
};



#endif 
