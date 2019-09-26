// example_03.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Call/Caller/Caller.h>
#include "conio.h"
#include <string>
using std::string;



class MyNode: public MagicNode
{
	// DATA
	public: int32 key;

	// METHODS
	public: MyNode(void): key(0)
	{
		printf("\nMyNode const");
	}

	public: MyNode(int32 _key): key(_key)
	{
		printf("\nMyNode const");
	}


	
	public: ~MyNode(void)
	{
		printf("\n~MyNode");
	}

	public: bool operator == (const MyNode& _node)
	{
		if(key == _node.key) { return true; }
		return false;
	}

	public: int32 Key(void)
	{
		return key;
	}
};


// выводит ключ узла
void PrintNodeKey(MyNode* _node)
{
	printf("(%d).Key: %d, ", _node, _node->Key());
}


// выводит состояние узла
void PrintNodeState(MagicBinaryTree<MyNode, int32, &MyNode::Key>& _tree, MyNode* _node)
{	
	if(_node)
	{
		printf("\n(%d).IsRoot(): %d", _node, _tree.IsRoot(_node));
		printf("\n(%d).IsLeaf(): %d", _node, _tree.IsLeaf(_node));
		printf("\n(%d).IsFull(): %d", _node, _tree.IsFull(_node));


		printf("\n(%d).Parent(): %d", _node, _tree.Parent(_node));
		printf("\n(%d).Child(0): %d", _node, _tree.Child(_node, 0));
		printf("\n(%d).Child(1): %d", _node, _tree.Child(_node, 1));
	}
}





// выводит состояние бинарного дерева
void PrintTreeState(MagicBinaryTree<MyNode, int32, &MyNode::Key>& _tree, StringANSI _treeName)
{
	printf("\nprint tree state...");
	printf("\n%s.Size: %d", _treeName.c_str(), _tree.Size());
	printf("\n%s.IsEmpty: %d", _treeName.c_str(), _tree.IsEmpty());
	printf("\n%s.Root: %d", _treeName.c_str(), _tree.Root());
	printf("\n%s.Height: %d", _treeName.c_str(), _tree.Height());
	printf("\n%s.ExcessHeight: %d", _treeName.c_str(), _tree.ExcessHeight());

	if(_tree.Size()) printf("\n%s:", _treeName.c_str());

	// вывод узлов...
	Caller<> caller;
	caller.Set(&PrintNodeKey);
	_tree.InfixTraverse(caller, true);
}




int _tmain(int argc, _TCHAR* argv[])
{
	// создать узел и поместить его в два дерева + удалить узел
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree2;

		printf("\nnew node(5)");
		MyNode* node = new MyNode(5);

		printf("\ntree1.Add(node)");
		tree1.Add(node);
		printf("\ntree2.Add(node)");
		tree2.Add(node);

		PrintTreeState(tree1, "tree1");
		PrintTreeState(tree2, "tree2");

		printf("\ndelete node(5)");
		delete node;
			
		PrintTreeState(tree1, "tree1");
		PrintTreeState(tree2, "tree2");

	}
	printf("\n");
	

	// создать узел и поместить его в два дерева + исключить узел
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree2;

		printf("\nnew node(6)");
		MyNode* node = new MyNode(6);

		printf("\ntree1.Add(node)");
		tree1.Add(node);
		printf("\ntree2.Add(node)");
		tree2.Add(node);
		PrintTreeState(tree1, "tree1");
		PrintTreeState(tree2, "tree2");

		printf("\ntree1.Remove(node(6))");
		tree1.Remove(node, true);
		PrintTreeState(tree1, "tree1");
		PrintTreeState(tree2, "tree2");

		PrintNodeState(tree1, node);
		PrintNodeState(tree2, node);
	}
	printf("\n");

	// создать узел и поместить его в два дерева + удалить контейнер
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree3;
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree4;

		printf("\nnew node(7)");
		MyNode* node = new MyNode(7);

		printf("\ntree3.Add(node)");
		tree3.Add(node);
		printf("\ntree4.Add(node)");
		tree4.Add(node);
		PrintTreeState(tree3, "tree3");
		PrintTreeState(tree4, "tree4");


		printf("\n tree3.Clear(MagicContainer::DELETE_NODE)");
		tree3.Clear(MagicContainer::DELETE_NODE);
		PrintTreeState(tree3, "tree3");
		PrintTreeState(tree4, "tree4");

		//PrintNodeState(tree1, node);
		//PrintNodeState(tree2, node);
	}
	printf("\n");


	// создать узел и поместить его в два дерева + присвоить одно дерево другому + сравнение деревьев
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree3;
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree4;

		printf("\nnew node(8)");
		MyNode* node = new MyNode(8);

		printf("\ntree3.Add(node)");
		tree3.Add(node);
		printf("\ntree4.Add(node)");
		tree4.Add(node);

		printf("\n tree3 = tree4");
		tree3 = tree4;
		PrintTreeState(tree3, "tree3");
		PrintTreeState(tree4, "tree4");

		printf("\n tree3 == tree4: %d", tree3 == tree4);
	}
	printf("\n");



	// добавить один и тот же узел в два разных дерева
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> t1;
		MagicBinaryTree<MyNode, int32, &MyNode::Key> t2;

		MyNode* node1 = new MyNode(55);
		MyNode* node2 = new MyNode(66);


		printf("\nt1.Add(node1:%d, %d)...", node1, node1->Key());
		t1.Add(node1);
		printf("\nt2.Add(node2:%d, %d)...", node2, node2->Key());
		t2.Add(node2);

		
		printf("\nt2.Add(node1)...");
		t2.Add(node1);
		
		printf("\nt1.Root/Size/Key: %d/%d/%d", t1.Root(), t1.Size(), t1.Root()->Key());
		printf("\nt2.Root/Size/Key: %d/%d/%d", t2.Root(), t2.Size(), t2.Root()->Key());


		printf("\ndelete node1...");
		delete node1;
		
		printf("\nt1.Root/Size: %d/%d", t1.Root(), t1.Size());
		printf("\nt2.Root/Size: %d/%d", t2.Root(), t2.Size());

	}


	getch();
	return 0;
}

