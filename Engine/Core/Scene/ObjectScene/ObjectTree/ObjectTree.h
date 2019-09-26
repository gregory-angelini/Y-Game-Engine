#ifndef OBJECTTREE_H
#define OBJECTTREE_H
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Object/ObjectBinaryTree/ObjectBinaryTree.h>
#include <Engine/Core/Container/Tree/SimpleBinaryTree/SimpleBinaryTree.h>
class ObjectTree: public ObjectBinaryTree
{
	friend class ObjectScene;
	private: SimpleBinaryTree<ObjectScene*> childrens;
	private: ObjectScene* scene;
	public: virtual ~ObjectTree(void);
	public: virtual AddressDerived* AddObject(AbstractObject* _object);
	private: bool IsParentScene(ObjectScene* _scene);
	public: virtual void RemoveNodeEvent(MagicNode* _node);
	public: virtual void AddNodeEvent(MagicNode* _node);
	public: virtual AbstractObject* FindObject(StringANSI _name);
};


#endif 