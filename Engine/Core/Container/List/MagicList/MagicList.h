#ifndef MAGICLIST_H
#define MAGICLIST_H
#include <Engine/Core/Container/MagicContainer.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Random/Random.h>
template<typename N>
class MagicList: public MagicContainer
{
	public: class AddressDerived: public MagicNode::Address
	{
		template<typename Node> friend class MagicList;
		private: AddressDerived* left;
		private: AddressDerived* right;
		private: MagicList<N>* list;
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
		public: int32 GetIndex(void)const
		{
			if(list)
			{
				AddressDerived* it = list->head;
				for(int32 index = 0; index < list->size; ++index) 
				{
					if(it == this)
					{
						return index;
					}
					it = it->right;
				}
			}
			return -1;
		}
		public: bool IsHead(void)const
		{
			if(list)
			{
				return (address == list->head);
			}
			return false;
		}
		public: bool IsTail(void)const
		{
			if(list)
			{
				return (address == list->tail);
			}
			return false;
		}
		public: bool IsFull(void)const
		{
			return (left && right);
		}
		public: bool IsLast(void)const
		{
			if(list)
			{
				return (list->size == 1);
			}
			return false;
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
					list->head = right;
						
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
					list->tail = left;
						
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
				list->size--;
				if(!container->IsDestroyed()) { list->RemoveNodeEvent(node); }
				list = NIL;
			}
		} 
	};
	private: int32 size;
	private: AddressDerived* head;
	private: AddressDerived* tail;
	public: MagicList(void): size(0), head(NIL), tail(NIL)
	{}
	public: MagicList(const MagicList<N>& _list): size(0), head(NIL), tail(NIL)
	{ 
		*this = _list;
	}
	public: virtual ~MagicList(void)
	{ 
		Clear();
	}
    public: MagicList<N>& operator = (const MagicList<N>& _list)
	{
		if(this == &_list)
		{
			return *this;
		} 
		Clear(MagicContainer::DELETE_UNIQUE_NODE);

		AddressDerived* it = _list.head;
		while(it)
		{ 
			AddToTail(it->node);
			it = it->right;
		}

		return *this;
	}
	public: bool operator == (const MagicList<N>& _list)
	{
		if(this == &_list)
		{
			return true;
		}
		if(size != _list.size) { return false; }

		AddressDerived* aNode = head;
		AddressDerived* bNode = _list.head;
		while(aNode && bNode)
		{ 
			if(*(aNode->node) == *(bNode->node))
			{
				aNode = aNode->right;
				bNode = bNode->right;
				continue;
			}
			return false;
		}
		return true;
	}
	public: N* operator [] (int32 _index)const
	{
		return Get(_index);
	}
	public: MagicList<N> operator + (const MagicList<N>& _list)const 
	{ 
		MagicList<N> list = *this;
		list += _list;
		return list;
	}
	public: MagicList<N>& operator += (const MagicList<N>& _list) 
	{
		AddressDerived* it = _list.head;
		while(it)
		{
			AddToTail(it->node);
			it = it->right;
		}
		return *this;
	}
	public: inline bool IsEmpty(void)const
	{
		return (head == NIL);	
	}
	public: inline bool IsNotEmpty(void)const
	{
		return (head != NIL);	
	}
	public: virtual int32 GetSize(void)const
	{
		return size;
	}
	public: int32 GetDistance(const N* _node1, const N* _node2)const
	{ 
		if(_node1 && _node2)
		{ 
			int32 indexA = IndexOf(_node1);
			int32 indexB = IndexOf(_node2);
			
			if(indexA >= 0 && indexB >= 0)
			{
				return indexB - indexA;
			}
		}
		return 0;
	}
	public: int32 GetIndexOf(const N* _node)const
	{
		if(_node)
		{
			AddressDerived* it = head;
			for(int32 index = 0; index < size; ++index) 
			{
				if(it->node == _node)
				{
					return index;
				}
				it = it->right;
			}
		}
		return -1;
	}
	public: AddressDerived* GetAddress(N* _node)const
	{
		if(_node)
		{
			return static_cast<AddressDerived*>(_node->connection.Find(this));
		}
		return NIL;
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
	public: inline N* GetRightOf(N* _node)const 
	{
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));

			if(address)
			{
				if(address->right)
				{
					return address->right->node; 
				}
			}
		}
		return NIL;
	}
	public: inline AddressDerived* GetRightAddressOf(AddressDerived* _address)const 
	{
		if(_address && _address->list == this)
		{
			return _address->right; 
		}
		return NIL;
	}
	public: inline N* GetLeftOf(N* _node)const 
	{
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));

			if(address)
			{	
				if(address->left)
				{
					return address->left->node; 
				}
			}
		}
		return NIL;
	}
	public: inline AddressDerived* GetLeftAddressOf(AddressDerived* _address)const 
	{
		if(_address && _address->list == this)
		{
			return _address->left; 
		}
		return NIL;
	}
	public: inline N* GetLoopedRightOf(N* _node)const 
	{
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));

			if(address)
			{
				if(tail == address)
				{
					return head->GetNode();
				}
				else if(address->right)
				{
					return address->right->node; 
				}
			}
		}
		return NIL;
	}
	public: inline AddressDerived* GetLoopedRightAddressOf(AddressDerived* _address)const 
	{
		if(_address && _address->list == this)
		{
			if(tail == _address)
			{
				return head;
			}
			return _address->right; 
		}
		return NIL;
	}
	public: inline N* GetLoopedLeftOf(N* _node)const 
	{
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));

			if(address)
			{	
				if(head == address)
				{
					return tail->GetNode();
				}
				else if(address->left)
				{
					return address->left->node; 
				}
			}
		}
		return NIL;
	}
	public: inline AddressDerived* GetLoopedLeftAddressOf(AddressDerived* _address)const 
	{
		if(_address && _address->list == this)
		{
			if(head == _address)
			{
				return tail;
			}
			return _address->left; 
		}
		return NIL;
	}
	public: N* Get(int32 _index)const
	{
		if(_index < 0)
		{
			_index = (size - 1) + _index;
		}

		if(_index >= 0 && _index < size)
		{
			if((size / 2) >= _index)
			{
				AddressDerived* it = head;
				for(int32 i = 0; i < _index; ++i)
				{
					it = it->right; 
				}
				return it->node;
			}
			else
			{
				AddressDerived* it = tail;

				_index = (size - 1) - _index;
				for(int32 i = 0; i < _index; ++i)
				{
					it = it->left; 
				}
				return it->node;
			}
		}
		return NIL;
	}
    public: N* GetFrom(const N* _node, int32 _distance)const
	{ 
		if(_node && static_cast<AddressDerived*>(_node->connection.Find(this)))
		{
			int32 startIndex = IndexOf(_node);
			
			if(startIndex < 0) { return NIL; }
			 
			int32 targetIndex = startIndex + _distance;
			
			if(targetIndex >= 0 && targetIndex < size)
			{
				int32 distanceFromHead = targetIndex;
				int32 distanceFromTail = (size - 1) - targetIndex;
				bool direction = _distance > 0 ? true : false;
				
				_distance = abs(_distance);
				if(_distance > distanceFromHead)
				{
					AddressDerived* it = head;
					for(int32 i = 0; i < distanceFromHead; ++i)
					{
						it = it->right; 
					}

					return it->node;
				} 
				else if(_distance > distanceFromTail)
				{
					AddressDerived* it = tail;
					for(int32 i = 0; i < distanceFromTail; ++i)
					{
						it = it->left; 
					}

					return it->node;
				}
				else
				{
					AddressDerived* it = static_cast<AddressDerived*>(_node->connection.Find(this));
					
					if(direction)
					{
						for(int32 i = 0; i < _distance; ++i)
						{
							it = it->right; 
						}
					}
					else
					{
						for(int32 i = 0; i < _distance; ++i)
						{
							it = it->left; 
						}
					}
					return it->node;
				} 
			}  
		}
		return NIL;
	}
	public: template<typename M, typename K> N* Find(K _key, M _method)const
	{
		AddressDerived* search = head;
		while(search)
		{  
			if((search->node->*_method)() == _key)
			{
				return search->node;
			}
			search = search->right;
		}

		return NIL;
	}
	public: template<typename M, typename K> void FindSimilar(K _key, M _method, MagicList<N>* _list = NIL, Action _action = TRANSFER)
	{
		AddressDerived* it = head;
		while(it)
		{
			if((it->node->*_method)() == _key)
			{
				if(_list)
				{
					if(_action == TRANSFER)
					{
						AddressDerived* itCopy = it;
						N* itNode = it->node;
						it = it->right;
						
						delete itCopy;
						_list->AddToTail(itNode);
						continue;
					}
					else if(_action == SHARE)
					{
						_list->AddToTail(it->node);
					}
				}
				else
				{ 
					AddressDerived* itCopy = it;
					it = it->right;
					delete itCopy->node;
					continue;
				}
			}
			it = it->right;
		}
	}
	public: template<typename T> N* GetExtremum(T _method, bool _max)const
	{ 
		if(head)
		{
			AddressDerived* search = head;
			AddressDerived* extremum = search;

			if(_max)
			{
				while(search)
				{
					if((extremum->node->*_method)() < (search->node->*_method)())
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
					if((extremum->node->*_method)() > (search->node->*_method)())
					{
						extremum = search;
					}
					search = search->right;
				}
			}
			return extremum->node;
		}
		return NIL;
	}
	public: N* GetRandom(void)const
	{
		if(size > 1)
		{
			return Get(Random::_RandomInt32(0, size - 1));
		}
		return GetHead();
	}
	public: AddressDerived* AddToTail(N* _node)
	{  
		if(_node)
		{
			if(_node->connection.Find(this))
			{
				return NIL;
			}
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
			++size;
			
			AddNodeEvent(_node);
			return newAddress;
		}
		return NIL;
	}
	public: AddressDerived* AddToHead(N* _node)
	{  
		if(_node)
		{
			if(_node->connection.Find(this))
			{
				return NIL;
			}
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
			++size;
			
			AddNodeEvent(_node);
			return newAddress;
		}
		return NIL;
	}
	public: AddressDerived* AddToRight(N* _head, N* _node)
	{
		if(_node)
		{
			if(_node->connection.Find(this))
			{
				return NIL;
			}
            if(_head)
			{
				AddressDerived* headAddress = static_cast<AddressDerived*>(_head->connection.Find(this));
		       
				if(headAddress)
				{
					AddressDerived* newAddress = new AddressDerived(_node);

					if(headAddress->right)
					{
						newAddress->right = headAddress->right;
						headAddress->right->left = newAddress;
					}
					else
					{
						tail = newAddress;
					}
					newAddress->left = headAddress;
					headAddress->right = newAddress;
					
					newAddress->list = this; 
					_node->connection.AddToTail(newAddress);
					++size;
					
					AddNodeEvent(_node);
					return newAddress;
				}
			}
		}
		return NIL;
	}
	public: AddressDerived* AddToLeft(N* _head, N* _node)
	{
		if(_node)
		{
			if(_node->connection.Find(this))
			{
				return NIL;
			}
            if(_head)
			{
				AddressDerived* headAddress = static_cast<AddressDerived*>(_head->connection.Find(this));
		       
				if(headAddress)
				{  
					AddressDerived* newAddress = new AddressDerived(_node);

					if(headAddress->left)
					{
						newAddress->left = headAddress->left;
						headAddress->left->right = newAddress;
					}
					else
					{
						head = newAddress;
					}
					newAddress->right = headAddress;
					headAddress->left = newAddress;
					
					newAddress->list = this; 
					_node->connection.AddToTail(newAddress);
					++size;
					
					AddNodeEvent(_node);
					return newAddress;
				}
			}
		}
		return NIL;
	}
	public: AddressDerived* AddTo(N* _node, int32 _index)
	{ 
		if(_node)
		{
			if(_index == 0)
			{
				return AddToHead(_node);
			}
			else if(_index == (size - 1))
			{
				return AddToLeft(GetTail(), _node);
			}
			else if(_index > (size - 1))
			{
				return AddToTail(_node);
			}
			else
			{
				N* it = Get(_index);

				if(it)
				{
					return AddToLeft(it, _node);
				}
			} 
		}
		return NIL;
	}
	public: AddressDerived* AddRandom(N* _node)
	{
		if(size > 1)
		{
			return AddTo(_node, Random::_RandomInt32(0, size));
		}
		else
		{
			if(Random::_RandomInt32(0, 1))
			{
				return AddToHead(_node);
			}
			else
			{
				return AddToTail(_node);
			}
		}
		return NIL;
	}
	public: void Share(int32 _x, int32 _y, MagicList<N>& _list, int32 _index)
	{
		if(this == &_list) { return; }

		if(_x >= 0 && _x < size)
		{ 
			if(_y >= 0 && _y < size)
			{
				if(_x < _y)
				{
					AddressDerived* it = head;
					for(int32 i = 0; i < _x; ++i)
					{
						it = it->right;
					}

					int32 range = (_y - _x) + 1;
					for(int32 i = 0; i < range; ++i)
					{
						_list.AddTo(it->node, _index + i);
						it = it->right;
					}
				}
				else
				{
					AddressDerived* it = tail;
					for(int32 i = 0, count = (size - 1) - _x; i < count; ++i)
					{
						it = it->left;
					}

					int32 range = (_x - _y) + 1;
					for(int32 i = 0; i < range; ++i)
					{ 
						_list.AddTo(it->node, _index + i);
						it = it->left;
					}
				}
			}
		}
	}
	public: void Share(MagicList<N>& _list, int32 _index)
	{
		if(this == &_list) { return; }

		AddressDerived* it = head;
		for(int32 i = 0; i < size; ++i)
		{
			_list.AddTo(it->node, _index + i);
			it = it->right;
		}
	}
	public: void Share(MagicList<N>& _list)
	{
		if(this == &_list) { return; }

		AddressDerived* it = head;
		while(it)
		{
			_list.AddToTail(it->node);
			it = it->right;
		}
	}
    public: void Remove(N* _node)
	{
		if(_node)
		{
			AddressDerived* it = static_cast<AddressDerived*>(_node->connection.Find(this));
		
			if(it)
			{
				if(removeMode == REMOVE_NODE)
				{
					delete it;
				}
				else if(removeMode == DELETE_NODE)
				{
					delete it->node;
				}
				else
				{
					if(it->node->connection.GetSize() > 1)
					{
						delete it;
					}
					else
					{
						delete it->node;
					}
				}
			}
		}
	}
    public: void Remove(N* _node, ClearMode _removeMode)
	{
		if(_node)
		{
			AddressDerived* it = static_cast<AddressDerived*>(_node->connection.Find(this));
		
			if(it)
			{
				if(_removeMode == REMOVE_NODE)
				{
					delete it;
				}
				else if(_removeMode == DELETE_NODE)
				{
					delete it->node;
				}
				else
				{
					if(it->node->connection.GetSize() > 1)
					{
						delete it;
					}
					else
					{
						delete it->node;
					}
				}
			}
		}
	}
	public: void RemoveHead(void)
	{
		delete head;
	}
	public: void RemoveTail(void)
	{
		delete tail;
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
	public: void MoveIndexRangeTo(int32 _x, int32 _y, MagicList<N>& _list, int32 _index)
	{
		if(this == &_list) { return; }

		if(_x >= 0 && _x < size)
		{ 
			if(_y >= 0 && _y < size)
			{
				if(_x < _y)
				{
					AddressDerived* it = head;
					for(int32 i = 0; i < _x; ++i)
					{
						it = it->right;
					}

					int32 range = (_y - _x) + 1;
					for(int32 i = 0; i < range; ++i)
					{
						AddressDerived* right = it->right;
						N* node = it->node;
						delete it;
						_list.AddTo(node, _index + i);
						it = right;
					}
				}
				else
				{
					AddressDerived* it = tail;
					for(int32 i = 0, count = (size - 1) - _x; i < count; ++i)
					{
						it = it->left;
					}

					int32 range = (_x - _y) + 1;
					for(int32 i = 0; i < range; ++i)
					{
						AddressDerived* left = it->left;
						N* node = it->node;
						delete it;
						_list.AddTo(node, _index + i);
						it = left;
					}
				}
			}
		}
	}
	public: void MoveIndexRangeTo(int32 _x, int32 _y, MagicList<N>& _list)
	{
		if(this == &_list) { return; }

		if(_x >= 0 && _x < size)
		{ 
			if(_y >= 0 && _y < size)
			{
				if(_x < _y)
				{
					AddressDerived* it = head;
					for(int32 i = 0; i < _x; ++i)
					{
						it = it->right;
					}

					int32 range = (_y - _x) + 1;
					for(int32 i = 0; i < range; ++i)
					{
						AddressDerived* right = it->right;
						N* node = it->node;
						delete it;
						_list.AddToTail(node);
						it = right;
					}
				}
				else
				{
					AddressDerived* it = tail;
					for(int32 i = 0, count = (size - 1) - _x; i < count; ++i)
					{
						it = it->left;
					}

					int32 range = (_x - _y) + 1;
					for(int32 i = 0; i < range; ++i)
					{
						AddressDerived* left = it->left;
						N* node = it->node;
						delete it;
						_list.AddToTail(node); 
						it = left;
					}
				}
			}
		}
	}
	public: void MoveNodeRangeTo(N* _head, N* _tail, MagicList<N>& _list, int32 _index)
	{
		if(this == &_list) { return; }

		if(_head && _tail)
		{ 
			AddressDerived* xAddress = static_cast<AddressDerived*>(_head->connection.Find(this));
		       
			if(xAddress)
			{
				AddressDerived* yAddress = static_cast<AddressDerived*>(_tail->connection.Find(this));
		       
				if(yAddress)
				{
					int32 i = 0;
					for(AddressDerived* it = xAddress; it != NIL;)
					{
						AddressDerived* right = it->right;
						N* node = it->node;
						
						if(it == yAddress)
						{
							delete it;
							_list.AddTo(node, _index + i);
							return;
						}
						else
						{
							delete it;
							_list.AddTo(node, _index + i);
						}

						i++;
						it = right;
					}
					for(AddressDerived* it = head; it != NIL;)
					{
						AddressDerived* right = it->right;
						N* node = it->node;
						
						if(it == yAddress)
						{
							delete it;
							_list.AddTo(node, _index + i);
							return;
						}
						else
						{
							delete it;
							_list.AddTo(node, _index + i);
						}

						i++;
						it = right;
					}
				}
			}
		}
	}
	public: void MoveNodeRangeTo(N* _head, N* _tail, MagicList<N>& _list)
	{
		if(this == &_list) { return; }

		if(_head && _tail)
		{ 
			AddressDerived* xAddress = static_cast<AddressDerived*>(_head->connection.Find(this));
		       
			if(xAddress)
			{
				AddressDerived* yAddress = static_cast<AddressDerived*>(_tail->connection.Find(this));
		       
				if(yAddress)
				{
					for(AddressDerived* it = xAddress; it != NIL;)
					{
						AddressDerived* right = it->right;
						N* node = it->node;
						
						if(it == yAddress)
						{
							delete it;
							_list.AddToTail(node); 
							return;
						}
						else
						{
							delete it;
							_list.AddToTail(node); 
						}
						it = right;
					}
					for(AddressDerived* it = head; it != NIL;)
					{
						AddressDerived* right = it->right;
						N* node = it->node;
						
						if(it == yAddress)
						{
							delete it;
							_list.AddToTail(node);
							return;
						}
						else
						{
							delete it;
							_list.AddToTail(node); 
						}
						it = right;
					}
				}
			}
		}
	}
	public: void MoveTo(MagicList<N>& _list, int32 _index)
	{
		if(this == &_list) { return; }

		int32 i = 0;
		AddressDerived* it = head;

		while(it)
		{
			AddressDerived* right = it->right;
			N* node = it->node;
			delete it;
			_list.AddTo(node, _index + i);
			++i;
			it = right;
		}
	}
	public: void RemoveRange(int32 _x, int32 _y, bool _delete)
	{
		if(_x >= 0 && _x < size)
		{ 
			if(_y >= 0 && _y < size)
			{
				if(_x < _y)
				{
					AddressDerived* it = head;
					for(int32 i = 0; i < _x; ++i)
					{
						it = it->right;
					}

					int32 range = (_y - _x) + 1;
					for(int32 i = 0; i < range; ++i)
					{
						AddressDerived* right = it->right;
						if(_delete) { delete it->node; }
						else { delete it; }
						it = right;
					}
				}
				else
				{
					AddressDerived* it = tail;
					for(int32 i = 0, count = (size - 1) - _x; i < count; ++i)
					{
						it = it->left;
					}

					int32 range = (_x - _y) + 1;
					for(int32 i = 0; i < range; ++i)
					{
						AddressDerived* left = it->left;
						if(_delete) { delete it->node; }
						else { delete it; }
						it = left;
					}
				}
			}
		}
	}
	public: static void _Swap(AddressDerived* _address1, AddressDerived* _address2)
	{

		if(_address1 && _address2 && _address1 != _address2)
		{
			N* node1 = _address1->node;
			N* node2 = _address2->node;

			node1->connection.Remove(_address1);
			node2->connection.Remove(_address2);

			node1->connection.AddToTail(_address2);
			node2->connection.AddToTail(_address1);

			_address1->node = node2;
			_address2->node = node1;
		}
	}
	public: void Rotate(bool _right)
	{
		if(_right)
		{
			AddressDerived* it = tail;
			for(int32 i = 0; i < size; ++i)
			{
				_Swap(it, it->GetLeft());
				it = it->GetLeft();
			} 
		}
		else
		{
			AddressDerived* it = head;
			for(int32 i = 0; i < size; ++i)
			{
				_Swap(it, it->GetRight());
				it = it->GetRight();
			}
		}
	}
	public: void Inverse(void)
	{
		if(size > 1)
		{
			AddressDerived* it = head;
			AddressDerived* right;
			while(it)
			{
				right = it->right;
				it->right = it->left;
				it->left = right;

				it = right;
			}
			AddressDerived* headCopy = head;
			head = tail;
			tail = headCopy;
		}
	}
	public: template<typename T> void Sort(T _method, bool _leftToRight)
	{
		for(int32 i = 0; i < size - 1; ++i)
		{
			AddressDerived* it = head;

			if(_leftToRight)
			{
				for(int32 j = 0; j < size - 1; ++j)
				{
					if((it->node->*_method)() > (it->right->node->*_method)())
					{ 
						_Swap(it, it->right);
					}
					else
					{
						it = it->right;
					}
				}
			}
			else
			{
				for(int32 j = 0; j < size - 1; ++j)
				{
					if((it->node->*_method)() < (it->right->node->*_method)())
					{
						_Swap(it, it->right);
					}
					else
					{
						it = it->right;
					}
				}
			}
		}
	}
	public: template<typename T> void Unique(T _method, MagicList<N>* _list = NIL, Action _action = TRANSFER)
	{
		if(size > 1)
		{
			AddressDerived* x = head;
			while(x)
			{
				AddressDerived* y = x->right;

				while(y)
				{  
					if((x->node->*_method)() == (y->node->*_method)())
					{ 
						if(_list)
						{
							if(_action == TRANSFER)
							{
								AddressDerived* yCopy = y;
								N* yNode = y->node;
								y = y->right;

								delete yCopy;
								_list->AddToTail(yNode);
								continue;
							}
							else if(_action == SHARE)
							{
								_list->AddToTail(y->node);
							}
						}
						else
						{ 
							AddressDerived* yCopy = y;
							y = y->right;
							delete yCopy->node;
							continue;
						}
					}
					y = y->right;
				}
				x = x->right;
			}
		}
	}
	public: void Swap(MagicList<N>* _list)
	{
		if(_list)
		{
			if(this == _list) { return; }
			AddressDerived* it = head;

			while(it)
			{
				it->list = _list;
				it = it->right;
			}
			it = _list->head;

			while(it)
			{
				it->list = this;
				it = it->right;
			}
			int32 sizeCopy = size;
			size = _list->size;
			_list->size = sizeCopy;
			it = head;
			head = _list->head; 
			_list->head = it;
			it = tail;
			tail = _list->tail;
			_list->tail = it;
		}
	}
	public: void RandomSwap(MagicList<N>& _list, int32 _swaps = 1) 
	{
		if(size > 0 && _list.size > 0 && _swaps > 0)
		{
			for(int32 i = 0; i < _swaps; ++i) 
			{
				N* node1 = Get(Random::_RandomInt32(0, size - 1));
				N* node2 = _list.Get(Random::_RandomInt32(0, _list.size - 1));
				
				if(node1 && node2)
				{
					_Swap(dynamic_cast<AddressDerived*>(node1->GetAddress(this)), dynamic_cast<AddressDerived*>(node2->GetAddress(&_list))); 
				}
			} 
		}
	}
	public: void RandomSwap(int32 _swaps = 1) 
	{
		if(size > 1 && _swaps > 0)
		{
			for(int32 i = 0; i < _swaps; ++i) 
			{
				N* node1 = Get(Random::_RandomInt32(0, size - 1));
				N* node2 = Get(Random::_RandomInt32(0, size - 1));
				
				if(node1 && node2)
				{
					_Swap(dynamic_cast<AddressDerived*>(node1->GetAddress(this)), dynamic_cast<AddressDerived*>(node2->GetAddress(this))); 
				}
			} 
		}
	}
	private: template<typename M> void PostfixTraverse(AddressDerived* _address, M& _caller)
	{
		if(_address)
		{ 
			PostfixTraverse(_address->right, _caller); 

			_caller(_address->node);
		}
	}
	public: template<typename M> void PostfixTraverse(M& _caller)
	{
		if(head)
		{ 
			PostfixTraverse(head, _caller);
		}
	}
	private: template<typename M> void PrefixTraverse(AddressDerived* _address, M& _caller)
	{
		if(_address)
		{
			_caller(_address->node); 
			PrefixTraverse(_address->right, _caller); 
		}  
	}
	public: template<typename M> void PrefixTraverse(M& _caller)
	{
		if(head)
		{ 
			PrefixTraverse(head, _caller);
		}
	}
};




#endif
