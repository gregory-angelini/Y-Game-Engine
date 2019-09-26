#include "Trigger.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



Trigger::ActionTree::ActionTree(void)
{}


Trigger::ActionTree::~ActionTree(void)
{
	Clear();
}


void Trigger::ActionTree::UpdateActionList(void)
{
	trigger->actionList.Clear(MagicContainer::REMOVE_NODE);
	for(SimpleList<StringANSI>::Node* it = trigger->actionExprParser.GetVariables().GetHead(); it != NIL; it = it->GetRight())
	{
		AbstractAction* action = dynamic_cast<AbstractAction*>(trigger->actions.FindObject(it->key));

		if(action) { trigger->actionList.AddToTail(action); }
	}
}


void Trigger::ActionTree::AddAction(AbstractObject* _action)
{
	AddObject(_action);
}



Trigger::ActionTree::AddressDerived* Trigger::ActionTree::AddObject(AbstractObject* _action)
{
	if(_action->IsClassPropertyExist(AbstractAction::ACTION_CLASS))
	{
		if(trigger->actionExprParser.IsVariable(_action->GetName()))
		{
			return ObjectBinaryTree::AddObject(_action);
		}
	}
	return NIL;
}



void Trigger::ActionTree::AddNodeEvent(MagicNode* _node)
{
	AbstractAction* action = dynamic_cast<AbstractAction*>(_node);

	action->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &Trigger::ActionTree::DestroyObjectEvent));
	action->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &Trigger::ActionTree::ActionIsRenamed));
	
	if(GetSize() == trigger->actionExprParser.GetAmountOfUniqueVariables())
	{
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
	}

	UpdateActionList(); 
}



void Trigger::ActionTree::ActionIsRenamed(AbstractObject::NameInfo* _info)
{
	Remove(_info->object);

	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &Trigger::ActionTree::AddAction));
} 


void Trigger::ActionTree::RemoveNodeEvent(MagicNode* _node)
{
	ObjectBinaryTree::RemoveNodeEvent(_node);

	AbstractAction* action = dynamic_cast<AbstractAction*>(_node);
	action->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
	action->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);

	UpdateActionList();
}


void Trigger::ActionTree::DestroyObjectEvent(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &Trigger::ActionTree::AddAction));
}
 