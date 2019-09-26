#ifndef RATIO_H
#define RATIO_H
#include <Engine/Core/Numerical/Numerical.h>
class Ratio
{
	public: enum Unknown 
	{
		WHOLE = 0,
		PART = 2,
		PART_RATIO = 3
	};
	private: static float floatMatrix[2][2];
	private: static int32 intMatrix[2][2];
	private: static int32 matrixWidth;
	public: template<typename Type>
	static void _SetKnown(Unknown _x, Type _value)
	{}
	public: template<>
	static void _SetKnown<float>(Unknown _x, float _value)
	{
		floatMatrix[_x / matrixWidth][_x % matrixWidth] = _value;
	}
	public: template<>
	static void _SetKnown<int32>(Unknown _x, int32 _value)
	{
		intMatrix[_x / matrixWidth][_x % matrixWidth] = _value;
	}
	public: template<typename Type>
	static Type _GetUnknown(Unknown _x)
	{}
	public: template<>
	static float _GetUnknown<float>(Unknown _x)
	{
		int32 x = _x / matrixWidth;
		int32 y = _x % matrixWidth;
	
		return floatMatrix[!x][!y] == 0.0f ? 0.0f
		                                   : floatMatrix[!x][y] * floatMatrix[x][!y] / floatMatrix[!x][!y];
	}
    public: template<>
	static int32 _GetUnknown<int32>(Unknown _x)
	{
		int32 x = _x / matrixWidth;
		int32 y = _x % matrixWidth;
	
		return intMatrix[!x][!y] == 0 ? 0
		                              : intMatrix[!x][y] * intMatrix[x][!y] / intMatrix[!x][!y];
	}
	public: static float _GetRatio(float _a, float _b);
};



#endif 
