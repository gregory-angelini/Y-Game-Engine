#include "Sphere.h"


Sphere::Sphere(void): radius(0.0f)
{}


Sphere::~Sphere(void)
{}



Sphere::Sphere(Vector3 _center, float _radius): center(_center), radius(_radius)
{}



bool Sphere::IsIntersected(Ray _ray)const
{
	Vector3 vector = _ray.origin - center;

	float b = 2.0f * Vector3::_GetDot(_ray.direction, vector);
	float c = vector.GetDot(vector) - (radius * radius);
	float discriminant = (b * b) - (4.0f * c);
	if(discriminant < 0.0f)
	{ 
		return false;
	}

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;
	if(s0 >= 0.0f || s1 >= 0.0f)
	{ 
		return true;
	}
	return false;
}


bool Sphere::IsPointInside(Vector3 _point)const
{
	Vector3 dif = center - _point;

	if(dif.GetDot(dif) < (radius * radius)) 
	{
		return true;
	}
	return false;
}


bool Sphere::IsIntersected(Line _line, Vector3* _intersectionPoint)
{
	Vector3 d = _line.end - _line.origin;

	float a = Vector3::_GetDot(d, d);
	float b = 2.0f * Vector3::_GetDot(d, _line.origin - center);
	float c = Vector3::_GetDot(center, center) + Vector3::_GetDot(_line.origin, _line.origin) - 2.0f * Vector3::_GetDot(center, _line.origin) - radius * radius;

	float test = b * b - 4.0f * a * c;

	if(test >= 0.0f) 
	{
		float u = (-b - sqrtf(test)) / (2.0f * a);
		if(_intersectionPoint) { *_intersectionPoint = _line.origin + (_line.end - _line.origin) * u; }
		return true;
	}
	return false;
}



bool Sphere::IsIntersected(Sphere _sphere)
{
	float dist = Line::_GetLength(center, _sphere.center);
	return dist <= (radius + _sphere.radius);
}



Vector3 Sphere::ClampPoint(Vector3 _point)
{
	if(IsPointInside(_point)) { return _point; }
	Vector3 direction = Line::_GetDirection(center, _point);
	return center + direction * radius;
}
