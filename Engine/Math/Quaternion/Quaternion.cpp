
#include "Quaternion.h"
#include <Engine/Math/Matrix/Matrix4x4/Matrix4x4.h>
#include <Engine/Math/Matrix/Matrix3x3/Matrix3x3.h>
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Core/Common/Common.h>

 

const Quaternion Quaternion::IDENTITY(0.0f, 0.0f, 0.0f, 1.0f);



Quaternion::Quaternion(void)
{
	Identity();
}


Quaternion::Quaternion(float _x, float _y, float _z, float _w): x(_x), y(_y), z(_z), w(_w)
{}



Quaternion::Quaternion(const Quaternion& _from, const Quaternion& _to)
{
	FromTo(_from, _to);
}


Quaternion::Quaternion(const Vector3& _from, const Vector3& _to)
{
	ShortestArc(_from, _to);
}



Quaternion::Quaternion(const AxisAngle& _axisAngle)
{
	FromAxisAngle(_axisAngle);
}



Quaternion::Quaternion(const Matrix4x4& _matrix)
{
	FromMatrix4x4(_matrix);
}


Quaternion::Quaternion(const Matrix3x3<Vector3>& _matrix)
{
	FromMatrix3x3(_matrix);
}



Quaternion::Quaternion(const Euler& _euler)
{
	FromEulerPitchYawRoll(_euler);
}



Vector3 Quaternion::RotatePointAroundPivot(Vector3 _point, const Vector3& _pivot)const
{
	_point -= _pivot;  
	_point = (*this) * _point;
	_point += _pivot;
	return _point;
}


Vector3 Quaternion::_RotatePointAroundPivot(const Quaternion& _rotation, Vector3 _point, const Vector3& _pivot)
{
	_point -= _pivot;  
	_point = _rotation * _point;
	_point += _pivot;
	return _point;
}



void Quaternion::Identity(void)
{
	x = y = z = 0.0f;
	w = 1.0f;
}

 
void Quaternion::Normalize(void)
{ 
	float length = sqrtf(x * x + y * y + z * z + w * w);

	if(length < Numerical<float>::TINY)
	{
		StabilizeLength();
		length = sqrtf(x * x + y * y + z * z + w * w);
	}
	(*this) *= 1.0f / length;

}


Quaternion Quaternion::operator * (const Quaternion& _quaternion)const 
{ 
	return _GetNormalize(Quaternion(w * _quaternion.x + x * _quaternion.w + y * _quaternion.z - z * _quaternion.y,
	                                w * _quaternion.y - x * _quaternion.z + y * _quaternion.w + z * _quaternion.x,
	                                w * _quaternion.z + x * _quaternion.y - y * _quaternion.x + z * _quaternion.w,
	                                w * _quaternion.w - x * _quaternion.x - y * _quaternion.y - z * _quaternion.z));
}


Quaternion& Quaternion::operator *= (Quaternion _quaternion)
{ 
	(*this) = (*this) * _quaternion;
	return *this;   
}



float& Quaternion::operator [] (int32 _index)
{ 
	return *(&x + _index);
}

float Quaternion::operator [] (int32 _index)const
{ 
	return *(&x + _index);
}


Quaternion Quaternion::operator + (const Quaternion& _quaternion)const
{
	return Quaternion(x + _quaternion.x, 
		              y + _quaternion.y, 
					  z + _quaternion.z, 
					  w + _quaternion.w);
}


Quaternion& Quaternion::operator += (Quaternion _quaternion)
{
	x += _quaternion.x;
	y += _quaternion.y;
	z += _quaternion.z;
	w += _quaternion.w;
	return *this;
}	


Quaternion Quaternion::operator - (const Quaternion& _quaternion)const
{
	return Quaternion(x - _quaternion.x, 
		              y - _quaternion.y, 
					  z - _quaternion.z, 
					  w - _quaternion.w);
}


Quaternion& Quaternion::operator -= (Quaternion _quaternion)
{
	x -= _quaternion.x;
	y -= _quaternion.y;
	z -= _quaternion.z;
	w -= _quaternion.w;
	return *this;
}


bool Quaternion::operator == (const Quaternion& _quaternion)const
{
	return(x == _quaternion.x && y == _quaternion.y && z == _quaternion.z && w == _quaternion.w);		
}


bool Quaternion::operator != (const Quaternion& _quaternion)const
{
	return !(*this == _quaternion);
}


Quaternion Quaternion::operator - (void)const
{
	 return Quaternion(-x, -y, -z, -w);
}




Quaternion Quaternion::operator * (float _scalar)const
{
	return Quaternion(x * _scalar, y * _scalar, z * _scalar, w * _scalar);
}



Quaternion& Quaternion::operator *= (float _scalar)
{
	x *= _scalar;
	y *= _scalar; 
	z *= _scalar;
	w *= _scalar;
	return *this;
}


Quaternion Quaternion::operator / (float _scalar)const
{
	if(_scalar != 0.0f)
	{
		return Quaternion(x / _scalar, y / _scalar, z / _scalar, w / _scalar);
	}
	return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}



