#ifndef QUATERNION_H
#define QUATERNION_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Rotation/AxisAngle/AxisAngle.h>
#include <Engine/Math/Rotation/Euler/Euler.h>
class Matrix4x4; 
template<typename VectorType> class Matrix3x3;
class Quaternion
{
	public: float x, y, z;
	public: float w;
	public: static const Quaternion IDENTITY;
	public: Quaternion(void);
	public: Quaternion(float _x, float _y, float _z, float _w);
    public: Quaternion(const Matrix4x4& _matrix);
    public: Quaternion(const Matrix3x3<Vector3>& _matrix);
	public: Quaternion(const AxisAngle& _axisAngle);
	public: Quaternion(const Euler& _euler);
	public: Quaternion(const Quaternion& _from, const Quaternion& _to);
	public: Quaternion(const Vector3& _from, const Vector3& _to);
	public: float& operator [] (int32 _index);
	public: float operator [] (int32 _index)const;
	public: void Identity(void);
	public: static Quaternion _GetIdentity(void);
    public: void Normalize(void);
    public: static Quaternion _GetNormalize(Quaternion _quaternion);
	public: Quaternion operator * (const Quaternion& _quaternion)const;
	public: Quaternion& operator *= (Quaternion _quaternion);
	public: Quaternion operator + (const Quaternion& _quaternion)const;
	public: Quaternion& operator += (Quaternion _quaternion);
	public: Quaternion operator - (const Quaternion& _quaternion)const;
	public: Quaternion& operator -= (Quaternion _quaternion);
	public: bool operator == (const Quaternion& _quaternion)const;
	public: bool operator != (const Quaternion& _quaternion)const;
	public: Quaternion operator * (float _scalar)const;
	public: Quaternion& operator *= (float _scalar);
	public: Quaternion operator / (float _scalar)const;
	public: Quaternion& operator /= (float _scalar);
	public: Quaternion operator - (void)const;
	public: float GetNorm(void)const;
	public: static float _GetNorm(const Quaternion& _quaternion);
	public: float GetAngle(void)const;
	public: float GetDot(const Quaternion& _quaternion)const;
    public: static float _GetDot(const Quaternion& _q1, const Quaternion& _q2);
	public: float GetLength(void)const;
	public: static float _GetLength(const Quaternion& _quaternion);
	private: void StabilizeLength(void);
	public: void Conjugate(void);
	public: static Quaternion _GetConjugate(Quaternion _quaternion);
    public: void Inverse(void);
    public: static Quaternion _GetInverse(Quaternion _quaternion);
	public: Vector3 operator * (const Vector3& _vector)const;
    public: Vector3 RotateVector(const Vector3& _vector)const;
	public: Vector3 RotatePointAroundPivot(Vector3 _point, const Vector3& _pivot)const;
	public: static Vector3 _RotatePointAroundPivot(const Quaternion& _rotation, Vector3 _point, const Vector3& _pivot);
	public: void ShortestArc(const Vector3& _from, const Vector3& _to);
	public: void FromTo(const Quaternion& _from, const Quaternion& _to);
    public: static bool _IsEquals(const Quaternion& _q1, const Quaternion& _q2, float _delta);
    public: Matrix4x4 ToMatrix4x4(void)const;
    public: void FromMatrix4x4(const Matrix4x4& _matrix);
    public: Matrix3x3<Vector3> ToMatrix3x3(void)const;
    public: void FromMatrix3x3(const Matrix3x3<Vector3>& _matrix);
	public: Vector3 GetAxisX(void)const;
	public: Vector3 GetAxisY(void)const;
	public: Vector3 GetAxisZ(void)const;
	public: void ToAxisXYZ(Vector3& _axisX, Vector3& _axisY, Vector3& _axisZ);
    public: AxisAngle ToAxisAngle(void)const;
	public: void FromAxisAngle(const AxisAngle& _axisAngle);
	public: void FromEulerPitchYawRoll(const Euler& _euler);
	public: Euler ToShortestEulerPitchYawRoll(void)const;
	public: void SwingTwistDecomposition(const Vector3& _direction, Quaternion& _swing, Quaternion& _twist);
};




#endif 
