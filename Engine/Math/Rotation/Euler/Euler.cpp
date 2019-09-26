
#include "Euler.h"


Euler::Euler(void): pitch(0.0f), yaw(0.0f), roll(0.0f)
{}


Euler::Euler(float _pitch, float _yaw, float _roll): pitch(_pitch), yaw(_yaw), roll(_roll)
{}



bool Euler::operator == (const Euler& _euler)const
{
	if(pitch == _euler.pitch && yaw == _euler.yaw && roll == _euler.roll)
	{
		return true;
	}
	return false;
}




bool Euler::operator != (const Euler& _euler)const
{
	return !(*this == _euler);
}

Euler Euler::operator - (const Euler& _euler)const
{
	return Euler(pitch -_euler.pitch, yaw - _euler.yaw, roll - _euler.roll); 
}