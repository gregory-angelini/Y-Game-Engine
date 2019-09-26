// example_03.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <Engine/Core/Random/Random.h>
#include "conio.h"
#include <string>
using std::string;





int _tmain(int argc, _TCHAR* argv[])
{

	int32 t;
	int i;

	i = 0;
	// поиск случайного числа от 0 до 1 миллион, которое меньше 100
	while((t = Random::_RandomInt32(0, 1000000)) > 100)
	{
		printf("\nRandonInt(0-1kk).Find < 100[%d]: %d", i, t);
		++i;
	}
	printf("\nRandonInt: %d", t);
		
	
	getch();// пауза; ожидается ввод с клавиатуры для продолжения

	i = 0;
	// поиск случайного числа от 0 до 1 миллион, которое меньше 10
	while((t = Random::_RandomInt32(0, 1000000)) > 10)
	{
		printf("\nRandonInt(0-1kk).Find < 10[%d]: %d", i, t);
		++i;
	}
	printf("\nRandonInt: %d", t);// всегда 0
		

	getch();// пауза; ожидается ввод с клавиатуры для продолжения


	i = 0;
	// поиск случайного числа от 0 до 10 миллион, которое меньше 1000
	while((t = Random::_RandomInt32(0, 10000000)) > 1000)
	{
		printf("\nRandonInt(0-10kk).Find < 1000[%d]: %d", i, t);
		++i;
	}
	printf("\nRandonInt: %d", t);
		
	
	getch();// пауза; ожидается ввод с клавиатуры для продолжения

	i = 0;
	// поиск случайного числа от 0 до 10 миллион, которое меньше 100
	while((t = Random::_RandomInt32(0, 10000000)) > 100)
	{
		printf("\nRandonInt(0-10kk).Find < 100[%d]: %d", i, t);
		++i;
	}
	printf("\nRandonInt: %d", t);
	

	getch();// пауза; ожидается ввод с клавиатуры для продолжения

	i = 0;
	// поиск случайного числа от 0 до 10 миллион, которое меньше 10
	while((t = Random::_RandomInt32(0, 10000000)) > 10)
	{
		printf("\nRandonInt(0-10kk).Find < 10[%d]: %d", i, t);
		++i;
	}
	printf("\nRandonInt: %d", t);// всегда 0
	


	getch();
	return 0;
}

