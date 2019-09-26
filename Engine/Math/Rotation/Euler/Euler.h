#ifndef EULER_H
#define EULER_H
#include <Engine/Core/Numerical/Numerical.h>
class Euler
{
	public: float pitch;
	public: float yaw;
	public: float roll;
	public: Euler(void);
	public: Euler(float _pitch, float _yaw, float _roll);
	public: bool operator == (const Euler& _euler)const;
	public: bool operator != (const Euler& _euler)const;
	public: Euler operator - (const Euler& _euler)const;
};



#endif 
