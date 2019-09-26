#include <string>
#include <cctype>
#include<iostream>
#include<map>
#include<conio.h>
#include <Engine/Core/Parser/BooleanExprParser/BooleanExprParser.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


BooleanExprParser parser;


void PrintResult(StringANSI _expr)
{
	if(!parser.Parse(_expr))// проанализировать логическое выражение
	{
		if(!parser.Calculate())// вычислить выражение
		{
			printf("%s = %d\n", _expr.c_str(), parser.GetResult());	
		}
	}

	if(parser.GetCurrentError())// если возникли ошибки при парсинге
	{
		printf("Error: %s", parser.GetCurrentErrorString().c_str());
		printf(": %s\n", parser.GetErrorArgument().c_str());
	}
}

void PrintBinaryTableForNOT(void)
{
	printf("__________NOT_____________\n");
	
	for(int i = 0; i < 2; ++i)
	{
		if(!parser.Parse("~" + NumberToString(i)))// проанализировать логическое выражение
		{
			if(!parser.Calculate())// вычислить выражение
			{
				printf("%s |%d\n", NumberToString(i).c_str(), parser.GetResult());	
			}
		}

		if(parser.GetCurrentError())// если возникли ошибки при парсинге
		{
			printf("Error: %s", parser.GetCurrentErrorString().c_str());
			printf(": %s\n", parser.GetErrorArgument().c_str());
		}
	}

	printf("__________________________\n");
}




void PrintBinaryTableForAND(void)
{
	printf("__________AND_____________\n");
	
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(!parser.Parse(NumberToString(i) + "&" + NumberToString(j)))// проанализировать логическое выражение
			{
				if(!parser.Calculate())// вычислить выражение
				{
					printf("%s %s|%d\n", NumberToString(i).c_str(), NumberToString(j).c_str(), parser.GetResult());	
				}
			}

			if(parser.GetCurrentError())// если возникли ошибки при парсинге
			{
				printf("Error: %s", parser.GetCurrentErrorString().c_str());
				printf(": %s\n", parser.GetErrorArgument().c_str());
			}
		}
	}

	printf("__________________________\n");
}
 


void PrintBinaryTableForOR(void)
{
	printf("___________OR_____________\n");
	
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(!parser.Parse(NumberToString(i) + "|" + NumberToString(j)))// проанализировать логическое выражение
			{
				if(!parser.Calculate())// вычислить выражение
				{
					printf("%s %s|%d\n", NumberToString(i).c_str(), NumberToString(j).c_str(), parser.GetResult());	
				}
			}

			if(parser.GetCurrentError())// если возникли ошибки при парсинге
			{
				printf("Error: %s", parser.GetCurrentErrorString().c_str());
				printf(": %s\n", parser.GetErrorArgument().c_str());
			}
		}
	}
	printf("__________________________\n");
}
 

void PrintBinaryTableForXOR(void)
{
	printf("___________XOR____________\n");
	
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(!parser.Parse(NumberToString(i) + "^" + NumberToString(j)))// проанализировать логическое выражение
			{
				if(!parser.Calculate())// вычислить выражение
				{
					printf("%s %s|%d\n", NumberToString(i).c_str(), NumberToString(j).c_str(), parser.GetResult());	
				}
			}

			if(parser.GetCurrentError())// если возникли ошибки при парсинге
			{
				printf("Error: %s", parser.GetCurrentErrorString().c_str());
				printf(": %s\n", parser.GetErrorArgument().c_str());
			}
		}
	}
	printf("__________________________\n");
}


int main()
{
	Variable<bool>* var1 = new Variable<bool>(false);
	var1->Rename("0");
	AssetLibrary::_GetAssets().AddObject(var1);

	Variable<bool>* var2 = new Variable<bool>(true);
	var2->Rename("1");
	AssetLibrary::_GetAssets().AddObject(var2);


	PrintBinaryTableForNOT();
	PrintBinaryTableForAND();
	PrintBinaryTableForOR();
	PrintBinaryTableForXOR();

   
	// выражение из одной переменной
	printf("_____SINGLE_VARIABLE______\n");
	PrintResult("0");
	PrintResult("1");
	printf("__________________________\n");


	printf("________PRIORITY__________\n");
	PrintResult("~0 & 0");
	printf("NEED: ~0 & 0 = %d", ~0 & 0);
	printf("\n\n");

	PrintResult("~0 & ~0");
	printf("NEED: ~0 & ~0 = %d", (~0 & ~0) ? 1 : 0);
	printf("\n\n");

	PrintResult("1 ^ 1 & 0");
	printf("NEED: 1 ^ 1 & 0 = %d", 1 ^ 1 & 0);
	printf("\n\n");

	PrintResult("(1 ^ 1) & 0");
	printf("NEED: (1 ^ 1) & 0 = %d", (1 ^ 1) & 0);
	printf("\n\n");

	PrintResult("1 | 0 & 1 ^ 1");
	printf("NEED: 1 | 0 & 1 ^ 1 = %d", 1 | 0 & 1 ^ 1);
	printf("\n\n");

	PrintResult("((1 | 0) & 1) ^ 1");
	printf("NEED: ((1 | 0) & 1) ^ 1 = %d", ((1 | 0) & 1) ^ 1);
	printf("\n\n");

	printf("__________________________\n");


	printf("________BRACKETS__________\n");
	PrintResult("~(0)");
	
	PrintResult("~(1 & ~1)");
	printf("BRACKETS LESS: "); PrintResult("~1 & ~1");

	PrintResult("(1 ^ ((1 ^ 1)) & ((1 ^ 1)) ^ 1)");
	printf("BRACKETS LESS: "); PrintResult("1 ^ 1 ^ 1 & 1 ^ 1 ^ 1");
	



	/*
	// различные недопустимые выражения
	PrintResult(":");
	PrintResult("~");
	PrintResult("|");
	PrintResult("~~0");
	PrintResult("&0");
	PrintResult("0|");
	PrintResult("()");
	PrintResult("(");
	PrintResult(")");
	PrintResult("(0)1");
	PrintResult("0(1)");
	PrintResult("(1 |) 0");
	*/ 
	

	getch();
	return 0;
}
