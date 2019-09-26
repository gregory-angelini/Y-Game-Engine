#include "Box.h"
#include <Engine/Core/Random/Random.h>


Box::Box(void)
{}


Box::~Box(void)
{}



Box::Box(float _leftBottomX, float _leftBottomY, float _leftBottomZ, float _rightTopX, float _rightTopY, float _rightTopZ)
{
	leftBottom.x = _leftBottomX;
	leftBottom.y = _leftBottomY;
	leftBottom.z = _leftBottomZ;

	rightTop.x = _rightTopX;
	rightTop.y = _rightTopY;
	rightTop.z = _rightTopZ;
}


Vector3 Box::GetLeftBottom(void)const
{
	return leftBottom;
}



Vector3 Box::GetRightTop(void)const
{
	return rightTop;
}


void Box::SetLeftBottom(const Vector3& _position)
{
	leftBottom = _position;
}


void Box::SetRightTop(const Vector3& _position)
{
	rightTop = _position;
}



Box::Box(const Vector3& _vertex1, const Vector3& _vertex2, const Vector3& _vertex3, const Vector3& _vertex4,
		 const Vector3& _vertex5, const Vector3& _vertex6, const Vector3& _vertex7, const Vector3& _vertex8)
{
	SetVertex1(_vertex1);
	SetVertex2(_vertex2);
	SetVertex3(_vertex3);
	SetVertex4(_vertex4);
	SetVertex5(_vertex5);
	SetVertex6(_vertex6);
	SetVertex7(_vertex7);
	SetVertex8(_vertex8);
}



void Box::SetVertex1(const Vector3& _position)
{
	leftBottom.x = _position.x;
	rightTop.y = _position.y;
	rightTop.z = _position.z;
}
			

Vector3 Box::GetVertex1(void)const
{
	return Vector3(leftBottom.x, rightTop.y, rightTop.z);
}


void Box::SetVertex2(const Vector3& _position)
{
	rightTop.x = _position.x;
	rightTop.y = _position.y;
	rightTop.z = _position.z;
}
			

Vector3 Box::GetVertex2(void)const
{
	return Vector3(rightTop.x, rightTop.y, rightTop.z);
}


void Box::SetVertex3(const Vector3& _position)
{
	rightTop.x = _position.x;
	leftBottom.y = _position.y;
	rightTop.z = _position.z;
}
			

Vector3 Box::GetVertex3(void)const
{
	return Vector3(rightTop.x, leftBottom.y, rightTop.z);
}

void Box::SetVertex4(const Vector3& _position)
{
	leftBottom.x = _position.x;
	leftBottom.y = _position.y;
	rightTop.z = _position.z;
}
			

Vector3 Box::GetVertex4(void)const
{
	return Vector3(leftBottom.x, leftBottom.y, rightTop.z);
}


void Box::SetVertex5(const Vector3& _position)
{
	leftBottom.x = _position.x;
	rightTop.y = _position.y;
	leftBottom.z = _position.z;
}
			

Vector3 Box::GetVertex5(void)const
{
	return Vector3(leftBottom.x, rightTop.y, leftBottom.z);
}
 

void Box::SetVertex6(const Vector3& _position)
{
	rightTop.x = _position.x;
	rightTop.y = _position.y;
	leftBottom.z = _position.z;
}
			

Vector3 Box::GetVertex6(void)const
{
	return Vector3(rightTop.x, rightTop.y, leftBottom.z);
}


void Box::SetVertex7(const Vector3& _position)
{
	rightTop.x = _position.x;
	leftBottom.y = _position.y;
	leftBottom.z = _position.z; 
}
			

Vector3 Box::GetVertex7(void)const
{ 
	return Vector3(rightTop.x, leftBottom.y, leftBottom.z);
}


void Box::SetVertex8(const Vector3& _position)
{
	leftBottom.x = _position.x;
	leftBottom.y = _position.y;
	leftBottom.z = _position.z; 
}
			

Vector3 Box::GetVertex8(void)const
{ 
	return Vector3(leftBottom.x, leftBottom.y, leftBottom.z);
}



