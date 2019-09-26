#include "UpdateList.h"
#include <Engine/Core/Scene/LogicScene/LogicScene.h>



UpdateList::UpdateList(void)
{}


UpdateList::~UpdateList(void)
{}



MagicList<LogicObject>::AddressDerived* UpdateList::AddObject(LogicObject* _object)
{
	if(_object == NIL || _object == scene) { return NIL; }

	if(_object->logicContainer.IsNotEmpty())
	{
		dynamic_cast<LogicScene*>(_object->logicContainer.Get())->GetUpdateList().Remove(_object, MagicContainer::REMOVE_NODE);
	}
		
	if(_object->IsClassPropertyExist(LogicScene::LOGIC_SCENE_CLASS))
	{
		LogicScene* it = dynamic_cast<LogicScene*>(_object);

		if(IsParentScene(it))
		{
			return NIL;
		}
	}
	return AddToTail(_object);
}



bool UpdateList::IsParentScene(LogicScene* _scene)
{
	if(scene->logicContainer.IsNotEmpty())
	{
		LogicScene* it = dynamic_cast<LogicScene*>(scene->logicContainer.Get());

		if(it == _scene)
		{
			return true;
		}
		return it->updateList.IsParentScene(_scene);
	}
	return false;
} 


void UpdateList::RemoveNodeEvent(MagicNode* _node)
{
	LogicObject* object = dynamic_cast<LogicObject*>(_node);

	object->logicContainer.Clear(MagicContainer::REMOVE_NODE);
	object->ResetDeltaScale();
	
	if(scene->IsReceiversExist(LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE)) { scene->SendPackage(LogicScene::REMOVE_FROM_UPDATE_LIST_MESSAGE, object); }
} 

 
void UpdateList::AddNodeEvent(MagicNode* _node)
{
	LogicObject* object = dynamic_cast<LogicObject*>(_node);

	 object->logicContainer.Attach(scene);
	 object->UpdateDeltaScale();
		
	if(scene->IsReceiversExist(LogicScene::ADD_TO_UPDATE_LIST_MESSAGE)) { scene->SendPackage(LogicScene::ADD_TO_UPDATE_LIST_MESSAGE, object); }
}


AbstractObject* UpdateList::FindObject(StringANSI _name)
{
	return Find(_name, &AbstractObject::GetName);
}

