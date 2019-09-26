
#include "Vector2.h"
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Matrix/Matrix3x3/Matrix3x3.h>
#include <math.h>


const Vector2 Vector2::UNIT_X(1.0f, 0.0f);
const Vector2 Vector2::UNIT_Y(0.0f, 1.0f);
const int32 Vector2::SIZE = 2;


Vector2::Vector2(void): x(0.0f), y(0.0f)
{}



Vector2::Vector2(float _x, float _y): x(_x), y(_y)
{}


void Vector2::Set(float _x, float _y)
{
	x = _x;
	y = _y;
}


Vector2 Vector2::_RotateAroundPivot(Vector2 _point, Vector2 _pivot, float _radians)
{
	Matrix3x3<Vector2> matrix(_radians);
	return matrix.RotatePointAroundPivot(_point, _pivot);
}



float& Vector2::operator [] (int32 _index)
{ 
	return *(&x + _index);
}

float Vector2::operator [] (int32 _index)const
{ 
	return *(&x + _index);
}


float Vector2::operator / (const Vector2& _vector)const
{
	return x * _vector.x + y * _vector.y; 
}


float Vector2::GetDot(const Vector2& _vector)const 
{ 
	return x * _vector.x + y * _vector.y; 
}
	

 float Vector2::GetLength(void)const 
{ 
	return sqrtf(GetDot(*this));
}
 


Vector2 Vector2::operator - (const Vector2& _vector)const 
{ 
	return Vector2(x -_vector.x, y - _vector.y); 
}


Vector2& Vector2::operator -= (Vector2 _vector)
{ 
	x -= _vector.x; 
	y -= _vector.y; 
	return *this;	
}



Vector2 Vector2::operator + (const Vector2& _vector)const 
{ 
	return Vector2(x + _vector.x, y + _vector.y); 
}


	

Vector2& Vector2::operator += (Vector2 _vector)
{ 
	x += _vector.x; 
	y += _vector.y; 
	return *this;	
}



bool Vector2::operator == (const Vector2& _vector)const 
{ 
	return (x == _vector.x && y == _vector.y);
}


			
bool Vector2::operator != (const Vector2& _vector)const 
{ 
	return !(*this == _vector);
}
 
 
Vector2 Vector2::operator / (float _scalar)const 
{ 
	if(_scalar != 0.0f)
	{
		return Vector2(x / _scalar, y / _scalar); 
	}
	return Vector2();
}




Vector2& Vector2::operator /= (float _scalar)
{ 
	if(_scalar != 0.0f)
	{
		x /= _scalar; 
		y /= _scalar; 
		return *this;   
	}
	x = y = 0.0f;
	return *this;   
}

Vector2 Vector2::operator - (void)const 
{ 
	return Vector2(-x, -y);
}


Vector2 Vector2::operator * (float _scalar)const 
{  
	return Vector2(x * _scalar, y * _scalar); 
}
	

Vector2& Vector2::operator *= (float _scalar)
{ 
	x *= _scalar; 
	y *= _scalar; 
	return *this;   
}



void Vector2::Cross(void)
{
	float memory = -x; 
	x = y;
	y = memory;
}



Vector2 Vector2::_GetInverse(Vector2 _vector)
{
	return -_vector;
}



void Vector2::Inverse(void)
{
	x = -x;
	y = -y;
}



void Vector2::Normalize(void)
{ 
	float length = GetLength();

	if(length != 0.0f)
	{
		length = 1.0f / length;
		(*this) *= length; 
	}
}



float Vector2::_GetAngle(Vector2 _vector1, Vector2 _vector2)
{
	return acosf(Vector2::_GetDot(Vector2::_GetNormalize(_vector1), Vector2::_GetNormalize(_vector2)));
}



float Vector2::GetAngleZ(void)const
{
	return atan2f(y, x);
}


float Vector2::_GetAngleZ(Vector2 _vector)
{
	return _vector.GetAngleZ();
}


void Vector2::Rotate(float _angle)
{
	Vector2 vector;

	float cosAngle = cosf(_angle);
	float sinAngle = sinf(_angle);
	vector.x = x * cosAngle - y * sinAngle;
	vector.y = x * sinAngle + y * cosAngle;

	x = vector.x;
	y = vector.y;
}



Vector2 Vector2::_GetCross(Vector2 _vector)
{
	_vector.Cross();
	return _vector;
}


Vector2 Vector2::_GetNormalize(Vector2 _vector)
{
	Vector2 vector = _vector;
	vector.Normalize();
	return vector;
}




float Vector2::_GetLength(Vector2 _vector)
{
	return _vector.GetLength();
}



float Vector2::_GetDot(Vector2 _vector1, Vector2 _vector2)
{
	return _vector1.GetDot(_vector2);
}



Vector2 Vector2::_Rotate(Vector2 _vector, float _angle)
{ 
	Vector2 vector = _vector;
	vector.Rotate(_angle);
	return vector;
}


Vector2 Vector2::_FromTo(Vector2 _from, Vector2 _to)
{
	return _to - _from;
}




Vector3 Vector2::ToVector3(void)
{
	return Vector3(x, y, 0.0f);
}



Vector3 Vector2::_ToVector3(Vector2 _vector)
{
	return Vector3(_vector.x, _vector.y, 0.0f);
}

Vector2 Vector2::_FromVector3(Vector3 _vector)
{
	return Vector2(_vector.x, _vector.y);
}



void Vector2::FromPolar(Vector2 _polar)
{
	x = _polar.y * cosf(_polar.x);
	y = _polar.y * sinf(_polar.x);
}


Vector2 Vector2::_FromPolar(Vector2 _polar)
{
	Vector2 vector;
	vector.FromPolar(_polar);
	return vector;
}


Vector2 Vector2::ToPolar(void)const
{
	float angle;
	float radius = pow((x * x + y * y), 0.5f);
	
	if(x == 0.0f)
	{
		if(y < 0.0f)
		{
			angle = Angle::PI / 2.0f;
		}
		else if(y > 0.0f)
		{
			angle = 3.0f * Angle::PI / 2.0f;
		}
		else 
		{
			angle = 0.0f;
		}
	}
	else
	{
		angle = atanf(y / x);

		if(x > 0.0f && y < 0.0f)
		{
			angle += 2.0f * Angle::PI;
		}
		if(x < 0.0f)
		{
			angle += Angle::PI;
		}
	}	

	return Vector2(angle, radius);
}


Vector2 Vector2::_ToPolar(Vector2 _vector)
{
	return _vector.ToPolar();
}
