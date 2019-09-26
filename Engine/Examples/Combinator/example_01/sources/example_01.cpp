// bNumerical.cpp: определяет точку входа для консольного приложения.
//



#include <iostream>
#include <conio.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/Combinator/Combinator.h>



void PrintList(SimpleList<char>& _l)
{
	for(int i = 0; i < _l.Size(); ++i)
	{
		printf("%c", _l[i]->key);
	}
	printf("\n");
}





int main() 
{
	int32 k = 2;
	SimpleList<char> listIn;

	listIn.AddToTail('1');
	listIn.AddToTail('0');
	//listIn.AddToTail('c');

	SimpleList<SimpleList<char>> listOut;

	Combinator::_MovePermutationsWithRepetitionToList(listIn, k, listOut);

	for(int i = 0; i < listOut.Size(); ++i)
	{
		PrintList(listOut[i]->key);
	}
	printf("%d/%d\n", listOut.Size(), Combinator::_AmountOfPermutationsWithRepetition(listIn.Size(), k));
	printf("\n");
	listOut.Clear();


	////////////////
	listIn.Clear();
	listIn.AddToTail('1');
	listIn.AddToTail('2');
	listIn.AddToTail('3');

	Combinator::_MovePermutationsToList(listIn, listOut);

	for(int i = 0; i < listOut.Size(); ++i)
	{
		PrintList(listOut[i]->key);
	}
	printf("%d/%d\n", listOut.Size(), Combinator::_AmountOfPermutations(listIn.Size()));
	printf("\n");
	listOut.Clear();

		
	////////////////
	listIn.Clear();
	listIn.AddToTail('a');
	listIn.AddToTail('b');


	Combinator::_MoveCombinationsWithRepetitionToList(listIn, k, listOut);

	for(int i = 0; i < listOut.Size(); ++i)
	{
		PrintList(listOut[i]->key);
	}
	printf("%d/%d\n", listOut.Size(), Combinator::_AmoutOfCombinationsWithRepetition(listIn.Size(), k));
	printf("\n");
	listOut.Clear();


	 	
	////////////////
	listIn.Clear();
	listIn.AddToTail('A');
	listIn.AddToTail('B');
	listIn.AddToTail('C');

	Combinator::_MoveCombinationsToList(listIn, listOut);

	for(int i = 0; i < listOut.Size(); ++i)
	{
		PrintList(listOut[i]->key);
	}
	printf("%d/%d\n", listOut.Size(), Combinator::_GetCombinations(listIn.Size()));
	printf("\n");
	listOut.Clear();



	getch();
    return 0;
}
