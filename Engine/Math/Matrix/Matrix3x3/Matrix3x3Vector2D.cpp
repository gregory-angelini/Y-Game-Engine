
#include "Matrix3x3.h"
#include <Engine/Core/Common/Common.h>


const Matrix3x3<Vector2> Matrix3x3<Vector2>::IDENTITY(1.0f, 0.0f, 0.0f, 
                                                      0.0f, 1.0f, 0.0f, 
                                                      0.0f, 0.0f, 1.0f);


Matrix3x3<Vector2>::Matrix3x3(float _radians)
{
	RotationZ(_radians);
}

Matrix3x3<Vector2>::Matrix3x3(void)
{
	Identity();
}


Matrix3x3<Vector2>::Matrix3x3(float _m11, float _m12, float _m13,
                              float _m21, float _m22, float _m23,
                              float _m31, float _m32, float _m33)
{
    m11 = _m11; m12 = _m12; m13 = _m13; 
    m21 = _m21; m22 = _m22; m23 = _m23; 
    m31 = _m31; m32 = _m32; m33 = _m33; 
}


Matrix3x3<Vector2>::Matrix3x3(Vector2 _x, Vector2 _y)
{
	SetX(_x);
	SetY(_y);
}


Matrix3x3<Vector2>::Matrix3x3(Vector2 _scale, float _rotation, Vector2 _position)
{
	MakeTransform(_scale, _rotation, _position);
}


void Matrix3x3<Vector2>::Set(float _m11, float _m12, float _m13, 
					         float _m21, float _m22, float _m23, 
		                     float _m31, float _m32, float _m33)
{
	m11 = _m11; m12 = _m12; m13 = _m13; 
    m21 = _m21; m22 = _m22; m23 = _m23; 
    m31 = _m31; m32 = _m32; m33 = _m33; 
}


void Matrix3x3<Vector2>::Swap(Matrix3x3 _matrix)
{
	Common::_Swap(m11, _matrix.m11);
    Common::_Swap(m12, _matrix.m12);
    Common::_Swap(m13, _matrix.m13);

    Common::_Swap(m21, _matrix.m21);
    Common::_Swap(m22, _matrix.m22);
    Common::_Swap(m23, _matrix.m23);

    Common::_Swap(m31, _matrix.m31);
    Common::_Swap(m32, _matrix.m32);
    Common::_Swap(m33, _matrix.m33);
}


void Matrix3x3<Vector2>::SetX(Vector2 _axis)
{
	m11 = _axis.x;
	m21 = _axis.y;
}


Vector2 Matrix3x3<Vector2>::GetX(void)const
{
	return Vector2(m11, m21);
}


void Matrix3x3<Vector2>::SetY(Vector2 _axis)
{
	m12 = _axis.x;
	m22 = _axis.y;
}


Vector2 Matrix3x3<Vector2>::GetY(void)const
{
	return Vector2(m12, m22);
}



void Matrix3x3<Vector2>::Identity(void)
{
	m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; 
	m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; 
	m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; 
}


void Matrix3x3<Vector2>::Transpose(void)
{
	float memory; 
	memory = m12; m12 = m21; m21 = memory;
    memory = m13; m13 = m31; m31 = memory;
	memory = m23; m23 = m32; m32 = memory;
}


void Matrix3x3<Vector2>::RotationZ(float _radians)
{
	m11 = cosf(_radians); 
	m12 = -sinf(_radians);
	m13 = 0.0f;

	m21 = sinf(_radians);
	m22 = cosf(_radians);
	m23 = 0.0f;

	m31 = 0.0f;
	m32 = 0.0f;
	m33 = 1.0f;
}


void Matrix3x3<Vector2>::Translation(Vector2 _vector)
{
	m11 = 1.0f;   m12 = 0.0f;   m13 = _vector.x; 
	m21 = 0.0f;   m22 = 1.0f;   m23 = _vector.y; 
	m31 = 0.0f;   m32 = 0.0f;   m33 = 1.0f;  
}


