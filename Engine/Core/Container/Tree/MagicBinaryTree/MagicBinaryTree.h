#ifndef MAGICBINARYTREE_H
#define MAGICBINARYTREE_H
#include <Engine/Core/Random/Random.h>
#include <Engine/Core/Container/MagicContainer.h>
#include <Engine/Core/Container/MagicNode.h>
template<typename N>
class MagicBinaryTree: public MagicContainer
{
	public: enum
	{
		LEFT_CHILD = 0,
		RIGHT_CHILD = 1
	};
    public: class AddressDerived: public MagicNode::Address
	{
		template<typename N> friend class MagicBinaryTree;
		private: AddressDerived* links[2];
		private: AddressDerived* parent;
		private: MagicBinaryTree<N>* tree;
		private: N* node;
		protected: AddressDerived(N* _node): tree(NIL), node(_node)
		{
			links[LEFT_CHILD] = links[RIGHT_CHILD] = parent = NIL;
		}
		public: virtual ~AddressDerived(void)
		{ 
			Remove(MagicBinaryTree::LEFT_CHILD);
		}
		public: AddressDerived* GetParent(void)const
		{
			return parent;
		}
		public: AddressDerived* GetChild(bool _direction)const
		{
			return links[_direction];
		}
		public: N* GetNode(void)const
		{
			return node;
		}
		public: virtual MagicContainer* GetContainer(void)
		{  
			return tree;
		}
		public: bool IsLeaf(void)const
		{
			return !(links[LEFT_CHILD] || links[RIGHT_CHILD]);
		}
		public: bool IsRoot(void)const
		{
			if(tree)
			{
				return (this == tree->root);
			}
			return false;
		}
		public: bool IsFull(void)const
		{
			return (links[LEFT_CHILD] && links[RIGHT_CHILD]); 
		}
		public: int32 GetSize(void)const
		{
			int32 size = 0;
			Counter(this, size);
			return size;
		}
		private: void Counter(const AddressDerived* _address, int32& _size)const
		{ 
			if(_address)
			{
				++_size;
				Counter(_address->links[LEFT_CHILD], _size);
				Counter(_address->links[RIGHT_CHILD], _size);
			}
		}
		public: int32 GetHeight(void)const
		{ 
			return GetHeight(this, 0);
		}
		private: int32 GetHeight(const AddressDerived* _address, int32 _height)const
		{ 
			if(_address)
			{ 

				++_height;

				int32 leftHeight  = Height(_address->links[LEFT_CHILD], _height); 
				int32 rightHeight = Height(_address->links[RIGHT_CHILD], _height);
				if(leftHeight > _height) { _height = leftHeight; }
				if(rightHeight > _height) { _height = rightHeight; }
							
				return _height;
			} 
			return 0;
		}
		public: int32 GetExcessHeight(void)const
		{ 
			int32 size = GetSize();
			int32 height = GetHeight();

			int32 perfectSize = 1;
			int32 levelSize = 1;
			int32 perfectHeight = 1;

			while(1)
			{
				if(perfectSize >= size)
				{
					break;
				}

				++perfectHeight;
				levelSize *= 2;
						
				perfectSize += levelSize;
			}
			return height - perfectHeight;
		}
        private: void Remove(bool _direction) 
		{ 
		
			if(tree)
			{
				if(links[LEFT_CHILD] && links[RIGHT_CHILD])
				{
					AddressDerived* heir = links[_direction];
				    
					bool heirPosition = !_direction;
					while(heir->links[heirPosition])
					{
						heir = heir->links[heirPosition];
					}

					AddressDerived* heirParent = heir->parent;
					AddressDerived* heirChild = heir->links[_direction];
					if(this == tree->root)
					{ 
						heir->parent = NIL;
						tree->root = heir;
					} 
					else
					{ 
						heir->parent = parent;
						parent->links[parent->links[RIGHT_CHILD] == this] = heir;
					}
					if(links[_direction] != heir)
					{
						heir->links[LEFT_CHILD] = links[LEFT_CHILD];
						heir->links[RIGHT_CHILD] = links[RIGHT_CHILD];
						links[LEFT_CHILD]->parent = heir;
						links[RIGHT_CHILD]->parent = heir;
						if(heirChild == NIL)
						{
							heirParent->links[heirParent->links[RIGHT_CHILD] == heir] = NIL;
							tree->size--;
							
							if(!container->IsDestroyed()) { tree->RemoveNodeEvent(node); }
							tree = NIL;
							return;
						}
						{
							heirParent->links[heirParent->links[RIGHT_CHILD] == heir] = heirChild;
							heirChild->parent = heirParent;
							tree->size--;
							
							if(!container->IsDestroyed()) { tree->RemoveNodeEvent(node); }
							tree = NIL;
							return;
						}
					}
					{
						heir->links[heirPosition] = links[heirPosition];
						links[heirPosition]->parent = heir; 

						tree->size--;
						
						if(!container->IsDestroyed()) { tree->RemoveNodeEvent(node); }
						tree = NIL;
						return;
					}
				}
			 
				if(IsLeaf())
				{
					RemoveLeaf();
					
					if(!container->IsDestroyed()) { tree->RemoveNodeEvent(node); }
					tree = NIL;
					return;
				}
				_direction = links[LEFT_CHILD] == NIL;
				if(parent)
				{
					bool removeNodePosition = parent->links[RIGHT_CHILD] == this;
					parent->links[removeNodePosition] = links[_direction];
					links[_direction]->parent = parent;
					
					tree->size--;

					if(!container->IsDestroyed()) { tree->RemoveNodeEvent(node); }
					tree = NIL;
					return;
				}
				{
					tree->root = links[_direction];
					links[_direction]->parent = NIL;
					
					tree->size--;

					if(!container->IsDestroyed()) { tree->RemoveNodeEvent(node); }
					tree = NIL;
					return;
				} 
			}
		}
		private: void RemoveLeaf(void)
		{ 
			if(parent)
			{
				parent->links[parent->links[RIGHT_CHILD] == this] = NIL;
			}
			else
			{
				tree->root = NIL;
			}
			
			tree->size--;
		}
	};
	private: int32 size;
	private: AddressDerived* root;
	public: MagicBinaryTree(void): size(0), root(NIL)
	{}
	public: MagicBinaryTree(const MagicBinaryTree<N>& _tree): size(0), root(NIL)
	{ 
		*this = _tree;
	}
	public: virtual ~MagicBinaryTree(void)
	{
		Clear();
	}
	private: void AddLeaf(AddressDerived* _address, AddressDerived* _parent, bool _direction)
	{
		if(_address)
		{
			N* it = _address->node;
			AddressDerived* newAddress = new AddressDerived(it);
			if(_parent)
			{
				_parent->links[_direction] = newAddress;
				newAddress->parent = _parent;
				newAddress->tree = this;
				size++;
			}
			else if(root == NIL)
			{
				root = newAddress;
				newAddress->tree = this;
				size++;
			}
			it->connection.AddToTail(newAddress);
			AddLeaf(_address->links[LEFT_CHILD], newAddress, LEFT_CHILD); 
			AddLeaf(_address->links[RIGHT_CHILD], newAddress, RIGHT_CHILD); 
		}
	}
    public: MagicBinaryTree<N>& operator = (const MagicBinaryTree<N>& _tree)
	{
		if(this == &_tree)
		{
			return *this;
		}			
		Clear(DELETE_UNIQUE_NODE);
		AddLeaf(_tree.root, NIL, LEFT_CHILD);
		return *this;
	}
	private: bool Equals(AddressDerived* _address1, AddressDerived* _address2)
	{
		if(_address1 && _address2)
		{ 
			if(!(*(_address1->node) == *(_address2->node)))
			{
				return false;
			}
			if(Equals(_address1->links[LEFT_CHILD], _address2->links[LEFT_CHILD]))
			{ 
				if(Equals(_address1->links[RIGHT_CHILD], _address2->links[RIGHT_CHILD]))
				{
					return true;
				}
			}
			return false;
		}
		else if(_address1 == _address2)
		{
			return true;
		}
		return false;
	}
	public: bool operator == (const MagicBinaryTree<N>& _tree)
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
	public: virtual int32 GetSize(void)const
	{
		return size;
	}
	public: int32 GetHeight(void)const
	{
		if(root) { return root->GetHeight(); }
		return 0;
	}
    public: int32 GetExcessHeight(void)const 
	{ 
		if(root) { return root->GetExcessHeight(); }
		return 0; 
	}
	public: bool IsLeaf(N* _node)const
	{
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));

			if(address)
			{
				return !(address->links[LEFT_CHILD] || address->links[RIGHT_CHILD]);
			}
		}
		return false;
	}
	public: bool IsRoot(N* _node)const
	{
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));

			if(address)
			{
				return (address == root);
			}
		}
		return false;
	}
	public: bool IsFull(N* _node)const
	{
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));

			if(address)
			{
				return (address->links[LEFT_CHILD] && address->links[RIGHT_CHILD]); 
			}
		}
		return false;
	}
	public: template<typename M> AddressDerived* Add(N* _node, M _method)
	{ 
		if(_node)
		{ 

			if(root)
			{
				AddressDerived* search = root;
	
				do
				{
					if((search->node->*_method)() == (_node->*_method)())
					{
						return NIL;
					} 
					bool direction = (search->node->*_method)() < (_node->*_method)();

					if(search->links[direction])
					{
						search = search->links[direction];
						continue;
					}
					AddressDerived* newAddress = new AddressDerived(_node);
					newAddress->parent = search;
					newAddress->tree = this;
					search->links[direction] = newAddress; 
					_node->connection.AddToTail(newAddress);
					size++;

					AddNodeEvent(_node);
					return newAddress;
				}
				while(search);
			}
			else
			{
				AddressDerived* newAddress = new AddressDerived(_node);
				newAddress->tree = this;
				root = newAddress; 
				_node->connection.AddToTail(newAddress);
				size++;

				AddNodeEvent(_node);
				return newAddress;
			}
		}
		return NIL;
	}
	public: template<typename M> AddressDerived* Rebuild(N* _node, M _method)
	{  
		if(_node)
		{  
			Remove(_node, MagicContainer::REMOVE_NODE);
			return Add(_node, _method);
		}
		return NIL;
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
	public: void RemoveRoot(void)
	{
		delete root;
	}
	public: void Clear(void)
	{ 
		if(removeMode == REMOVE_NODE)
		{
			while(root)
			{
				delete root;
			}
		}
		else if(removeMode == DELETE_NODE)
		{
			while(root)
			{
				delete root->node;
			}
		}
		else
		{
			while(root)
			{
				if(root->node->connection.GetSize() > 1)
				{
					delete root;
				}
				else
				{
					delete root->node;
				}
			}
		}
	}
	public: void Clear(ClearMode _removeMode)
	{ 
		if(_removeMode == REMOVE_NODE)
		{
			while(root)
			{
				delete root;
			}
		}
		else if(_removeMode == DELETE_NODE)
		{
			while(root)
			{
				delete root->node;
			}
		}
		else
		{
			while(root)
			{
				if(root->node->connection.GetSize() > 1)
				{
					delete root;
				}
				else
				{
					delete root->node;
				}
			}
		}
	}
	public: template<typename M> void Share(MagicBinaryTree<N>& _tree, M _method)
	{
		if(this == &_tree) { return; }
		Share(root, _tree, _method);
	}
	private: template<typename M> void Share(AddressDerived* _address, MagicBinaryTree<N>& _tree, M _method)
	{
		if(_address)
		{
			Share(_address->links[LEFT_CHILD], _tree, _method); 
			Share(_address->links[RIGHT_CHILD], _tree, _method);	

			_tree.Add(_address->node, _method);
		}  
	}
	public: template<typename M> void MoveTo(MagicBinaryTree<N>& _tree, M _method)
	{
		if(this == &_tree) { return; }
		
		while(root)
		{
			N* node = root->node;
			delete root;
			_tree.Add(node, _method);
		}
	}
	public: N* GetRoot(void)const
	{
		if(root)
		{
			return root->node;
		}
		return NIL;
	}
	public: AddressDerived* GetRootAddress(void)const
	{
		return root;
	}
	public: N* GetParentOf(N* _node)const
	{	
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));

			if(address)
			{
				if(address->parent)
				{
					return address->parent->node;
				}
			}
		}
		return NIL; 
	}
	public: AddressDerived* GetParentAddressOf(AddressDerived* _address)const
	{
		if(_address && _address->tree == this)
		{
			return _address->parent;
		}
		return NIL; 
	}
	public: N* GetChildOf(N* _node, bool _direction)const
	{
		if(_node)
		{
			AddressDerived* address = static_cast<AddressDerived*>(_node->connection.Find(this));
 
			if(address)
			{
				if(address->links[_direction])
				{
					return address->links[_direction]->node;
				}
			}
		}
		return NIL; 
	}
	public: AddressDerived* GetChildAddressOf(AddressDerived* _address, bool _direction)const
	{
		if(_address && _address->tree == this)
		{
			return _address->links[_direction];
		}
		return NIL; 
	}
	public: template<typename M, typename K> N* Find(K _key, M _method)const
	{ 
		if(root)
		{
			AddressDerived* search = root;
			K searchKey;
	       
			do
			{ 
				searchKey = (search->node->*_method)();

				if(searchKey == _key)
				{ 
					return search->node;
				}
				search = search->links[searchKey < _key];
			} 
			while(search);
		}
		return NIL;
	}
	public: N* GetExtremum(bool _direction)
	{
		if(root)
		{  
			AddressDerived* extremum = root;
			while(extremum->links[_direction])
			{
				extremum = extremum->links[_direction];
			}
				
			return extremum->node;
		}
		return NIL;
	}
	private: template<typename M> void PrefixTraverse(AddressDerived* _address, M& _caller, bool _direction)
	{
		if(_address)
		{
			_caller(_address->node);
			PrefixTraverse(_address->links[_direction], _caller, _direction); 
			PrefixTraverse(_address->links[!_direction], _caller, _direction);	
		}  
	}
	public: template<typename M> void PrefixTraverse(M& _caller, bool _direction = true)
	{
		PrefixTraverse(root, _caller, _direction);
	}
	private: template<typename M> void PostfixTraverse(AddressDerived* _address, M& _caller, bool _direction)
	{
		if(_address)
		{
			PostfixTraverse(_address->links[_direction], _caller, _direction); 
			PostfixTraverse(_address->links[!_direction], _caller, _direction);		
		
			_caller(_address->node);
		}
	}
	public: template<typename M> void PostfixTraverse(M& _caller, bool _direction = true)
	{
		PostfixTraverse(root, _caller, _direction);
	}
	private: template<typename M> void InfixTraverse(AddressDerived* _address, M& _caller, bool _direction)
	{
		if(_address)
		{
			InfixTraverse(_address->links[_direction], _caller, _direction); 		

			_caller(_address->node);

			InfixTraverse(_address->links[!_direction], _caller, _direction);	
		}
	}
	public: template<typename M> void InfixTraverse(M& _caller, bool _direction = true)
	{
		InfixTraverse(root, _caller, _direction);	
	}
	private: void InfixTraverse(AddressDerived* _address, MagicList<N>& _list)
	{
		if(_address)
		{
			InfixTraverse(_address->links[LEFT_CHILD], _list); 		
			_list.AddToTail(_address->node);
			InfixTraverse(_address->links[RIGHT_CHILD], _list);	
		}
	}
	public: MagicList<N> ToList(void)
	{
		MagicList<N> list;
		InfixTraverse(root, list);
		return list;
	}
	private: template<typename M> N* CheckNode(N* _node, M _method)
	{
		if(_node)
		{
			if(ChildOf(_node, LEFT_CHILD))
			{
				if((_node->*_method)() < (ChildOf(_node, LEFT_CHILD)->*_method)())
				{
					return _node;
				}
			}

			if(ChildOf(_node, RIGHT_CHILD))
			{
				if((ChildOf(_node, RIGHT_CHILD)->*_method)() < (_node->*_method)())
				{
					return _node;
				}
			}
		}

		return NIL;
	}
	public: template<typename M> N* CheckTree(N* _root, M _method)
	{
		if(_root)
		{
			if(CheckNode(_root, _method))
			{
				return _root;
			}

			N* leftNode = CheckTree(ChildOf(_root, LEFT_CHILD), _method);
			
			if(leftNode)
			{
				return leftNode;
			}

			N* rightNode = CheckTree(ChildOf(_root, RIGHT_CHILD), _method);

			if(rightNode)
			{
				return rightNode;
			}
		}
		return NIL;
	}
	public: template<typename M> void Test(M _method, int32 _lower, int32 _upper)
	{
		Add(new N(Random::_RandomInt32(_lower, _upper)), _method); 

		if(root)
		{
			N* search = Find(Random::_RandomInt32(_lower, _upper), _method);
			
			if(search)
			{
				Remove(search, MagicContainer::REMOVE_NODE);
  
				Add(search, _method);
			}
			search = Find(Random::_RandomInt32(_lower, _upper), _method);
			
			if(search)
			{
				Rebuild(search, Random::_RandomInt32(0, 1), _method);
			}
			search = Find(Random::_RandomInt32(_lower, _upper), _method);
		
			if(search)
			{
				delete search;
			}
		}
		if(CheckTree(Root(), _method))
		{
			WinAPI::_MessageBox("CheckTree", "Error");
		}
	}
};



#endif
