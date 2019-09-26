#include "Triangle.h"
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Output/Graphics/Renderer/Renderer.h>


const int32 Triangle::SIZE = 3;


Triangle::Triangle(void)
{}

	 
Triangle::~Triangle(void)
{}



Triangle::Triangle(Vector3 _v1, Vector3 _v2, Vector3 _v3): v1(_v1), v2(_v2), v3(_v3)
{} 



const float* Triangle::GetData(void)const
{
	return v1.GetData();
}




bool Triangle::IsPointInside(Vector3 _point)
{
	return((Line::_GetVertexPosition(Line(v1, v3), _point) != Line::LEFT) &&
		   (Line::_GetVertexPosition(Line(v3, v2), _point) != Line::LEFT) &&
		   (Line::_GetVertexPosition(Line(v2, v1), _point) != Line::LEFT));
}



bool Triangle::IsIntersected(Triangle _triangle)
{ 
	if(_triangle.IsPointInside(v1) ||
	   _triangle.IsPointInside(v2) ||
	   _triangle.IsPointInside(v3))
	{
		return true;
	} 
	if(IsPointInside(_triangle.v1) ||
	   IsPointInside(_triangle.v2) ||
	   IsPointInside(_triangle.v3))
	{
		return true;
	} 
	if(Line::_IsIntersected(Line(v1, v2), Line(_triangle.v1, _triangle.v2)) ||
	   Line::_IsIntersected(Line(v1, v2), Line(_triangle.v2, _triangle.v3)) ||
	   Line::_IsIntersected(Line(v1, v2), Line(_triangle.v3, _triangle.v1)) ||
	   Line::_IsIntersected(Line(v2, v3), Line(_triangle.v1, _triangle.v2)) ||
	   Line::_IsIntersected(Line(v2, v3), Line(_triangle.v2, _triangle.v3)) ||
	   Line::_IsIntersected(Line(v2, v3), Line(_triangle.v3, _triangle.v1)) ||
	   Line::_IsIntersected(Line(v3, v1), Line(_triangle.v1, _triangle.v2)) ||
	   Line::_IsIntersected(Line(v3, v1), Line(_triangle.v2, _triangle.v3)) ||
	   Line::_IsIntersected(Line(v3, v1), Line(_triangle.v3, _triangle.v1)))
	{
		return true;
	}

	return false;
}
	



void Triangle::Transform(Matrix4x4 _transformation)
{
	v1 = _transformation.TransformVector(v1);
	v2 = _transformation.TransformVector(v2);
	v3 = _transformation.TransformVector(v3);
}


Triangle Triangle::_Transform(Triangle _triangle, Matrix4x4 _transformation)
{
	return Triangle(_transformation.TransformVector(_triangle.v1), 
					_transformation.TransformVector(_triangle.v2),
					_transformation.TransformVector(_triangle.v3));
}


bool Triangle::IsClockwise(void)
{ 
	float dx1 = v2.x - v1.x;
	float dx2 = v3.x - v2.x;

	float dy1 = v2.y - v1.y;
	float dy2 = v3.y - v2.y;

	float r = dx1 * dy2 - dx2 * dy1;
			
	if(r < 0)
	{
		return true;
	}
	return false;
}


void Triangle::InverseVertices(void) 
{
	Common::_Swap(v1, v3);
}


Vector3 Triangle::GetCenter(void)const
{
	return (v1 + v2 + v3) / SIZE;
}


Vector3 Triangle::_GetCenter(Vector3 _v1, Vector3 _v2, Vector3 _v3)
{
	return (_v1 + _v2 + _v3) / SIZE;
}


Vector3 Triangle::GetNormal(void)const
{
	return Vector3::_GetNormalize((v2 - v1) * (v3 - v1));
}


Vector3 Triangle::_GetNormal(Vector3 _v1, Vector3 _v2, Vector3 _v3)
{
	return Vector3::_GetNormalize((_v1 - _v2) * (_v2 - _v3));
}


bool Triangle::IsIntersected(Line _line, int32 _mode, Vector3* _intersectionPoint)
{
	Vector3 normal = GetNormal();
	if((_mode & AbstractSurface::CHECK_ONLY_FRONT_FACE) && _line.GetDirection().GetDot(normal) > 0) { return false; }
	float r1 = Vector3::_GetDot(normal, _line.origin - v1);
	float r2 = Vector3::_GetDot(normal, _line.end - v1);
	if(Numerical<float>::_Sign(r1) == Numerical<float>::_Sign(r2)) 
	{
		return false;
	}
	Vector3 intersectionPoint = (_line.origin + ((_line.end - _line.origin) * (-r1 / (r2 - r1))));
	if(Vector3::_GetDot(Vector3::_GetCross(v2 - v1, intersectionPoint - v1), normal) > 0.0f) 
	{
		if(Vector3::_GetDot(Vector3::_GetCross(v3 - v2, intersectionPoint - v2), normal) > 0.0f) 
		{
			if(Vector3::_GetDot(Vector3::_GetCross(v1 - v3, intersectionPoint - v3), normal) > 0.0f)
			{
				if(_intersectionPoint) { (*_intersectionPoint) = intersectionPoint; } 
				return true; 
			}
		}
	}
	return false;
}
