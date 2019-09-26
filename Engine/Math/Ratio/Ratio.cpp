
#include "Ratio.h"
float Ratio::floatMatrix[2][2] = { 0.0f, 100.0f, 0.0f, 0.0f };
int32 Ratio::intMatrix[2][2] = { 0, 100, 0, 0 };
int32 Ratio::matrixWidth = 2;




float Ratio::_GetRatio(float _a, float _b)
{
	return _b == 0.0f ? 0.0f
	                  : _a / _b; 
}