Box::Box(const Vector3& _center, const Vector3& _size)
{
	Set(_center, _size);
}

 
void Box::Set(const Vector3& _center, const Vector3& _size)
{
	leftBottom.x = _center.x - (_size.x / 2.0f);
	leftBottom.y = _center.y - (_size.y / 2.0f);
	leftBottom.z = _center.z - (_size.z / 2.0f);
	rightTop.x = leftBottom.x + _size.x;
	rightTop.y = leftBottom.y + _size.y;
	rightTop.z = leftBottom.z + _size.z;
}



void Box::SetSize(const Vector3& _size)
{
	Set(GetCenter(), _size);
}
	

Vector3 Box::GetSize(void)const
{
	return Vector3(fabsf(rightTop.x - leftBottom.x), fabsf(rightTop.y - leftBottom.y), fabsf(rightTop.z - leftBottom.z));
} 




void Box::SetCenter(const Vector3& _center)
{ 
	Set(_center, GetSize());
}



Vector3 Box::GetCenter(void)const
{
	return Vector3(leftBottom.x + (GetSize().x / 2.0f), 
		           leftBottom.y + (GetSize().y / 2.0f),
				   leftBottom.z + (GetSize().z / 2.0f)); 
}




float Box::GetWidth(void)const
{
	 return GetSize().x;	    
}



float Box::GetHeight(void)const
{
	return GetSize().y;
}


float Box::GetLength(void)const
{
	return GetSize().z;
}



void Box::SetWidth(float _width)
{
	SetSize(Vector3(_width, GetSize().y, GetSize().z));
}


void Box::SetHeight(float _height)
{
	SetSize(Vector3(GetSize().x, _height, GetSize().z));
}


void Box::SetLength(float _length)
{
	SetSize(Vector3(GetSize().x, GetSize().y, _length));
}



bool Box::IsIntersected(Line _line, Vector3* _intersectedPoint)
{ 
	
	bool accept = false, done = false;
	int32 outcode0 = GetCode(_line.origin); 
	int32 outcode1 = GetCode(_line.end);
	
	do
	{
		float x, y, z;

		if (!(outcode0 | outcode1))
		{
			accept = true;
			done = true;
		}
		else if(outcode0 & outcode1)
		{
			done = true;
		}
		else
		{
			int32 outcode = outcode0 ? outcode0 : outcode1;
			
			if(outcode & TOP) 
			{
				x = _line.origin.x + (_line.end.x - _line.origin.x) * (GetVertex5().y - _line.origin.y) / (_line.end.y - _line.origin.y);
				z = _line.origin.z + (_line.end.z - _line.origin.z) * (GetVertex5().y - _line.origin.y) / (_line.end.y - _line.origin.y);
				y = GetVertex5().y;
			}
			else if(outcode & BOTTOM) 
			{
				x = _line.origin.x + (_line.end.x - _line.origin.x) * (GetVertex3().y - _line.origin.y) / (_line.end.y - _line.origin.y);
				z = _line.origin.z + (_line.end.z - _line.origin.z) * (GetVertex3().y - _line.origin.y) / (_line.end.y - _line.origin.y);
				y = GetVertex3().y;
			}
			else if(outcode & RIGHT) 
			{
				y = _line.origin.y + (_line.end.y - _line.origin.y) * (GetVertex3().x - _line.origin.x) / (_line.end.x - _line.origin.x);
				z = _line.origin.z + (_line.end.z - _line.origin.z) * (GetVertex3().x - _line.origin.x) / (_line.end.x - _line.origin.x);
				x = GetVertex3().x;
			}
			else if(outcode & LEFT) 
			{
				y = _line.origin.y + (_line.end.y - _line.origin.y) * (GetVertex5().x - _line.origin.x) / (_line.end.x - _line.origin.x);
				z = _line.origin.z + (_line.end.z - _line.origin.z) * (GetVertex5().x - _line.origin.x) / (_line.end.x - _line.origin.x);
				x = GetVertex5().x;
			}
			else if(outcode & FRONT) 
			{
				x = _line.origin.x + (_line.end.x - _line.origin.x) * (GetVertex5().z - _line.origin.z) / (_line.end.z - _line.origin.z);
				y = _line.origin.y + (_line.end.y - _line.origin.y) * (GetVertex5().z - _line.origin.z) / (_line.end.z - _line.origin.z);
				z = GetVertex5().z;
			}
			else if(outcode & BACK) 
			{
				x = _line.origin.x + (_line.end.x - _line.origin.x) * (GetVertex3().z - _line.origin.z) / (_line.end.z - _line.origin.z);
				y = _line.origin.y + (_line.end.y - _line.origin.y) * (GetVertex3().z - _line.origin.z) / (_line.end.z - _line.origin.z);
				z = GetVertex3().z;
			}

			if(outcode == outcode0) 
			{ 
				if(_intersectedPoint) { *_intersectedPoint = Vector3(x, y, z); }
				outcode0 = GetCode(Vector3(x, y, z));
			}
			else 
			{
				outcode1 = GetCode(Vector3(x, y, z));
			}
		}
	}
	while(!done);
	
	return accept;
}



