
#include "Matrix4x4.h"
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Core/Common/Common.h>
#include <Engine/Math/Matrix/Matrix3x3/Matrix3x3.h>




  
const Matrix4x4 Matrix4x4::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f);



Matrix4x4::Matrix4x4(Vector3 _scale, Quaternion _rotation, Vector3 _position)
{
	MakeTransform(_scale, _rotation, _position);
}


Matrix4x4::Matrix4x4(void)
{
	Identity();
}


Matrix4x4::Matrix4x4(float _m11, float _m12, float _m13, float _m14,
                     float _m21, float _m22, float _m23, float _m24,
                     float _m31, float _m32, float _m33, float _m34,
                     float _m41, float _m42, float _m43, float _m44)
{
    m11 = _m11; m12 = _m12; m13 = _m13; m14 = _m14;
    m21 = _m21; m22 = _m22; m23 = _m23; m24 = _m24;
    m31 = _m31; m32 = _m32; m33 = _m33; m34 = _m34;
    m41 = _m41; m42 = _m42; m43 = _m43; m44 = _m44;
}

void Matrix4x4::Set(float _m11, float _m12, float _m13, float _m14,
					float _m21, float _m22, float _m23, float _m24,
					float _m31, float _m32, float _m33, float _m34,
					float _m41, float _m42, float _m43, float _m44)
{
    m11 = _m11; m12 = _m12; m13 = _m13; m14 = _m14;
    m21 = _m21; m22 = _m22; m23 = _m23; m24 = _m24;
    m31 = _m31; m32 = _m32; m33 = _m33; m34 = _m34;
    m41 = _m41; m42 = _m42; m43 = _m43; m44 = _m44;
}
	


Matrix4x4::Matrix4x4(Quaternion _quaternion)
{
	FromQuaternion(_quaternion);
}




Matrix4x4::Matrix4x4(AxisAngle _axisAngle)
{
	FromAxisAngle(_axisAngle);
}


Matrix4x4::Matrix4x4(Vector3 _from, Vector3 _to, Vector3 _up)
{
	LookAt(_from, _to, _up);
}


Matrix4x4::Matrix4x4(Vector3 _x, Vector3 _y, Vector3 _z, Vector3 _w)
{
	SetX(_x);
	SetY(_y);
	SetZ(_z);
	SetW(_w);
}



Matrix4x4::Matrix4x4(Euler _euler)
{
	FromEulerPitchYawRoll(_euler);
}



Matrix4x4::Matrix4x4(Vector3 _vector, float _radians)
{
	FromAxisAngle(_vector, _radians);
}
    


void Matrix4x4::MakeTransform(Vector3 _scale, Quaternion _rotation, Vector3 _position)
{
	float x2 = _rotation.x + _rotation.x;
	float y2 = _rotation.y + _rotation.y;
	float z2 = _rotation.z + _rotation.z;

    float xx = _rotation.x * x2;
	float xy = _rotation.x * y2;
	float xz = _rotation.x * z2;
    float yy = _rotation.y * y2;
	float yz = _rotation.y * z2;
	float zz = _rotation.z * z2;

    float wx = _rotation.w * x2;
	float wy = _rotation.w * y2;
	float wz = _rotation.w * z2;

    m11 = 1.0f - (yy + zz);
	m12 = xy - wz;
	m13 = xz + wy;

	m21 = xy + wz;
	m22 = 1.0f - (xx + zz);
    m23 = yz - wx;

	m31 = xz - wy;
	m32 = yz + wx;
	m33 = 1.0f - (xx + yy);
    m11 *= _scale.x; m12 *= _scale.y; m13 *= _scale.z; m14 = _position.x;
    m21 *= _scale.x; m22 *= _scale.y; m23 *= _scale.z; m24 = _position.y;
    m31 *= _scale.x; m32 *= _scale.y; m33 *= _scale.z; m34 = _position.z;
    m41 = 0.0f;      m42 = 0.0f;      m43 = 0.0f;      m44 = 1.0f;
}



void Matrix4x4::MakeTransform(Vector2 _scale, float _rotation, Vector2 _position)
{
	Matrix3x3<Vector3> rotation(Quaternion(AxisAngle(Vector3::UNIT_Z, _rotation)));

    m11 = _scale.x * rotation.m11; m12 = _scale.y * rotation.m12; m13 = 0.0f; m14 = _position.x;
    m21 = _scale.x * rotation.m21; m22 = _scale.y * rotation.m22; m23 = 0.0f; m24 = _position.y;
    m31 = 0.0f;                    m32 = 0.0f;                    m33 = 1.0f; m34 = 0.0f;
    m41 = 0.0f;                    m42 = 0.0f;                    m43 = 0.0f; m44 = 1.0f;
}


