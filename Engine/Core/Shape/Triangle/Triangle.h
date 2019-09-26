#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Core/Shape/Line/Line.h>
#include <Engine/Math/Matrix/Matrix4x4/Matrix4x4.h>
#include <Engine/Core/Surface/AbstractSurface/AbstractSurface.h>
class Triangle
{
	public: Vector3 v1, v2, v3;
	public: static const int32 SIZE;
	public: Triangle(void);
	public: virtual ~Triangle(void);
	public: Triangle(Vector3 _v1, Vector3 _v2, Vector3 _v3);
	public: const float* GetData(void)const;
	public: bool IsClockwise(void);
	public: Vector3 GetCenter(void)const;
	public: static Vector3 _GetCenter(Vector3 _v1, Vector3 _v2, Vector3 _v3);
	public: Vector3 GetNormal(void)const;
	public: static Vector3 _GetNormal(Vector3 _v1, Vector3 _v2, Vector3 _v3);
	public: void Transform(Matrix4x4 _transformation);
	public: static Triangle _Transform(Triangle _triangle, Matrix4x4 _transformation);
	public: void InverseVertices(void);
	public: bool IsIntersected(Line _line, int32 _mode = AbstractSurface::CHECK_ONLY_FRONT_FACE, Vector3* _intersectionPoint = NIL);
	public: bool IsPointInside(Vector3 _point);
	public: bool IsIntersected(Triangle _triangle);
};




#endif 