#ifndef CIRCLE_H
#define CIRCLE_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Container/vector/vector.h>
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Core/Shape/Line/Line.h>
class Circle
{
	public: enum
	{
		GET_CIRCLE_LENGTH_MESSAGE = 206,
		GET_RADIUS_FROM_2_POINTS_MESSAGE = 249,
		GET_RADIUS_FROM_3_POINTS_MESSAGE = 250
	};
    public: Vector3 center;
	public: float radius;
	public: Circle(void);
	public: Circle(Vector3 _center, float _radius);
	public: Circle(Vector3 _p1, Vector3 _p2, Vector3 _p3);
	public: Circle(Vector3 _p1, Vector3 _p2);
	public: virtual ~Circle(void);
	public: float GetLength(void)const;
	public: static float _GetLength(float _radius);
	public: static float _GetDiameter(float _radius);
	public: void Set(Vector3 _center, float _radius);
	public: void Set(Vector3 _p1, Vector3 _p2);
	public: bool Set(Vector3 _p1, Vector3 _p2, Vector3 _p3);
    private: bool IsPerpendicular(Vector3 _p1, Vector3 _p2, Vector3 _p3);
    private: bool CalculateCircle(Vector3 _p1, Vector3 _p2, Vector3 _p3);
	public: void Scale(float _scale);
	public: void BuildCircle(int32 _segments, vector<Vector3>& _vertices);
    public: bool IsPointInside(Vector3 _point);
	public: Vector3 ClampPoint(Vector3 _point);
	public: bool IsIntersected(Circle _circle);
    public: bool IsIntersected(Line _line, Vector3* _intersectionPoint = NIL, Vector3 _direction = Vector3::UNIT_Z);
};


#endif 