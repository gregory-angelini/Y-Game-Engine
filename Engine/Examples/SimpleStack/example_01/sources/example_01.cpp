#include <string>
#include <cctype>
#include<iostream>
#include<map>
#include<conio.h>
using namespace std;
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Container/MagicNode.h>
#include <Engine/Core/Container/MagicContainer.h>
#include <Engine/Core/Container/SimpleStack/SimpleStack.h>



// отображает значение узлов
void PrintNods(SimpleStack<int>& _stack)
{
	for(int i = 0; i < _stack.Size(); ++i)
	{
		printf("key[%d]: %d\n", i, _stack.Get(i)->key);
	}
	printf("\n");
}


// отображает всю информацию о стеке
void PrintStack(SimpleStack<int>& _stack)
{
	printf("IsEmpty: %d\n", _stack.IsEmpty());
	printf("Top: %d\n", _stack.GetTop());
	printf("Size: %d\n", _stack.Size());

	PrintNods(_stack);
}


int main()
{
	SimpleStack<int> stack1;

	PrintStack(stack1);

	printf("Push(5)...\n");
	stack1.Push(5);
	PrintStack(stack1);

	printf("RemoveTop()...\n");
	stack1.RemoveTop();
	PrintStack(stack1);

	printf("--------------------------\n");	
	printf("Push(1)...\n");
	stack1.Push(1);
	PrintStack(stack1);

	printf("Push(1)...\n");
	stack1.Push(1);
	PrintStack(stack1);


	// обход стека и модификация данных
	printf("Traverse...\n");
	int i = 0;
	for(SimpleStack<int>::Node* it = stack1.GetTop(); it != NIL; it = it->GetBottom())
	{
		it->key += ++i;
	}
	PrintNods(stack1);

	printf("RemoveTop()...\n");
	stack1.RemoveTop();
	PrintStack(stack1);

		
	printf("Push(1)...\n");
	stack1.Push(1);
	PrintStack(stack1);


	printf("--------------------------\n");	
	SimpleStack<int> stack2;
	stack2 = stack1;
	printf("stack1...\n");
	PrintStack(stack1);
	printf("stack2...\n");
	PrintStack(stack2);
	
		
	printf("stack1 == stack2: %d\n\n", stack1 == stack2);
	
	for(int i = 0; i < stack2.Size(); ++i)
	{
		printf("index[%d]: %d\n", i, stack2.GetIndexOf(stack2.Get(i)));
	}
	printf("\n");

	printf("Clear()...\n");
	stack2.Clear();
	PrintStack(stack2);


	getch();
	return 0;
}
