
#include "Ray.h"
	

Ray::Ray(void)
{}



Ray::Ray(Vector3 _origin, Vector3 _direction)
{
	origin = _origin; 
	direction = _direction;
}



Ray::Ray(Line _line)
{
	origin = _line.origin;
	direction = _line.GetDirection();
}


void Ray::Set(Vector3 _origin, Vector3 _direction)
{
	origin = _origin; 
	direction = _direction;
}



Vector3 Ray::GetPoint(float _t)
{
	if(_t < 0.0f) { return origin; }
	return origin + direction * _t;
}



Line Ray::ToLine(float _length)
{
	return Line(origin, origin + direction * _length);
}



bool Ray::_IsIntersected(Ray _a, Ray _b, float& _t)
{
	Vector3 a = _a.origin;
	Vector3 b = _a.origin + _a.direction;
	Vector3 c = _b.origin;
	Vector3 d = _b.origin + _b.direction;
	Vector3 n = Vector3((d - c).y, (c - d).x, 0.0f);
	float denom = Vector3::_GetDot(n, b - a);
	
	if(denom <= 0.0f)
	{
		return false;
	}
	float num = Vector3::_GetDot(n, a - c);
	_t = -num / denom;
	return true;
}


bool Ray::_IsIntersected(Ray _a, Ray _b, Vector3* _intersection)
{
	float t;
	bool result = _IsIntersected(_a, _b, t);

	if(result)
	{
		if(_intersection)
		{
			*_intersection = _a.GetPoint(t);
		}
	}
	return result;
}


void Ray::FromLine(Line _line)
{
	origin = _line.origin;
	direction = _line.GetDirection();
}
