// example_03.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include "conio.h"
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
#include <string>
using std::string;



class MyNode: public MagicNode
{
	// DATA
	public: int32 key;

	// METHODS
	// конструктор по умолчанию
	public: MyNode(void): key(0)
	{
		printf("\nMyNode const");
	}

	// инициализирующий конструктор
	public: MyNode(int32 _key): key(_key)
	{
		printf("\nMyNode const");
	}


	
	// деструктор
	public: ~MyNode(void)
	{
		printf("\n~MyNode");
	}

	// оператор сравнения
	public: bool operator == (const MyNode& _node)
	{
		if(key == _node.key)
		{
			return true;
		}
		return false;
	}


	public: int32 Key(void)const
	{
		return key;
	}
};




// выводит состояние списка _list
void PrintMagicListState(MagicList<MyNode>& _list, StringANSI _string)
{
	printf("\n%s.Head: %d", _string.c_str(), _list.Head());
	printf("\n%s.Tail: %d", _string.c_str(), _list.Tail());
	printf("\n%s.IsEmpty: %d", _string.c_str(), _list.IsEmpty());
	printf("\n%s.Size: %d", _string.c_str(), _list.Size());
}

// выводит состояние дерева _tree
void PrintMagicBinaryTreeState(MagicBinaryTree<MyNode>& _tree, StringANSI _string)
{
	printf("\n%s.Root: %d", _string.c_str(), _tree.Root());
	printf("\n%s.IsEmpty: %d", _string.c_str(), _tree.IsEmpty());
	printf("\n%s.Size: %d", _string.c_str(), _tree.Size());
}


// выводит состояние узла _node
void PrintNodeState(MyNode* _node, MagicList<MyNode>& _list, StringANSI _string)
{
	if(_node)
	{
		printf("\n%s[%d]: ", _string.c_str(), _list.IndexOf(_node));

		printf("\n(%d).Key: %d", _node, _node->Key());
		printf("\n(%d).Left: %d", _node, _list.LeftOf(_node));
		printf("\n(%d).Right: %d", _node, _list.RightOf(_node));
	}
}

// вывести состояние всех узлов списка _list
void PrintAllNodes(MagicList<MyNode>& _list, StringANSI _string)
{
	// обойти весь список
	MyNode* it = _list.Head();
	while(it)
	{
		PrintNodeState(static_cast<MyNode*>(it), _list, _string);// вывести состояние узла
		it = _list.RightOf(it);// взять правого соседа
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	// Inverse + Unique + Sort
	{
		MagicList<MyNode> list1;
		MagicList<MyNode> list2;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		list1.AddToTail(node = new MyNode(2));
		list1.AddToTail(node = new MyNode(2));
		PrintAllNodes(list1, "list1");

		printf("\nlist1.Inverse()...");
		list1.Inverse();
		PrintAllNodes(list1, "list1");

		printf("\nlist1.Unique(&MyNode::Key, 0)...");
		list1.Unique(&MyNode::Key, 0);
		PrintAllNodes(list1, "list1");

		list1.AddToTail(node = new MyNode(8));
		list1.AddToTail(node = new MyNode(5));
		
		
		PrintAllNodes(list1, "list1");
		printf("\nlist1.Sort(&MyNode::Key, true)...");
		list1.Sort(&MyNode::Key, true);
		PrintAllNodes(list1, "list1");
	}
	printf("\n");


	// FindSimilar + GetRandom
	{
		MagicList<MyNode> list1;
		MagicList<MyNode> list2;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		list1.AddToTail(node = new MyNode(3));
		list1.AddToTail(node = new MyNode(3));
		PrintAllNodes(list1, "list1");

		printf("\nlist1.FindSimilar(&MyNode::Key, 3, &list2, MagicContainer::TRANSFER)...");
		list1.FindSimilar(3, &MyNode::Key, &list2, MagicContainer::TRANSFER);
		PrintAllNodes(list1, "list1");
		PrintAllNodes(list2, "list2");

		printf("\nlist1.GetRandom(): %d", list1.GetRandom());
	}
	printf("\n");


	// RandomSwap
	{
		MagicList<MyNode> list1;
		MagicList<MyNode> list2;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		list1.AddToTail(node = new MyNode(2));
		list1.AddToTail(node = new MyNode(3));
		
		list2.AddToTail(node = new MyNode(4));
		list2.AddToTail(node = new MyNode(5));
		list2.AddToTail(node = new MyNode(6));
		PrintAllNodes(list1, "list1");
		PrintAllNodes(list2, "list2");
	
		printf("\nlist1.RandomSwap(list1, 2)...");
		list1.RandomSwap(list2, 2);
		PrintAllNodes(list1, "list1");
		PrintAllNodes(list2, "list2");
	}
	printf("\n");

	// Swap + Swap
	{
		MagicList<MyNode> list1;
		MagicList<MyNode> list2;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		list1.AddToTail(node = new MyNode(2));

		list2.AddToTail(node = new MyNode(4));
		list2.AddToTail(node = new MyNode(5));

		PrintAllNodes(list1, "list1");
		PrintAllNodes(list2, "list2");

		printf("\nlist1.Swap(list2)...");
		list1.Swap(list2);
		
		PrintAllNodes(list1, "list1");
		PrintAllNodes(list2, "list2");

		printf("\n_Swap(list1.Head()->Adress(&list1), list2.Head()->Adress(&list2))...");
		MagicList<MyNode>::_Swap(dynamic_cast<MagicList<MyNode>::LogicAdressDerived*>(list1.Head()->Adress(&list1)), dynamic_cast<MagicList<MyNode>::LogicAdressDerived*>(list2.Head()->Adress(&list2)));
			
		PrintAllNodes(list1, "list1");
		PrintAllNodes(list2, "list2");

	}
	printf("\n");

	// '=', '=='
	{
		MagicList<MyNode> list1;
		MagicList<MyNode> list2;

		MyNode* node = new MyNode(1);
		list1.AddToTail(node);
		list1.AddToTail(node = new MyNode(2));
		list2.AddToTail(node);
		PrintAllNodes(list1, "list1");
		PrintAllNodes(list2, "list2");
		printf("\nlist1 == list2: %d", list1 == list2); 

		printf("\nlist2 = list1...");
		list2 = list1;
		PrintAllNodes(list1, "list1");
		PrintAllNodes(list2, "list2");
		printf("\nlist1 == list2: %d", list1 == list2); 
	}
	printf("\n");

	

	getch();
	return 0;
}

