
#include "Matrix3x3.h"
#include <Engine/Core/Common/Common.h>



const Matrix3x3<Vector3> Matrix3x3<Vector3>::IDENTITY(1.0f, 0.0f, 0.0f, 
                                                      0.0f, 1.0f, 0.0f, 
                                                      0.0f, 0.0f, 1.0f);


Matrix3x3<Vector3>::Matrix3x3(void)
{
	Identity();
}


Matrix3x3<Vector3>::Matrix3x3(float _m11, float _m12, float _m13,
                              float _m21, float _m22, float _m23,
                              float _m31, float _m32, float _m33)
{
    m11 = _m11; m12 = _m12; m13 = _m13; 
    m21 = _m21; m22 = _m22; m23 = _m23; 
    m31 = _m31; m32 = _m32; m33 = _m33; 
}


void Matrix3x3<Vector3>::Set(float _m11, float _m12, float _m13, 
					         float _m21, float _m22, float _m23, 
		                     float _m31, float _m32, float _m33)
{
	m11 = _m11; m12 = _m12; m13 = _m13; 
    m21 = _m21; m22 = _m22; m23 = _m23; 
    m31 = _m31; m32 = _m32; m33 = _m33; 
}



Matrix3x3<Vector3>::Matrix3x3(Quaternion _quaternion)
{
	FromQuaternion(_quaternion);
}

Matrix3x3<Vector3>::Matrix3x3(Vector3 _vector, float _radians)
{
	FromAxisAngle(_vector, _radians);
}


Matrix3x3<Vector3>::Matrix3x3(float _pitch, float _yaw, float _roll)
{
	FromEulerPitchYawRoll(_pitch, _yaw, _roll);
}


Quaternion Matrix3x3<Vector3>::ToQuaternion(void)const
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



void Matrix3x3<Vector3>::FromQuaternion(Quaternion _quaternion)
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

	m21 = xy + wz;
	m22 = 1.0f - (xx + zz);
    m23 = yz - wx;

	m31 = xz - wy;
	m32 = yz + wx;
	m33 = 1.0f - (xx + yy);
}

Matrix3x3<Vector3>::Matrix3x3(AxisAngle _axisAngle)
{
	FromAxisAngle(_axisAngle);
}



Matrix3x3<Vector3>::Matrix3x3(Euler _euler)
{
	FromEulerPitchYawRoll(_euler);
}




Matrix3x3<Vector3>::Matrix3x3(Vector3 _x, Vector3 _y, Vector3 _z)
{
	SetX(_x);
	SetY(_y);
	SetZ(_z);
}

    



void Matrix3x3<Vector3>::Identity(void)
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; 
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; 
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; 
}


void Matrix3x3<Vector3>::Transpose(void)
{
	float memory; 
	memory = m12; m12 = m21; m21 = memory;
    memory = m13; m13 = m31; m31 = memory;
	memory = m23; m23 = m32; m32 = memory;
}


 
 
AxisAngle Matrix3x3<Vector3>::ToAxisAngle(void)const
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


void Matrix3x3<Vector3>::FromAxisAngle(AxisAngle _axisAngle)
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

    m21 = fXYOneMinusCos + fZSin;
    m22 = _axisAngle.axis.y * _axisAngle.axis.y * oneMinusCos + fCos;
    m23 = fYZOneMinusCos - fXSin;

    m31 = fXZOneMinusCos - fYSin;
    m32 = fYZOneMinusCos + fXSin;
    m33 = _axisAngle.axis.z * _axisAngle.axis.z * oneMinusCos + fCos;
}


void Matrix3x3<Vector3>::FromAxisAngle(Vector3 _vector, float _radians)
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

    m21 = fXYOneMinusCos + fZSin;
    m22 = _vector.y * _vector.y * oneMinusCos + fCos;
    m23 = fYZOneMinusCos - fXSin;

    m31 = fXZOneMinusCos - fYSin;
    m32 = fYZOneMinusCos + fXSin;
    m33 = _vector.z * _vector.z * oneMinusCos + fCos;
}


