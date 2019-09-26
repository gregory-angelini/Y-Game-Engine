
#include "Angle.h"


double Angle::PI = 3.14159265358979323846;
double Angle::HALF_PI = PI / 2.0;
double Angle::DOUBLE_PI = PI * 2.0;
double Angle::DEGREES_TO_RADIANS = PI / 180.0;
double Angle::RADIANS_TO_DEGREES = 180.0 / PI;
double Angle::DEGREES_IN_CIRCLE = 360.0f;
double Angle::RADIANS_IN_CIRCLE = PI * 2.0;



float Angle::_ArcCos(float _radians)
{
	if(-1.0f < _radians)
    {
		if( _radians < 1.0f)
		{
			return acosf(_radians);
		}
        else
		{
			return 0.0f;
		}
    }
	else
    {
		return PI;
    }
}

float Angle::_ArcSin(float _radians)
{
	if(-1.0f < _radians)
    {
		if( _radians < 1.0f)
		{
			return asinf(_radians);
		}
        else
		{
			return HALF_PI;
        }
	}
    else
	{
		return -HALF_PI;
    }
}

float Angle::_DegreesToRadians(float _degrees)
{
	return _degrees * (float)DEGREES_TO_RADIANS;
}


float Angle::_RadiansToDegrees(float _radians)
{
	return _radians * (float)RADIANS_TO_DEGREES;
}


float Angle::_ClampRadians(float _radians)
{
	while(_radians < 0.0f)
	{
		_radians += (float)RADIANS_IN_CIRCLE;
	}

	while(_radians > (float)RADIANS_IN_CIRCLE)
	{
		_radians -= (float)RADIANS_IN_CIRCLE;
	}
	return _radians;
}


float Angle::_ClampDegrees(float _degrees)
{
	while(_degrees < 0.0f)
	{
		_degrees += (float)DEGREES_IN_CIRCLE;
	}

	while(_degrees > (float)DEGREES_IN_CIRCLE)
	{
		_degrees -= (float)DEGREES_IN_CIRCLE;
	}
	return _degrees;
}