bool Box::IsIntersected(Line _line, Line* _intersection)
{ 
	
	bool accept = false, done = false;
	int32 outcode0 = GetCode(_line.origin); 
	int32 outcode1 = GetCode(_line.end);
	
	do
	{
		float x, y, z;

		if (!(outcode0 | outcode1))
		{
			accept = true;
			done = true;
		}
		else if(outcode0 & outcode1)
		{
			done = true;
		}
		else
		{
			int32 outcode = outcode0 ? outcode0 : outcode1;
			
			if(outcode & TOP) 
			{
				x = _line.origin.x + (_line.end.x - _line.origin.x) * (GetVertex5().y - _line.origin.y) / (_line.end.y - _line.origin.y);
				z = _line.origin.z + (_line.end.z - _line.origin.z) * (GetVertex5().y - _line.origin.y) / (_line.end.y - _line.origin.y);
				y = GetVertex5().y;
			}
			else if(outcode & BOTTOM) 
			{
				x = _line.origin.x + (_line.end.x - _line.origin.x) * (GetVertex3().y - _line.origin.y) / (_line.end.y - _line.origin.y);
				z = _line.origin.z + (_line.end.z - _line.origin.z) * (GetVertex3().y - _line.origin.y) / (_line.end.y - _line.origin.y);
				y = GetVertex3().y;
			}
			else if(outcode & RIGHT) 
			{
				y = _line.origin.y + (_line.end.y - _line.origin.y) * (GetVertex3().x - _line.origin.x) / (_line.end.x - _line.origin.x);
				z = _line.origin.z + (_line.end.z - _line.origin.z) * (GetVertex3().x - _line.origin.x) / (_line.end.x - _line.origin.x);
				x = GetVertex3().x;
			}
			else if(outcode & LEFT) 
			{
				y = _line.origin.y + (_line.end.y - _line.origin.y) * (GetVertex5().x - _line.origin.x) / (_line.end.x - _line.origin.x);
				z = _line.origin.z + (_line.end.z - _line.origin.z) * (GetVertex5().x - _line.origin.x) / (_line.end.x - _line.origin.x);
				x = GetVertex5().x;
			}
			else if(outcode & FRONT) 
			{
				x = _line.origin.x + (_line.end.x - _line.origin.x) * (GetVertex5().z - _line.origin.z) / (_line.end.z - _line.origin.z);
				y = _line.origin.y + (_line.end.y - _line.origin.y) * (GetVertex5().z - _line.origin.z) / (_line.end.z - _line.origin.z);
				z = GetVertex5().z;
			}
			else if(outcode & BACK) 
			{
				x = _line.origin.x + (_line.end.x - _line.origin.x) * (GetVertex3().z - _line.origin.z) / (_line.end.z - _line.origin.z);
				y = _line.origin.y + (_line.end.y - _line.origin.y) * (GetVertex3().z - _line.origin.z) / (_line.end.z - _line.origin.z);
				z = GetVertex3().z;
			}

			if(outcode == outcode0) 
			{
				if(_intersection) { _intersection->origin = Vector3(x, y, z); }
				outcode0 = GetCode(Vector3(x, y, z));
			}
			else 
			{
				if(_intersection) { _intersection->end = Vector3(x, y, z); }
				outcode1 = GetCode(Vector3(x, y, z));
			}
		}
	}
	while(!done);
	
	return accept;
}


