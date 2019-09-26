#include "ObjectTree.h"
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>


ObjectTree::~ObjectTree(void)
{}


ObjectBinaryTree::AddressDerived* ObjectTree::AddObject(AbstractObject* _object)
{
	if(_object == NIL || _object == scene) { return NIL; }

	if(_object->IsClassPropertyExist(ObjectScene::OBJECT_SCENE_CLASS))
	{
		ObjectScene* objectScene = dynamic_cast<ObjectScene*>(_object);

		if(IsParentScene(objectScene))
		{
			return NIL;
		}
	}
	return ObjectBinaryTree::AddObject(_object);
}



bool ObjectTree::IsParentScene(ObjectScene* _scene)
{
	if(_scene->objectTree.childrens.Find(scene) != NIL)
	{
		return true;
	}
	SimpleList<ObjectScene*> list = _scene->objectTree.childrens.ToList();
	SimpleList<ObjectScene*>::Node* it = list.GetHead();

	while(it)
	{
		if(IsParentScene(it->key))
		{
			return true;
		}
		it = it->GetRight();
	}

	return false;
}


void ObjectTree::AddNodeEvent(MagicNode* _node)
{
	AbstractObject* object = dynamic_cast<AbstractObject*>(_node);

	if(object->IsClassPropertyExist(ObjectScene::OBJECT_SCENE_CLASS))
	{
		childrens.Add(dynamic_cast<ObjectScene*>(object));
	}

	ObjectBinaryTree::AddNodeEvent(_node);

	if(scene->IsReceiversExist(ObjectScene::ADD_TO_LAYER_MESSAGE)) { scene->SendPackage(ObjectScene::ADD_TO_LAYER_MESSAGE, object); }

}


void ObjectTree::RemoveNodeEvent(MagicNode* _node)
{
	AbstractObject* object = dynamic_cast<AbstractObject*>(_node);
	
	if(object && object->IsClassPropertyExist(ObjectScene::OBJECT_SCENE_CLASS))
	{
		ObjectScene* objectScene = dynamic_cast<ObjectScene*>(object);

		childrens.Remove(objectScene);
	}

	ObjectBinaryTree::RemoveNodeEvent(_node);
	
	if(scene->IsReceiversExist(ObjectScene::REMOVE_FROM_LAYER_MESSAGE)) { scene->SendPackage(ObjectScene::REMOVE_FROM_LAYER_MESSAGE, object); }
}



AbstractObject* ObjectTree::FindObject(StringANSI _name)
{
	AbstractObject* object = ObjectBinaryTree::FindObject(_name);

	if(object) { return object; }

	if(childrens.IsNotEmpty())
	{
		SimpleList<ObjectScene*> list = childrens.ToList();
		SimpleList<ObjectScene*>::Node* head = list.GetHead();
		
		while(head)
		{
			object = head->key->FindObject(_name);
			if(object) { return object; }
			head = head->GetRight();
		}
	}
	return NIL;
}