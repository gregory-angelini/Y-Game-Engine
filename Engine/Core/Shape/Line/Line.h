#ifndef LINE_H
#define LINE_H
#include <Engine/Math/Vector/Vector2/Vector2.h>  
#include <Engine/Math/Vector/Vector3/Vector3.h>  
#include <Engine/Math/Matrix/Matrix4x4/Matrix4x4.h>
class Ray;
class Line
{
	public: enum
	{
		GET_LINE_ANGLE_Z_MESSAGE = 195,
		GET_LINE_LENGTH_MESSAGE = 207,
		GET_LINE_DIRECTION_MESSAGE = 236
	};
	public: enum 
	{
		LEFT,
		RIGHT,
		BEYOND,
		BEHIND,
		BETWEEN,
		ORIGIN,
		END
	};
	public: Vector3 origin;
    public: Vector3 end;
	public: Line(void);
	public: virtual ~Line(void);
    public: Line(Vector3 _origin, Vector3 _end);
	public: void Set(Vector3 _origin, Vector3 _end);
	public: Vector3 GetCenter(void)const;
	public: static Vector3 _GetCenter(Vector3 _origin, Vector3 _end);
	public: void MoveCenterTo(Vector3 _position);
	public: Vector3 GetDirection(void)const;
	public: static Vector3 _GetDirection(Vector3 _origin, Vector3 _end);
	public: static float _GetAngleZ(Vector2 _origin, Vector2 _end);
	public: static float _GetAngleZ(Vector3 _origin, Vector3 _end);
	public: void Rotate(void);
	public: float GetLength(void)const;
	public: static float _GetLength(Vector3 _origin, Vector3 _end);
	public: static float _GetLength(Vector2 _origin, Vector2 _end);
	public: void SetLength(float _length);
	public: void SetLengthFromEndToOrigin(float _length);
	public: void SetLengthFromOriginToEnd(float _length);
	public: void Scale(float _scale);
	public: void Transform(Matrix4x4 _transformation);
	public: static Line _Transform(Line _line, Matrix4x4 _transformation);
	public: static int32 _GetVertexPosition(Line _line, Vector3 _point);
	public: static bool _IsIntersected(Line _a, Line _b, Vector3* _intersection = NIL);
	private: static bool _IsIntersectedEx(Line _a, Line _b, float& _t);
	private: static bool _IsIntersected(Line _a, Line _b, float& _t);
	public: Ray ToRay(void)const;
	public: void FromRay(Ray _ray, float _length);
};





#endif 