Vector3 Matrix3x3<Vector3>::operator * (const Vector3& _vector)const
{
	return Vector3((_vector.x * m11 + _vector.y * m12 + _vector.z * m13),
                    (_vector.x * m21 + _vector.y * m22 + _vector.z * m23),
                    (_vector.x * m31 + _vector.y * m32 + _vector.z * m33));
}




Matrix3x3<Vector3> Matrix3x3<Vector3>::operator * (const Matrix3x3<Vector3>& _matrix)const
{
	Matrix3x3 matrix;
	matrix.m11 = m11 * _matrix.m11 + m12 * _matrix.m21 + m13 * _matrix.m31;
    matrix.m12 = m11 * _matrix.m12 + m12 * _matrix.m22 + m13 * _matrix.m32;
    matrix.m13 = m11 * _matrix.m13 + m12 * _matrix.m23 + m13 * _matrix.m33;
  
	matrix.m21 = m21 * _matrix.m11 + m22 * _matrix.m21 + m23 * _matrix.m31;
    matrix.m22 = m21 * _matrix.m12 + m22 * _matrix.m22 + m23 * _matrix.m32;
    matrix.m23 = m21 * _matrix.m13 + m22 * _matrix.m23 + m23 * _matrix.m33;  

	matrix.m31 = m31 * _matrix.m11 + m32 * _matrix.m21 + m33 * _matrix.m31;
    matrix.m32 = m31 * _matrix.m12 + m32 * _matrix.m22 + m33 * _matrix.m32;
    matrix.m33 = m31 * _matrix.m13 + m32 * _matrix.m23 + m33 * _matrix.m33;

	return matrix;
}



Matrix3x3<Vector3>& Matrix3x3<Vector3>::operator *= (Matrix3x3<Vector3> _matrix)
{
	(*this) = (*this) * _matrix;  
	return *this;
}




bool Matrix3x3<Vector3>::operator == (const Matrix3x3<Vector3>& _matrix)const 
{
	if(m11 == _matrix.m11 && m12 == _matrix.m12 && m13 == _matrix.m13 && 
	   m21 == _matrix.m21 && m22 == _matrix.m22 && m23 == _matrix.m23 && 
	   m31 == _matrix.m31 && m32 == _matrix.m32 && m33 == _matrix.m33)
	{
		return true;
	}
	return false;
}


Matrix3x3<Vector3> Matrix3x3<Vector3>::_LookAt(Vector3 _from, Vector3 _to, Vector3 _up)
{
	Matrix3x3 matrix;
	matrix.LookAt(_from, _to, _up);
	return matrix;
}


void Matrix3x3<Vector3>::LookAt(Vector3 _from, Vector3 _to, Vector3 _up)
{ 
    Vector3 x, y, z;

	z = Vector3::_FromTo(_from, _to);
    z.Normalize();

    y = _up;

    x = y * z;
	x.Normalize();
    
	y = z * x;
    y.Normalize();

    m11 = x.x;  m21 = x.y;  m31 = x.z;
    m12 = y.x;  m22 = y.y;  m32 = y.z;
    m13 = z.x;  m23 = z.y;  m33 = z.z;
}


void Matrix3x3<Vector3>::RotationX(float _radians)
{
	float cos = cosf(_radians);
    float sin = sinf(_radians);
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
	m21 = 0.0f; m22 = cos;  m23 = -sin;
	m31 = 0.0f; m32 = sin;  m33 = cos;
}

void Matrix3x3<Vector3>::RotationY(float _radians)
{
	float cos = cosf(_radians);
    float sin = sinf(_radians);
	m11 = cos;  m12 = 0.0f; m13 = sin;
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
	m31 = -sin; m32 = 0.0f; m33 = cos;
} 



