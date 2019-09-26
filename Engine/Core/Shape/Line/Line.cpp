#include "Line.h"
#include <Engine/Math/Ray/Ray.h>
#include <Engine/Math/Angle/Angle.h>



Line::Line(void)
{}



Line::~Line(void)
{}


Line::Line(Vector3 _origin, Vector3 _end): origin(_origin), end(_end)
{}



void Line::Set(Vector3 _origin, Vector3 _end)
{
	 origin = _origin;
	 end = _end;
}



void Line::Rotate(void)
{
	Vector3 m = (origin + end) * 0.5f;
	Vector3 v = end - origin;
	Vector3 n(v.y, -v.x, 0.0f);
	origin = m - n * 0.5f;
	end = m + n * 0.5f;
}



bool Line::_IsIntersected(Line _a, Line _b, float& _t)
{
	Vector2 a(_a.origin.x, _a.origin.y); 
	Vector2 b(_a.end.x, _a.end.y);
	Vector2 c(_b.origin.x, _b.origin.y);
	Vector2 d(_b.end.x, _b.end.y);
	Vector2 n = Vector2((d - c).y, (c - d).x);
	float denom = Vector2::_GetDot(n, b - a);
	
	if(denom == 0.0f)
	{
		return false;
	}
	float num = Vector2::_GetDot(n, a - c);
	_t = -num / denom;
	return true;
}



bool Line::_IsIntersectedEx(Line _a, Line _b, float& _t)
{
	if(!_IsIntersected(_b, _a, _t)) 
	{
		return false;
	} 
	
	if((_t < 0.0f) || (_t > 1.0f))
	{
		return false;
	}

	_IsIntersected(_a, _b, _t);

	if((_t >= 0.0f) && (_t <= 1.0f))
	{
		return true;
	}
	return false;
}



bool Line::_IsIntersected(Line _a, Line _b, Vector3* _intersection)
{
	float t;
	bool result = _IsIntersectedEx(_a, _b, t);

	if(result)
	{
		if(_intersection)
		{
			*_intersection = _a.ToRay().GetPoint(t * _a.GetLength());
		}
	}
	return result;
}




int32 Line::_GetVertexPosition(Line _line, Vector3 _point)
{
	if(_line.origin == _point) { return ORIGIN; }
	if(_line.end == _point) { return END; }

	Vector3 a = _line.end - _line.origin;
	Vector3 b = _point - _line.origin;
	float sa = a.x * b.y - b.x * a.y;

	if(sa > 0.0f) { return LEFT; }
	if(sa < 0.0f) { return RIGHT; }
	if(a.GetLength() < b.GetLength()) { return BEYOND; }
	if((a.x * b.x < 0.0f) || (a.y * b.y < 0.0f)) { return BEHIND; }
	return BETWEEN;
}



float Line::_GetAngleZ(Vector2 _origin, Vector2 _end)
{
	return -atan2f(_origin.x - _end.x, _origin.y - _end.y) - (float)Angle::HALF_PI;
}



float Line::_GetAngleZ(Vector3 _origin, Vector3 _end)
{
	return -atan2f(_origin.x - _end.x, _origin.y - _end.y) - (float)Angle::HALF_PI;
}



Vector3 Line::GetCenter(void)const
{
	return (origin + end) * 0.5f;
}


Vector3 Line::_GetCenter(Vector3 _origin, Vector3 _end)
{
	return (_origin + _end) * 0.5f;
}


	
Vector3 Line::_GetDirection(Vector3 _origin, Vector3 _end)
{
	return Vector3::_GetNormalize(Vector3::_FromTo(_origin, _end));
}



Vector3 Line::GetDirection(void)const
{
	return Vector3::_GetNormalize(Vector3::_FromTo(origin, end));
}

	

void Line::Transform(Matrix4x4 _transformation)
{
	origin = _transformation.TransformVector(origin); 
	end = _transformation.TransformVector(end);
}



Line Line::_Transform(Line _line, Matrix4x4 _transformation)
{
	return Line(_transformation.TransformVector(_line.origin), 
		        _transformation.TransformVector(_line.end));
}


float Line::GetLength(void)const
{
	return (origin - end).GetLength();
}


float Line::_GetLength(Vector3 _origin, Vector3 _end)
{
	return (_origin - _end).GetLength(); 
}



float Line::_GetLength(Vector2 _origin, Vector2 _end)
{
	return (_origin - _end).GetLength(); 
}



void Line::SetLength(float _length)
{
	Vector3 center = GetCenter();
	Vector3 direction = GetDirection();
	float halfLength = _length / 2.0f;

	origin = center + -direction * halfLength;
	end = center + direction * halfLength;
}



void Line::SetLengthFromEndToOrigin(float _length)
{
	Vector3 direction = GetDirection();
	origin = end + -direction * _length;
}



void Line::SetLengthFromOriginToEnd(float _length)
{
	Vector3 direction = GetDirection();
	end = origin + direction * _length;
}
 
	

void Line::Scale(float _scale)
{
	Vector3 previousPosition = GetCenter();
	MoveCenterTo(Vector3());
	origin *= _scale;
	end *= _scale;
	MoveCenterTo(previousPosition);
} 



void Line::MoveCenterTo(Vector3 _position)
{
	float halfLength = GetLength() / 2.0f;
	Vector3 direction = GetDirection();

	origin = _position + -direction * halfLength;
	end = _position + direction * halfLength;
}





Ray Line::ToRay(void)const
{
	return Ray(origin, GetDirection());
}



void Line::FromRay(Ray _ray, float _length)
{
	origin = _ray.origin;
	end = _ray.origin + _ray.direction * _length;
}