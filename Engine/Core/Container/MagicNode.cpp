
#include "MagicNode.h"
#include <stdio.h>




MagicNode::MagicNode(void)
{} 

		
MagicNode::MagicNode(const MagicNode& _node)
{
	*this = _node;
}
		 

MagicNode::~MagicNode(void)
{}


MagicNode& MagicNode::operator = (const MagicNode& _node)
{
	return *this;
}
		

bool MagicNode::operator == (const MagicNode& _node)
{
	return true;
}



MagicNode::Address* MagicNode::GetAddress(MagicContainer* _container)const
{
	return connection.Find(_container); 
}
 


void MagicNode::DestroyAllConnections(void)
{
	connection.Clear();
}



SimpleList<MagicContainer*> MagicNode::GetContainers(void)const
{
	SimpleList<MagicContainer*> list;

	Address* node = connection.GetHead();

	while(node)
	{
		list.AddToTail(node->GetContainer());
		node = node->right;
	}
	return list;

}


SimpleList<MagicNode::Address*> MagicNode::GetConnections(void)
{
	SimpleList<Address*> list;
	
	Address* node = connection.GetHead();

	while(node)
	{
		list.AddToTail(node);
		node = node->right;
	}
	return list;

}



int32 MagicNode::GetAmountOfContainers(void)const
{
	return connection.GetSize();
}