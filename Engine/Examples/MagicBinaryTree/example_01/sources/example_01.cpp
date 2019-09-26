// example_01.cpp: определяет точку входа для консольного приложения.
//


#include <iostream>
#include <conio.h>


#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
//#include <Engine/Core/Container/Tree/SmartBinaryTree/SmartBinaryTree.h>
#include <Engine/Core/Call/Caller/Caller.h>


// узел дерева
class MyNode: public MagicNode
{
	// DATA
	private: int32 key;

	// METHODS
	// конструктор по умолчанию
	public: MyNode(void)
	{}


	// инициализирующий конструктор
	public: MyNode(int32 _key): key(_key)
	{}

	// возвращает ключ
	public: int32 Key(void)
	{
		return key;
	}

	// оператор сравнения
	public: bool operator == (const MyNode& _node)
	{
		if(key == _node.key)
			return true;
		return false;
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
	printf("\n(%d).IsRoot(): %d", _node, _tree.IsRoot(_node));
	printf("\n(%d).IsLeaf(): %d", _node, _tree.IsLeaf(_node));
	printf("\n(%d).IsFull(): %d", _node, _tree.IsFull(_node));


	printf("\n(%d).Parent(): %d", _node, _tree.Parent(_node));
	printf("\n(%d).Child(0): %d", _node, _tree.Child(_node, 0));
	printf("\n(%d).Child(1): %d", _node, _tree.Child(_node, 1));
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

	// вывод узлов...
	printf("\n");
	Caller<> caller;
	caller.Set(&PrintNodeKey);
	_tree.InfixTraverse(caller, true);
}


int main()
{
	// добавление узла + деструктор дерева
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;// создать бинарное дерево
		MyNode* myNode = new MyNode(5);//создать узел
		
		printf("\ntree1.Add(myNode(5)): %d", tree1.Add(myNode));
		PrintTreeState(tree1, "tree1");
	}
	printf("\n");

	
	// добавление узла + удаление узла через delete + защита от дубликатов
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;// создать бинарное дерево
		MyNode* myNode = new MyNode(6);//создать узел
		
		printf("\ntree1.Add(myNode(6)): %d", tree1.Add(myNode));
		printf("\ndelete myNode(6)");
		delete myNode;
		PrintTreeState(tree1, "tree1");
	
		myNode = new MyNode(5);//создать узел
		printf("\ntree1.Add(myNode(5)): %d", tree1.Add(myNode));

		myNode = new MyNode(5);//создать узел
		printf("\ntree1.Add(myNode(5)): %d", tree1.Add(myNode));

