
#include "Spherical.h"

Spherical::Spherical(void): radius(1.0f), vertical(0.0f), horizontal(0.0f)
{}


Spherical::Spherical(float _radius, float _vertical, float _horizontal): radius(_radius), vertical(_vertical), horizontal(_horizontal)
{}


bool Spherical::operator == (const Spherical& _spherical)const
{
	if(radius == _spherical.radius && vertical == _spherical.vertical && horizontal == _spherical.horizontal)
	{
		return true;
	}
	return false;
}


bool Spherical::operator != (const Spherical& _spherical)const
{
	return !(*this == _spherical);
}
