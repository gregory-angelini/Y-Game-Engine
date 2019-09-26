// example_01.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <Engine/Core/Random/Random.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h>
#include <Engine/Core/Call/Caller/Caller.h>
#include <Engine/Core/Parser/ArithmeticExprParser/ArithmeticExprParser.h>
#include <Engine/Core/Combinator/Combinator.h>
#include "conio.h"
#include <string>
using std::string;
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


ArithmeticExprParser<float> parser;

void PrintResult(StringANSI _expr)
{
	if(!parser.Parse(_expr))// проанализировать логическое выражение
	{
		if(!parser.Calculate())// вычислить выражение
		{
			printf("%s = %f\n", _expr.c_str(), parser.GetResult());	
		}
	}

	if(parser.GetCurrentError())// если возникли ошибки при парсинге/вычислении
	{
		printf("Error: %s", parser.GetCurrentErrorString().c_str());
		printf(": %s\n", parser.GetErrorArgument().c_str());
	}
}



int main()
{
	// FLOAT
	{
		Variable<float>* var0 = new Variable<float>(0.0f);
		var0->Rename("var0");
		AssetLibrary::_GetAssets().AddObject(var0);
		
		Variable<float>* var1 = new Variable<float>(1.0f);
		var1->Rename("var1");
		AssetLibrary::_GetAssets().AddObject(var1);
		
		Variable<float>* var2 = new Variable<float>(2.0f);
		var2->Rename("var2");
		AssetLibrary::_GetAssets().AddObject(var2);

		Variable<float>* var3 = new Variable<float>(3.0f);
		var3->Rename("var3");
		AssetLibrary::_GetAssets().AddObject(var3);
		
		Variable<float>* var4 = new Variable<float>(4.0f);
		var4->Rename("var4");
		AssetLibrary::_GetAssets().AddObject(var4);

		Variable<float>* var5 = new Variable<float>(5.0f);
		var5->Rename("var5");
		AssetLibrary::_GetAssets().AddObject(var5);

		Variable<float>* var6 = new Variable<float>(1.0f);
		var6->Rename("1u");
		AssetLibrary::_GetAssets().AddObject(var6);


		PrintResult("var2 + var1 - var5 * (var3 / var4)");
		printf("need: %f\n", var2->GetValue() + var1->GetValue() - var5->GetValue() * (var3->GetValue() / var4->GetValue()));
		printf("\n_________________________________\n");

		
		PrintResult("var2 * var1 / var5 + var3 - var4");
		printf("need: %f\n", var2->GetValue() * var1->GetValue() / var5->GetValue() + var3->GetValue() - var4->GetValue());
		printf("\n_________________________________\n");


		PrintResult("var2 / var1 * var5 - var3 + var4");
		printf("need: %f\n", var2->GetValue() / var1->GetValue() * var5->GetValue() - var3->GetValue() + var4->GetValue());
		printf("\n_________________________________\n");
		
		
		PrintResult("var2 + var1 - var5 * var3 / var4");
		printf("need: %f\n", var2->GetValue() + var1->GetValue() - var5->GetValue() * var3->GetValue() / var4->GetValue());
		printf("\n_________________________________\n");


		PrintResult("var2 - var1 + var5 / var3 * var4");
		printf("need: %f\n", var2->GetValue() - var1->GetValue() + var5->GetValue() / var3->GetValue() * var4->GetValue());
		printf("\n_________________________________\n");


		PrintResult("var2 - (var1 + var5) / (var3 * var4)");
		printf("need: %f\n", var2->GetValue() - (var1->GetValue() + var5->GetValue()) / (var3->GetValue() * var4->GetValue()));
		printf("\n_________________________________\n");




		PrintResult(" - (-var2)");
		printf("need: %f\n", -(-var2->GetValue()));
		printf("\n_________________________________\n");


		PrintResult("-var2");
		printf("need: %f\n", -var2->GetValue());
		printf("\n_________________________________\n");
	}
		
	PrintResult("3.14 + -2.0");
	PrintResult("0.9 + 0.2");
	PrintResult("var1 + 2.0");
	PrintResult("var2 * (1.5 + 0.1) - 1u");
	printf("\n_________________________________\n");

	

	/*
	//различные недопустимые выражения
    PrintResult("8:");
    PrintResult("8 /0.1 ");
    PrintResult(" 1.0 / 0.0");
	PrintResult("()");
	PrintResult("var(k)");
	PrintResult("(var)k");
	PrintResult("8.8var");
	PrintResult("3.12.345");
	PrintResult("3.1234567 + 2.0");
	PrintResult("-i)");
	PrintResult("--7");
	PrintResult("(i");
	PrintResult("+(j)");
	PrintResult("(j+)");
	PrintResult("a + / b");
	PrintResult("(var_2 +) var_1");
	*/ 





	getch();
	return 0;
}