#include "Quad.h"
#include <Engine/Core/Common/Common.h>
#include <Engine/Core/Shape/Plane/Plane.h>




Quad::Quad(void)
{
	Set(Vector3(), Vector2(1.0f, 1.0f));
}


Quad::~Quad(void)
{}


Quad::Quad(Vector3 _leftBottom, Vector3 _rightTop)
{
	Set(_leftBottom, _rightTop);
}


void Quad::Set(Vector3 _leftBottom, Vector3 _rightTop)
{
	leftBottom = _leftBottom;
	rightTop = _rightTop;
}


void Quad::SetLeftBottom(Vector3 _position)
{
	leftBottom = _position;
}	


void Quad::SetRightTop(Vector3 _position)
{
	rightTop = _position;
}
				

Vector3 Quad::GetLeftBottom(void)const
{
	return leftBottom;
}


Vector3 Quad::GetRightTop(void)const
{
	return rightTop;
}
 
	
Quad::Quad(Vector3 _vertex1, Vector3 _vertex2, Vector3 _vertex3, Vector3 _vertex4)
{
	SetVertex1(_vertex1);
	SetVertex2(_vertex2);
	SetVertex3(_vertex3);
	SetVertex4(_vertex4);
}


void Quad::Set(Vector3 _vertex1, Vector3 _vertex2, Vector3 _vertex3, Vector3 _vertex4)
{
	SetVertex1(_vertex1);
	SetVertex2(_vertex2);
	SetVertex3(_vertex3);
	SetVertex4(_vertex4);
}

void Quad::SetVertex1(Vector3 _position)
{
	leftBottom = _position;
}	
				

Vector3 Quad::GetVertex1(void)const
{
	return leftBottom;
}



void Quad::SetVertex2(Vector3 _position)
{
	rightTop.x = _position.x;
	rightTop.z = _position.z;

	leftBottom.y = _position.y;
	leftBottom.z = _position.z;
}
		

Vector3 Quad::GetVertex2(void)const
{
	return Vector3(rightTop.x, leftBottom.y, leftBottom.z);
}

		 
void Quad::SetVertex3(Vector3 _position)
{
	rightTop = _position; 
}

			
Vector3 Quad::GetVertex3(void)const
{
	return rightTop;
}

void Quad::SetVertex4(Vector3 _position)
{
	leftBottom.x = _position.x;
	leftBottom.z = _position.z;

	rightTop.y = _position.y;
	rightTop.z = _position.z;
}
	
			
Vector3 Quad::GetVertex4(void)const
{
	return Vector3(leftBottom.x, rightTop.y, rightTop.z);
}


Quad::Quad(Vector3 _center, Vector2 _size) 
{
	Set(_center, _size);
}
	 

void Quad::Set(Vector3 _center, Vector2 _size)
{
	leftBottom.x = _center.x - (_size.x / 2.0f);
	leftBottom.y = _center.y - (_size.y / 2.0f);
	leftBottom.z = _center.z;
	rightTop.x = leftBottom.x + _size.x;
	rightTop.y = leftBottom.y + _size.y;
	rightTop.z = _center.z;
}


void Quad::SetSize(Vector2 _size)
{ 
	Set(GetCenter(), _size);
}


Vector2 Quad::GetSize(void)const
{
	return Vector2(fabsf(rightTop.x - leftBottom.x), fabsf(rightTop.y - leftBottom.y));
} 


void Quad::SetWidth(float _width)
{
	SetSize(Vector2(_width, GetSize().y));
}


void Quad::SetHeight(float _height)
{
	SetSize(Vector2(GetSize().x, _height));
}


float Quad::GetWidth(void)const
{
	return GetSize().x;
}


float Quad::GetHeight(void)const
{
	return GetSize().y;
}


void Quad::SetCenter(Vector3 _center)
{ 
	Set(_center, GetSize());
}


Vector3 Quad::GetCenter(void)const
{
	return Vector3(leftBottom.x + (GetSize().x / 2.0f), leftBottom.y + (GetSize().y / 2.0f), leftBottom.z);
}



bool Quad::IsIntersected(Line _line, Vector3* _intersectionPoint)
{
	Vector3 point;

	if(Plane::_IsIntersected(GetVertex1(), GetVertex2(), GetVertex3(), _line, 0, &point) != -1)
	{
		if(IsPointInside(point))
		{
			if(_intersectionPoint) { *_intersectionPoint = point; } 
			return true;
		}
	}
	return false;
}



bool Quad::IsPointInside(Vector3 _point)
{
	int32 collision = 0;
 
	if(leftBottom.x > rightTop.x)
	{
		if(_point.x >= rightTop.x && _point.x <= leftBottom.x) 
		{
			collision++;
		}
	}
	else
	{
		if(_point.x >= leftBottom.x && _point.x <= rightTop.x) 
		{
			collision++;
		}
	}

	if(leftBottom.y > rightTop.y)
	{
		if(_point.y >= rightTop.y && _point.y <= leftBottom.y) 
		{
			collision++;
		}
	}
	else
	{
		if(_point.y >= leftBottom.y && _point.y <= rightTop.y) 
		{
			collision++;
		}
	}

	if(collision == 2) 
	{ 
		return true; 
	} 
	return false;
}


Vector3 Quad::ClampPoint(Vector3 _point)
{
	if(_point.x < leftBottom.x)
	{
		_point.x = leftBottom.x;
	}
	else if(_point.x > rightTop.x)
	{
		_point.x = rightTop.x;
	}
	if(_point.y > rightTop.y)
	{
		_point.y = rightTop.y;
	}
	else if(_point.y < leftBottom.y)
	{
		_point.y = leftBottom.y;
	}

	return _point;
}


bool Quad::IsIntersected(Quad _quad, Quad* _intersection)
{
	if(leftBottom.y > _quad.rightTop.y || rightTop.y < _quad.leftBottom.y || leftBottom.x > _quad.rightTop.x || rightTop.x < _quad.leftBottom.x)
    {
        return false;
    }
    else
    {
		if(_intersection)
		{
			_intersection->rightTop.x = Numerical<float>::_GetMin(rightTop.x, _quad.rightTop.x);
			_intersection->rightTop.y = Numerical<float>::_GetMin(rightTop.y,_quad.rightTop.y);
			_intersection->rightTop.z = rightTop.z;
			
			_intersection->leftBottom.x = Numerical<float>::_GetMax(leftBottom.x,_quad.leftBottom.x);
			_intersection->leftBottom.y = Numerical<float>::_GetMax(leftBottom.y,_quad.leftBottom.y);
			_intersection->leftBottom.z = leftBottom.z;
		}
		return true;
    } 
	return false;
}