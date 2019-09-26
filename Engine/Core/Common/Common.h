#ifndef COMMON_H
#define COMMON_H
#include <Engine/Core/Numerical/Numerical.h>
namespace Common
{
	template<typename Type> static void _Swap(Type& _a, Type& _b)
	{ 

		Type _aCopy = _a;
		_a = _b;
		_b = _aCopy;
	}
    static int32 _GetNextPowerOf2(int32 _value)
	{ 
		int32 nextPowerOf2, i = 0;

		if((_value & (_value - 1)) != 0) 
		{
			while((_value >> (++i)) > 0 && (nextPowerOf2 = int32(1) << (i + 1)))
			{}
		}
		else
		{
			nextPowerOf2 = _value;
		}
		return nextPowerOf2;
	}
};


#endif
