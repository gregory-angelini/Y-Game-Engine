#ifndef MATRIX4X4_H
#define MATRIX4X4_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Rotation/Euler/Euler.h>
#include <Engine/Math/Quaternion/Quaternion.h>
#include <Engine/Math/Vector/Vector4/Vector4.h>
template<typename VectorType> class Matrix3x3;
class Matrix4x4
{
	public: float m11, m21, m31, m41,
			      m12, m22, m32, m42,
				  m13, m23, m33, m43,
				  m14, m24, m34, m44;

	public: static const Matrix4x4 IDENTITY;
	public: Matrix4x4(void);
	public: Matrix4x4(float _m11, float _m12, float _m13, float _m14,
                      float _m21, float _m22, float _m23, float _m24,
                      float _m31, float _m32, float _m33, float _m34,
                      float _m41, float _m42, float _m43, float _m44);
	public: Matrix4x4(Vector3 _from, Vector3 _to, Vector3 _up);
    public: Matrix4x4(Vector3 _x, Vector3 _y, Vector3 _z, Vector3 _w);
	public: Matrix4x4(Quaternion _quaternion);
	public: Matrix4x4(AxisAngle _axisAngle);
	public: Matrix4x4(Vector3 _vector, float _radians);
	public: Matrix4x4(Euler _euler);
	public: Matrix4x4(float _pitch, float _yaw, float _roll);
	public: Matrix4x4(Vector3 _scale, Quaternion _rotation, Vector3 _position);
	public: Matrix4x4(Matrix3x3<Vector2> _matrix);
	public: void Swap(Matrix4x4 _matrix);
	public: void Set(float _m11, float _m12, float _m13, float _m14,
					 float _m21, float _m22, float _m23, float _m24,
					 float _m31, float _m32, float _m33, float _m34,
					 float _m41, float _m42, float _m43, float _m44);
	public: inline const float* GetData(void)const
	{
		return &m11;
	}
	public: void SetX(Vector3 _axis);
	public: Vector3 GetX(void)const;
	public: void SetY(Vector3 _axis);
	public: Vector3 GetY(void)const;
	public: void SetZ(Vector3 _axis);
	public: Vector3 GetZ(void)const;
	public: void SetW(Vector3 _axis);
	public: Vector3 GetW(void)const;
	public: void Identity(void);
	public: static Matrix4x4 _Identity(void);
	public: void Transpose(void);
	public: static Matrix4x4 _Transpose(Matrix4x4 _matrix);
	public: Vector3 operator * (const Vector3& _vector)const;
    public: Vector3 TransformVector(const Vector3& _vector)const;
    public: static Vector3 _TransformVector(Matrix4x4 _matrix, Vector3 _vector);
	Vector4 operator * (const Vector4& _vector)const;
	public: Matrix4x4 operator + (const Matrix4x4& _matrix)const;
	public: Matrix4x4& operator += (Matrix4x4 _matrix);
	public: Matrix4x4 operator - (const Matrix4x4& _matrix)const;
	public: Matrix4x4& operator -= (Matrix4x4 _matrix);
	public: Matrix4x4 operator * (const Matrix4x4& _matrix)const;
	public: Matrix4x4& operator *= (Matrix4x4 _matrix);
	public: bool operator == (const Matrix4x4& _matrix)const;
	public: bool operator != (const Matrix4x4& _matrix)const;
	public: void operator = (Matrix3x3<Vector3> _matrix);
	public: void MakeTransform(Vector3 _scale, Quaternion _rotation, Vector3 _position);
	public: void MakeTransform(Vector2 _scale, float _rotation, Vector2 _position);
	public: void Scaling(Vector3 _scale);
    public: static Matrix4x4 _Scaling(Vector3 _scale);
	public: bool IsScaled(void)const;
	public: void Translation(Vector3 _vector);
	public: static Matrix4x4 _Translation(Vector3 _vector);
	public: void RotationX(float _radians);
	public: static Matrix4x4 _RotationX(float _radians);
	public: void RotationY(float _radians);
	public: static Matrix4x4 _RotationY(float _radians);
	public: void RotationZ(float _radians);
	public: static Matrix4x4 _RotationZ(float _radians);
	public: Vector3 RotatePointAroundPivot(Vector3 _point, const Vector3& _pivot)const;
	public: static Matrix4x4 _RotateAroundPivot(Matrix4x4 _rotation, Vector3 _pivot);
	public: void Inverse(void);
	public: static Matrix4x4 _Inverse(Matrix4x4 _matrix);
	public: void InvertTranslation(void);
	public: static Matrix4x4 _InvertTranslation(Matrix4x4 _translation);
	public: void FromQuaternion(Quaternion _quaternion);
	public: static Matrix4x4 _FromQuaternion(Quaternion _quaternion);
	public: Quaternion ToQuaternion(void)const;
	public: static Quaternion _ToQuaternion(Matrix4x4 _matrix);
	public: void FromEulerPitchYawRoll(Euler _euler);
	public: static Matrix4x4 _FromEulerPitchYawRoll(Euler _euler);
	public: void FromEulerPitchYawRoll(float _pitch, float _yaw, float _roll);
	public: Euler ToEulerPitchYawRoll(void);
	public: static Euler _ToEulerPitchYawRoll(Matrix4x4 _matrix);
	public: void FromAxisAngle(AxisAngle _axisAngle);
	public: void FromAxisAngle(Vector3 _vector, float _radians);
	public: static Matrix4x4 _FromAxisAngle(AxisAngle _axisAngle);
	public: AxisAngle ToAxisAngle(void)const;
	public: static AxisAngle _ToAxisAngle(Matrix4x4 _matrix);
	public: void LookAt(Vector3 _from, Vector3 _to, Vector3 _up);
	public: void OrthographicProjection(float _xLeft, float _xRight, float _yBottom, float _yTop, float _zNear, float _zFar);
	public: void Frustum(float _xLeft, float _xRight, float _yBottom, float _yTop, float _zNear, float _zFar);
	public: void Perspective1Projection(float _fov, float _aspectRatio, float _zNear, float _zFar);
};


#endif 
