#ifndef PLANE_H
#define PLANE_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Core/Shape/Line/Line.h>
#include <Engine/Core/Surface/AbstractSurface/AbstractSurface.h>
#include <Engine/Core/Object/TranslatableObject/TranslatableObject.h>
class Plane: public virtual TranslatableObject,
	         public virtual AbstractSurface
{
	public: enum: int64 
	{
		PLANE_CLASS = 2097152
	};
	public: enum
	{
		CHANGE_PLANE_NORMAL_MESSAGE = 80
	};
	public: enum
	{
		XY_PLANE,
		XZ_PLANE,
		YZ_PLANE
	};
	private: Vector3 position;
	private: Vector3 normal;
	public: Plane(void);
	public: Plane(Vector3 _p0, Vector3 _normal);
	public: Plane(Vector3 _p1, Vector3 _p2, Vector3 _p3);
	public: virtual ~Plane(void);
	public: void Set(Vector3 _p0, Vector3 _normal);
	public: void Set(Vector3 _p1, Vector3 _p2, Vector3 _p3);
	public: float DistanceTo(Vector3 _point);
	public: static float _DistanceTo(Vector3 _p1, Vector3 _p2, Vector3 _p3, Vector3 _point);
	public: static float _DistanceTo(Vector3 _p0, Vector3 _normal, Vector3 _point);
	public: bool operator == (Plane& _plane);
	public: Vector3 GetNormal(void)const;
	public: static Vector3 _GetNormal(Vector3 _p1, Vector3 _p2, Vector3 _p3);
	public: void SetNormal(Vector3 _normal);
	public: virtual int32 IsIntersected(Line _line, Vector3* _intersectionPoint = NIL);
	public: static int32 _IsIntersected(Vector3 _p1, Vector3 _p2, Vector3 _p3, Line _line, int32 _intersectionMode = AbstractSurface::CHECK_ONLY_FRONT_FACE, Vector3* _intersectionPoint = NIL);
	public: static int32 _IsIntersected(Vector3 _p0, Vector3 _normal, Line _line, int32 _intersectionMode = AbstractSurface::CHECK_ONLY_FRONT_FACE, Vector3* _intersectionPoint = NIL);
	public: virtual void SetPosition(Vector3 _position);
	public: virtual void DisplacePosition(Vector3 _offset);
	public: virtual void MoveTo(Vector3 _direction, float _units);
	public: virtual Vector3 GetPosition(void);
	public: virtual Vector3 GetAbsolutePosition(void);
	public: using AbstractObject::SaveToFile;
	public: virtual bool SaveToFile(File& _file);
	protected: void SaveDerivedToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	public: using AbstractObject::LoadFromFile;
	protected: void LoadDerivedFromFile(File& _file);
	protected: virtual bool LoadFromFile(File& _file);
};



#endif 
