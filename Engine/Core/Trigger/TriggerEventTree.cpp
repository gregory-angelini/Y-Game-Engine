#include "Trigger.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


 
Trigger::EventTree::EventTree(void) 
{} 


Trigger::EventTree::~EventTree(void)
{
	Clear();
}


void Trigger::EventTree::AddVariable(AbstractObject* _event)
{
	AddObject(_event);
}



ObjectBinaryTree::AddressDerived* Trigger::EventTree::AddObject(AbstractObject* _event)
{
	if(_event->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
	{
		if(_event != trigger)
		{
			if(trigger->eventExprParser.IsVariable(_event->GetName()))
			{
				return ObjectBinaryTree::AddObject(_event);
			}
		}
	}
	return NIL;
}


void Trigger::EventTree::ChangeValue(AbstractVariable* _event)
{ 
	if(dynamic_cast<Variable<bool>*>(_event)->GetValue())
	{
		trigger->CalculateBooleanExpr2();
	}
	else
	{
		trigger->CalculateBooleanExpr1();
	}
}



void Trigger::EventTree::AddNodeEvent(MagicNode* _node)
{
	AbstractVariable* event = dynamic_cast<AbstractVariable*>(_node);
			
	event->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &Trigger::EventTree::DestroyObjectEvent));
	event->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &Trigger::EventTree::EventIsRenamed));
	event->Connect(AbstractVariable::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &Trigger::EventTree::ChangeValue));
	if(GetSize() == trigger->eventExprParser.GetAmountOfUniqueVariables())
	{
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);

		trigger->CalculateBooleanExpr1();
	}
}



void Trigger::EventTree::EventIsRenamed(AbstractObject::NameInfo* _info)
{
	Remove(_info->object);

	trigger->CalculateBooleanExpr1();

	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &Trigger::EventTree::AddVariable));
} 



void Trigger::EventTree::RemoveNodeEvent(MagicNode* _node)
{
	ObjectBinaryTree::RemoveNodeEvent(_node);
 
	AbstractVariable* event = dynamic_cast<AbstractVariable*>(_node);
 
	event->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
	event->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
	event->Disconnect(AbstractVariable::CHANGE_VALUE_MESSAGE, this);

}


void Trigger::EventTree::DestroyObjectEvent(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &Trigger::EventTree::AddVariable));

	trigger->CalculateBooleanExpr1();
}
 