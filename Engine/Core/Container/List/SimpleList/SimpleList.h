#ifndef SIMPLELIST_H
#define SIMPLELIST_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Common/Common.h>
#include <Engine/Core/Memory/Memory.h>
template<typename Type>
class SimpleList
{
	public: class Node
	{
		template<typename Type> friend class SimpleList;
		private: Node* right;
		private: Node* left;
		public: Type key;
		public: Node(Type _key): key(_key), right(NIL), left(NIL)
		{}
		public: virtual ~Node(void)
		{}
		public: inline Node* GetRight(void)const 
		{
			return right;
		}
		public: inline Node* GetLeft(void)const 
		{
			return left;
		}
	};
	private: Node* head;
	private: Node* tail;
	private: int32 size;
	public: SimpleList(void): head(NIL), tail(NIL), size(0)
	{}
	public: SimpleList(const SimpleList<Type>& _list): head(NIL), tail(NIL), size(0)
	{ 
		*this = _list;
	}
	public: SimpleList(int32 _size, Type _key): head(NIL), tail(NIL), size(0)
	{
		Resize(_size, _key);
	}
	public: virtual ~SimpleList(void)
	{
		Clear();
	}
	public: int32 GetSize(void)const
	{
		return size; 
	}
	public: bool IsEmpty(void)const
	{
		return (head == NIL);
	}
	public: bool IsNotEmpty(void)const
	{
		return (head != NIL);
	}
	public: Node* AddToTail(Type _key)
	{ 
		Node* it = new Node(_key);
		if(head)
		{
			it->left = tail;
			tail->right = it;
		}
		else
		{
			head = it;
		}
		tail = it;
		++size;
		return it;
	}
	public: Node* AddToHead(Type _key)
	{
		Node* it = new Node(_key);
		if(head)
		{
			it->right = head;
			head->left = it;
		}
		else
		{
			tail = it;
		}
		head = it;
		++size;
		return it;
	}
	public: Node* AddUniqueToHead(Type _key)
	{
		if(Find(_key) == NIL) { return AddToHead(_key); }
		return NIL;
	}
	public: Node* AddUniqueToTail(Type _key)
	{ 
		if(Find(_key) == NIL) { return AddToTail(_key); }
		return NIL;
	}
	public: void Resize(int32 _size, Type _key)
	{
		if(_size == size) { return; }

		int32 newSize = _size - size;
		
		if(newSize < 0)
		{
			newSize = abs(newSize);
			for(int32 i = 0; i < newSize; ++i)
			{
				RemoveTail();
			}
		}
		else
		{
			for(int32 i = 0; i < newSize; ++i)
			{
				AddToTail(_key);
			}
		}
	}
	public: void Clear(void)
	{
		Node* it;
		while(head)
		{
			it = head;
			head = head->right;
			delete it;
		}

		tail = NIL;
		size = 0;
	}
	public: void RemoveHead(void)
	{
		if(head)
		{
			Node* it = head;
			head = head->right;
			if(head)
			{
				head->left = NIL;
			}
			else
			{
				tail = NIL;
			}

			delete it;
			--size;
		}
	}
	public: void RemoveTail(void)
	{
		if(tail)
		{
			Node* it = tail;
			tail = tail->left;
			if(tail)
			{
				tail->right = NIL;
			}
			else
			{
				head = NIL;
			}

			delete it;
			--size;
		}
	}
    public: bool RemoveAt(int32 _index)
	{ 
		return Remove(Get(_index));
	}
    public: bool Remove(Type _key)
	{ 
		return Remove(Find(_key));
	}
    private: bool Remove(Node* _node)
	{ 
		if(_node)
		{
			if(_node == head) 
			{
				RemoveHead();
				return true;
			} 
			else if(_node == tail)
			{
				RemoveTail();
				return true;
			}
			_node->left->right = _node->right;
			_node->right->left = _node->left;

			delete _node;
			--size;
			return true;
		}
		return false;
	}
	public: Node* GetHead(void)const
	{
		return head; 
	}
	public: Node* GetTail(void)const
	{
		return tail; 
	}
	public: Node* Get(int32 _index)const
	{
		if(_index < size && _index > -size)
		{
			Node* it = NIL;

			if(_index < 0)
			{
				_index = (size - 1) + _index;
			}
			if((size / 2) >= _index)
			{
				it = head;
				for(int32 i = 0; i < _index; ++i)
				{
					it = it->right;
				}
			}
			else
			{
				it = tail;

				_index = (size - 1) - _index;
				for(int32 i = 0; i < _index; ++i)
				{
					it = it->left; 
				}
			}
			return it;
		}
		return NIL;
	}
	public: int32 IndexOf(const Type& _key)const
	{
		Node* it = head;
		int32 index = 0;
		while(it)
		{  
			if(it->key == _key)
			{ 
				return index;
			}

			it = it->right;
			++index;
		}

		return -1;
	}
	public: Node* Find(Type _key)const
	{
		Node* it = head;
		while(it)
		{  
			if(it->key == _key)
			{ 
				return it;
			}

			it = it->right;
		}

		return NIL;
	}
	public: Node* GetExtremum(bool _max)const
	{ 
		if(head)
		{
			Node* search = head;
			Node* extremum = search;

			if(_max)
			{
				while(search)
				{
					if(extremum->key < search->key)
					{
						extremum = search;
					}
					search = search->right;
				}
			}
			else
			{
				while(search)
				{
					if(extremum->key > search->key)
					{
						extremum = search;
					}
					search = search->right;
				}
			}
			return extremum;
		}
		return NIL;
	}
	public: void SwapKey(int32 _index1, int32 _index2)
	{
		if(_index1 != _index2)
		{
			Node* node1 = Get(_index1);
			Node* node2 = Get(_index2);

			if(node1 && node2)
			{
				Common::_Swap(node1->key, node2->key);
			}
		}
	}
	public: void Inverse(void)
	{
		if(size > 1)
		{
			Node* it = head;
            Node* right;

			while(it)
			{
				right = it->right;
				it->right = it->left;
				it->left = right;

				it = right;
			}
			it = head;
			head = tail;
			tail = it;
		}
	}
	public: void Unique(void)
	{   
		if(size > 1)
		{
			Node* x = head;
			while(x)
			{
				Node* y = x->right;
				while(y)
				{
					if(x->key == y->key)
					{ 
						Node* yCopy = y;
						y = y->right;

						Remove(yCopy);
						continue;
					}
					y = y->right;
				}

				x = x->right;
			}
		}
	}
	public: void Sort(bool _leftToRight)
	{
		for(int32 i = 0; i < size - 1; ++i)
		{
			Node* it = head;

			if(_leftToRight)
			{
				for(int32 j = 0; j < size - 1; ++j)
				{
					if(it->key > it->right->key)
					{
						Common::_Swap(it->key, it->right->key);
					}
					it = it->right;
				}
			}
			else
			{
				for(int32 j = 0; j < size - 1; ++j)
				{
					if(it->key < it->right->key)
					{
						Common::_Swap(it->key, it->right->key);
					}
					it = it->right;
				}
			}
		}
	}
    public: SimpleList<Type>& operator = (const SimpleList<Type>& _list)
	{
		if(this == &_list)
		{
			return *this;
		}

		Clear();
	
		Node* it = _list.head;
		while(it)
		{
			AddToTail(it->key);
			it = it->right;
		}

		return *this;
	}
	public: bool operator == (const SimpleList<Type>& _list)
	{
		if(this == &_list)
		{
			return true;
		}
		if(size != _list.size) { return false; }

		Node* aNode = head;
		Node* bNode = _list.head;
		while(aNode && bNode)
		{ 
			if(aNode->key == bNode->key)
			{
				aNode = aNode->right;
				bNode = bNode->right;
				continue;
			}
			return false;
		}

		return true;
	}
	public: Node* operator [] (int32 _index)const
	{
		return Get(_index);
	}
	public: SimpleList<Type> operator + (const Type& _key)const 
	{ 
		SimpleList<Type> list = *this;

		list.AddToTail(_key);
		
		return list;
	}
	public: SimpleList<Type>& operator += (const Type& _key) 
	{
		AddToTail(_key);
		return *this;
	}
	public: SimpleList<Type> operator + (const SimpleList<Type>& _list)const 
	{ 
		SimpleList<Type> list = *this;

		list += _list;
		
		return list;
	}
	public: SimpleList<Type>& operator += (const SimpleList<Type>& _list) 
	{
		Node* it = _list.head;
		while(it)
		{
			AddToTail(it->key);
			it = it->right;
		}
		
		return *this;
	}
};



#endif
