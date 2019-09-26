#include "Circle.h"
#include <Engine/Core/Shape/Line/Line.h>
#include <Engine/Core/Shape/Plane/Plane.h>




Circle::Circle(void)
{
	Set(Vector3(), 1.0f);
}



Circle::Circle(Vector3 _center, float _radius)
{
	Set(_center, _radius);
}


Circle::Circle(Vector3 _p1, Vector3 _p2, Vector3 _p3)
{
	Set(_p1, _p2, _p3);
}


Circle::Circle(Vector3 _p1, Vector3 _p2)
{
	Set(_p1, _p2);
}



Circle::~Circle(void)
{}



float Circle::GetLength(void)const
{
	return radius * 2.0f * Angle::PI;
}



float Circle::_GetLength(float _radius)
{
	return _radius * 2.0f * Angle::PI;
}


float Circle::_GetDiameter(float _radius)
{
	return _radius * 2.0f;
}



bool Circle::Set(Vector3 _p1, Vector3 _p2, Vector3 _p3)
{ 
	     if (!IsPerpendicular(_p1, _p2, _p3)) { return CalculateCircle(_p1, _p2, _p3); }	
	else if (!IsPerpendicular(_p1, _p3, _p2)) { return CalculateCircle(_p1, _p3, _p2); }	
	else if (!IsPerpendicular(_p2, _p1, _p3)) { return CalculateCircle(_p2, _p1, _p3); }	
	else if (!IsPerpendicular(_p2, _p3, _p1)) { return CalculateCircle(_p2, _p3, _p1); }	
	else if (!IsPerpendicular(_p3, _p2, _p1)) { return CalculateCircle(_p3, _p2, _p1); }
	else if (!IsPerpendicular(_p3, _p1, _p2)) { return CalculateCircle(_p3, _p1, _p2); }
	return false;
}


bool Circle::IsPerpendicular(Vector3 _p1, Vector3 _p2, Vector3 _p3)
{
	float aDeltaY = _p2.y - _p1.y;
	float aDeltaX = _p2.x - _p1.x;
	float bDeltaY = _p3.y - _p2.y;
	float bDeltaX = _p3.x - _p2.x;
	
	if(fabs(aDeltaX) <= Numerical<float>::TINY && fabs(bDeltaY) <= Numerical<float>::TINY)
	{
		return false;
	}

	if(fabs(aDeltaY) <= Numerical<float>::TINY)
	{
		return true;
	}
	else if(fabs(bDeltaY) <= Numerical<float>::TINY)
	{
		return true;
	}
	else if(fabs(aDeltaX) <= Numerical<float>::TINY)
	{
		return true;
	}
	else if(fabs(bDeltaX) <= Numerical<float>::TINY)
	{
		return true;
	}
	return false;
}


bool Circle::CalculateCircle(Vector3 _p1, Vector3 _p2, Vector3 _p3)
{
	float aDeltaY = _p2.y - _p1.y;
	float aDeltaX = _p2.x - _p1.x;
	float bDeltaY = _p3.y - _p2.y;
	float bDeltaX = _p3.x - _p2.x;
	
	if(fabs(aDeltaX) <= Numerical<float>::TINY && fabs(bDeltaY) <= Numerical<float>::TINY)
	{
		center.x = 0.5f * (_p2.x + _p3.x);
		center.y = 0.5f * (_p1.y + _p2.y);
		center.z = _p1.z;
		radius = Line::_GetLength(center, _p1);
		return true;
	}
	
	float aSlope = aDeltaY / aDeltaX; 
	float bSlope = bDeltaY / bDeltaX;

	if(fabs(aSlope - bSlope) <= Numerical<float>::TINY)
	{	
		return false;
	}
	center.x = (aSlope * bSlope * (_p1.y - _p3.y) + bSlope * (_p1.x + _p2.x) - aSlope * (_p2.x + _p3.x)) / (2.0f * (bSlope - aSlope));
	center.y = -1.0f * (center.x - (_p1.x + _p2.x) / 2.0f) / aSlope + (_p1.y + _p2.y) / 2.0f;
	center.z = _p1.z;
	radius =  Line::_GetLength(center, _p1);
	return true;
}



void Circle::Set(Vector3 _p1, Vector3 _p2)
{
	center = Line::_GetCenter(_p1, _p2);
	radius = Line::_GetLength(_p1, _p2) / 2.0f; 
}



void Circle::Set(Vector3 _center, float _radius)
{
	center = _center;
	radius = _radius;
}


void Circle::Scale(float _scale)
{
	radius *= _scale;
}


void Circle::BuildCircle(int32 _segments, vector<Vector3>& _vertices)
{
	if(_segments < 3) { return; }

	Vector3 vertex;
	_vertices.resize(_segments, Vector3());
	for(int32 i = 0; i < _segments; ++i)
	{
		float angle = Angle::DOUBLE_PI * (float)i / _segments;
		vertex.x = center.x + cosf(angle) * radius;
		vertex.y = center.y + sinf(angle) * radius;

		_vertices[i] = vertex;
	}
}



bool Circle::IsPointInside(Vector3 _point)
{
	float dx = _point.x - center.x;
	float dy = _point.y - center.y;

	return !((radius * radius) <= (dx * dx + dy * dy));
}



bool Circle::IsIntersected(Circle _circle)
{
	float dist = Line::_GetLength(center, _circle.center);
	return dist <= (radius + _circle.radius);
}



Vector3 Circle::ClampPoint(Vector3 _point)
{
	if(IsPointInside(_point)) { return _point; }
	Vector3 direction = Line::_GetDirection(center, _point);
	return center + direction * radius;
}


bool Circle::IsIntersected(Line _line, Vector3* _intersectionPoint, Vector3 _direction)
{
	Vector3 point;

	if(Plane::_IsIntersected(center, _direction, _line, 0, &point) != -1)
	{
		if(IsPointInside(point))
		{
			if(_intersectionPoint) { *_intersectionPoint = point; } 
			return true;
		}
	}
	return false;
}