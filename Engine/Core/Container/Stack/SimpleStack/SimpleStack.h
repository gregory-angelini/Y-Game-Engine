#ifndef SIMPLESTACK_H
#define SIMPLESTACK_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Common/Common.h>
#include <Engine/Core/Memory/Memory.h>
template<typename Type>
class SimpleStack
{
	public: class Node
	{
		template<typename Type> friend class SimpleStack;
		private: Node* bottom;
		public: Type key;
		public: Node(Type _key): key(_key), bottom(NIL)
		{}
		public: virtual ~Node(void)
		{}
		public: inline Node* GetBottom(void)const 
		{
			return bottom;
		}
	};
	private: Node* top;
	private: int32 size;
	public: SimpleStack(void): top(NIL), size(0)
	{}
	public: SimpleStack(const SimpleStack<Type>& _stack): top(NIL), size(0)
	{ 
		*this = _stack;
	}
	public: virtual ~SimpleStack(void)
	{
		Clear();
	}
	public: int32 Size(void)const
	{
		return size; 
	}
	public: bool IsEmpty(void)const
	{
		return (top == NIL);
	}
	public: bool IsNotEmpty(void)const
	{
		return (top != NIL);
	}
	public: void Push(Type _key)
	{ 
		Node* it = new Node(_key);
		if(top)
		{
			it->bottom = top;
		}
		top = it;
		++size;
	}
	public: void RemoveTop(void)
	{
		if(top)
		{
			Node* it = top;
			top = top->bottom;

			delete it;
			--size;
		}
	}
	public: Type Pop(void)
	{
		if(top)
		{
			Node* it = top;
			Type key = top->key;
			top = top->bottom;
			
			delete it;
			--size;
			return key;
		}
		return Type();
	}
	public: void Clear(void)
	{
		Node* it;
		while(top)
		{
			it = top;
			top = top->bottom;
			delete it;
		}

		size = 0;
	}
	public: int32 GetIndexOf(Node* _node)const
	{
		Node* it = top;
		int32 index = 0;
	    
		while(it)
		{  
			if(it == _node)
			{
				return index;
			}

			it = it->bottom;
			++index;
		}

		return -1;
	}
	public: Node* Get(int32 _index)const
	{
		if(_index >= 0 && _index < size)
		{
			Node* it = top;
			for(int32 i = 0; i < _index; ++i)
			{
				it = it->bottom; 
			}

			return it;
		}
		return NIL;
	}
	public: Node* GetTop(void)const
	{
		return top; 
	}
	public: Type Peek(void)const
	{
		if(top) { return top->key; } 
		return Type(); 
	}
    public: SimpleStack<Type>& operator = (const SimpleStack<Type>& _stack)
	{
		if(this == &_stack)
		{
			return *this;
		}

		Clear();
	
		Copy(_stack.top);

		return *this;
	}
	private: void Copy(Node* _top)
	{
		if(_top)
		{
			Copy(_top->bottom);
			Add(_top->key);
		}
	}
	public: bool operator == (const SimpleStack<Type>& _stack)
	{
		if(this == &_stack)
		{
			return true;
		}
		if(size != _stack.size) { return false; }

		Node* aNode = top;
		Node* bNode = _stack.top;
		while(aNode && bNode)
		{ 
			if(aNode->key == bNode->key)
			{
				aNode = aNode->bottom;
				bNode = bNode->bottom;
				continue;
			}
			return false;
		}

		return true;
	}
};




#endif