void Matrix4x4::SetX(Vector3 _axis)
{
	m11 = _axis.x;
	m21 = _axis.y;
	m31 = _axis.z;
}


Vector3 Matrix4x4::GetX(void)const
{
	return Vector3(m11, m21, m31);
}


void Matrix4x4::SetY(Vector3 _axis)
{
	m12 = _axis.x;
	m22 = _axis.y;
	m32 = _axis.z;
}


Vector3 Matrix4x4::GetY(void)const
{
	return Vector3(m12, m22, m32);
}


void Matrix4x4::SetZ(Vector3 _axis)
{
	m13 = _axis.x;
	m23 = _axis.y;
	m33 = _axis.z;
}


Vector3 Matrix4x4::GetZ(void)const
{
	return Vector3(m13, m23, m33);
}


void Matrix4x4::SetW(Vector3 _axis)
{
	m14 = _axis.x; 
	m24 = _axis.y;
	m34 = _axis.z;
}


Vector3 Matrix4x4::GetW(void)const
{
	return Vector3(m14, m24, m34);
}



void Matrix4x4::Swap(Matrix4x4 _matrix)
{
	Common::_Swap(m11, _matrix.m11);
    Common::_Swap(m12, _matrix.m12);
    Common::_Swap(m13, _matrix.m13);
    Common::_Swap(m14, _matrix.m14);

    Common::_Swap(m21, _matrix.m21);
    Common::_Swap(m22, _matrix.m22);
    Common::_Swap(m23, _matrix.m23);
    Common::_Swap(m24, _matrix.m24);

    Common::_Swap(m31, _matrix.m31);
    Common::_Swap(m32, _matrix.m32);
    Common::_Swap(m33, _matrix.m33);
    Common::_Swap(m34, _matrix.m34);

    Common::_Swap(m41, _matrix.m41);
    Common::_Swap(m42, _matrix.m42);
    Common::_Swap(m43, _matrix.m43);
    Common::_Swap(m44, _matrix.m44);
}


void Matrix4x4::Identity(void)
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}



void Matrix4x4::InvertTranslation(void)
{
	m14 = -m14; 
	m24 = -m24; 
	m34 = -m34;  
}


Matrix4x4 Matrix4x4::_InvertTranslation(Matrix4x4 _translation)
{
	_translation.InvertTranslation();
	return _translation;
}



Matrix4x4 Matrix4x4::_FromAxisAngle(AxisAngle _axisAngle)
{
	return Matrix4x4(_axisAngle);
}


AxisAngle Matrix4x4::_ToAxisAngle(Matrix4x4 _matrix)
{
	return _matrix.ToAxisAngle();
}



Matrix4x4 Matrix4x4::_FromEulerPitchYawRoll(Euler _euler)
{
	return Matrix4x4(_euler);
}



Euler Matrix4x4::_ToEulerPitchYawRoll(Matrix4x4 _matrix)
{
	return _matrix.ToEulerPitchYawRoll();
}



Matrix4x4::Matrix4x4(Matrix3x3<Vector2> _matrix)
{
	m11 = _matrix.m11; m12 = _matrix.m12; m13 = 0.0f; m14 = _matrix.m13;
	m21 = _matrix.m21; m22 = _matrix.m22; m23 = 0.0f; m24 = _matrix.m23;
	m31 = _matrix.m31; m32 = _matrix.m32; m33 = 1.0f; m34 = _matrix.m33;
	m41 = 0.0f;        m42 = 0.0f;        m43 = 0.0f;        m44 = 1.0f;
}



void Matrix4x4::Transpose(void)
{
	float memory; 
	memory = m12; m12 = m21; m21 = memory;
    memory = m13; m13 = m31; m31 = memory;
	memory = m14; m14 = m41; m41 = memory;

	memory = m23; m23 = m32; m32 = memory;
	memory = m24; m24 = m42; m42 = memory;
	memory = m34; m34 = m43; m43 = memory;
}



Matrix4x4 Matrix4x4::operator + (const Matrix4x4& _matrix)const 
{ 
	Matrix4x4 matrix;

	matrix.m11 = m11 + _matrix.m11; matrix.m12 = m12 + _matrix.m12; matrix.m13 = m13 + _matrix.m13; matrix.m14 = m14 + _matrix.m14;
	matrix.m21 = m21 + _matrix.m21; matrix.m22 = m22 + _matrix.m22; matrix.m23 = m23 + _matrix.m23; matrix.m24 = m24 + _matrix.m24;
	matrix.m31 = m31 + _matrix.m31; matrix.m32 = m32 + _matrix.m32; matrix.m33 = m33 + _matrix.m33; matrix.m34 = m34 + _matrix.m34;
	matrix.m41 = m41 + _matrix.m41; matrix.m42 = m42 + _matrix.m42; matrix.m43 = m43 + _matrix.m43; matrix.m44 = m44 + _matrix.m44;

	return matrix;
}