void Matrix3x3<Vector3>::RotationZ(float _radians)
{
	float cos = cosf(_radians);
    float sin = sinf(_radians);
	m11 = cos;  m12 = -sin; m13 = 0.0f;
	m21 = sin;  m22 = cos;  m23 = 0.0f;
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
}  



Matrix3x3<Vector3> Matrix3x3<Vector3>::_FromAxisAngle(AxisAngle _axisAngle)
{
	return Matrix3x3<Vector3>(_axisAngle);
}


void Matrix3x3<Vector3>::FromEulerPitchYawRoll(float _pitch, float _yaw, float _roll)
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
}


void Matrix3x3<Vector3>::FromEulerPitchYawRoll(Euler _euler)
{
	float cos, sin;
	cos = cosf(_euler.pitch);
    sin = sinf(_euler.pitch);
	Matrix3x3 xMatrix(1.0f, 0.0f, 0.0f,
			          0.0f, cos,  -sin,
					  0.0f, sin,  cos);

	cos = cosf(_euler.yaw);
    sin = sinf(_euler.yaw);
	Matrix3x3 yMatrix(cos,  0.0f, sin, 
			          0.0f, 1.0f, 0.0f,
					  -sin, 0.0f, cos);

	cos = cosf(_euler.roll);
    sin = sinf(_euler.roll);
	Matrix3x3 zMatrix(cos,  -sin, 0.0f,
			          sin,  cos,  0.0f,
					  0.0f, 0.0f, 1.0f);

	*this = xMatrix * (yMatrix * zMatrix);
}


Quaternion Matrix3x3<Vector3>::_ToQuaternion(Matrix3x3<Vector3> _matrix)
{
	return _matrix.ToQuaternion();
}


Matrix3x3<Vector3> Matrix3x3<Vector3>::_FromQuaternion(Quaternion _quaternion)
{
	return Matrix3x3<Vector3>(_quaternion);
}


Euler Matrix3x3<Vector3>::ToEulerPitchYawRoll(void)
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

void Matrix3x3<Vector3>::operator = (Matrix4x4 _matrix)
{
	m11 = _matrix.m11; m12 = _matrix.m12; m13 = _matrix.m13;
    m21 = _matrix.m21; m22 = _matrix.m22; m23 = _matrix.m23;
    m31 = _matrix.m31; m32 = _matrix.m32; m33 = _matrix.m33;
}


AxisAngle Matrix3x3<Vector3>::_ToAxisAngle(Matrix3x3<Vector3> _matrix)
{
	return _matrix.ToAxisAngle();
}

Euler Matrix3x3<Vector3>::_ToEulerPitchYawRoll(Matrix3x3<Vector3> _matrix)
{
	return _matrix.ToEulerPitchYawRoll();
}


Matrix3x3<Vector3> Matrix3x3<Vector3>::_FromEulerPitchYawRoll(Euler _euler)
{
	return Matrix3x3(_euler);
}



void Matrix3x3<Vector3>::SetX(Vector3 _axis)
{
	m11 = _axis.x;
	m21 = _axis.y;
	m31 = _axis.z;
}


Vector3 Matrix3x3<Vector3>::GetX(void)const
{
	return Vector3(m11, m21, m31);
}


void Matrix3x3<Vector3>::SetY(Vector3 _axis)
{
	m12 = _axis.x;
	m22 = _axis.y;
	m32 = _axis.z;
}


Vector3 Matrix3x3<Vector3>::GetY(void)const
{
	return Vector3(m12, m22, m32);
}


void Matrix3x3<Vector3>::SetZ(Vector3 _axis)
{
	m13 = _axis.x;
	m23 = _axis.y;
	m33 = _axis.z;
}


Vector3 Matrix3x3<Vector3>::GetZ(void)const
{
	return Vector3(m13, m23, m33);
}




bool Matrix3x3<Vector3>::operator != (const Matrix3x3<Vector3>& _matrix)const 
{ 
	return !(*this == _matrix);
}






