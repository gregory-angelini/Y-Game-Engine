#ifndef BOX_H
#define BOX_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Ray/Ray.h>
#include <Engine/Core/Common/Common.h>
class Box
{
	private: enum
	{
		BOTTOM = 1,
		LEFT = 2,
		TOP = 4,
		RIGHT = 8,
		BACK = 16,
		FRONT = 32,
	};
    public: Vector3 leftBottom;
	public: Vector3 rightTop;
	public: Box(void);
	public: virtual ~Box(void);
	public: Box(float _leftBottomX, float _leftBottomY, float _leftBottomZ, float _rightTopX, float _rightTopY, float _rightTopZ);
	public: Vector3 GetLeftBottom(void)const;
	public: Vector3 GetRightTop(void)const;
	public: void SetLeftBottom(const Vector3& _position);
	public: void SetRightTop(const Vector3& _position);
	public: Box(const Vector3& _vertex1, const Vector3& _vertex2, const Vector3& _vertex3, const Vector3& _vertex4,
				const Vector3& _vertex5, const Vector3& _vertex6, const Vector3& _vertex7, const Vector3& _vertex8);
	public: void SetVertex1(const Vector3& _position);
    public: Vector3 GetVertex1(void)const;
	public: void SetVertex2(const Vector3& _position);
    public: Vector3 GetVertex2(void)const;
	public: void SetVertex3(const Vector3& _position);
    public: Vector3 GetVertex3(void)const;
	public: void SetVertex4(const Vector3& _position);
    public: Vector3 GetVertex4(void)const;
	public: void SetVertex5(const Vector3& _position);
    public: Vector3 GetVertex5(void)const;
	public: void SetVertex6(const Vector3& _position);
    public: Vector3 GetVertex6(void)const;
	public: void SetVertex7(const Vector3& _position);
    public: Vector3 GetVertex7(void)const;
	public: void SetVertex8(const Vector3& _position);
    public: Vector3 GetVertex8(void)const;
	public: Box(const Vector3& _center, const Vector3& _size);
	public: void Set(const Vector3& _center, const Vector3& _size);
	public: void SetSize(const Vector3& _size);
	public: Vector3 GetSize(void)const;
	public: void SetCenter(const Vector3& _center);
	public: Vector3 GetCenter(void)const;
	public: float GetWidth(void)const;
	public: float GetHeight(void)const;
	public: float GetLength(void)const;
	public: void SetWidth(float _width);
	public: void SetHeight(float _height);
	public: void SetLength(float _length);
	public: Vector3 GetRandomVector(void)const;
    public: bool IsPointInside(Vector3 _point)const;
	public: bool IsIntersected(Ray _ray)const;
	private: int32 GetCode(Vector3& _point);
	public: Vector3 ClampPoint(Vector3 _point);
	public: bool IsIntersected(Box _box);
	public: bool IsIntersected(Line _line, Line* _intersection);
    public: bool IsIntersected(Line _line, Vector3* _intersectionPoint = NIL);
};




#endif 