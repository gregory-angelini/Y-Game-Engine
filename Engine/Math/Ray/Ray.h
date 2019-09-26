#ifndef RAY_H
#define RAY_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Shape/Line/Line.h>
class Ray
{
	public: Vector3 origin;
	public: Vector3 direction;
	public: Ray(void);
	public: Ray(Vector3 _origin, Vector3 _direction);
	public: Ray(Line _line);
	public: void Set(Vector3 _origin, Vector3 _direction);
	public: Vector3 GetPoint(float _t);
	public: static bool _IsIntersected(Ray _a, Ray _b, Vector3* _intersection = NIL);
	private: static bool _IsIntersected(Ray _a, Ray _b, float& _t);
	public: Line ToLine(float _length);
	public: void FromLine(Line _line);
};



#endif 
