// example_01.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <Engine/Core/Random/Random.h>
#include "conio.h"
#include <string>
using std::string;





int _tmain(int argc, _TCHAR* argv[])
{
	float t;

	// поиск случайного числа от 0 до 1 миллион, которое меньше 100
	while((t = Random::_RandomFloat(0, 1000000)) > 100)
	{
		printf("\nRandonFloat(0-1kk).Find < 100: %f", t);
	}
	printf("\nRandonFloat: %f", t);
		
	
	getch();// пауза; ожидается ввод с клавиатуры для продолжения

	// поиск случайного числа от 0 до 1 миллион, которое меньше 10
	while((t = Random::_RandomFloat(0, 1000000)) > 10)
	{
		printf("\nRandonFloat(0-1kk).Find < 10: %f", t);
	}
	printf("\nRandonFloat: %f", t);// всегда 0
		

	getch();// пауза; ожидается ввод с клавиатуры для продолжения


	// поиск случайного числа от 0 до 100 тысяч, которое меньше 100
	while((t = Random::_RandomFloat(0, 100000)) > 100)
	{
		printf("\nRandonFloat(0-100k).Find < 100: %f", t);
	}
	printf("\nRandonFloat: %f", t);
		
	
	getch();// пауза; ожидается ввод с клавиатуры для продолжения

	// поиск случайного числа от 0 до 100 тысяч, которое меньше 10
	while((t = Random::_RandomFloat(0, 100000)) > 10)
	{
		printf("\nRandonFloat(0-100k).Find < 10: %f", t);
	}
	printf("\nRandonFloat: %f", t);
	

	getch();// пауза; ожидается ввод с клавиатуры для продолжения

	// поиск случайного числа от 0 до 100 тысяч, которое меньше 1
	while((t = Random::_RandomFloat(0, 100000)) > 1)
	{
		printf("\nRandonFloat(0-100k).Find < 1: %f", t);
	}
	printf("\nRandonFloat: %f", t);// всегда 0
	


	getch();
	return 0;
}