		myNode = new MyNode(6);//создать узел
		printf("\ntree1.Add(myNode(6)): %d", tree1.Add(myNode));
		PrintTreeState(tree1, "tree1");
	}
	printf("\n");


	// добавление узла + вывод состояния узла
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;// создать бинарное дерево
		MyNode* myNode = new MyNode(1);//создать узел
		printf("\ntree1.Add(myNode(1)): %d", tree1.Add(myNode));
		PrintNodeState(tree1, tree1.Root());

		myNode = new MyNode(0);//создать узел
		printf("\ntree1.Add(myNode(0)): %d", tree1.Add(myNode));
        PrintNodeState(tree1, tree1.Root());
	    
		myNode = new MyNode(2);//создать узел
		printf("\ntree1.Add(myNode(2)): %d", tree1.Add(myNode));
		PrintNodeState(tree1, tree1.Root());	

		myNode = new MyNode(4);//создать узел
		printf("\ntree1.Add(myNode(4)): %d", tree1.Add(myNode));
		
		myNode = new MyNode(5);//создать узел
		printf("\ntree1.Add(myNode(5)): %d", tree1.Add(myNode));
		
		PrintTreeState(tree1, "tree1");
	}
	printf("\n");


	// добавление узла + присваивание и сравнение деревьев + исключение узла + пересоздание узла
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;// создать бинарное дерево
		MyNode* myNode = new MyNode(1);//создать узел
		printf("\ntree1.Add(myNode(1)): %d", tree1.Add(myNode));
		
		myNode = new MyNode(2);//создать узел
		printf("\ntree1.Add(myNode(2)): %d", tree1.Add(myNode));

		printf("\ntree2 = tree1");
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree2 = tree1;
		PrintTreeState(tree1, "tree1");
		PrintTreeState(tree2, "tree2");

		printf("\ntree1 == tree2: %d", tree1 == tree2);
		
		printf("\ntree2.Remove(tree2.Root())");
		tree2.Remove(tree2.Root());
		PrintTreeState(tree2, "tree2");
		printf("\ntree1 == tree2: %d", tree1 == tree2);

		printf("\ntree2.Add(myNode(1)): %d", tree2.Add(new MyNode(1)));
		printf("\ntree1 == tree2: %d", tree1 == tree2);
		
		printf("\ntree1.Rebuild(tree1.Root())");
		tree1.Rebuild(tree1.Root(), true);
		printf("\ntree1 == tree2: %d", tree1 == tree2);
	}
	printf("\n");

	 

	// добавление узла + поиск узла + поиск экстремума + очищение дерева
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;// создать бинарное дерево
		MyNode* myNode = new MyNode(3);//создать узел
		printf("\ntree1.Add(myNode(3)): %d", tree1.Add(myNode));
		
		myNode = new MyNode(2);//создать узел
		printf("\ntree1.Add(myNode(2)): %d", tree1.Add(myNode));

		myNode = new MyNode(1);//создать узел
		printf("\ntree1.Add(myNode(1)): %d", tree1.Add(myNode));
		PrintTreeState(tree1, "tree1");

		printf("\ntree1.Extremum(false).Key: %d", tree1.Extremum(false)->Key());
		
		printf("\ntree1.Find(2).Key: %d", tree1.Find(2)->Key());

		printf("\ntree1.Clear(MagicContainer::REMOVE_NODE)");
		tree1.Clear(MagicContainer::REMOVE_NODE);
		PrintTreeState(tree1, "tree1");

		printf("\ntree1.Clear(MagicContainer::DELETE_NODE)");
		tree1.Clear(MagicContainer::DELETE_NODE);
		PrintTreeState(tree1, "tree1");
	}
	printf("\n");

	
	/*
	// добавление узла + очищение дерева
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;// создать бинарное дерево
		MyNode* myNode = new MyNode(3);//создать узел
		printf("\ntree1.Add(myNode(3)): %d", tree1.Add(myNode));
		
		myNode = new MyNode(2);//создать узел
		printf("\ntree1.Add(myNode(2)): %d", tree1.Add(myNode));

		myNode = new MyNode(1);//создать узел
		printf("\ntree1.Add(myNode(1)): %d", tree1.Add(myNode));
		PrintTreeState(tree1, "tree1");

		printf("\ntree1.Clear(MagicContainer::REMOVE_NODE)");
		tree1.Clear(MagicContainer::REMOVE_NODE);
		PrintTreeState(tree1, "tree1");
	}
	printf("\n");
	
	// добавление узла + удаление дерева
	{
		MagicBinaryTree<MyNode, int32, &MyNode::Key> tree1;// создать бинарное дерево
		MyNode* myNode = new MyNode(3);//создать узел
		printf("\ntree1.Add(myNode(3)): %d", tree1.Add(myNode));
		
		myNode = new MyNode(2);//создать узел
		printf("\ntree1.Add(myNode(2)): %d", tree1.Add(myNode));

		myNode = new MyNode(1);//создать узел
		printf("\ntree1.Add(myNode(1)): %d", tree1.Add(myNode));
		PrintTreeState(tree1, "tree1");

		printf("\ntree1.Clear(MagicContainer::DELETE_NODE)");
		tree1.Clear(MagicContainer::DELETE_NODE);
		PrintTreeState(tree1, "tree1");
	}
	 
	 */ 
	printf("\nEND");


	getch();
    return 0;
}