Matrix4x4& Matrix4x4::operator += (Matrix4x4 _matrix)
{ 
	(*this) = (*this) + _matrix;
	return *this;
}



Matrix4x4 Matrix4x4::operator - (const Matrix4x4& _matrix)const 
{ 
	Matrix4x4 matrix;
	
	matrix.m11 = m11 - _matrix.m11; matrix.m12 = m12 - _matrix.m12; matrix.m13 = m13 - _matrix.m13; matrix.m14 = m14 - _matrix.m14;
	matrix.m21 = m21 - _matrix.m21; matrix.m22 = m22 - _matrix.m22; matrix.m23 = m23 - _matrix.m23; matrix.m24 = m24 - _matrix.m24;
	matrix.m31 = m31 - _matrix.m31; matrix.m32 = m32 - _matrix.m32; matrix.m33 = m33 - _matrix.m33; matrix.m34 = m34 - _matrix.m34;
	matrix.m41 = m41 - _matrix.m41; matrix.m42 = m42 - _matrix.m42; matrix.m43 = m43 - _matrix.m43; matrix.m44 = m44 - _matrix.m44;

	return matrix;
}



Matrix4x4& Matrix4x4::operator -= (Matrix4x4 _matrix)
{ 
	(*this) = (*this) - _matrix;
	return *this;	
}



void Matrix4x4::operator = (Matrix3x3<Vector3> _matrix)
{
	m11 = _matrix.m11; m12 = _matrix.m12; m13 = _matrix.m13; m14 = 0.0f;
	m21 = _matrix.m21; m22 = _matrix.m22; m23 = _matrix.m23; m24 = 0.0f;
	m31 = _matrix.m31; m32 = _matrix.m32; m33 = _matrix.m33; m34 = 0.0f;
	m41 = 0.0f;        m42 = 0.0f;        m43 = 0.0f;        m44 = 1.0f;
}


bool Matrix4x4::operator == (const Matrix4x4& _matrix)const 
{ 
	if(m11 == _matrix.m11 && m12 == _matrix.m12 && m13 == _matrix.m13 && m14 == _matrix.m14 &&
	   m21 == _matrix.m21 && m22 == _matrix.m22 && m23 == _matrix.m23 && m24 == _matrix.m24 && 
	   m31 == _matrix.m31 && m32 == _matrix.m32 && m33 == _matrix.m33 && m34 == _matrix.m34 &&
	   m41 == _matrix.m41 && m42 == _matrix.m42 && m43 == _matrix.m43 && m44 == _matrix.m44)
	{
		return true;
	}
	return false;
}



bool Matrix4x4::operator != (const Matrix4x4& _matrix)const 
{ 
	return !(*this == _matrix);
}


Matrix4x4 Matrix4x4::operator * (const Matrix4x4& _matrix)const
{
	Matrix4x4 matrix;
	matrix.m11 = m11 * _matrix.m11 + m12 * _matrix.m21 + m13 * _matrix.m31 + m14 * _matrix.m41;
    matrix.m12 = m11 * _matrix.m12 + m12 * _matrix.m22 + m13 * _matrix.m32 + m14 * _matrix.m42;
    matrix.m13 = m11 * _matrix.m13 + m12 * _matrix.m23 + m13 * _matrix.m33 + m14 * _matrix.m43;
    matrix.m14 = m11 * _matrix.m14 + m12 * _matrix.m24 + m13 * _matrix.m34 + m14 * _matrix.m44;
    
	matrix.m21 = m21 * _matrix.m11 + m22 * _matrix.m21 + m23 * _matrix.m31 + m24 * _matrix.m41;
    matrix.m22 = m21 * _matrix.m12 + m22 * _matrix.m22 + m23 * _matrix.m32 + m24 * _matrix.m42;
    matrix.m23 = m21 * _matrix.m13 + m22 * _matrix.m23 + m23 * _matrix.m33 + m24 * _matrix.m43;
    matrix.m24 = m21 * _matrix.m14 + m22 * _matrix.m24 + m23 * _matrix.m34 + m24 * _matrix.m44;

	matrix.m31 = m31 * _matrix.m11 + m32 * _matrix.m21 + m33 * _matrix.m31 + m34 * _matrix.m41;
    matrix.m32 = m31 * _matrix.m12 + m32 * _matrix.m22 + m33 * _matrix.m32 + m34 * _matrix.m42;
    matrix.m33 = m31 * _matrix.m13 + m32 * _matrix.m23 + m33 * _matrix.m33 + m34 * _matrix.m43;
    matrix.m34 = m31 * _matrix.m14 + m32 * _matrix.m24 + m33 * _matrix.m34 + m34 * _matrix.m44;

	matrix.m41 = m41 * _matrix.m11 + m42 * _matrix.m21 + m43 * _matrix.m31 + m44 * _matrix.m41;
    matrix.m42 = m41 * _matrix.m12 + m42 * _matrix.m22 + m43 * _matrix.m32 + m44 * _matrix.m42;
    matrix.m43 = m41 * _matrix.m13 + m42 * _matrix.m23 + m43 * _matrix.m33 + m44 * _matrix.m43;
    matrix.m44 = m41 * _matrix.m14 + m42 * _matrix.m24 + m43 * _matrix.m34 + m44 * _matrix.m44;
	
	return matrix;
}



