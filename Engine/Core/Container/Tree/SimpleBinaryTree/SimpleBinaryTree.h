#ifndef SIMPLEBINARYTREE_H
#define SIMPLEBINARYTREE_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
template<typename Type>
class SimpleBinaryTree
{
	public: enum
	{
		LEFT_CHILD = 0,
		RIGHT_CHILD = 1
	};
	public: class Node
	{
		friend class SimpleBinaryTree;
		private: Node* links[2];
		private: Node* parent;
		public: Type key;
		public: Node(Type _key): key(_key)
		{
			links[LEFT_CHILD] = links[RIGHT_CHILD] = parent = NIL;
		}
		public: Node(void)
		{
			links[LEFT_CHILD] = links[RIGHT_CHILD] = parent = NIL;
		}
		public: virtual ~Node(void)
		{}
		public: Node* GetParent(void)const
		{	
			return parent;
		}
		public: Node* GetChild(bool _direction)const
		{
			return links[_direction];
		}
	};
	private: int32 size;
	private: Node* root;
	public: SimpleBinaryTree(void): size(0), root(NIL)
	{}
	public: SimpleBinaryTree(const SimpleBinaryTree<Type>& _tree): size(0), root(NIL)
	{ 
		*this = _tree;
	}
	public: virtual ~SimpleBinaryTree(void)
	{
		Clear();
	}
	private: void AddLeaf(Node* _node, Node* _parent, bool _direction)
	{
		if(_node)
		{
			Node* it = new Node();
			*it = *_node;
			if(_parent)
			{
				_parent->links[_direction] = it;
				it->parent = _parent;
				size++;
			}
			else if(root == NIL)
			{
				root = it;
				size++;
			}
			AddLeaf(_node->links[LEFT_CHILD], it, LEFT_CHILD);
			AddLeaf(_node->links[RIGHT_CHILD], it, RIGHT_CHILD);
		}
	}
    public: SimpleBinaryTree<Type>& operator = (const SimpleBinaryTree<Type>& _tree)
	{
		if(this == &_tree)
		{
			return *this;
		}
	
		Clear();
		AddLeaf(_tree.root, NIL, LEFT_CHILD);

		return *this;
	}
	private: bool Equals(Node* _node1, Node* _node2)
	{
		if(_node1 && _node2)
		{ 
			if(!(*_node1 == *_node2))
			{
				return false;
			}
			if(Equals(_node1->links[LEFT_CHILD], _node2->links[LEFT_CHILD]))
			{ 
				if(Equals(_node1->links[RIGHT_CHILD], _node2->links[RIGHT_CHILD]))
				{
					return true;
				}
			}
			return false;
		}
		else if(_node1 == _node2)
		{
			return true;
		}
		return false;
	}
	public: bool operator == (const SimpleBinaryTree<Type>& _tree)
	{
		if(this == &_tree)
		{
			return true;
		}
		if(size != _tree.size) { return false; }

		return Equals(root, _tree.root);
	}
	public: bool IsEmpty(void)const
	{
		return (root == NIL); 
	}
	public: bool IsNotEmpty(void)const
	{
		return (root != NIL); 
	}
	public: int32 GetSize(void)const
	{ 
		return size;
	}
	public: Node* GetRoot(void)const
	{
		return root;
	}
	public: Node* GetExtremum(bool _direction)
	{
		if(root)
		{ 
			Node* extremum = root;
			while(extremum->links[_direction])
			{
				extremum = extremum->links[_direction];
			}
				
			return extremum;
		}
		return NIL;
	}
	public: Node* Find(Type _key)const
	{   
		if(root)
		{
			Node* search = root;

			do
			{ 
				if(search->key == _key)
				{ 
					return search;
				}
				search = search->links[search->key < _key];
			} 
			while(search);
		}
		return NIL;
	}
	public: bool Add(Type _key)
	{   
		if(root)
		{
			Node* search = root;

			do
			{
				if(search->key == _key)
				{
					return false;
				}
				bool direction = search->key < _key;

				if(search->links[direction])
				{
					search = search->links[direction];
					continue;
				}
				Node* it = new Node(_key);
				search->links[direction] = it;
				it->parent = search;
				size++;
				return true;
			}
			while(search);
		}
		else
		{
			Node* it = new Node(_key);
			root = it;
			size++;
			return true;
		}
		return false; 
	}
	public: void Clear(void)
	{
		if(root)
		{
			FastClear(root);
		
			root = NIL;
			size = 0;
		}
	}
	private: void FastClear(Node* _node)
	{ 
		if(_node)
		{
			FastClear(_node->links[LEFT_CHILD]);
			FastClear(_node->links[RIGHT_CHILD]);
			delete _node;
		}
	}
    public: bool Remove(Type _key)
	{
		return Remove(Find(_key));
	}
    public: bool RemoveExtremum(bool _direction)
	{
		return Remove(GetExtremum(_direction));
	}
	private: bool Remove(Node* _node)
	{
		if(_node)
		{ 
					
			bool direction = LEFT_CHILD;
			if(_node->links[LEFT_CHILD] && _node->links[RIGHT_CHILD]) 
			{
				Node* heir = _node->links[direction];
									    
				bool heirPosition = !direction;
				while(heir->links[heirPosition])
				{
					heir = heir->links[heirPosition];
				}

				Node* heirParent = heir->parent;
				Node* heirChild = heir->links[direction];
				if(_node == root)
				{ 
					heir->parent = NIL;
					root = heir;
				} 
				else
				{ 
					heir->parent = _node->parent;
					_node->parent->links[_node->parent->links[RIGHT_CHILD] == _node] = heir;
				}
				if(_node->links[direction] != heir)
				{
					heir->links[LEFT_CHILD] = _node->links[LEFT_CHILD];
					heir->links[RIGHT_CHILD] = _node->links[RIGHT_CHILD];
					_node->links[LEFT_CHILD]->parent = heir;
					_node->links[RIGHT_CHILD]->parent = heir;
					if(heirChild == NIL)
					{
						heirParent->links[heirParent->links[RIGHT_CHILD] == heir] = NIL;
						size--;
						
						delete _node;
						return true;
					}
					{
						heirParent->links[heirParent->links[RIGHT_CHILD] == heir] = heirChild;
						heirChild->parent = heirParent;
						size--;

						delete _node;
						return true;
					}
				}
				{
					heir->links[heirPosition] = _node->links[heirPosition];
					_node->links[heirPosition]->parent = heir; 
					size--;

					delete _node;
					return true;
				}
			}
								 
			if(_node->links[LEFT_CHILD] == NIL && _node->links[RIGHT_CHILD] == NIL)
			{ 
				if(_node->parent)
				{
					_node->parent->links[_node->parent->links[RIGHT_CHILD] == _node] = NIL;
				}
				else
				{
					root = NIL;
				}
				size--;

				delete _node;
				return true;
			}
			direction = _node->links[LEFT_CHILD] == NIL;
			if(_node->parent)
			{
				bool removeNodePosition = _node->parent->links[RIGHT_CHILD] == _node;
				_node->parent->links[removeNodePosition] = _node->links[direction];
				_node->links[direction]->parent = _node->parent;
			}  
			else
			{
				root = _node->links[direction];
				_node->links[direction]->parent = NIL;
			} 

			size--;
			delete _node;
			return true;
		}
		return false;
	}
	private: void InfixTraverse(Node* _root, SimpleList<Type>& _list)const
	{
		if(_root)
		{
			InfixTraverse(_root->links[LEFT_CHILD], _list); 
			_list.AddToTail(_root->key);
			InfixTraverse(_root->links[RIGHT_CHILD], _list);	
		}  
	}
	public: SimpleList<Type> ToList(void)const
	{
		SimpleList<Type> list;
		InfixTraverse(root, list);
		return list;
	}
	private: Node* CheckNode(Node* _node)
	{
		if(_node)
		{
			if(_node->GetChild(LEFT_CHILD))
			{
				if(_node->key < _node->links[LEFT_CHILD]->key)
				{
					return _node;
				}
			}

			if(_node->GetChild(RIGHT_CHILD))
			{
				if(_node->links[RIGHT_CHILD]->key < _node->key)
				{
					return _node;
				}
			}
		}

		return NIL;
	}
	public: Node* CheckTree(Node* _root)
	{
		if(_root)
		{
			if(CheckNode(_root))
			{
				return _root;
			}

			Node* leftNode = CheckTree(_root->GetChild(LEFT_CHILD));
			
			if(leftNode)
			{
				return leftNode;
			}

			Node* rightNode = CheckTree(_root->GetChild(RIGHT_CHILD));

			if(rightNode)
			{
				return rightNode;
			}
		}
		return NIL;
	}
	public: void Test(int32 _lower, int32 _upper)
	{
		Add(Random::_RandomInt32(_lower, _upper)); 

		if(root)
		{
			Node* search = Find(Random::_RandomInt32(_lower, _upper));
			search = Find(Random::_RandomInt32(_lower, _upper));
		
			if(search)
			{
				Remove(search);
			}
		}
		if(CheckTree(Root()))
		{
			WinAPI::_MessageBox("CheckTree", "Error");
		}
	}
};

#endif