Quaternion& Quaternion::operator /= (float _scalar)
{
	if(_scalar != 0.0f)
	{
		x /= _scalar;
		y /= _scalar; 
		z /= _scalar;
		w /= _scalar;
		return *this;
	}
	x = y = z = w = 0.0f;
	return *this;
}



float Quaternion::_GetDot(const Quaternion& _q1, const Quaternion& _q2)
{
	return _q1.GetDot(_q2);
}



float Quaternion::GetDot(const Quaternion& _quaternion)const
{
	 return x * _quaternion.x + y * _quaternion.y + z * _quaternion.z + w * _quaternion.w;
	
}



void Quaternion::StabilizeLength(void) 
{
	float cs = (float)(fabs(x) + fabs(y) + fabs(z) + fabs(w));
		
	if(cs > 0.0f)
	{
		x /= cs;  
		y /= cs;  
		z /= cs;  
		w /= cs;
	}
	else
	{
		Identity();
	}
}



float Quaternion::GetNorm(void)const 
{
	return x * x + y * y + z * z + w * w;
}


float Quaternion::GetLength(void)const 
{
	return sqrtf(x * x + y * y + z * z + w * w);
}



void Quaternion::Conjugate(void) 
{
	x = -x; 
	y = -y; 
	z = -z;
}



void Quaternion::Inverse(void) 
{
	float norm = x * x + y * y + z * z + w * w;

	norm = 1.0f / norm;
	     
	x = -x * norm;
	y = -y * norm;
	z = -z * norm;
	w =  w * norm;
}

 
bool Quaternion::_IsEquals(const Quaternion& _q1, const Quaternion& _q2, float _delta)
{
	 return (fabs(_q1.x - _q2.x) < _delta) &&
            (fabs(_q1.y - _q2.y) < _delta) &&
            (fabs(_q1.z - _q2.z) < _delta) &&
            (fabs(_q1.w - _q2.w) < _delta);
}


Vector3 Quaternion::GetAxisX(void)const
{
    float y2 = 2.0f * y;
    float z2 = 2.0f * z;
    float y2w = y2 * w;
    float z2w = z2 * w;
    float y2x = y2 * x;
    float z2x = z2 * x;
    float y2y = y2 * y;
    float z2z = z2 * z;

	return Vector3(1.0f - (y2y + z2z), y2x + z2w, z2x - y2w);
}


Vector3 Quaternion::GetAxisY(void)const
{
	float x2 = 2.0f * x;
    float y2 = 2.0f * y;
    float z2 = 2.0f * z;
    float x2w = x2 * w;
    float z2w = z2 * w;
    float x2x = x2 * x;
    float y2x = y2 * x;
    float z2y = z2 * y;
    float z2z = z2 * z;

    return Vector3(y2x - z2w, 1.0f - (x2x + z2z), z2y + x2w);
}


Vector3 Quaternion::GetAxisZ(void)const
{
	float x2 = 2.0f * x;
    float y2 = 2.0f * y;
    float z2 = 2.0f * z;
    float x2w = x2 * w;
    float y2w = y2 * w;
    float x2x = x2 * x;
    float z2x = z2 * x;
    float y2y = y2 * y;
    float z2y = z2 * y;

    return Vector3(z2x + y2w, z2y - x2w, 1.0f - (x2x + y2y));
}



void Quaternion::ToAxisXYZ(Vector3& _axisX, Vector3& _axisY, Vector3& _axisZ)
{
	Matrix3x3<Vector3> matrix(*this);

	_axisX.x = matrix.m11;
    _axisX.y = matrix.m21;
    _axisX.z = matrix.m31;

    _axisY.x = matrix.m12;
    _axisY.y = matrix.m22;
    _axisY.z = matrix.m32;

    _axisZ.x = matrix.m13;
    _axisZ.y = matrix.m23;
    _axisZ.z = matrix.m33;
}



Vector3 Quaternion::operator * (const Vector3& _vector)const
{
	float norm = x * x + y * y + z * z + w * w;

	if(norm != 0.0f)
	{
		Quaternion quaternion(_vector.x * w + _vector.z * y - _vector.y * z,
							  _vector.y * w + _vector.x * z - _vector.z * x,
							  _vector.z * w + _vector.y * x - _vector.x * y,
							  _vector.x * x + _vector.y * y + _vector.z * z);

		return Vector3(w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y,
					   w * quaternion.y + y * quaternion.w + z * quaternion.x - x * quaternion.z,
					   w * quaternion.z + z * quaternion.w + x * quaternion.y - y * quaternion.x) * (1.0f / norm);
	}
	return Vector3();
}



Vector3 Quaternion::RotateVector(const Vector3& _vector)const
{
	return (*this) * _vector;
}



Matrix3x3<Vector3> Quaternion::ToMatrix3x3(void)const
{	
    return Matrix3x3<Vector3>(*this);
}


void Quaternion::FromMatrix3x3(const Matrix3x3<Vector3>& _matrix)
{
	(*this) = _matrix.ToQuaternion();
}