Matrix4x4& Matrix4x4::operator *= (Matrix4x4 _matrix)
{
	(*this) = (*this) * _matrix;  
	return *this;
}




void Matrix4x4::Scaling(Vector3 _scale)
{
	m11 = _scale.x;  m12 = 0.0f;      m13 = 0.0f;      m14 = 0.0f; 
	m21 = 0.0f;      m22 = _scale.y;  m23 = 0.0f;      m24 = 0.0f; 
	m31 = 0.0f;      m32 = 0.0f;      m33 = _scale.z;  m34 = 0.0f; 
	m41 = 0.0f;      m42 = 0.0f;      m43 = 0.0f;      m44 = 1.0f;
}  
 


void Matrix4x4::Translation(Vector3 _vector)
{
	m11 = 1.0f;   m12 = 0.0f;   m13 = 0.0f;   m14 = _vector.x; 
	m21 = 0.0f;   m22 = 1.0f;   m23 = 0.0f;   m24 = _vector.y; 
	m31 = 0.0f;   m32 = 0.0f;   m33 = 1.0f;   m34 = _vector.z; 
	m41 = 0.0f;   m42 = 0.0f;   m43 = 0.0f;   m44 = 1.0f;
}


void Matrix4x4::RotationX(float _radians)
{
	float cos = cosf(_radians);
    float sin = sinf(_radians);
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f; 
	m21 = 0.0f; m22 = cos;  m23 = -sin; m24 = 0.0f; 
	m31 = 0.0f; m32 = sin;  m33 = cos;  m34 = 0.0f; 
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}



void Matrix4x4::RotationY(float _radians)
{
	float cos = cosf(_radians);
    float sin = sinf(_radians);
	m11 = cos;  m12 = 0.0f; m13 = sin;  m14 = 0.0f; 
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f; 
	m31 = -sin; m32 = 0.0f; m33 = cos;  m34 = 0.0f; 
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
} 



void Matrix4x4::RotationZ(float _radians)
{
	float cos = cosf(_radians);
    float sin = sinf(_radians);
	m11 = cos;  m12 = -sin; m13 = 0.0f; m14 = 0.0f; 
	m21 = sin;  m22 = cos;  m23 = 0.0f; m24 = 0.0f; 
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f; 
	m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
}  

 


void Matrix4x4::FromEulerPitchYawRoll(float _pitch, float _yaw, float _roll)
{
	float cos, sin;
	cos = cosf(_pitch);
    sin = sinf(_pitch);
	Matrix3x3<Vector3> xMatrix(1.0f, 0.0f, 0.0f,
			                   0.0f, cos,  -sin,
					           0.0f, sin,  cos);

	cos = cosf(_yaw);
    sin = sinf(_yaw);
	Matrix3x3<Vector3> yMatrix(cos,  0.0f, sin, 
			                   0.0f, 1.0f, 0.0f,
					           -sin, 0.0f, cos);

	cos = cosf(_roll);
    sin = sinf(_roll);
	Matrix3x3<Vector3> zMatrix(cos,  -sin, 0.0f,
			                   sin,  cos,  0.0f,
					           0.0f, 0.0f, 1.0f);

	*this = xMatrix * (yMatrix * zMatrix);

	m14 = 0.0f;
	m24 = 0.0f;
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}


void Matrix4x4::FromEulerPitchYawRoll(Euler _euler)
{
	float cos, sin;
	cos = cosf(_euler.pitch);
    sin = sinf(_euler.pitch);
	Matrix3x3<Vector3> xMatrix(1.0f, 0.0f, 0.0f,
			                   0.0f, cos,  -sin,
					           0.0f, sin,  cos);

	cos = cosf(_euler.yaw);
    sin = sinf(_euler.yaw);
	Matrix3x3<Vector3> yMatrix(cos,  0.0f, sin, 
			                   0.0f, 1.0f, 0.0f,
					           -sin, 0.0f, cos);

	cos = cosf(_euler.roll);
    sin = sinf(_euler.roll);
	Matrix3x3<Vector3> zMatrix(cos,  -sin, 0.0f,
			                   sin,  cos,  0.0f,
					           0.0f, 0.0f, 1.0f);

	*this = xMatrix * (yMatrix * zMatrix);

	m14 = 0.0f;
	m24 = 0.0f;
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}



