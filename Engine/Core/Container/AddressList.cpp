
#include "MagicNode.h"

MagicNode::AddressList::AddressList(void): size(0), isDestroyed(false)
{
	head = tail = NIL;
}


MagicNode::AddressList::AddressList(const AddressList& _container): size(0), isDestroyed(false)
{ 
	head = tail = NIL;
	*this = _container;
}


MagicNode::AddressList::~AddressList(void)
{
	isDestroyed = true;
		
	DestroyList(head);
} 


bool MagicNode::AddressList::IsDestroyed(void)
{
	return isDestroyed;
}


void MagicNode::AddressList::DestroyList(MagicNode::Address* _node)
{
	if(_node)
	{
		DestroyList(_node->right);

		delete _node;
	}
}


int32 MagicNode::AddressList::GetSize(void)const
{ 
	return size;
}


MagicNode::Address* MagicNode::AddressList::GetHead(void)const
{
	return head;
}


MagicNode::Address* MagicNode::AddressList::GetTail(void)const
{
	return tail;
}


MagicNode::Address* MagicNode::AddressList::Find(const MagicContainer* _key)const
{
	Address* search = head;
	while(search)
	{  
		if(search->GetContainer() == _key)
		{
			return search;
		}
		search = search->right;
	}
	return NIL;
}
	  

void MagicNode::AddressList::Remove(Address* _node)
{
	if(_node && _node->container == this)
	{
		_node->Remove();
		_node->left = NIL;
		_node->right = NIL;
	}
}


void MagicNode::AddressList::AddToTail(Address* _node)
{
	if(_node)
	{
		if(head)
		{
			_node->left = tail;
			tail->right = _node;
		}
		else
		{
			head = _node;
		}
		tail = _node;
		_node->container = this;
		++size;
	}
} 
	  
		
void MagicNode::AddressList::Clear(void)
{ 
	while(head)
	{
		delete head;
	}
}