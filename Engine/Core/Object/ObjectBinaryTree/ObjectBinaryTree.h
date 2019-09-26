#ifndef OBJECTBINARYTREE_H
#define OBJECTBINARYTREE_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
#include <Engine/Core/MessageHandler/MessageHandler.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
class ObjectBinaryTree: public MagicBinaryTree<AbstractObject>,
	                    public MessageHandler<int32>::Receiver
{
	public: ObjectBinaryTree(void);
	public: ObjectBinaryTree(const ObjectBinaryTree& _tree);
	public: virtual ~ObjectBinaryTree(void);
	public: virtual void RemoveNodeEvent(MagicNode* _node);
	public: virtual AddressDerived* AddObject(AbstractObject* _object);
	public: virtual void AddNodeEvent(MagicNode* _node);
	public: virtual void RenameObjectEvent(AbstractObject::NameInfo* _info);
	private: void CheckObjectEvent(AbstractObject::RenameInfo* _info);
	public: virtual AbstractObject* FindObject(StringANSI _name);
};


#endif 