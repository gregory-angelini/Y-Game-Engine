// example_01.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <Engine/Core/Random/Random.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h>
#include <Engine/Core/Call/Caller/Caller.h>
#include "conio.h"
#include <string>
using std::string;




/* Здесь тестируются вызовы различных функций и методов с различной сигнатурой, с помощью объекта Caller */

void F1(void)
{
	printf("func void(void): 10\n");
}

void F2(int32 _i)
{
	printf("func void(int32): %d\n", _i);
}



float F3(void)
{
	printf("func float(void)\nreturn:");
	return 3.14f;
}

int32 F4(int32 _i)
{
	printf("func int32(int32)\nreturn:");
	return _i;
}
			

void F7(int32 _i, float _t)
{
	printf("func void(int32, float): %d,%f\n", _i, _t);
}

void F8(int32 _i, float _t, char _a)
{
	printf("func void(int32, float, char): %d,%f,%c\n", _i, _t, _a);
}


void F9(int32 &_i)
{
	_i = 0;
}


void F10(int32 *_i)
{
	(*_i) = 0;
}

int32 k = 0;

int32& F11(void)
{
	k++;
	return k;
}


class TestClassBase
{
	public: void F1(void)
	{
		printf("method void F1(void): 10\n");
	}

	public: void F1(int32 _i)
	{
		printf("method void F1(_i): %d\n", _i);
	}

    public: void F2(int32 _i)
	{
		printf("method void(int32): %d\n");
	}


	public: void F7(int32 _i, float _t)
	{
		printf("method void(int32, float): %d,%f\n", _i, _t);
	}

	public: void F8(int32 _i, float _t, char _a)
	{
		printf("method void(int32, float, char): %d,%f,%c\n", _i, _t, _a);
	}


	public: float F3(void)
	{
		printf("method float(void)\nreturn:");
		return 3.14f;
	}

	public: int32 F4(int32 _i)
	{
		printf("method int32(int32)\nreturn:");
		return _i;
	}
			

	public: void PrintMessageConst0(void)const
	{
		printf("method void(void)const: message\n");
	}




	public: static void StaticPrintFloat(float _value)
	{
		printf("static method void(float): %.2f\n", _value);
	}


	public: virtual void Base(void)
	{
		printf("Base");
	}


	public: template<typename K> void TemplatePrint(K _value)
	{
		printf("template void(K): %d", _value);
	}
};


class TestClassDerived: public TestClassBase
{
	// переопределение виртуального метода
	public: virtual void Base(void)
	{
		printf("Derived");
	}
};



template<typename K> void Get(K _value)
{
	printf("template void(K): %d", _value);
}



TestClassBase testObject1;
TestClassDerived testObject2;
TestClassBase* testObject3 = new TestClassDerived;



int main()
{
	/*
	// неправильные аргументы коллера
	{
		Caller<> caller1(&testObject1, &TestClassBase::F2);
		caller1("42", 8, "sds");
		caller1();  
		printf("\n");
	}
	*/ 



	// вызов методов класса с параметрами и возвращаемым значением
	{
		Caller<> caller(&testObject1, &TestClassBase::F2);
		caller(10); 
	}

	{
		Caller<> caller(&testObject1, &TestClassBase::F7);
		caller(10, 0.1f);
	}

	{
		Caller<> caller(&testObject1, &TestClassBase::F8);
		caller(10, 0.1f, 't');
	}

 
	{
		Caller<float> caller(&testObject1, &TestClassBase::F3);
		printf(":%f\n", caller());
	}
	

	{  
		Caller<int32> caller(&testObject1, &TestClassBase::F4);
		printf(":%d\n", caller(10));
	}


	printf("\n\n");


	/*
	// возвращение ссылки в качестве возвращаемого значения
	{
		Caller<int32&> caller(F11);
		int32& var = caller();
		printf(":%d/%d\n", var, caller());
	}
	*/ 



	
	/*
	// вызов функций и копирование коллеров...
	{
		Caller<> caller1(&F1);
		caller1();


	    Caller<> caller2 = caller1;// копирование функтора
		printf("caller2 = caller1...\n"); 
		caller2();
	}

	
	{
		Caller<> caller(&F2);
		caller(10); 
	}

	{
		Caller<> caller(&F7);
		caller(10, 0.1f);
	}

	{
		Caller<> caller(&F8);
		caller(10, 0.1f, 't');
	}


	{
		Caller<float> caller(&testObject1, &TestClassBase::F3);
		printf(":%f\n", caller());
	}
	

	{
		Caller<int32> caller(&testObject1, &TestClassBase::F4);
		printf(":%d\n", caller(10));
	}

	printf("\n\n");
	*/



	/*
	// вызов константных методов
	{  
		Caller<> caller1(&testObject1, &TestClassBase::PrintMessageConst0);
		caller1();
	}

	printf("\n\n");
	*/ 



	/*
	// вызов статических методов
	{
		Caller<> caller1(&TestClassBase::StaticPrintFloat);
		caller1(1.11f);
	}
	

	// вызов шаблонных методов
	{
		Caller<> caller1(&testObject1, &TestClassBase::TemplatePrint<int>);
		caller1(1777);
	}
	 
	printf("\n\n");

	*/ 


	
/*
	// virtual method void(void) 
	// вызов TestClassDerived->TestClassDerived::Base
	{
		// интерпретация вызова виртуальной функции зависит от типа объекта для которого он вызывается  
		Caller<> caller(&testObject2, &TestClassDerived::Base);
		caller();
		printf(" - need ");
		testObject2.Base(); 
		printf("\n");
	}


	
	// вызов TestClassDerived->TestClassBase::Base
	{
		Caller<> caller(&testObject2, &TestClassBase::Base);
		caller();
		printf(" - need ");
		testObject2.TestClassBase::Base();
		printf("\n");
	}
	

 

	// вызов (TestClass)TestClassDerived->TestClassDerived::Base
	{
		Caller<> caller(static_cast<TestClassBase*>(&testObject2), &TestClassDerived::Base);
		caller();
		printf(" - need ");
		(static_cast<TestClassBase*>(&testObject2))->Base();
		printf("\n");
	}
	
	

	// вызов (TestClass)TestClassDerived->TestClassBase::Base
	{
		Caller<> caller(static_cast<TestClassBase*>(&testObject2), &TestClassBase::Base);
		caller();
		printf(" - need ");
		(static_cast<TestClassBase*>(&testObject2))->TestClassBase::Base();
		printf("\n"); 
	}


	
	// вызов (TestClass)TestClassDerived->TestClassBase::Base
	{
		Caller<> caller(testObject3, &TestClassBase::Base);
		caller();
		printf(" - need ");
		testObject3->Base();
		printf("\n"); 
	} 

	*/ 


	/*
    // попытка вызова перегруженной функции
	Caller<> caller1(&testObject1, &TestClassBase::F1);
	caller1();
	*/ 



	getch();
	return 0;
}