void Quaternion::FromEulerPitchYawRoll(const Euler& _euler)
{
	float cos1 = cosf(_euler.roll * 0.5f);
    float cos2 = cosf(_euler.yaw * 0.5f);
    float cos3 = cosf(_euler.pitch * 0.5f);
    float sin1 = sinf(_euler.roll * 0.5f);
    float sin2 = sinf(_euler.yaw * 0.5f);
    float sin3 = sinf(_euler.pitch * 0.5f);

	x = cos1 * cos2 * sin3 - sin1 * sin2* cos3;
    y = cos1 * sin2 * cos3 + sin1 * cos2 * sin3;
    z = sin1 * cos2 * cos3 - cos1 * sin2 * sin3;
    w = cos1 * cos2 * cos3 + sin1 * sin2 * sin3;
}




void Quaternion::FromAxisAngle(const AxisAngle& _axisAngle)
{

	float halfAngle = _axisAngle.angle * 0.5f;
	float sinAngle = sinf(halfAngle);

    x = _axisAngle.axis.x * sinAngle;
	y = _axisAngle.axis.y * sinAngle;
	z = _axisAngle.axis.z * sinAngle;
	w = cosf(halfAngle);
}


AxisAngle Quaternion::ToAxisAngle(void)const 
{
	AxisAngle axisAngle;

	float length = sqrtf(x * x + y * y + z * z);
	
	if(length > Numerical<float>::TINY)
    {
		float invLength = 1.0f / length;
		
		axisAngle.axis = Vector3(x * invLength, y * invLength, z * invLength);
            
		if(w < 0.0f)
		{
			axisAngle.angle = 2.0f * atan2f(-length, -w);
		}
        else
		{
			axisAngle.angle = 2.0f * atan2f(length, w);
        }
	}
	else
	{
		axisAngle.axis = Vector3::UNIT_X;
	}

	return axisAngle;
}




Quaternion Quaternion::_GetIdentity(void)
{
	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}




 Quaternion Quaternion::_GetInverse(Quaternion _quaternion)
{
	_quaternion.Inverse();
	return _quaternion;
}


float Quaternion::_GetNorm(const Quaternion& _quaternion)
{
	return _quaternion.GetNorm();
}


float Quaternion::_GetLength(const Quaternion& _quaternion)
{
	return _quaternion.GetLength();
}



float Quaternion::GetAngle(void)const
{
	if(w < 0.0f)
	{
		return 2.0f * atan2f(-sqrtf(x * x + y * y + z * z), -w);
	}
	return 2.0f * atan2f(sqrtf(x * x + y * y + z * z), w);
}


Quaternion Quaternion::_GetNormalize(Quaternion _quaternion)
{
	_quaternion.Normalize();
	return _quaternion;
}



void Quaternion::FromTo(const Quaternion& _from, const Quaternion& _to)
{
	(*this) = Quaternion::_GetNormalize(Quaternion::_GetConjugate(_from) * _to);
}



void Quaternion::ShortestArc(const Vector3& _from, const Vector3& _to)
{ 
	Vector3 c = _from * _to;

	x = c.x;
	y = c.y;
	z = c.z; 
	w = Vector3::_GetDot(_from, _to);
	
	Normalize();
    
	w += 1.0f;

	if(w <= Numerical<float>::TINY)
    {
		if((_from.z * _from.z) > (_from.x * _from.x))
		{
			x = 0.0f;
			y = _from.z;
			z = -_from.y;
		}
        else
		{
			x = _from.y;
			y = -_from.x;
			z = 0.0f;
		}
	}

    Normalize(); 
}


Quaternion Quaternion::_GetConjugate(Quaternion _quaternion)
{ 
	_quaternion.Conjugate();
	return _quaternion;
}



Matrix4x4 Quaternion::ToMatrix4x4(void)const
{	
    return Matrix4x4(*this);
}



void Quaternion::FromMatrix4x4(const Matrix4x4& _matrix)
{
	(*this) = _matrix.ToQuaternion();
}



void Quaternion::SwingTwistDecomposition(const Vector3& _direction, Quaternion& _swing, Quaternion& _twist)
{
	Vector3 r(x, y, z);
	Vector3 p = r.Project(_direction);
	_twist = Quaternion(p.x, p.y, p.z, w);
	_twist.Normalize();
	_swing = (*this) * Quaternion::_GetConjugate(_twist);
}




Euler Quaternion::ToShortestEulerPitchYawRoll(void)const 
{
	Euler euler;
 
    float xx = x * x;
    float yy = y * y;
    float zz = z * z;
	float ww = w * w;
    
	euler.yaw = asinf(2.0f * (w * y - x * z));

	if(((float)Angle::HALF_PI - fabsf(euler.yaw)) != 0.0f) 
	{
        euler.roll = atan2f(2.0f * (x * y + w * z), xx - yy - zz + ww);
        euler.pitch = atan2f(2.0f * (w * x + y * z), ww - xx - yy + zz);
	} 
	else 
	{
        euler.roll = atan2f(2.0f * y * z - 2.0f * x * w, 2.0f * x * z + 2.0f * y * w);
        euler.pitch = 0.0f;
        
		if(euler.yaw < 0.0f)
		{ 
			euler.roll = (float)Angle::PI - euler.roll; 
		}
	}
	return euler;
}


