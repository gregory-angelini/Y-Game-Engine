#ifndef SCREENQUAD_H
#define SCREENQUAD_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Core/Shape/Quad/Quad.h>
#include <Engine/Math/Matrix/Matrix3x3/Matrix3x3.h>
class ScreenQuad
{
    public: Vector2 leftBottom;
	public: Vector2 rightTop;
	public: ScreenQuad(void);
	public: virtual ~ScreenQuad(void);
	public: void SetLeftBottom(Vector2 _position);
    public: void SetRightTop(Vector2 _position);
    public: Vector2 LeftBottom(void)const;
    public: Vector2 RightTop(void)const;
	public: ScreenQuad(Vector2 _vertex1, Vector2 _vertex2, Vector2 _vertex3, Vector2 _vertex4);
	public: void Set(Vector2 _vertex1, Vector2 _vertex2, Vector2 _vertex3, Vector2 _vertex4);
	public: void SetVertex1(Vector2 _position);
    public: Vector2 Vertex1(void)const;
	public: void SetVertex2(Vector2 _position);
    public: Vector2 Vertex2(void)const;
	public: void SetVertex3(Vector2 _position);
    public: Vector2 Vertex3(void)const;
	public: void SetVertex4(Vector2 _position);
    public: Vector2 Vertex4(void)const;
	public: ScreenQuad(Vector2 _center, Vector2 _size);
	public: void Set(Vector2 _center, Vector2 _size);
	public: void SetSize(Vector2 _size);
	public: Vector2 Size(void)const;
	public: void SetWidth(float _width);
	public: void SetHeight(float _height);
	public: float Width(void)const;
	public: float Height(void)const;
	public: void SetCenter(Vector2 _center);
	public: Vector2 Center(void)const;
    public: bool IsPointInside(Vector2 _point);
	public: Vector2 ClampPoint(Vector2 _point);
};


#endif 