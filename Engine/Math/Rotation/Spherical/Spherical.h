#ifndef SPHERICAL_H
#define SPHERICAL_H
#include <Engine/Core/Numerical/Numerical.h>
class Spherical
{
	public: float radius;
	public: float vertical;
	public: float horizontal;
	public: Spherical(void);
	public: Spherical(float _radius, float _vertical, float _horizontal);
	public: bool operator == (const Spherical& _spherical)const;
	public: bool operator != (const Spherical& _spherical)const;
};



#endif 
