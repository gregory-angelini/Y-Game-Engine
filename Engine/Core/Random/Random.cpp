

#include "Random.h"
#include <Engine/Core/Time/SystemTime/SystemTime.h>
uint32 Random::seed = SystemTime::_GetSystemTimeInMs();


Random::Random(void)
{}


void Random::_SetSeed(uint32 _seed)
{

	seed = _seed;
}


float Random::_RandomFloat(float _lowerBound, float _upperBound)
{
	if(_lowerBound >= _upperBound)
	{ return _lowerBound; }

	seed = 214013 * seed + 2531011;
	return (seed >> 16) * (0.000015f) * (_upperBound - _lowerBound) + _lowerBound; 
}



int32 Random::_RandomInt32(int32 _lowerBound, int32 _upperBound)
{ 
	if(_lowerBound >= _upperBound)
	{ return _lowerBound; } 
	seed = 214013 * seed + 2531011;
	return (seed ^ seed >> 15) % (_upperBound - _lowerBound + 1) + _lowerBound; 										
}