Matrix4x4 Matrix4x4::_FromQuaternion(Quaternion _quaternion)
{  
	return Matrix4x4(_quaternion);
}


Quaternion Matrix4x4::_ToQuaternion(Matrix4x4 _matrix)
{
	return _matrix.ToQuaternion();
}


Euler Matrix4x4::ToEulerPitchYawRoll(void)
{ 

	Euler euler;

	euler.yaw = Angle::_ArcSin(m13);
        
	if(euler.yaw < Angle::HALF_PI)
    {
		if(euler.yaw > -Angle::HALF_PI)
        {
			euler.pitch = atan2f(-m23, m33);
            euler.roll = atan2f(-m12, m11);
		}
        else
		{
            float yaw = atan2f(m21, m22);
            euler.roll = 0.0f; 
            euler.pitch = euler.roll - yaw;
        }
	}
	else
    {
        float yaw = atan2f(m21, m22);
		euler.roll = 0.0f; 
		euler.pitch = yaw - euler.roll;
    }
	return euler;
}


AxisAngle Matrix4x4::ToAxisAngle(void)const
{

	AxisAngle angleAxis;

    float fTrace = m11 + m22 + m33;
    float fCos = 0.5f * (fTrace - 1.0f);
	angleAxis.angle = Angle::_ArcCos(fCos);

    if(angleAxis.angle > 0.0f)
	{
		if(angleAxis.angle < Angle::PI)
		{
			angleAxis.axis.x = m32 - m23;
            angleAxis.axis.y = m13 - m31;
            angleAxis.axis.z = m21 - m12;
            angleAxis.axis.Normalize();
        }
		else 
		{
            float fHalfInverse;
                
		    if(m11 >= m22)
            {
                if(m11 >= m33)
                {
					if(angleAxis.axis.x != 0.0f)
					{
						angleAxis.axis.x = 0.5f * sqrtf(m11 - m22 - m33 + 1.0f);
						fHalfInverse = 0.5f / angleAxis.axis.x;
						angleAxis.axis.y = fHalfInverse * m12;
						angleAxis.axis.z = fHalfInverse * m13;
					}
                }
				else
                {
					if(angleAxis.axis.z != 0.0f)
					{
						angleAxis.axis.z = 0.5f * sqrtf(m33 - m11 - m22 + 1.0f);
						fHalfInverse = 0.5f / angleAxis.axis.z;
						angleAxis.axis.x = fHalfInverse * m13;
						angleAxis.axis.y = fHalfInverse * m23;
					}
				}
			}
			else
			{
                if(m22 >= m33)
				{
					if(angleAxis.axis.y != 0.0f)
					{
						angleAxis.axis.y = 0.5f * sqrtf(m22 - m11 - m33 + 1.0f);
						fHalfInverse  = 0.5f / angleAxis.axis.y;
						angleAxis.axis.x = fHalfInverse * m12;
						angleAxis.axis.z = fHalfInverse * m23;
					}
				}
                else
				{
					if(angleAxis.axis.z != 0.0f)
					{
						angleAxis.axis.z = 0.5f * sqrtf(m33 - m11 - m22 + 1.0f);
						fHalfInverse = 0.5f / angleAxis.axis.z;
						angleAxis.axis.x = fHalfInverse * m13;
						angleAxis.axis.y = fHalfInverse * m23;
					}
				}
			}
		}
	}
    else
	{
		angleAxis.axis.x = 1.0;
		angleAxis.axis.y = 0.0;
		angleAxis.axis.z = 0.0;
	}
	return angleAxis;
}


void Matrix4x4::FromAxisAngle(Vector3 _vector, float _radians)
{
	float fCos = cosf(_radians);
    float fSin = sinf(_radians);
    float oneMinusCos = 1.0f - fCos;
    float fXYOneMinusCos = _vector.x * _vector.y * oneMinusCos;
    float fXZOneMinusCos = _vector.x * _vector.z * oneMinusCos;
    float fYZOneMinusCos = _vector.y * _vector.z * oneMinusCos;
    float fXSin = _vector.x * fSin;
    float fYSin = _vector.y * fSin;
    float fZSin = _vector.z * fSin;

    m11 = _vector.x * _vector.x * oneMinusCos + fCos;
    m12 = fXYOneMinusCos - fZSin;
    m13 = fXZOneMinusCos + fYSin;
	m14 = 0.0f;

    m21 = fXYOneMinusCos + fZSin;
    m22 = _vector.y * _vector.y * oneMinusCos + fCos;
    m23 = fYZOneMinusCos - fXSin;
	m24 = 0.0f;

    m31 = fXZOneMinusCos - fYSin;
    m32 = fYZOneMinusCos + fXSin;
    m33 = _vector.z * _vector.z * oneMinusCos + fCos;
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}

