#ifndef QUAD_H
#define QUAD_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Core/Shape/Line/Line.h>
class Quad
{
    public: Vector3 leftBottom;
	public: Vector3 rightTop;
	public: Quad(void);
	public: virtual ~Quad(void);
	public: Quad(Vector3 _leftBottom, Vector3 _rightTop);
	public: void Set(Vector3 _leftBottom, Vector3 _rightTop);
	public: void SetLeftBottom(Vector3 _position);
    public: void SetRightTop(Vector3 _position);
    public: Vector3 GetLeftBottom(void)const;
    public: Vector3 GetRightTop(void)const;
	public: Quad(Vector3 _vertex1, Vector3 _vertex2, Vector3 _vertex3, Vector3 _vertex4);
	public: void Set(Vector3 _vertex1, Vector3 _vertex2, Vector3 _vertex3, Vector3 _vertex4);
	public: void SetVertex1(Vector3 _position);
    public: Vector3 GetVertex1(void)const;
	public: void SetVertex2(Vector3 _position);
    public: Vector3 GetVertex2(void)const;
	public: void SetVertex3(Vector3 _position);
    public: Vector3 GetVertex3(void)const;
	public: void SetVertex4(Vector3 _position);
    public: Vector3 GetVertex4(void)const;
	public: Quad(Vector3 _center, Vector2 _size);
	public: void Set(Vector3 _center, Vector2 _size);
	public: void SetSize(Vector2 _size);
	public: Vector2 GetSize(void)const;
	public: void SetWidth(float _width);
	public: void SetHeight(float _height);
	public: float GetWidth(void)const;
	public: float GetHeight(void)const;
	public: void SetCenter(Vector3 _center);
	public: Vector3 GetCenter(void)const;
    public: bool IsPointInside(Vector3 _point);
	public: Vector3 ClampPoint(Vector3 _point);
	public: bool IsIntersected(Quad _quad, Quad* _intersection = NIL);
    public: bool IsIntersected(Line _line, Vector3* _intersectionPoint = NIL);
};


#endif 