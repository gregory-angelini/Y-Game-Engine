#ifndef MATH_H
#define MATH_H
#include <Engine/Core/Numerical/Numerical.h>
namespace Math
{
	static int32 _Factorial(int32 _n)
	{
		if(_n < 0)
		{
			return 0; 
		}
		if(_n == 0)
		{
			return 1; 
		}
		else
		{
			return _n * _Factorial(_n - 1); 
		}
	}
};


#endif