Matrix4x4::Matrix4x4(float _pitch, float _yaw, float _roll)
{
	FromEulerPitchYawRoll(_pitch, _yaw, _roll);
}



void Matrix4x4::FromAxisAngle(AxisAngle _axisAngle)
{
	float fCos = cosf(_axisAngle.angle);
    float fSin = sinf(_axisAngle.angle);
    float oneMinusCos = 1.0f - fCos;
    float fXYOneMinusCos = _axisAngle.axis.x * _axisAngle.axis.y * oneMinusCos;
    float fXZOneMinusCos = _axisAngle.axis.x * _axisAngle.axis.z * oneMinusCos;
    float fYZOneMinusCos = _axisAngle.axis.y * _axisAngle.axis.z * oneMinusCos;
    float fXSin = _axisAngle.axis.x * fSin;
    float fYSin = _axisAngle.axis.y * fSin;
    float fZSin = _axisAngle.axis.z * fSin;

    m11 = _axisAngle.axis.x * _axisAngle.axis.x * oneMinusCos + fCos;
    m12 = fXYOneMinusCos - fZSin;
    m13 = fXZOneMinusCos + fYSin;
	m14 = 0.0f;

    m21 = fXYOneMinusCos + fZSin;
    m22 = _axisAngle.axis.y * _axisAngle.axis.y * oneMinusCos + fCos;
    m23 = fYZOneMinusCos - fXSin;
	m24 = 0.0f;

    m31 = fXZOneMinusCos - fYSin;
    m32 = fYZOneMinusCos + fXSin;
    m33 = _axisAngle.axis.z * _axisAngle.axis.z * oneMinusCos + fCos;
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}


Vector3 Matrix4x4::TransformVector(const Vector3& _vector)const
{
	return Vector3((_vector.x * m11 + _vector.y * m12 + _vector.z * m13) + m14,
                   (_vector.x * m21 + _vector.y * m22 + _vector.z * m23) + m24,
                   (_vector.x * m31 + _vector.y * m32 + _vector.z * m33) + m34);
}



Vector3 Matrix4x4::operator * (const Vector3& _vector)const
{
	return Vector3((_vector.x * m11 + _vector.y * m12 + _vector.z * m13) + m14,
                   (_vector.x * m21 + _vector.y * m22 + _vector.z * m23) + m24,
                   (_vector.x * m31 + _vector.y * m32 + _vector.z * m33) + m34);
}


Vector4 Matrix4x4::operator * (const Vector4& _vector)const
{
	return Vector4(m11 * _vector.x + m12 * _vector.y + m13 * _vector.z + m14 * _vector.w, 
                   m21 * _vector.x + m22 * _vector.y + m23 * _vector.z + m24 * _vector.w,
                   m31 * _vector.x + m32 * _vector.y + m33 * _vector.z + m34 * _vector.w,
                   m41 * _vector.x + m42 * _vector.y + m43 * _vector.z + m44 * _vector.w);
}



Matrix4x4 Matrix4x4::_Inverse(Matrix4x4 _matrix)
{
	_matrix.Inverse();
	return _matrix;
}




