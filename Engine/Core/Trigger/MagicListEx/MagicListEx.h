#ifndef MAGICLISTEX_H
#define MAGICLISTEX_H
#include <Engine/Core/Container/MagicContainer.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Random/Random.h>
template<typename N>
class MagicListEx: public MagicContainer
{
	public: class AddressDerived: public MagicNode::Address
	{
		template<typename Node> friend class MagicListEx;
		private: AddressDerived* left;
		private: AddressDerived* right;
		private: MagicListEx* list;
		private: N* node;
		protected: AddressDerived(N* _node): list(NIL), node(_node)
		{
			right = left = NIL;
		}
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
			return list;
		}
		public: inline AddressDerived* GetRight(void)const 
		{
			return right; 
		}
		public: inline AddressDerived* GetLeft(void)const 
		{
			return left; 
		}
		private: void Remove(void)
		{

			if(list)
			{
				if(this == list->head)
				{
					list->head = list->head->right;
						
					if(list->head)
					{
						list->head->left = NIL;
					}
					else
					{
						list->tail = NIL;
					}
				}
				else if(this == list->tail)
				{
					list->tail = list->tail->left;
						
					if(list->tail)
					{
						list->tail->right = NIL;
					}
					else
					{
						list->head = NIL;
					}
				}
				else
				{
					left->right = right;
					right->left = left;
				}
				list = NIL;
			}
		}  
	};
	private: AddressDerived* head;
	private: AddressDerived* tail;
	public: MagicListEx(void): head(NIL), tail(NIL)
	{}
	public: virtual ~MagicListEx(void)
	{ 
		Clear();
	}
	public: inline bool IsEmpty(void)const
	{
		return (head == NIL);	
	}
	public: inline bool IsNotEmpty(void)const
	{
		return (head != NIL);	
	}
	public: inline N* GetHead(void)const
	{
		if(head)
		{
			return head->node;
		}
		return NIL;
	}
	public: inline AddressDerived* GetHeadAddress(void)const
	{
		return head;
	}
	public: inline N* GetTail(void)const
	{
		if(tail)
		{
			return tail->node;
		}
		return NIL;
	}
	public: inline AddressDerived* GetTailAddress(void)const
	{
		return tail;
	}
	public: AddressDerived* AddToHead(N* _node)
	{
		if(_node)
		{
			AddressDerived* newAddress = new AddressDerived(_node);

			if(head)
			{
				newAddress->right = head;
				head->left = newAddress;
			}
			else
			{
				tail = newAddress;
			}
			head = newAddress;

			newAddress->list = this;
			_node->connection.AddToTail(newAddress);
			return newAddress;
		}
		return NIL;
	}
	public: AddressDerived* AddToTail(N* _node)
	{ 
		if(_node)
		{
			AddressDerived* newAddress = new AddressDerived(_node);

			if(head)
			{
				newAddress->left = tail;
				tail->right = newAddress;
			}
			else
			{
				head = newAddress;
			}
			tail = newAddress;

			newAddress->list = this;
			_node->connection.AddToTail(newAddress);
			return newAddress;
		}
		return NIL;
	}
	public: void ShareToHead(MagicListEx<N>& _list)
	{
		if(this == &_list) { return; }

		AddressDerived* it = tail;
		while(it)
		{
			_list.AddToHead(it->node);
			it = it->left;
		}
	}
	public: void ShareToTail(MagicListEx<N>& _list)
	{
		if(this == &_list) { return; }

		AddressDerived* it = head;
		while(it)
		{
			_list.AddToTail(it->node);
			it = it->right;
		}
	}
	public: void Clear(void)
	{ 
		if(removeMode == REMOVE_NODE)
		{
			while(head)
			{
				delete head;
			}
		}
		else if(removeMode == DELETE_NODE)
		{
			while(head)
			{
				delete head->node;
			}
		}
		else
		{
			while(head)
			{
				if(head->node->connection.GetSize() > 1)
				{
					delete head;
				}
				else
				{
					delete head->node;
				}
			}
		}
	}
	public: void Clear(ClearMode _removeMode)
	{ 
		if(_removeMode == REMOVE_NODE)
		{
			while(head)
			{
				delete head;
			}
		}
		else if(_removeMode == DELETE_NODE)
		{
			while(head)
			{
				delete head->node;
			}
		}
		else
		{
			while(head)
			{
				if(head->node->connection.GetSize() > 1)
				{
					delete head;
				}
				else
				{
					delete head->node;
				}
			}
		}
	}  
};




#endif
