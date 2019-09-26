
#include "MagicNode.h"



MagicNode::Address::Address(void): container(NIL)
{
	left = right = NIL;
}



MagicNode::Address::Address(const Address& _node): container(NIL)
{
	left = right = NIL;
	*this = _node;
}


MagicNode::Address::~Address(void)
{
	Remove();
}



MagicNode::Address& MagicNode::Address::operator = (const Address& _node)
{
	return *this;
}



bool MagicNode::Address::operator == (const Address& _node)
{
	return true;
}




void MagicNode::Address::Remove(void)
{
	if(!container->isDestroyed && container)
	{
		if(this == container->head)
		{
			container->head = right;
						
			if(container->head)
			{
				container->head->left = NIL;
			}
			else
			{
				container->tail = NIL;
			}
		}
		else if(this == container->tail)
		{
			container->tail = left;
						
			if(container->tail)
			{
				container->tail->right = NIL;
			}
			else
			{
				container->head = NIL;
			}
		}
		else
		{
			left->right = right;
			right->left = left;
		}
		container->size--;
		container = NIL;
	}
}
