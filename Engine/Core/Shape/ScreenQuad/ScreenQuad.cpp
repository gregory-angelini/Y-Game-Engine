#include "ScreenQuad.h"


ScreenQuad::ScreenQuad(void)
{
	Set(Vector2(), Vector2(1.0f, 1.0f));
}



ScreenQuad::~ScreenQuad(void)
{}



void ScreenQuad::SetLeftBottom(Vector2 _position)
{
	leftBottom = _position;
}	


void ScreenQuad::SetRightTop(Vector2 _position)
{
	rightTop = _position;
}
				


Vector2 ScreenQuad::LeftBottom(void)const
{
	return leftBottom;
}



Vector2 ScreenQuad::RightTop(void)const
{
	return rightTop;
}
 
	

ScreenQuad::ScreenQuad(Vector2 _vertex1, Vector2 _vertex2, Vector2 _vertex3, Vector2 _vertex4)
{
	SetVertex1(_vertex1);
	SetVertex2(_vertex2);
	SetVertex3(_vertex3);
	SetVertex4(_vertex4);
}



void ScreenQuad::Set(Vector2 _vertex1, Vector2 _vertex2, Vector2 _vertex3, Vector2 _vertex4)
{
	SetVertex1(_vertex1);
	SetVertex2(_vertex2);
	SetVertex3(_vertex3);
	SetVertex4(_vertex4);
}


void ScreenQuad::SetVertex1(Vector2 _position)
{
	leftBottom = _position;
}	
				

Vector2 ScreenQuad::Vertex1(void)const
{
	return leftBottom;
}



void ScreenQuad::SetVertex2(Vector2 _position)
{
	rightTop.x = _position.x;
	leftBottom.y = _position.y;
}
		


Vector2 ScreenQuad::Vertex2(void)const
{
	Vector2 vertex;
	vertex.x = rightTop.x;
	vertex.y = leftBottom.y;
	return vertex;
}

		 

void ScreenQuad::SetVertex3(Vector2 _position)
{
	rightTop = _position; 
}

			

Vector2 ScreenQuad::Vertex3(void)const
{
	return rightTop;
}



void ScreenQuad::SetVertex4(Vector2 _position)
{
	leftBottom.x = _position.x;
	rightTop.y = _position.y;
}
	
			

Vector2 ScreenQuad::Vertex4(void)const
{
	Vector2 vertex;
	vertex.x = leftBottom.x;
	vertex.y = rightTop.y;
	return vertex;
}



ScreenQuad::ScreenQuad(Vector2 _center, Vector2 _size) 
{
	Set(_center, _size);
}
	 

void ScreenQuad::Set(Vector2 _center, Vector2 _size)
{
	leftBottom.x = _center.x - (_size.x / 2.0f);
	leftBottom.y = _center.y + (_size.y / 2.0f);
	rightTop.x = leftBottom.x + _size.x;
	rightTop.y = leftBottom.y - _size.y;
}



void ScreenQuad::SetSize(Vector2 _size)
{ 
	Set(Center(), _size);
}



Vector2 ScreenQuad::Size(void)const
{
	return Vector2(fabsf(rightTop.x - leftBottom.x), fabsf(rightTop.y - leftBottom.y));
} 



void ScreenQuad::SetWidth(float _width)
{
	SetSize(Vector2(_width, Size().y));
}



void ScreenQuad::SetHeight(float _height)
{
	SetSize(Vector2(Size().x, _height));
}



float ScreenQuad::Width(void)const
{
	return Size().x;
}




float ScreenQuad::Height(void)const
{
	return Size().y;
}



void ScreenQuad::SetCenter(Vector2 _center)
{ 
	Set(_center, Size());
}



Vector2 ScreenQuad::Center(void)const
{
	Vector2 center;
	center.x = leftBottom.x + (Size().x / 2.0f);
	center.y = leftBottom.y - (Size().y / 2.0f);
	return center;
}

	 


bool ScreenQuad::IsPointInside(Vector2 _point)
{
	if(_point.x >= leftBottom.x && _point.x <= rightTop.x && _point.y <= leftBottom.y && _point.y >= rightTop.y) 
	{
		return true;
	}

	return false;
}



Vector2 ScreenQuad::ClampPoint(Vector2 _point)
{
	if(_point.x < leftBottom.x)
	{
		_point.x = leftBottom.x;
	}
	else if(_point.x > rightTop.x)
	{
		_point.x = rightTop.x;
	}
	if(_point.y < rightTop.y)
	{
		_point.y = rightTop.y;
	}
	else if(_point.y > leftBottom.y)
	{
		_point.y = leftBottom.y;
	}

	return _point;
}