void Matrix4x4::Inverse(void)
{
	float v0 = m31 * m42 - m32 * m41;
    float v1 = m31 * m43 - m33 * m41;
    float v2 = m31 * m44 - m34 * m41;
    float v3 = m32 * m43 - m33 * m42;
    float v4 = m32 * m44 - m34 * m42;
    float v5 = m33 * m44 - m34 * m43;

    float t00 = + (v5 * m22 - v4 * m23 + v3 * m24);
    float t10 = - (v5 * m21 - v2 * m23 + v1 * m24);
    float t20 = + (v4 * m21 - v2 * m22 + v0 * m24);
    float t30 = - (v3 * m21 - v1 * m22 + v0 * m23);

    float invDet = 1.0f / (t00 * m11 + t10 * m12 + t20 * m13 + t30 * m14);

    float d00 = t00 * invDet;
    float d10 = t10 * invDet;
    float d20 = t20 * invDet;
    float d30 = t30 * invDet;

    float d01 = - (v5 * m12 - v4 * m13 + v3 * m14) * invDet;
    float d11 = + (v5 * m11 - v2 * m13 + v1 * m14) * invDet;
    float d21 = - (v4 * m11 - v2 * m12 + v0 * m14) * invDet;
    float d31 = + (v3 * m11 - v1 * m12 + v0 * m13) * invDet;

    v0 = m21 * m42 - m22 * m41;
    v1 = m21 * m43 - m23 * m41;
    v2 = m21 * m44 - m24 * m41;
    v3 = m22 * m43 - m23 * m42;
    v4 = m22 * m44 - m24 * m42;
    v5 = m23 * m44 - m24 * m43;

    float d02 = + (v5 * m12 - v4 * m13 + v3 * m14) * invDet;
    float d12 = - (v5 * m11 - v2 * m13 + v1 * m14) * invDet;
    float d22 = + (v4 * m11 - v2 * m12 + v0 * m14) * invDet;
    float d32 = - (v3 * m11 - v1 * m12 + v0 * m13) * invDet;

    v0 = m32 * m21 - m31 * m22;
    v1 = m33 * m21 - m31 * m23;
    v2 = m34 * m21 - m31 * m24;
    v3 = m33 * m22 - m32 * m23;
    v4 = m34 * m22 - m32 * m24;
    v5 = m34 * m23 - m33 * m24;

    float d03 = - (v5 * m12 - v4 * m13 + v3 * m14) * invDet;
    float d13 = + (v5 * m11 - v2 * m13 + v1 * m14) * invDet;
    float d23 = - (v4 * m11 - v2 * m12 + v0 * m14) * invDet;
    float d33 = + (v3 * m11 - v1 * m12 + v0 * m13) * invDet;

    (*this) = Matrix4x4(d00, d01, d02, d03,
                        d10, d11, d12, d13,
                        d20, d21, d22, d23,
                        d30, d31, d32, d33); 
}



Quaternion Matrix4x4::ToQuaternion(void)const
{
	float trace = m11 + m22 + m33;
	
	if(trace > 0.0f)
	{
		Quaternion quaternion = Quaternion(m32 - m23, 
			                               m13 - m31, 
							               m21 - m12, 
							               trace + 1.0f);
		
		quaternion *= (0.5f / sqrtf(quaternion.w));
		return quaternion;
	}
	else if((m11 > m22) && (m11 > m33))
	{
		Quaternion quaternion = Quaternion(1.0f + m11 - m22 - m33, 
				                           m12 + m21,
                                           m13 + m31, 
							               m32 - m23);

		quaternion *= (0.5f / sqrtf(quaternion.x));
		return quaternion;
	}
	else if(m22 > m33)
	{
		Quaternion quaternion = Quaternion(m12 + m21, 
				                           1.0f + m22 - m11 - m33,
                                           m23 + m32, 
							               m13 - m31); 

		quaternion *= (0.5f / sqrtf(quaternion.y));
		return quaternion;
	}
	else
	{
		Quaternion quaternion = Quaternion(m13 + m31, 
										   m23 + m32,
										   1.0f + m33 - m11 - m22, 
										   m21 - m12);

		quaternion *= (0.5f / sqrtf(quaternion.z));
		return quaternion;
	}
	return Quaternion();
}



void Matrix4x4::FromQuaternion(Quaternion _quaternion)
{
	float x2 = _quaternion.x + _quaternion.x;
	float y2 = _quaternion.y + _quaternion.y;
	float z2 = _quaternion.z + _quaternion.z;

    float xx = _quaternion.x * x2;
	float xy = _quaternion.x * y2;
	float xz = _quaternion.x * z2;
    float yy = _quaternion.y * y2;
	float yz = _quaternion.y * z2;
	float zz = _quaternion.z * z2;

    float wx = _quaternion.w * x2;
	float wy = _quaternion.w * y2;
	float wz = _quaternion.w * z2;

    m11 = 1.0f - (yy + zz);
	m12 = xy - wz;
	m13 = xz + wy;
	m14 = 0.0f; 

	m21 = xy + wz;
	m22 = 1.0f - (xx + zz);
    m23 = yz - wx;
	m24 = 0.0f; 

	m31 = xz - wy;
	m32 = yz + wx;
	m33 = 1.0f - (xx + yy);
	m34 = 0.0f; 

	m41 = 0.0f; 
	m42 = 0.0f; 
	m43 = 0.0f;
	m44 = 1.0f;
}


bool Matrix4x4::IsScaled(void)const
{

	float norm = m11 * m11 + m21 * m21 + m31 * m31;
	if(norm != 1.0f)
	{
		return true;
	}
            
	norm = m12 * m12 + m22 * m22 + m32 * m32;
    if(norm != 1.0f)
	{
		return true;
	}
            
	norm = m13 * m13 + m23 * m23 + m33 * m33;
	if(norm != 1.0f)
	{
		return true;
	}

	return false;
}



