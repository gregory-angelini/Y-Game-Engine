#include "Numerical.h"


const float Numerical<float>::TINY = 0.0000001f;



bool Numerical<int64>::_Sign(int64 _number)
{
	return (_number < 0) ? false : true;
}


int64 Numerical<int64>::_LowerValue(void)
{
	return -9223372036854775808;
}


int64 Numerical<int64>::_UpperValue(void)
{
	return 9223372036854775807;
}

 
int32 Numerical<int64>::_AmountOfDigits(int64 _number)
{
	int32 count = 0; 

	while(_number)
	{
		_number /= 10;
		++count;
	}
	return count;
}
 

bool Numerical<int64>::_IsOdd(int64 _number)
{
	return (bool)(_number % 2);
}


bool Numerical<int64>::_IsOverflowPlus(int64 _a, int64 _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}



int64 Numerical<int64>::_GetMin(int64 _a, int64 _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


int64 Numerical<int64>::_GetMax(int64 _a, int64 _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}


int64 Numerical<int64>::_GetMin(int64 _a, int64 _b, int64 _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}


int64 Numerical<int64>::_GetMax(int64 _a, int64 _b, int64 _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}


bool Numerical<uint64>::_Sign(uint64 _number)
{
	return (_number < 0) ? false : true;
}


uint64 Numerical<uint64>::_LowerValue(void)
{
	return 0;
}


uint64 Numerical<uint64>::_UpperValue(void)
{
	return 18446744073709551615;
}


int32 Numerical<uint64>::_AmountOfDigits(uint64 _number)
{
	int32 count = 0; 

	while(_number)
	{
		_number /= 10;
		++count;
	}
	return count;
}
 
 
bool Numerical<uint64>::_IsOdd(uint64 _number)
{
	return (bool)(_number % 2);
}

 
bool Numerical<uint64>::_IsOverflowPlus(uint64 _a, uint64 _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}

uint64 Numerical<uint64>::_GetMin(uint64 _a, uint64 _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


uint64 Numerical<uint64>::_GetMax(uint64 _a, uint64 _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}


uint64 Numerical<uint64>::_GetMin(uint64 _a, uint64 _b, uint64 _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

		return _c;
	}


uint64 Numerical<uint64>::_GetMax(uint64 _a, uint64 _b, uint64 _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}




bool Numerical<int32>::_Sign(int32 _number)
{
	return (_number < 0) ? false : true;
}

	

int32 Numerical<int32>::_LowerValue(void)
{
	return -2147483648;
}


int32 Numerical<int32>::_UpperValue(void)
{
	return 2147483647; 
}


int32 Numerical<int32>::_AmountOfDigits(int32 _number)
{
	int32 count = 0; 

	while(_number)
	{
		_number /= 10;
		++count;
	}
	return count;
}
 

bool Numerical<int32>::_IsOdd(int32 _number)
{
	return (bool)(_number % 2);
}


int32 Numerical<int32>::_FromFloat(float _value)
{
	return _value > 0.0f ? _value + 0.5f : _value - 0.5f; 

}


bool Numerical<int32>::_IsOverflowPlus(int32 _a, int32 _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}


int32 Numerical<int32>::_GetMin(int32 _a, int32 _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


int32 Numerical<int32>::_GetMax(int32 _a, int32 _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}


int32 Numerical<int32>::_GetMin(int32 _a, int32 _b, int32 _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}


int32 Numerical<int32>::_GetMax(int32 _a, int32 _b, int32 _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}


bool Numerical<uint32>::_Sign(uint32 _number)
{
	return (_number < 0) ? false : true;
}



uint32 Numerical<uint32>::_LowerValue(void)
{
	return 0;
}


uint32 Numerical<uint32>::_UpperValue(void)
{
	return 4294967295;
}


int32 Numerical<uint32>::_AmountOfDigits(uint32 _number)
{
	int32 count = 0; 

	while(_number)
	{
		_number /= 10;
		++count;
	}
	return count;
}
 

bool Numerical<uint32>::_IsOdd(uint32 _number)
{
	return (bool)(_number % 2);
}


bool Numerical<uint32>::_IsOverflowPlus(uint32 _a, uint32 _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}


uint32 Numerical<uint32>::_GetMin(uint32 _a, uint32 _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


uint32 Numerical<uint32>::_GetMax(uint32 _a, uint32 _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}


uint32 Numerical<uint32>::_GetMin(uint32 _a, uint32 _b, uint32 _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}


uint32 Numerical<uint32>::_GetMax(uint32 _a, uint32 _b, uint32 _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}



bool Numerical<int16>::_Sign(int16 _number)
{
	return (_number < 0) ? false : true;
}


int16 Numerical<int16>::_LowerValue(void)
{
	return -32768;
}


int16 Numerical<int16>::_UpperValue(void)
{
	return 32767;
}


int32 Numerical<int16>::_AmountOfDigits(int16 _number)
{
	int32 count = 0; 

	while(_number)
	{
		_number /= 10;
		++count;
	}
	return count;
}
 

bool Numerical<int16>::_IsOdd(int16 _number)
{
	return (bool)(_number % 2);
}


bool Numerical<int16>::_IsOverflowPlus(int16 _a, int16 _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}


int16 Numerical<int16>::_GetMin(int16 _a, int16 _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


int16 Numerical<int16>::_GetMax(int16 _a, int16 _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}


int16 Numerical<int16>::_GetMin(int16 _a, int16 _b, int16 _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}


int16 Numerical<int16>::_GetMax(int16 _a, int16 _b, int16 _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}



bool Numerical<uint16>::_Sign(uint16 _number)
{
	return (_number < 0) ? false : true;
}


uint16 Numerical<uint16>::_LowerValue(void)
{
	return 0;
}


uint16 Numerical<uint16>::_UpperValue(void)
{
	return 65535;
}


int32 Numerical<uint16>::_AmountOfDigits(uint16 _number)
{
	int32 count = 0; 

	while(_number)
	{
		_number /= 10;
		++count;
	}
	return count;
}
 

bool Numerical<uint16>::_IsOdd(uint16 _number)
{
	return (bool)(_number % 2);
}


bool Numerical<uint16>::_IsOverflowPlus(uint16 _a, uint16 _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}


uint16 Numerical<uint16>::_GetMin(uint16 _a, uint16 _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


uint16 Numerical<uint16>::_GetMax(uint16 _a, uint16 _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}


uint16 Numerical<uint16>::_GetMin(uint16 _a, uint16 _b, uint16 _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}


uint16 Numerical<uint16>::_GetMax(uint16 _a, uint16 _b, uint16 _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}



bool Numerical<int8>::_Sign(int8 _number)
{
	return (_number < 0) ? false : true;
}


int8 Numerical<int8>::_LowerValue(void)
{
	return -128;
}


int8 Numerical<int8>::_UpperValue(void)
{
	return 127;
}


int32 Numerical<int8>::_AmountOfDigits(int8 _number)
{
	int32 count = 0; 

	while(_number)
	{
		_number /= 10;
		++count;
	}
	return count;
}
 

bool Numerical<int8>::_IsOdd(int8 _number)
{
	return (bool)(_number % 2);
}


bool Numerical<int8>::_IsOverflowPlus(int8 _a, int8 _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}



int8 Numerical<int8>::_GetMin(int8 _a, int8 _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


int8 Numerical<int8>::_GetMax(int8 _a, int8 _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}



int8 Numerical<int8>::_GetMin(int8 _a, int8 _b, int8 _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}



int8 Numerical<int8>::_GetMax(int8 _a, int8 _b, int8 _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}


bool Numerical<uint8>::_Sign(uint8 _number)
{
	return (_number < 0) ? false : true;
}


uint8 Numerical<uint8>::_LowerValue(void)
{
	return 0;
}


uint8 Numerical<uint8>::_UpperValue(void)
{
	return 255;
}


int32 Numerical<uint8>::_AmountOfDigits(uint8 _number)
{
	int32 count = 0; 

	while(_number)
	{
		_number /= 10;
		++count;
	}
	return count;
}
 

bool Numerical<uint8>::_IsOdd(uint8 _number)
{
	return (bool)(_number % 2);
}


bool Numerical<uint8>::_IsOverflowPlus(uint8 _a, uint8 _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}


uint8 Numerical<uint8>::_GetMin(uint8 _a, uint8 _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}



uint8 Numerical<uint8>::_GetMax(uint8 _a, uint8 _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}


uint8 Numerical<uint8>::_GetMin(uint8 _a, uint8 _b, uint8 _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}


uint8 Numerical<uint8>::_GetMax(uint8 _a, uint8 _b, uint8 _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}


bool Numerical<float>::_Sign(float _number)
{
	return (_number < 0) ? false : true;
}


float Numerical<float>::_LowerValue(void)
{
	return -3.402823466e+38F;
}


float Numerical<float>::_UpperValue(void)
{
	return 3.402823466e+38F;
}


bool Numerical<float>::_IsOverflowPlus(float _a, float _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}


bool Numerical<float>::_Equals(float _lValue, float _rValue)
{
	return fabsf(_lValue - _rValue) < 1.192092896e-07F ? true : false;
}



float Numerical<float>::_GetMin(float _a, float _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


float Numerical<float>::_GetMax(float _a, float _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}


float Numerical<float>::_GetMin(float _a, float _b, float _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}


float Numerical<float>::_GetMax(float _a, float _b, float _c)
{ 
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}


bool Numerical<double>::_Sign(double _number)
{
	return (_number < 0) ? false : true;
}


double Numerical<double>::_LowerValue(void)
{
	return -1.7976931348623158e+308;
}


double Numerical<double>::_UpperValue(void)
{
	return 1.7976931348623158e+308;
}


bool Numerical<double>::_IsOverflowPlus(double _a, double _b)
{
	if(Numerical::_UpperValue() - _a < _b)
	{
		return true;
	}
	return false;
}


bool Numerical<double>::_Equals(double _lValue, double _rValue)
{
	 return fabsf(_lValue - _rValue) < 2.2204460492503131e-016 ? true : false;
}


double Numerical<double>::_GetMin(double _a, double _b)
{
	if(_b > _a)
	{
		return _a;
	}
	return _b;
}


double Numerical<double>::_GetMax(double _a, double _b)
{
	if(_b < _a)
	{
		return _a;
	}
	return _b;
}



double Numerical<double>::_GetMin(double _a, double _b, double _c)
{
	if(_b > _a)
	{
		if(_c > _a)
		{
			return _a;
		}
	}
	else if(_a > _b)
	{
		if(_c > _b)
		{
			return _b;
		}
	}

	return _c;
}



double Numerical<double>::_GetMax(double _a, double _b, double _c)
{
	if(_b < _a)
	{
		if(_c < _a)
		{
			return _a;
		}
	}
	else if(_a < _b)
	{
		if(_c < _b)
		{
			return _b;
		}
	}

	return _c;
}