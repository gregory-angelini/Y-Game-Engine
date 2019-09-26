

#include <iostream>
#include <conio.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>




// отобразить состояние списка
void PrintState(SimpleList<int32>& _l, StringANSI _listName)
{
	printf("\n");

	printf("%s.Size: %d\n", _listName.c_str(), _l.Size());
	printf("%s.Head: %d\n", _listName.c_str(), _l.Head());
	printf("%s.Tail: %d\n", _listName.c_str(), _l.Tail());
	printf("%s.IsEmpty: %d\n", _listName.c_str(), _l.IsEmpty());

	for(int i = 0; i < _l.Size(); ++i)
	{
		printf("%s.[%d]: %d", _listName.c_str(), i, _l[i]->data);
		
		if(_l[i + 1])
		{
			printf(", ");
		}
	}
	printf("\n");
}

int main()
{
	SimpleList<int32> list1;
	PrintState(list1, "list1");

	list1.AddToTail(1);
	list1.AddToHead(2);
	PrintState(list1, "list1");
	
	list1.Clear();
	printf("list1.Clear()");
	PrintState(list1, "list1");

	list1.AddToTail(0);
	list1.AddToTail(3);
	list1.AddToTail(5);
	PrintState(list1, "list1");
	printf("list1.Get(2): %d\n", list1.Get(2)->data);

	printf("list1.RemoveHead()\n");
	list1.RemoveHead();
	printf("list1.RemoveTail()\n");
	list1.RemoveTail();
	PrintState(list1, "list1");

	list1.AddToHead(2);
	list1.AddToHead(1);
	PrintState(list1, "list1");

	printf("list1.Remove(list1.Get(1))\n");
	list1.Remove(list1.Get(1));
	PrintState(list1, "list1");

	printf("list1.IndexOf(list1.Find(3)): %d\n", list1.IndexOf(list1.Find(3)));
	//printf("list1.IndexOf(3): %d\n", list1.IndexOf(3));
	
	list1.AddToHead(1);
	list1.AddToHead(6);
	PrintState(list1, "list1");

	printf("list1.Inverse()\n");
	list1.Inverse();
	PrintState(list1, "list1");

	
	list1.Unique();
	printf("list1.Unique()\n");
	PrintState(list1, "list1");


	SimpleList<int32> list2;
	list2 = list1;
	printf("list2 = list1\n");
	PrintState(list2, "list2");
	
	printf("\nlist1 == list2: %d\n", list1 == list2);
	printf("list2.Remove(list2.Get(0))\n");
	list2.Remove(list2.Get(0));
	PrintState(list2, "list2");
	printf("list1 == list2: %d\n", list1 == list2);


	printf("\nlist1 += list2\n");
	list1 += list2;
	PrintState(list1, "list1");
	PrintState(list2, "list2");

	printf("\nlist1.Sort(true)\n");
	list1.Sort(true);
	PrintState(list1, "list1");
	
	printf("\nlist1.Sort(false)\n");
	list1.Sort(false);
	PrintState(list1, "list1");
	getch();
    return 0;
}

