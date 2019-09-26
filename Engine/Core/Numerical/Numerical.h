#ifndef NUMERICAL_H
#define NUMERICAL_H
#include <math.h>
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef short int16;
typedef unsigned short uint16;

typedef wchar_t char16;
typedef char char8;

typedef char int8;
typedef unsigned char uint8;


const int32 NIL = 0;
template<typename Type> class Numerical
{};
template<> class Numerical<int64>
{
	public: static bool _Sign(int64 _number);
	public: static int64 _LowerValue(void);
	public: static int64 _UpperValue(void);
	public: static int32 _AmountOfDigits(int64 _number);
	public: static bool _IsOdd(int64 _number);
	public: static bool _IsOverflowPlus(int64 _a, int64 _b);
	public: static int64 _GetMin(int64 _a, int64 _b);
	public: static int64 _GetMax(int64 _a, int64 _b);
	public: static int64 _GetMin(int64 _a, int64 _b, int64 _c);
	public: static int64 _GetMax(int64 _a, int64 _b, int64 _c);
};
template<> class Numerical<uint64>
{
	public: static bool _Sign(uint64 _number);
	public: static uint64 _LowerValue(void);
	public: static uint64 _UpperValue(void);
	public: static int32 _AmountOfDigits(uint64 _number);
	public: static bool _IsOdd(uint64 _number);
	public: static bool _IsOverflowPlus(uint64 _a, uint64 _b);
	public: static uint64 _GetMin(uint64 _a, uint64 _b);
	public: static uint64 _GetMax(uint64 _a, uint64 _b);
	public: static uint64 _GetMin(uint64 _a, uint64 _b, uint64 _c);
	public: static uint64 _GetMax(uint64 _a, uint64 _b, uint64 _c);
};
template<> class Numerical<int32>
{
	public: static bool _Sign(int32 _number);
	public: static int32 _LowerValue(void);
	public: static int32 _UpperValue(void);
	public: static int32 _AmountOfDigits(int32 _number);
	public: static bool _IsOdd(int32 _number);
	public: static bool _IsOverflowPlus(int32 _a, int32 _b);
	public: static int32 _FromFloat(float _value);
	public: static int32 _GetMin(int32 _a, int32 _b);
	public: static int32 _GetMax(int32 _a, int32 _b);
	public: static int32 _GetMin(int32 _a, int32 _b, int32 _c);
	public: static int32 _GetMax(int32 _a, int32 _b, int32 _c);
};
template<> class Numerical<uint32>
{
	public: static bool _Sign(uint32 _number);
	public: static uint32 _LowerValue(void);
	public: static uint32 _UpperValue(void);
	public: static int32 _AmountOfDigits(uint32 _number);
	public: static bool _IsOdd(uint32 _number);
	public: static bool _IsOverflowPlus(uint32 _a, uint32 _b);
	public: static uint32 _GetMin(uint32 _a, uint32 _b);
	public: static uint32 _GetMax(uint32 _a, uint32 _b);
	public: static uint32 _GetMin(uint32 _a, uint32 _b, uint32 _c);
	public: static uint32 _GetMax(uint32 _a, uint32 _b, uint32 _c);
};
template<> class Numerical<int16>
{
	public: static bool _Sign(int16 _number);
	public: static int16 _LowerValue(void);
	public: static int16 _UpperValue(void);
	public: static int32 _AmountOfDigits(int16 _number);
	public: static bool _IsOdd(int16 _number);
	public: static bool _IsOverflowPlus(int16 _a, int16 _b);
	public: static int16 _GetMin(int16 _a, int16 _b);
	public: static int16 _GetMax(int16 _a, int16 _b);
	public: static int16 _GetMin(int16 _a, int16 _b, int16 _c);
	public: static int16 _GetMax(int16 _a, int16 _b, int16 _c);
};
template<> class Numerical<uint16>
{
	public: static bool _Sign(uint16 _number);
	public: static uint16 _LowerValue(void);
	public: static uint16 _UpperValue(void);
	public: static int32 _AmountOfDigits(uint16 _number);
	public: static bool _IsOdd(uint16 _number);
	public: static bool _IsOverflowPlus(uint16 _a, uint16 _b);
	public: static uint16 _GetMin(uint16 _a, uint16 _b);
	public: static uint16 _GetMax(uint16 _a, uint16 _b);
	public: static uint16 _GetMin(uint16 _a, uint16 _b, uint16 _c);
	public: static uint16 _GetMax(uint16 _a, uint16 _b, uint16 _c);
};
template<> class Numerical<int8>
{
	public: static bool _Sign(int8 _number);
	public: static int8 _LowerValue(void);
	public: static int8 _UpperValue(void);
	public: static int32 _AmountOfDigits(int8 _number);
	public: static bool _IsOdd(int8 _number);
	public: static bool _IsOverflowPlus(int8 _a, int8 _b);
	public: static int8 _GetMin(int8 _a, int8 _b);
	public: static int8 _GetMax(int8 _a, int8 _b);
	public: static int8 _GetMin(int8 _a, int8 _b, int8 _c);
	public: static int8 _GetMax(int8 _a, int8 _b, int8 _c);
};
template<> class Numerical<uint8>
{
	public: static bool _Sign(uint8 _number);
	public: static uint8 _LowerValue(void);
	public: static uint8 _UpperValue(void);
	public: static int32 _AmountOfDigits(uint8 _number);
	public: static bool _IsOdd(uint8 _number);
	public: static bool _IsOverflowPlus(uint8 _a, uint8 _b);
	public: static uint8 _GetMin(uint8 _a, uint8 _b);
	public: static uint8 _GetMax(uint8 _a, uint8 _b);
	public: static uint8 _GetMin(uint8 _a, uint8 _b, uint8 _c);
	public: static uint8 _GetMax(uint8 _a, uint8 _b, uint8 _c);
};
template<> class Numerical<float>
{
	public: static const float TINY;
	public: static bool _Sign(float _number);
	public: static float _LowerValue(void);
	public: static float _UpperValue(void);
	public: static bool _IsOverflowPlus(float _a, float _b);
	public: static bool _Equals(float _lValue, float _rValue);
	public: static float _GetMin(float _a, float _b);
	public: static float _GetMax(float _a, float _b);
	public: static float _GetMin(float _a, float _b, float _c);
	public: static float _GetMax(float _a, float _b, float _c);
};
template<> class Numerical<double>
{
	public: static bool _Sign(double _number);
	public: static double _LowerValue(void);
	public: static double _UpperValue(void);
	public: static bool _IsOverflowPlus(double _a, double _b);
	public: static bool _Equals(double _lValue, double _rValue);
	public: static double _GetMin(double _a, double _b);
	public: static double _GetMax(double _a, double _b);
	public: static double _GetMin(double _a, double _b, double _c);
	public: static double _GetMax(double _a, double _b, double _c);
};


#endif
