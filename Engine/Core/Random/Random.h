#ifndef RANDOM_H
#define RANDOM_H
#include <Engine/Core/Numerical/Numerical.h>
class Random
{
	public: enum
	{
		GET_RANDOM_INTEGER_MESSAGE = 104,
		GET_RANDOM_FLOAT_MESSAGE = 105,
		GET_RANDOM_BOOLEAN_MESSAGE = 106,
		CHANGE_RANDOM_SEED_MESSAGE = 107
	};
	private: static uint32 seed;
	private: Random(void);
	public: static float _RandomFloat(float _lowerBound, float _upperBound);
	public: static int32 _RandomInt32(int32 _lowerBound, int32 _upperBound); 
	public: static void _SetSeed(uint32 _seed);
};


#endif
