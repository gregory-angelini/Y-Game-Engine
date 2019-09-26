// example_01.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <Engine/Core/Random/Random.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h>
#include <Engine/Core/Call/Caller/Caller.h>
#include <Engine/Core/Parser/ArithmeticExprParser/ArithmeticExprParser.h>
#include <Engine/Core/Combinator/Combinator.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include "conio.h"
#include <string>
using std::string;


// пользовательский объект
class MyObject: public Object// базовые свойства объекта
{
	// METHODS
	// деструктор
	public: virtual ~MyObject(void)
	{
		printf("~%s\n", GetName().c_str());
	}
		
	// оператор ==
    public: bool operator == (const MyObject& _object)
	{
		if(GetName() == _object.GetName()) { return true; }
		return false;
    }
};

int main()
{
	MagicPointer<MyObject> pointer1;
	MagicPointer<MyObject> pointer2;

	MyObject* object1 = new MyObject();
	object1->Rename("object1");

	MyObject* object2 = new MyObject();
	object2->Rename("object2");

	printf("\npointer1.Size: %d\n", pointer1.GetSize());
	printf("pointer1.IsEmpty: %d\n", pointer1.IsEmpty());
	printf("pointer1.Content: %d\n", pointer1.GetContent());

	printf("\npointer1.Add(object1)...\n");
	pointer1.Add(object1);

	printf("\npointer1.Size: %d\n", pointer1.GetSize());
	printf("pointer1.IsEmpty: %d\n", pointer1.IsEmpty());
	printf("pointer1.Content: %d\n", pointer1.GetContent());


	printf("\npointer1.Clear()...\n");
	pointer1.Clear();

	printf("\npointer1.Size: %d\n", pointer1.GetSize());
	printf("pointer1.IsEmpty: %d\n", pointer1.IsEmpty());
	printf("pointer1.Content: %d\n", pointer1.GetContent());


	printf("\npointer1.Add(object2)...\n");
	pointer1.Add(object2);

	printf("\npointer2.Add(object2)...\n");
	pointer2.Add(object2);

	
	printf("\npointer1 == pointer2: %d...\n", pointer1 == pointer2);
	

	printf("\npointer1.Remove(object2)...\n");
	delete pointer1.GetContentAddress();
	printf("\npointer1.Size: %d\n", pointer1.GetSize());
	printf("pointer1.IsEmpty: %d\n", pointer1.IsEmpty());
	printf("pointer1.Content: %d\n", pointer1.GetContent());


	printf("\npointer1 == pointer2: %d...\n", pointer1 == pointer2);

	printf("\npointer1 = pointer2...\n");
	pointer1 = pointer2;

	printf("\npointer1.Size: %d\n", pointer1.GetSize());
	printf("pointer1.IsEmpty: %d\n", pointer1.IsEmpty());
	printf("pointer1.Content: %d\n", pointer1.GetContent());

	printf("\npointer2.Size: %d\n", pointer2.GetSize());
	printf("pointer2.IsEmpty: %d\n", pointer2.IsEmpty());
	printf("pointer2.Content: %d\n", pointer2.GetContent());



	printf("\ndelete object2...\n");
	delete object2;

	printf("\npointer1.Size: %d\n", pointer1.GetSize());
	printf("pointer1.IsEmpty: %d\n", pointer1.IsEmpty());
	printf("pointer1.Content: %d\n", pointer1.GetContent());

	printf("\npointer2.Size: %d\n", pointer2.GetSize());
	printf("pointer2.IsEmpty: %d\n", pointer2.IsEmpty());
	printf("pointer2.Content: %d\n", pointer2.GetContent());

	getch();
	return 0;
}