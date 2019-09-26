#ifndef SPHERE3D_H
#define SPHERE3D_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Ray/Ray.h>
class Sphere
{
    public: Vector3 center;
	public: float radius;
	public: Sphere(void);
	public: virtual ~Sphere(void);
	public: Sphere(Vector3 _center, float _radius);
    public: bool IsPointInside(Vector3 _point)const;
	public: bool IsIntersected(Ray _ray)const;
	public: Vector3 ClampPoint(Vector3 _point);
	public: bool IsIntersected(Sphere _sphere);
    public: bool IsIntersected(Line _line, Vector3* _intersectionPoint = NIL);
};






#endif 