Matrix3x3<Vector3> Matrix3x3<Vector3>::_RotationX(float _radians)
{
	Matrix3x3<Vector3> matrix;
	matrix.RotationX(_radians);
	return matrix;
}

Matrix3x3<Vector3> Matrix3x3<Vector3>::_RotationY(float _radians)
{
	Matrix3x3<Vector3> matrix;
	matrix.RotationY(_radians);
	return matrix;
}

Matrix3x3<Vector3> Matrix3x3<Vector3>::_RotationZ(float _radians)
{
	Matrix3x3<Vector3> matrix;
	matrix.RotationZ(_radians);
	return matrix;
}


Matrix4x4 Matrix3x3<Vector3>::_ToMatrix4x4(Matrix3x3<Vector3> _matrix)
{
	Matrix4x4 matrix;

	matrix.m11 = _matrix.m11; matrix.m12 = _matrix.m12; matrix.m13 = _matrix.m13; matrix.m14 = 0.0f;
	matrix.m21 = _matrix.m21; matrix.m22 = _matrix.m22; matrix.m23 = _matrix.m23; matrix.m24 = 0.0f;
	matrix.m31 = _matrix.m31; matrix.m32 = _matrix.m32; matrix.m33 = _matrix.m33; matrix.m34 = 0.0f;
	matrix.m41 = 0.0f;        matrix.m42 = 0.0f;        matrix.m43 = 0.0f;        matrix.m44 = 1.0f;
	
	return matrix;
}

Matrix3x3<Vector3> Matrix3x3<Vector3>::_FromMatrix4x4(Matrix4x4 _matrix)
{
	Matrix3x3<Vector3> matrix;

	matrix.m11 = _matrix.m11; matrix.m12 = _matrix.m12; matrix.m13 = _matrix.m13; 
	matrix.m21 = _matrix.m21; matrix.m22 = _matrix.m22; matrix.m23 = _matrix.m23; 
	matrix.m31 = _matrix.m31; matrix.m32 = _matrix.m32; matrix.m33 = _matrix.m33; 

	return matrix;
}


    
Matrix3x3<Vector3> Matrix3x3<Vector3>::operator + (const Matrix3x3<Vector3>& _matrix)const
{
	Matrix3x3<Vector3> matrix;

	matrix.m11 = m11 + _matrix.m11; matrix.m12 = m12 + _matrix.m12; matrix.m13 = m13 + _matrix.m13; 
	matrix.m21 = m21 + _matrix.m21; matrix.m22 = m22 + _matrix.m22; matrix.m23 = m23 + _matrix.m23; 
	matrix.m31 = m31 + _matrix.m31; matrix.m32 = m32 + _matrix.m32; matrix.m33 = m33 + _matrix.m33;

	return matrix;
}


Matrix3x3<Vector3>& Matrix3x3<Vector3>::operator += (Matrix3x3<Vector3> _matrix)
{ 
	(*this) = (*this) + _matrix;
	return *this;
}



Matrix3x3<Vector3> Matrix3x3<Vector3>::operator - (const Matrix3x3<Vector3>& _matrix)const 
{ 
	Matrix3x3<Vector3> matrix;

	matrix.m11 = m11 - _matrix.m11; matrix.m12 = m12 - _matrix.m12; matrix.m13 = m13 - _matrix.m13; 
	matrix.m21 = m21 - _matrix.m21; matrix.m22 = m22 - _matrix.m22; matrix.m23 = m23 - _matrix.m23; 
	matrix.m31 = m31 - _matrix.m31; matrix.m32 = m32 - _matrix.m32; matrix.m33 = m33 - _matrix.m33;

	return matrix;
}



Matrix3x3<Vector3>& Matrix3x3<Vector3>::operator -= (Matrix3x3<Vector3> _matrix)
{ 
	(*this) = (*this) - _matrix;
	return *this;	
}
