
#include "Plane.h"
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>

	
Plane::Plane(void)
{
	AddClassProperty(PLANE_CLASS);
}


Plane::Plane(Vector3 _p0, Vector3 _normal)
{
	AddClassProperty(PLANE_CLASS);
	Set(_p0,_normal);
}


Plane::Plane(Vector3 _p1, Vector3 _p2, Vector3 _p3)
{
	AddClassProperty(PLANE_CLASS);
	Set(_p1, _p2, _p3);
}



Plane::~Plane(void)
{
	Destroying();
}



void Plane::Set(Vector3 _p1, Vector3 _p2, Vector3 _p3)
{
	SetPosition(_p1);
	normal = Vector3::_GetNormalize((_p2 - _p1) * (_p3 - _p1));
}


void Plane::Set(Vector3 _p0, Vector3 _normal)
{
	SetPosition(_p0);
	normal = _normal;
}


float Plane::DistanceTo(Vector3 _point)
{
	return Vector3::_GetDot(normal, _point - GetPosition());
}


float Plane::_DistanceTo(Vector3 _p1, Vector3 _p2, Vector3 _p3, Vector3 _point)
{
	return Vector3::_GetDot(Vector3::_GetNormalize((_p2 - _p1) * (_p3 - _p1)), _point - _p1);
}


float Plane::_DistanceTo(Vector3 _p0, Vector3 _normal, Vector3 _point)
{
	return Vector3::_GetDot(_normal, _point - _p0);
}



bool Plane::operator == (Plane& _plane)
{
	if(this == &_plane)
	{
		return true;
	}
	return (GetPosition() == _plane.GetPosition() && normal == _plane.normal);
}

				
int32 Plane::IsIntersected(Line _line, Vector3* _intersectionPoint)
{
	if((intersectionMode & AbstractSurface::CHECK_ONLY_FRONT_FACE) && !(_line.GetDirection().GetDot(normal) < 0)) { return -1; }
	float r1 = Vector3::_GetDot(normal, _line.origin - GetPosition());
	float r2 = Vector3::_GetDot(normal, _line.end - GetPosition());
	if(Numerical<float>::_Sign(r1) == Numerical<float>::_Sign(r2)) 
	{
		return -1;
	}
	if(_intersectionPoint) { (*_intersectionPoint) = (_line.origin + ((_line.end - _line.origin) * (-r1 / (r2 - r1)))); }
	return 0;
}



int32 Plane::_IsIntersected(Vector3 _p0, Vector3 _normal, Line _line, int32 _intersectionMode, Vector3* _intersectionPoint)
{
	if((_intersectionMode & AbstractSurface::CHECK_ONLY_FRONT_FACE) && !(_line.GetDirection().GetDot(_normal) < 0)) { return -1; }
	float r1 = Vector3::_GetDot(_normal, _line.origin - _p0);
	float r2 = Vector3::_GetDot(_normal, _line.end - _p0);
	if(Numerical<float>::_Sign(r1) == Numerical<float>::_Sign(r2)) 
	{
		return -1;
	}
	if(_intersectionPoint) { (*_intersectionPoint) = (_line.origin + ((_line.end - _line.origin) * (-r1 / (r2 - r1)))); }
	return 0;
}



int32 Plane::_IsIntersected(Vector3 _p1, Vector3 _p2, Vector3 _p3, Line _line, int32 _intersectionMode, Vector3* _intersectionPoint)
{
	Vector3 normal = Vector3::_GetNormalize((_p2 - _p1) * (_p3 - _p1));
	if((_intersectionMode & AbstractSurface::CHECK_ONLY_FRONT_FACE) && !(_line.GetDirection().GetDot(normal) < 0)) { return -1; }
	float r1 = Vector3::_GetDot(normal, _line.origin - _p1);
	float r2 = Vector3::_GetDot(normal, _line.end - _p1);
	if(Numerical<float>::_Sign(r1) == Numerical<float>::_Sign(r2)) 
	{
		return -1;
	}
	if(_intersectionPoint) { (*_intersectionPoint) = (_line.origin + ((_line.end - _line.origin) * (-r1 / (r2 - r1)))); }
	return 0;
}


Vector3 Plane::_GetNormal(Vector3 _p1, Vector3 _p2, Vector3 _p3)
{
	return Vector3::_GetNormalize((_p2 - _p1) * (_p3 - _p1));
}



Vector3 Plane::GetNormal(void)const
{
	return normal;
}

			
void Plane::SetNormal(Vector3 _normal)
{
	normal = _normal;
}




void Plane::SetPosition(Vector3 _position)
{
	position = _position;
}



void Plane::DisplacePosition(Vector3 _offset)
{
	SetPosition(position + _offset);
}


Vector3 Plane::GetPosition(void)
{
	return position;
}

		
Vector3 Plane::GetAbsolutePosition(void)
{
	return position;
}


void Plane::MoveTo(Vector3 _direction, float _units)
{
	if(_units != 0.0f) { SetPosition(position + _direction * _units); }
}




bool Plane::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		if(AbstractObject::SaveToFile(_file))
		{
			SaveDerivedToFile(_file);
			return true;
		}
	}
	return false;
}



bool Plane::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Plane::SaveAsToFile(File& _file, StringANSI _name)
{
	if(AbstractObject::SaveAsToFile(_file, _name))
	{ 
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}


void Plane::SaveDerivedToFile(File& _file)
{
	_file.Write(intersectionMode);
	_file.Write(GetPosition());
	_file.Write(normal);
}



bool Plane::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		LoadDerivedFromFile(_file);
		return true;
	} 
	return false;
}


void Plane::LoadDerivedFromFile(File& _file)
{
	_file.Read(intersectionMode);
	Vector3 position;
	_file.Read(position);
	SetPosition(position);
	_file.Read(normal);
}