int32 Box::GetCode(Vector3& _point) 
{ 

	int32 code = 0;

	if(_point.y > GetVertex5().y)
	{
		code |= TOP;
	}
	else if (_point.y < GetVertex3().y)
	{
		code |= BOTTOM;
	}
	
	if(_point.x > GetVertex3().x)
	{
		code |= RIGHT;
	}
	else if(_point.x < GetVertex5().x)
	{
		code |= LEFT;
	}

	if(_point.z < GetVertex5().z)
	{
		code |= FRONT;
	}
	else if(_point.z > GetVertex3().z)
	{
		code |= BACK;
	}
	return code;
}


Vector3 Box::GetRandomVector(void)const
{
	return Vector3(Random::_RandomFloat(leftBottom.x, rightTop.x), 
		           Random::_RandomFloat(leftBottom.y, rightTop.y), 
			       Random::_RandomFloat(leftBottom.z, rightTop.z));
}



bool Box::IsPointInside(Vector3 _point)const
{
	if(_point.x >= leftBottom.x && _point.y >= leftBottom.y && _point.z >= leftBottom.z &&
	   _point.x <= rightTop.x && _point.y <= rightTop.y && _point.z <= rightTop.z)
	{ 
		return true; 
	} 
	return false;  
}


bool Box::IsIntersected(Ray _ray)const
{ 
	float txmin = (leftBottom.x - _ray.origin.x) / _ray.direction.x;
	float txmax = (rightTop.x - _ray.origin.x) / _ray.direction.x; 
	if(txmin > txmax) { Common::_Swap(txmin, txmax); }

	float tymin = (leftBottom.y - _ray.origin.y) / _ray.direction.y;
	float tymax = (rightTop.y - _ray.origin.y) / _ray.direction.y;
	if(tymin > tymax) { Common::_Swap(tymin, tymax); }
	
	if((txmin > tymax) || (tymin > txmax)) { return false; }
	if(tymin > txmin) { txmin = tymin; }
	if(tymax < txmax) { txmax = tymax; }

	float tzmin = (leftBottom.z - _ray.origin.z) / _ray.direction.z;
	float tzmax = (rightTop.z - _ray.origin.z) / _ray.direction.z;
	if(tzmin > tzmax) { Common::_Swap(tzmin, tzmax); }

	if((txmin > tzmax) || (tzmin > txmax)) { return false; }
	return true;
}



Vector3 Box::ClampPoint(Vector3 _point)
{
    if(_point.x > rightTop.x)
	{
		_point.x = rightTop.x;
	}
    else if(_point.x < leftBottom.x)
	{
        _point.x = leftBottom.x;
	}
	if(_point.y > rightTop.y)
	{
		_point.y = rightTop.y;
	}
    else if(_point.y < leftBottom.y)
	{
        _point.y = leftBottom.y;
	}
	if(_point.z > rightTop.z)
	{
		_point.z = rightTop.z;
	}
    else if(_point.z < leftBottom.z)
	{
        _point.z = leftBottom.z;
	}

	return _point;
}


 bool Box::IsIntersected(Box _box)
 {
	 Vector3 box1Center = GetCenter();
	 Vector3 box1Size = GetSize();
	 Vector3 box2Center = _box.GetCenter();
	 Vector3 box2Size = _box.GetSize();

	 if(fabs(box1Center.x - box2Center.x) > (box1Size.x / 2.0f + box2Size.x / 2.0f)) { return false; }
     if(fabs(box1Center.y - box2Center.y) > (box1Size.y / 2.0f + box2Size.y / 2.0f)) { return false; }
     if(fabs(box1Center.z - box2Center.z) > (box1Size.z / 2.0f + box2Size.z / 2.0f)) { return false; }
	 
	 return true;
}