// Numerical.cpp: определяет точку входа для консольного приложения.
//


#include <Engine/Core/Numerical/Numerical.h>
#include <conio.h>
#include <windows.h>
#include <string>




int main () 
{
	printf("sign 6: %d\n", Numerical<int32>::_Sign(6));
	printf("sign -5: %d\n", Numerical<int32>::_Sign(-5));
	
	
	printf("min(5,4): %d\n", Numerical<int32>::_Min(5, 4));
	printf("max(5,4): %d\n", Numerical<int32>::_Max(5,4 ));
	
		
	
	printf("int digit count(128): %d\n", Numerical<int32>::_IntDigitCount(128));
	printf("is odd(10): %d\n", Numerical<int32>::_IsOdd(10));
	printf("next power of 2(3): %d\n", Numerical<int32>::_NextPowerOf2(3));
	


	{
	int64 min = Numerical<int64>::_LowerValue();
	int64 max = Numerical<int64>::_UpperValue();
	printf("int64 lower: %I64d\n", min);
	printf("int64 upper: %I64d\n", max);
	}

	{
	uint64 min = Numerical<uint64>::_LowerValue();
	uint64 max = Numerical<uint64>::_UpperValue();
	printf("uint64 lower: %I64u\n", min);
	printf("uint64 upper: %I64u\n", max);
	}

	{
	int32 min = Numerical<int32>::_LowerValue();
	int32 max = Numerical<int32>::_UpperValue();
	printf("int32 lower: %d\n", min);
	printf("int32 upper: %d\n", max);
	}
		
	{
	uint32 min = Numerical<uint32>::_LowerValue();
	uint32 max = Numerical<uint32>::_UpperValue();
	printf("uint32 lower: %lu\n", min);
	printf("uint32 upper: %lu\n", max);
	}

	{
	int16 min = Numerical<int16>::_LowerValue();
	int16 max = Numerical<int16>::_UpperValue();
	printf("int16 lower: %d\n", min);
	printf("int16 upper: %d\n", max);
	}

	{
	uint16 min = Numerical<uint16>::_LowerValue();
	uint16 max = Numerical<uint16>::_UpperValue();
	printf("uint16 lower: %d\n", min);
	printf("uint16 upper: %d\n", max);
	}


	{
	int8 min = Numerical<int8>::_LowerValue();
	int8 max = Numerical<int8>::_UpperValue();
	printf("int8 lower: %d\n", min);
	printf("int8 upper: %d\n", max);
	}


	{
	uint8 min = Numerical<uint8>::_LowerValue();
	uint8 max = Numerical<uint8>::_UpperValue();
	printf("uint8 lower: %d\n", min);
	printf("uint8 upper: %d\n", max);
	}

		
	{
	float min = Numerical<float>::_LowerValue();
	float max = Numerical<float>::_UpperValue();
	printf("float lower: %f\n", min);
	printf("float upper: %f\n", max);
	}

	{
	float f1 = 0.0f;
	float f2 = 0.00000010f; - 0.0000009f;// до 6 знаков после плавающей точки; начиная с 7-го, выражение будет считаться равным
	printf("if (%f == %f): %d\n", f1, f2, Numerical<float>::_Equals(f1, f2));
	}

		
	{
	double min = Numerical<double>::_LowerValue();
	double max = Numerical<double>::_UpperValue();
	printf("double lower: %f\n", min);
	printf("double upper: %f\n", max);
	}

	{
	double d1 = 0.0;
	double d2 = 0.00000000000000010; - 0.0000000000000009;// до 14 знаков после плавающей точки; начиная с 15-го, выражение будет считаться равным
	printf("if (%f == %f): %d\n", d1, d2, Numerical<double>::_Equals(d1, d2));
	}

	

	getch();
	return 0;
}

