#ifndef VECTOR2_H
#define VECTOR2_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class Vector2
{
	public: enum
	{
		X = 0,
		Y = 1
	};
	public: float x, y;
	public: static const Vector2 UNIT_X;
    public: static const Vector2 UNIT_Y;
	public: static const int32 SIZE;
	public: Vector2(void);
    public: Vector2(float _x, float _y);
	public: void Set(float _x, float _y);
	public: float& operator [] (int32 _index);
	public: float operator [] (int32 _index)const;
	public: inline const float* GetData(void)const
	{
		return &x;
	}
	public: Vector2 operator - (const Vector2& _vector)const;
	public: Vector2& operator -= (Vector2 _vector);
	public: Vector2 operator + (const Vector2& _vector)const;
	public: Vector2& operator += (Vector2 _vector);
	public: bool operator == (const Vector2& _vector)const;
	public: bool operator != (const Vector2& _vector)const;
	public: Vector2 operator / (float _scalar)const;
	public: Vector2& operator /= (float _scalar);
	public: Vector2 operator * (float _scalar)const;
	public: Vector2& operator *= (float _scalar);
	public: Vector2 operator - (void)const;
	public: void Inverse(void);
	public: static Vector2 _GetInverse(Vector2 _vector);
	public: float GetDot(const Vector2& _vector)const;
	public: static float _GetDot(Vector2 _vector1, Vector2 _vector2);
	public: float operator / (const Vector2& _vector)const;
	public: void Cross(void);
	public: static Vector2 _GetCross(Vector2 _vector);
	public: float GetLength(void)const;
	public: static float _GetLength(Vector2 _vector);
	public: void Normalize(void);
	public: static Vector2 _GetNormalize(Vector2 _vector);
	public: static Vector2 _FromTo(Vector2 _from, Vector2 _to);
	public: static float _GetAngle(Vector2 _vector1, Vector2 _vector2);
	public: float GetAngleZ(void)const;
	public: static float _GetAngleZ(Vector2 _vector);
	public: void Rotate(float _angle);
	public: static Vector2 _Rotate(Vector2 _vector, float _angle);
	public: static Vector2 _RotateAroundPivot(Vector2 _point, Vector2 _pivot, float _radians);
	public: void FromPolar(Vector2 _polar);
	public: static Vector2 _FromPolar(Vector2 _polar);
	public: Vector2 ToPolar(void)const;
	public: static Vector2 _ToPolar(Vector2 _vector);
	public: Vector3 ToVector3(void);
	public: static Vector3 _ToVector3(Vector2 _vector);
	public: static Vector2 _FromVector3(Vector3 _vector);
};



#endif 
