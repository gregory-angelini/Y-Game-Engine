#ifndef VECTOR4_H
#define VECTOR4_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
class Vector4
{
	public: enum
	{
		X = 0,
		Y = 1,
		Z = 2,
		W = 3
	};
	public: float x, y, z, w;
	public: static const int32 SIZE;
	public: Vector4(void);
    public: Vector4(float _x, float _y, float _z, float _w);
    public: Vector4(Vector3 _vector, float _w);
	public: inline const float* GetData(void)const
	{
		return &x;
	}
	public: float operator [] (int32 _index)const;
	public: void Set(float _x, float _y, float _z, float _w);
	public: void Set(Vector3 _vector, float _w);
	public: float& operator [] (int32 _index);
	public: bool operator == (const Vector4& _vector)const;
	public: bool operator != (const Vector4& _vector)const;
};



#endif 
