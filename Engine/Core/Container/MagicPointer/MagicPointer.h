#ifndef MAGICPOINTER_H
#define MAGICPOINTER_H
#include <Engine/Core/Container/MagicContainer.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Random/Random.h>
template<typename N>
class MagicPointer: public MagicContainer
{
	public: class AddressDerived: public MagicNode::Address
	{
		template<typename Node> friend class MagicPointer;
		private: MagicPointer<N>* pointer;
		private: N* node;
		protected: AddressDerived(N* _node): pointer(NIL), node(_node)
		{}
		public: virtual ~AddressDerived(void)
		{
			Remove();
		}
		public: N* GetNode(void)const
		{
			return node;
		}
		public: virtual MagicContainer* GetContainer(void)
		{ 
			return pointer;
		}
		private: void Remove(void)
		{

			if(pointer)
			{
				pointer->data = NIL;
				if(!container->IsDestroyed()) { pointer->RemoveNodeEvent(node); }
				pointer = NIL;
			}
		} 
	};
	private: AddressDerived* data;
	public: MagicPointer(void): data(NIL)
	{}
	public: MagicPointer(const MagicPointer<N>& _pointer): data(NIL)
	{ 
		*this = _pointer;
	}
	public: virtual ~MagicPointer(void)
	{ 
		Clear();
	}
    public: MagicPointer<N>& operator = (const MagicPointer<N>& _pointer)
	{
		if(this == &_pointer)
		{
			return *this;
		} 
		Clear(MagicContainer::DELETE_UNIQUE_NODE);

		AddressDerived* it = _pointer.data;
		if(it)
		{ 
			Attach(it->node);
		}
		return *this;
	}
	public: bool operator == (const MagicPointer<N>& _pointer)
	{
		if(this == &_pointer)
		{
			return true;
		}
		if(GetSize() != _pointer.GetSize()) { return false; }

		AddressDerived* aNode = data;
		AddressDerived* bNode = _pointer.data;
		if(aNode && bNode)
		{ 
			if(!(*(aNode->node) == *(bNode->node)))
			{
				return false;
			}
		}
		return true;
	}
	public: inline bool IsEmpty(void)const
	{
		return (data == NIL);	
	}
	public: inline bool IsNotEmpty(void)const
	{
		return (data != NIL);	
	}
	public: virtual int32 GetSize(void)const
	{
		return (int32)IsNotEmpty();
	}
	public: inline N* Get(void)const
	{
		if(data)
		{
			return data->node;
		}
		return NIL;
	}
	public: inline AddressDerived* GetAddress(void)const
	{
		return data;
	}
	public: AddressDerived* Attach(N* _node)
	{ 
		if(_node && data == NIL)
		{
			AddressDerived* newAddress = new AddressDerived(_node);
			data = newAddress;
			newAddress->pointer = this; 
			_node->connection.AddToTail(newAddress);
			
			AddNodeEvent(_node);
			return newAddress;
		}
		return NIL;
	}
	public: void Clear(void)
	{ 
		if(removeMode == REMOVE_NODE)
		{
			while(data)
			{
				delete data;
			}
		}
		else if(removeMode == DELETE_NODE)
		{
			while(data)
			{
				delete data->node;
			}
		}
		else
		{
			while(data)
			{
				if(data->node->connection.GetSize() > 1)
				{
					delete data;
				}
				else
				{
					delete data->node;
				}
			}
		}
	}
	public: void Clear(ClearMode _removeMode)
	{ 
		if(_removeMode == REMOVE_NODE)
		{
			while(data)
			{
				delete data;
			}
		}
		else if(_removeMode == DELETE_NODE)
		{
			while(data)
			{
				delete data->node;
			}
		}
		else
		{
			while(data)
			{
				if(data->node->connection.GetSize() > 1)
				{
					delete data;
				}
				else
				{
					delete data->node;
				}
			}
		}
	} 
};




#endif
