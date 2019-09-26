#include "ObjectBinaryTree.h"


ObjectBinaryTree::ObjectBinaryTree(void)
{}


ObjectBinaryTree::ObjectBinaryTree(const ObjectBinaryTree& _tree)
{ 
	*this = _tree;
}


ObjectBinaryTree::~ObjectBinaryTree(void)
{}


MagicBinaryTree<AbstractObject>::AddressDerived* ObjectBinaryTree::AddObject(AbstractObject* _object)
{
	return Add(_object, &AbstractObject::GetName);
}


void ObjectBinaryTree::AddNodeEvent(MagicNode* _node)
{
	AbstractObject* object = dynamic_cast<AbstractObject*>(_node);

	if(!object->renaming)
	{
		object->Connect(AbstractObject::CHECK_OBJECT_NAME_MESSAGE, this, Caller<>(this, &ObjectBinaryTree::CheckObjectEvent));
		object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ObjectBinaryTree::RenameObjectEvent));
	}
}


void ObjectBinaryTree::RemoveNodeEvent(MagicNode* _node)
{
	AbstractObject* object = dynamic_cast<AbstractObject*>(_node);

	if(!object->renaming)
	{
		object->Disconnect(AbstractObject::CHECK_OBJECT_NAME_MESSAGE, this);
		object->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
	}
}


void ObjectBinaryTree::CheckObjectEvent(AbstractObject::RenameInfo* _info)
{
	if(!_info->reserved)
	{
		if(FindObject(_info->newName))
		{
			_info->reserved = true;
		}
	}
}


void ObjectBinaryTree::RenameObjectEvent(AbstractObject::NameInfo* _info)
{

	_info->object->name = _info->oldName;
	Remove(_info->object, MagicContainer::REMOVE_NODE);

	_info->object->name = _info->newName;
	Add(_info->object, &AbstractObject::GetName);
}


AbstractObject* ObjectBinaryTree::FindObject(StringANSI _name)
{
	return Find(_name, &AbstractObject::GetName);
}