Vector3 Matrix4x4::RotatePointAroundPivot(Vector3 _point, const Vector3& _pivot)const
{
	_point -= _pivot;  
	_point = (*this) * _point;
	_point += _pivot;
	return _point;
}

Matrix4x4 Matrix4x4::_RotateAroundPivot(Matrix4x4 _rotation, Vector3 _pivot)
{
	return Matrix4x4::_Translation(_pivot) * _rotation * Matrix4x4::_Translation(-_pivot);
}



Matrix4x4 Matrix4x4::_Identity(void)
{
	Matrix4x4 matrix;
	matrix.Identity();
	return matrix;
}


Matrix4x4 Matrix4x4::_Transpose(Matrix4x4 _matrix)
{
	_matrix.Transpose();
	return _matrix;
}



Matrix4x4 Matrix4x4::_Scaling(Vector3 _scale)
{
	Matrix4x4 matrix;
	matrix.Scaling(_scale);
	return matrix;
}



Matrix4x4 Matrix4x4::_Translation(Vector3 _vector)
{
	Matrix4x4 matrix; 
	matrix.Translation(_vector);
	return matrix; 
}




Matrix4x4 Matrix4x4::_RotationX(float _radians)
{
	Matrix4x4 matrix;
	matrix.RotationX(_radians);
    return matrix;
}


Matrix4x4 Matrix4x4::_RotationY(float _radians)
{    
	Matrix4x4 matrix;
	matrix.RotationY(_radians);
    return matrix;
} 


    
Matrix4x4 Matrix4x4::_RotationZ(float _radians)
{    
	Matrix4x4 matrix;
    matrix.RotationZ(_radians);
    return matrix;
}  




Vector3 Matrix4x4::_TransformVector(Matrix4x4 _matrix, Vector3 _vector)
{
	return _matrix.TransformVector(_vector);
}



void Matrix4x4::OrthographicProjection(float _xLeft, float _xRight, float _yBottom, float _yTop, float _zNear, float _zFar)
{
	m11 = 2.0f / (_xRight - _xLeft);
	m21 = 0.0f;
	m31 = 0.0f;
	m41 = 0.0f;

	m12 = 0.0f;
	m22 = 2.0f / (_yTop - _yBottom);
	m32 = 0.0f;
	m42 = 0.0f;

	m13 = 0.0f;
	m23 = 0.0f;
	m33 = -2.0f / (_zFar - _zNear);
	m43 = 0.0f;

	m14 = -((_xRight + _xLeft) / (_xRight - _xLeft));
	m24 = -((_yTop + _yBottom) / (_yTop - _yBottom));
	m34 = -((_zFar + _zNear) / (_zFar - _zNear));
	m44 = 1.0f;
}


void Matrix4x4::Frustum(float _xLeft, float _xRight, float _yBottom, float _yTop, float _zNear, float _zFar)
{
	m11 = 2.0f * _zNear / (_xRight - _xLeft);
	m12 = 0.0f;
	m13 = (_xRight + _xLeft) / (_xRight - _xLeft);
	m14 = 0.0f;

	m21 = 0.0f;
	m22 = 2.0f * _zNear / (_yTop - _yBottom);
	m23 = (_yTop + _yBottom) / (_yTop - _yBottom);
	m24 = 0.0f;

	m31 = 0.0f;
	m32 = 0.0f;
	m33 = -((_zFar + _zNear) / (_zFar - _zNear));
	m34 = -2.0f * _zFar * _zNear / (_zFar - _zNear);

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = -1.0f;
	m44 = 0.0f;
}


void Matrix4x4::Perspective1Projection(float _fov, float _aspectRatio, float _zNear, float _zFar)
{
	_fov = _fov * Angle::RADIANS_TO_DEGREES;

	float fovY = tanf(_fov / 360.0f * Angle::PI) * _zNear;
	float fovX = fovY * _aspectRatio;
	Frustum(-fovX, fovX, -fovY, fovY, _zNear, _zFar); 
}



void Matrix4x4::LookAt(Vector3 _from, Vector3 _to, Vector3 _up)
{ 
    Vector3 x, y, z;

	z = Vector3::_FromTo(_from, _to);
    z.Normalize();

    y = _up;

    x = y * z;
	x.Normalize();
    
	y = z * x;
    y.Normalize();

    m11 = x.x;  m21 = x.y;  m31 = x.z;  m41 = 0.0f;
    m12 = y.x;  m22 = y.y;  m32 = y.z;  m42 = 0.0f;
    m13 = z.x;  m23 = z.y;  m33 = z.z;  m43 = 0.0f;
	m14 = _from.x; m24 = _from.y; m34 = _from.z; m44 = 1.0f;
}