void Matrix3x3<Vector2>::Scaling(Vector2 _scale)
{
	m11 = _scale.x;  m12 = 0.0f;      m13 = 0.0f;
	m21 = 0.0f;      m22 = _scale.y;  m23 = 0.0f;
	m31 = 0.0f;      m32 = 0.0f;      m33 = 1.0f;
}


Matrix3x3<Vector2> Matrix3x3<Vector2>::operator + (const Matrix3x3<Vector2>& _matrix)const
{
	Matrix3x3<Vector2> matrix;

	matrix.m11 = m11 + _matrix.m11; matrix.m12 = m12 + _matrix.m12; matrix.m13 = m13 + _matrix.m13; 
	matrix.m21 = m21 + _matrix.m21; matrix.m22 = m22 + _matrix.m22; matrix.m23 = m23 + _matrix.m23; 
	matrix.m31 = m31 + _matrix.m31; matrix.m32 = m32 + _matrix.m32; matrix.m33 = m33 + _matrix.m33;

	return matrix;
}


Matrix3x3<Vector2>& Matrix3x3<Vector2>::operator += (Matrix3x3<Vector2> _matrix)
{ 
	(*this) = (*this) + _matrix;
	return *this;
}



Matrix3x3<Vector2> Matrix3x3<Vector2>::operator - (const Matrix3x3<Vector2>& _matrix)const 
{ 
	Matrix3x3<Vector2> matrix;

	matrix.m11 = m11 - _matrix.m11; matrix.m12 = m12 - _matrix.m12; matrix.m13 = m13 - _matrix.m13; 
	matrix.m21 = m21 - _matrix.m21; matrix.m22 = m22 - _matrix.m22; matrix.m23 = m23 - _matrix.m23; 
	matrix.m31 = m31 - _matrix.m31; matrix.m32 = m32 - _matrix.m32; matrix.m33 = m33 - _matrix.m33;

	return matrix;
}



Matrix3x3<Vector2>& Matrix3x3<Vector2>::operator -= (Matrix3x3<Vector2> _matrix)
{ 
	(*this) = (*this) - _matrix;
	return *this;	
}


Vector2 Matrix3x3<Vector2>::TransformVector(const Vector2& _vector)const
{
	return Vector2((_vector.x * m11 + _vector.y * m12) + m13,
                   (_vector.x * m21 + _vector.y * m22) + m23);
}


Vector2 Matrix3x3<Vector2>::operator * (const Vector2& _vector)const
{
	return Vector2(_vector.x * m11 + _vector.y * m12,
                   _vector.x * m21 + _vector.y * m22);
}




Matrix3x3<Vector2> Matrix3x3<Vector2>::operator * (const Matrix3x3<Vector2>& _matrix)const
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



Matrix3x3<Vector2>& Matrix3x3<Vector2>::operator *= (Matrix3x3<Vector2> _matrix)
{
	(*this) = (*this) * _matrix;  
	return *this;
}


bool Matrix3x3<Vector2>::operator == (const Matrix3x3<Vector2>& _matrix)const 
{ 
	if(m11 == _matrix.m11 && m12 == _matrix.m12 && m13 == _matrix.m13 && 
	   m21 == _matrix.m21 && m22 == _matrix.m22 && m23 == _matrix.m23 && 
	   m31 == _matrix.m31 && m32 == _matrix.m32 && m33 == _matrix.m33)
	{
		return true;
	}
	return false;
}



bool Matrix3x3<Vector2>::operator != (const Matrix3x3<Vector2>& _matrix)const 
{ 
	return !(*this == _matrix);
}


void Matrix3x3<Vector2>::MakeTransform(Vector2 _scale, float _rotation, Vector2 _position)
{
	Matrix3x3<Vector2> rotation(_rotation);

    m11 = _scale.x * rotation.m11; m12 = _scale.y * rotation.m12; m13 = _position.x;
    m21 = _scale.x * rotation.m21; m22 = _scale.y * rotation.m22; m23 = _position.y;
    m31 = 0.0f;                    m32 = 0.0f;                    m33 = 1.0f;
}


Vector2 Matrix3x3<Vector2>::RotatePointAroundPivot(Vector2 _point, const Vector2& _pivot)const
{
	_point -= _pivot;  
	_point = (*this) * _point;
	_point += _pivot;
	return _point;
}
