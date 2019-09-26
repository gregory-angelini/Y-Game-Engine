// example_02.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
#include <Engine/Core/Call/Caller/Caller.h>
#include "conio.h"
#include <windows.h>
#include <string>
using std::string;



class MyNode: public MagicNode
{
	// DATA
	public: int32 key;


	// METHODS
	public: MyNode(void): key(0)
	{}

	public: MyNode(int32 _key): key(_key)
	{}

	public: bool operator == (const MyNode& _node)
	{
		if(key == _node.key)
			return true;
		return false;
	}

	public: int32 Key(void)
	{
		return key;
	}
};

class MyTree: public MagicBinaryTree<MyNode, int32, &MyNode::Key>
{
	public: static void _PrintKey(MyNode* _node)
	{
		if(_node)
		{
			printf("%d ", _node->Key());
		}
	}
};




int _tmain(int argc, _TCHAR* argv[])
{
	MyTree tree1;
	MyTree tree2;

	Caller<> caller;
	caller.Set(&MyTree::_PrintKey); 
	

	// добавить 10 случайных узлов
	tree1.Add(new MyNode(5));
	tree1.Add(new MyNode(3));
	tree1.Add(new MyNode(8));
	tree1.Add(new MyNode(1));
	tree1.Add(new MyNode(7));

	printf("------5------\n");
	printf("----/---\\----\n");
	printf("---3-----8---\n");
	printf("-/-----/-----\n");
	printf("1----7-------\n");

	
	printf("tree.size == tree2.size: %d\n", tree1.Size() == tree2.Size());
 

	printf("tree1 keys: ");
	tree1.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");
		

	printf("tree2 keys: ");
	tree2.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");

	printf("tree1 == tree2: %d\n", tree1 == tree2);
	

	////////////////////////////////////////////////////////////////////////
	printf("--------------------------------------------------------\n");
	printf("tree2 = tree1...\n");
	tree2 = tree1;
	

	printf("tree1.size == tree2.size: %d\n", tree1.Size() == tree2.Size());


	printf("tree1 keys: ");
	tree1.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");
		

	printf("tree2 keys: ");
	tree2.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");

	printf("tree1 == tree2: %d\n", tree1 == tree2);
	

	///////////////////////////////////////////////////////////////
	printf("--------------------------------------------------------\n");
	MyNode* leaf = tree2.Find(1);
	int32 key = -1;
	if(leaf) { key = leaf->Key(); }
	tree2.Remove(leaf, 0);// удалить лист
	printf("tree2.Remove leaf: %d...\n", key);


	printf("tree1.size == tree2.size: %d\n", tree1.Size() == tree2.Size());

	
	printf("tree1 keys: ");
	tree1.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");
		
	
	printf("tree2 keys: ");
	tree2.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");

	printf("tree1 == tree2: %d\n", tree1 == tree2);
	

	//////////////////////////////////////////////////////////////////////
	printf("--------------------------------------------------------\n");
	tree2.Add(new MyNode(2));
	printf("tree2.Add leaf: 2...\n");


	printf("tree1.size == tree2.size: %d\n", tree1.Size() == tree2.Size());

	
	printf("tree1 keys: ");
	tree1.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");
		
	
	printf("tree2 keys: ");
	tree2.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");

	printf("tree1 == tree2: %d\n", tree1 == tree2);
	


		///////////////////////////////////////////////////////////////
	printf("--------------------------------------------------------\n");
	leaf = tree2.Find(2);
	key = -1;
	if(leaf) { key = leaf->Key(); }
	tree2.Remove(leaf, 0);// удалить лист
	printf("tree2.Remove leaf: %d...\n", key);


	printf("tree1.size == tree2.size: %d\n", tree1.Size() == tree2.Size());


	printf("tree1 keys: ");
	tree1.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");
		

	printf("tree2 keys: ");
	tree2.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");

	printf("tree1 == tree2: %d\n", tree1 == tree2);
	

	//////////////////////////////////////////////////////////////////////
	printf("--------------------------------------------------------\n");
	tree2.Add(new MyNode(1));
	printf("tree2.Add leaf: 1...\n");


	printf("tree1.size == tree2.size: %d\n", tree1.Size() == tree2.Size());

	
	printf("tree1 keys: ");
	tree1.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");
		
	
	printf("tree2 keys: ");
	tree2.InfixTraverse(caller, 0);// обход от минимального до максимального ключа
	printf("\n");

	printf("tree1 == tree2: %d\n", tree1 == tree2);
	

	getch();
	return 0;
}

