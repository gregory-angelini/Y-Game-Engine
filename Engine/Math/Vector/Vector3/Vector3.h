#ifndef VECTOR3_H
#define VECTOR3_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Rotation/Spherical/Spherical.h>
class AxisAngle;
class Vector2;
class Vector3
{
	public: enum
	{
		X = 0,
		Y = 1,
		Z = 2
	};
	public: float x, y, z;
	public: static const Vector3 UNIT_X;
    public: static const Vector3 UNIT_Y;
    public: static const Vector3 UNIT_Z;
	public: static const int32 SIZE;
	public: Vector3(void);
    public: Vector3(float _x, float _y, float _z);
	public: Vector3(Spherical _spherical);
	public: inline const float* GetData(void)const
	{
		return &x;
	}
	public: float operator [] (int32 _index)const;
	public: void Set(float _x, float _y, float _z);
	public: float& operator [] (int32 _index);
	public: Vector3 operator * (float _scalar)const;
	public: Vector3& operator *= (float _scalar);
	public: Vector3 operator / (float _scalar)const;
	public: Vector3& operator /= (float _scalar);
	public: Vector3 operator + (const Vector3& _vector)const;
	public: Vector3& operator += (Vector3 _vector);
	public: Vector3 operator - (const Vector3& _vector)const;
	public: Vector3& operator -= (Vector3 _vector);
	public: bool operator == (const Vector3& _vector)const;
	public: bool operator != (const Vector3& _vector)const;
	public: Vector3 operator - (void)const;
	public: void Inverse(void);
	public: static Vector3 _GetInverse(Vector3 _vector);
	public: Vector3 operator * (const Vector3& _vector)const;
	public: Vector3& operator *= (Vector3 _vector);
	public: static Vector3 _GetCross(Vector3 _vector1, Vector3 _vector2);
	public: float GetDot(const Vector3& _vector)const;
	public: static float _GetDot(Vector3 _vector1, Vector3 _vector2);
	public: float operator / (const Vector3& _vector)const;
	public: float GetNorm(void)const;
	public: float GetLength(void)const;
	public: static float _GetLength(Vector3 _vector);
    public: void Normalize(void);
    public: static Vector3 _GetNormalize(Vector3 _vector);
	public: static Vector3 _FromTo(Vector3 _from, Vector3 _to);
	public: static float _GetAngle(Vector3 _vector1, Vector3 _vector2);
	public: void RotateX(float _angle);
	public: static Vector3 _RotateX(Vector3 _vector, float _angle);
	public: void RotateY(float _angle);
    public: static Vector3 _RotateY(Vector3 _vector, float _angle);
	public: void RotateZ(float _angle);
	public: static Vector3 _RotateZ(Vector3 _vector, float _angle);
    public: void RotateAxisAngle(AxisAngle _axisAngle);
	public: float GetAngleZ(void)const;
	public: static float _GetAngleZ(Vector3 _vector);
	public: float GetAngleX(void)const;
	public: static float _GetAngleX(Vector3 _vector);
	public: float GetAngleY(void)const;
	public: static float _GetAngleY(Vector3 _vector);
	public: Vector3 Project(Vector3 _vector);
	public: Vector3 Reject(Vector3 _vector);
	public: void FromSpherical(Spherical _spherical);
	public: static Vector3 _FromSpherical(Spherical _spherical);
	public: Spherical ToSpherical(void)const;
	public: static Spherical _ToSpherical(Vector3 _vector);
	public: Vector2 ToVector2(void);
	public: static Vector2 _ToVector2(Vector3 _vector);
	public: static Vector3 _FromVector2(Vector2 _vector); 
};



#endif 
