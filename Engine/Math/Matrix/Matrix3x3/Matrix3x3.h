#ifndef MATRIX3X3_H
#define MATRIX3X3_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Matrix/Matrix4x4/Matrix4x4.h>
#include <Engine/Math/Angle/Angle.h>
template<typename VectorType>
class Matrix3x3
{};
template<>
class Matrix3x3<Vector3>
{
	public: float m11, m21, m31,
				  m12, m22, m32,
				  m13, m23, m33;

    public: static const Matrix3x3 IDENTITY;
	public: Matrix3x3(void);
	public: Matrix3x3(float _m11, float _m12, float _m13,
                      float _m21, float _m22, float _m23,
                      float _m31, float _m32, float _m33);
	public: Matrix3x3(Vector3 _x, Vector3 _y, Vector3 _z);
	public: Matrix3x3(AxisAngle _axisAngle);
	public: Matrix3x3(Vector3 _vector, float _radians);
	public: Matrix3x3(Euler _euler);
	public: Matrix3x3(float _pitch, float _yaw, float _roll);
	public: Matrix3x3(Quaternion _quaternion);
	public: void Swap(Matrix3x3 _matrix);
	public: void Set(float _m11, float _m12, float _m13, 
					 float _m21, float _m22, float _m23, 
					 float _m31, float _m32, float _m33);
	public: void SetX(Vector3 _axis);
	public: Vector3 GetX(void)const;
	public: void SetY(Vector3 _axis);
	public: Vector3 GetY(void)const;
	public: void SetZ(Vector3 _axis);
	public: Vector3 GetZ(void)const;
	public: void Identity(void);
	public: void Transpose(void);
	public: Matrix3x3 operator + (const Matrix3x3& _matrix)const;
	public: Matrix3x3& operator += (Matrix3x3 _matrix);
	public: Matrix3x3 operator - (const Matrix3x3& _matrix)const;
	public: Matrix3x3& operator -= (Matrix3x3 _matrix);
	public: Vector3 operator * (const Vector3& _vector)const;
	public: Matrix3x3 operator * (const Matrix3x3& _matrix)const;
	public: Matrix3x3& operator *= (Matrix3x3 _matrix);
	public: bool operator == (const Matrix3x3& _matrix)const;
	public: bool operator != (const Matrix3x3& _matrix)const;
	public: void operator = (Matrix4x4 _matrix);
	public: void LookAt(Vector3 _from, Vector3 _to, Vector3 _up);
	public: static Matrix3x3 _LookAt(Vector3 _from, Vector3 _to, Vector3 _up);
	public: void RotationX(float _radians);
	public: static Matrix3x3 _RotationX(float _radians);
	public: void RotationY(float _radians);
	public: static Matrix3x3 _RotationY(float _radians);
	public: void RotationZ(float _radians);
	public: static Matrix3x3 _RotationZ(float _radians);
	public: void FromQuaternion(Quaternion _quaternion);
	public: static Matrix3x3 _FromQuaternion(Quaternion _quaternion);
	public: Quaternion ToQuaternion(void)const;
	public: static Quaternion _ToQuaternion(Matrix3x3 _matrix);
	public: void FromEulerPitchYawRoll(Euler _euler);
	public: static Matrix3x3 _FromEulerPitchYawRoll(Euler _euler);
	public: void FromEulerPitchYawRoll(float _pitch, float _yaw, float _roll);
	public: Euler ToEulerPitchYawRoll(void);
	public: static Euler _ToEulerPitchYawRoll(Matrix3x3 _matrix);
	public: void FromAxisAngle(AxisAngle _axisAngle);
	public: void FromAxisAngle(Vector3 _vector, float _radians);
	public: static Matrix3x3 _FromAxisAngle(AxisAngle _axisAngle);
	public: AxisAngle ToAxisAngle(void)const;
	public: static AxisAngle _ToAxisAngle(Matrix3x3 _matrix);
	public: static Matrix4x4 _ToMatrix4x4(Matrix3x3 _matrix);
    public: static Matrix3x3 _FromMatrix4x4(Matrix4x4 _matrix);
};
template<>
class Matrix3x3<Vector2>
{
	public: float m11, m21, m31,
				  m12, m22, m32,
				  m13, m23, m33;

    public: static const Matrix3x3 IDENTITY;
	public: Matrix3x3(void);
	public: Matrix3x3(float _m11, float _m12, float _m13,
                      float _m21, float _m22, float _m23,
                      float _m31, float _m32, float _m33);
	public: Matrix3x3(Vector2 _x, Vector2 _y);
	public: Matrix3x3(float _radians);
	public: Matrix3x3(Vector2 _scale, float _rotation, Vector2 _position);
	public: void Swap(Matrix3x3 _matrix);
	public: void Set(float _m11, float _m12, float _m13, 
					 float _m21, float _m22, float _m23, 
					 float _m31, float _m32, float _m33);
	public: void SetX(Vector2 _axis);
	public: Vector2 GetX(void)const;
	public: void SetY(Vector2 _axis);
	public: Vector2 GetY(void)const;
	public: void Identity(void);
	public: void Transpose(void);
	public: Matrix3x3 operator + (const Matrix3x3& _matrix)const;
	public: Matrix3x3& operator += (Matrix3x3 _matrix);
	public: Matrix3x3 operator - (const Matrix3x3& _matrix)const;
	public: Matrix3x3& operator -= (Matrix3x3 _matrix);
	public: Vector2 operator * (const Vector2& _vector)const;
    public: Vector2 TransformVector(const Vector2& _vector)const;
	public: Matrix3x3 operator * (const Matrix3x3& _matrix)const;
	public: Matrix3x3& operator *= (Matrix3x3 _matrix);
	public: bool operator == (const Matrix3x3& _matrix)const;
	public: bool operator != (const Matrix3x3& _matrix)const;
	public: void MakeTransform(Vector2 _scale, float _rotation, Vector2 _position);
	public: void RotationZ(float _radians);
	public: Vector2 RotatePointAroundPivot(Vector2 _point, const Vector2& _pivot)const;
	public: void Translation(Vector2 _vector);
	public: void Scaling(Vector2 _scale);
};


#endif 
