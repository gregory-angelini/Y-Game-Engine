#include "AddToListAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Parser/List/AbstractObjectList/AbstractObjectList.h>
#include <Engine/Core/Parser/Set/Set.h>



AddToListAction::AddToListAction(void)
{}


AddToListAction::AddToListAction(int32 _type): AbstractAction(_type)
{}



AddToListAction::~AddToListAction(void)
{
	Destroying();
}


void AddToListAction::UpdateValid(void)
{
	valid = false; 

	if(arg1.IsNotEmpty() && arg2.IsNotEmpty() && arg3.IsNotEmpty() && target.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
		valid = true;
	}
}

void AddToListAction::SetVariableArg(int32 _index, StringANSI _name)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg1.Destroy();
			
			SetArgMode(_index, VARIABLE);

			Arg1IsLost(); 
			
			arg1Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg1Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg1Name);

			if(arg1.IsEmpty())
			{
				if(object)
				{
					if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(arg1.Attach(object))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
			break;
		}

		case ARG2:
		{
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg2.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg2IsLost(); 
			
			arg2Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg2Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg2Name);

			if(arg2.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
					{
						if(arg2.Attach(dynamic_cast<Variable<int32>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg2IsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}

		case ARG3:
		{
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg3.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg3IsLost(); 
			
			arg3Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg3Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg3Name);

			if(arg3.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS)) 
					{
						if(arg3.Attach(dynamic_cast<Variable<int32>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg3IsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}

		case TARGET:
		{  
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateTarget.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateTarget.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::LIST_ASSET;

			TargetIsLost();
  
			targetName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(targetName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(targetName);

			if(target.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(AbstractList::ABSTRACT_LIST_CLASS)) 
					{
						if(target.Attach(dynamic_cast<AbstractList*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::TargetIsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}
	}
}


void AddToListAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &AddToListAction::TemplateArg1IsCompleted));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &AddToListAction::Arg1IsLost));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &AddToListAction::TemplateArg1IsChanged));
			
			SetArgMode(_index, TEMPLATE);

			Arg1IsLost(); 

			if(templateArg1.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg1IsCompleted();
			}
			break;
		}  

		case ARG2:
		{ 
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &AddToListAction::TemplateArg2IsCompleted));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &AddToListAction::Arg2IsLost));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &AddToListAction::TemplateArg2IsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg2IsLost(); 

			if(templateArg2.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg2IsCompleted();
			}
			break;
		}  

		case ARG3:
		{
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &AddToListAction::TemplateArg3IsCompleted));
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &AddToListAction::Arg3IsLost));
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &AddToListAction::TemplateArg3IsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg3IsLost(); 

			if(templateArg3.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg3IsCompleted();
			}
			break;
		}  

		case TARGET:
		{
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &AddToListAction::TemplateTargetIsCompleted));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &AddToListAction::TargetIsLost));
			templateTarget.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &AddToListAction::TemplateTargetIsChanged));
			
			SetArgMode(_index, TEMPLATE);  
			state[ASSET_TYPE][_index] = AssetLibrary::LIST_ASSET;

			TargetIsLost(); 

			if(templateTarget.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateTargetIsCompleted();
			}
			break;
		}
	}
}



void AddToListAction::TemplateArg1IsCompleted(void)
{
	if(templateArg1.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg1.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg1.GetResult());

		if(arg1.IsEmpty())
		{
			if(object)
			{
				if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
				if(arg1.Attach(object))
				{
					object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
					object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg1IsRenamed));
					UpdateValid();
				}
				return;
			}
		}
	}
}


void AddToListAction::Arg1IsLost(void)
{
	if(arg1.IsNotEmpty())
	{ 
		arg1.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg1.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg1.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsCreated));
	valid = false;
}



void AddToListAction::TemplateArg1IsChanged(void)
{
	Arg1IsLost();

	TemplateArg1IsCompleted(); 
}



void AddToListAction::TemplateArg2IsCompleted(void)
{
	if(templateArg2.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg2.GetResult(), state[ASSET_TYPE][ARG2]) : AssetLibrary::_GetAssets().FindObject(templateArg2.GetResult());

		if(arg2.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS)) 
				{
					if(arg2.Attach(dynamic_cast<Variable<int32>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void AddToListAction::Arg2IsLost(void)
{
	if(arg2.IsNotEmpty())
	{ 
		arg2.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg2.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg2.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsCreated));
	valid = false;
}



void AddToListAction::TemplateArg2IsChanged(void)
{
	Arg2IsLost();

	TemplateArg2IsCompleted(); 
}



void AddToListAction::TemplateArg3IsCompleted(void)
{
	if(templateArg3.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg3.GetResult(), state[ASSET_TYPE][ARG3]) : AssetLibrary::_GetAssets().FindObject(templateArg3.GetResult());

		if(arg3.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS)) 
				{
					if(arg3.Attach(dynamic_cast<Variable<int32>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void AddToListAction::Arg3IsLost(void)
{
	if(arg3.IsNotEmpty())
	{ 
		arg3.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg3.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg3.Clear();
		
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsCreated));
	valid = false;
}



void AddToListAction::TemplateArg3IsChanged(void)
{
	Arg3IsLost();

	TemplateArg3IsCompleted(); 
}



void AddToListAction::TemplateTargetIsCompleted(void)
{
	if(templateTarget.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateTarget.GetResult(), state[ASSET_TYPE][TARGET]) : AssetLibrary::_GetAssets().FindObject(templateTarget.GetResult());

		if(target.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(AbstractList::ABSTRACT_LIST_CLASS)) 
				{
					if(target.Attach(dynamic_cast<AbstractList*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void AddToListAction::TargetIsLost(void)
{
	if(target.IsNotEmpty())
	{
		target.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		target.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		target.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsCreated));
	valid = false;
}



void AddToListAction::TemplateTargetIsChanged(void)
{
	TargetIsLost();

	TemplateTargetIsCompleted(); 
}



StringANSI AddToListAction::GetTemplateArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return templateArg1.GetStringExpr();
		}

		case ARG2:
		{
			return templateArg2.GetStringExpr();
		}

		case ARG3:
		{
			return templateArg3.GetStringExpr();
		}

		case TARGET:
		{
			return templateTarget.GetStringExpr();
		}
	}
	return "";
}



StringANSI AddToListAction::GetVariableArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return arg1Name;
		}

		case ARG2:
		{
			return arg2Name;
		}
		
		case ARG3:
		{
			return arg3Name;
		}

		case TARGET:
		{
			return targetName;
		}
	}
	return "";
}


int32 AddToListAction::GetIntegerKeyArg(int32 _index)
{
	switch(_index)
	{
		case ARG2:
		{
			if(arg2.IsNotEmpty())
			{
				return arg2.Get()->GetValue();
			}
		}

		case ARG3:
		{
			if(arg3.IsNotEmpty())
			{
				return arg3.Get()->GetValue();
			}
		}
	}
	return -1;
}



void AddToListAction::SetIntegerKeyArg(int32 _index, int32 _value)
{
	switch(_index)
	{
		case ARG2:
		{ 
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg2.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg2.Destroy();
			
			SetArgMode(_index, KEY);

			Arg2IsLost();
			Variable<int32>* object = new Variable<int32>();
			object->SetValue(_value);
			
			if(arg2.Attach(object))
			{
				UpdateValid();
			}
			break;
		}

		case ARG3:
		{
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg3.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg3.Destroy();
			
			SetArgMode(_index, KEY);

			Arg3IsLost();
			Variable<int32>* object = new Variable<int32>();
			object->SetValue(_value);
			
			if(arg3.Attach(object))
			{
				UpdateValid();
			}
			break;
		}
	}
}


void AddToListAction::operator () (void)
{
	if(IsValid()) 
	{
		switch(arg3.Get()->GetValue())
		{
			case AbstractList::SHARE_ITEM:
			{
				switch(target.Get()->GetValueType())
				{
					case List<AbstractObject>::OBJECT_TYPE:
					{
						target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						break;
					}
					
					case List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{ 
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<ScalableObject>::SCALABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<RotatableObject>::ROTATABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						} 
						else if(arg1.Get()->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<ColorableObject>::COLORABLE_OBJECT_TYPE:
					{ 
						if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<Set>::SET_TYPE:
					{ 
						if(arg1.Get()->IsClassPropertyExist(Set::SET_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}  
					
					case List<int32>::INTEGER_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<float>::FLOAT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<bool>::BOOLEAN_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					} 

					case List<StringANSI>::STRING_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<Vector2>::VECTOR_2_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<Vector3>::VECTOR_3_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}

					case List<Quaternion>::QUATERNION_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS))
						{
							target.Get()->AddItem(arg1.Get(), arg2.Get()->GetValue());
						}
						break;
					}
				}
				break;
			}
			
			case AbstractList::COPY_ONLY_DATA:
			{ 
				switch(target.Get()->GetValueType())
				{
					case List<AbstractObject>::OBJECT_TYPE:
					{
						break;
					}
					
					case List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{ 
							TranslatableObject* position = dynamic_cast<TranslatableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							TranslatableObject* arg = dynamic_cast<TranslatableObject*>(arg1.Get());

							if(position && arg) { position->SetPosition(arg->GetPosition()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							TranslatableObject* position = dynamic_cast<TranslatableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							ScalableObject* arg = dynamic_cast<ScalableObject*>(arg1.Get());

							if(position && arg) { position->SetPosition(arg->GetScale()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{
							TranslatableObject* position = dynamic_cast<TranslatableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							ColorableObject* arg = dynamic_cast<ColorableObject*>(arg1.Get());

							if(position && arg) { position->SetPosition(arg->GetColorRGB()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							TranslatableObject* position = dynamic_cast<TranslatableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<Vector3>* arg = dynamic_cast<Variable<Vector3>*>(arg1.Get());

							if(position && arg) { position->SetPosition(arg->GetValue()); } 
						}
						break;
					}

					case List<ScalableObject>::SCALABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							ScalableObject* scaling = dynamic_cast<ScalableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							ScalableObject* arg = dynamic_cast<ScalableObject*>(arg1.Get());

							if(scaling && arg) { scaling->SetScale(arg->GetScale()); }  
						}
						else if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{ 
							ScalableObject* scaling = dynamic_cast<ScalableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							TranslatableObject* arg = dynamic_cast<TranslatableObject*>(arg1.Get());

							if(scaling && arg) { scaling->SetScale(arg->GetPosition()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{ 
							ScalableObject* scaling = dynamic_cast<ScalableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							ColorableObject* arg = dynamic_cast<ColorableObject*>(arg1.Get());

							if(scaling && arg) { scaling->SetScale(arg->GetColorRGB()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							ScalableObject* scaling = dynamic_cast<ScalableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<Vector3>* arg = dynamic_cast<Variable<Vector3>*>(arg1.Get());

							if(scaling && arg) { scaling->SetScale(arg->GetValue()); } 
						}
						break;
					}

					case List<RotatableObject>::ROTATABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS))
						{
							RotatableObject* rotation = dynamic_cast<RotatableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							RotatableObject* arg = dynamic_cast<RotatableObject*>(arg1.Get());

							if(rotation && arg) { rotation->SetRotation(arg->GetRotation()); }  
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS))
						{
							RotatableObject* rotation = dynamic_cast<RotatableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<Quaternion>* arg = dynamic_cast<Variable<Quaternion>*>(arg1.Get());

							if(rotation && arg) { rotation->SetRotation(arg->GetValue()); }  
						}
						break;
					}

					case List<ColorableObject>::COLORABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{
							ColorableObject* color = dynamic_cast<ColorableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							ColorableObject* arg = dynamic_cast<ColorableObject*>(arg1.Get());

							if(color && arg) { color->SetColorRGBA(arg->GetColorRGBA()); }  
						}
						else if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{ 
							ColorableObject* color = dynamic_cast<ColorableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							TranslatableObject* arg = dynamic_cast<TranslatableObject*>(arg1.Get());

							if(color && arg) { color->SetColorRGB(arg->GetPosition()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							ColorableObject* color = dynamic_cast<ColorableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							ScalableObject* arg = dynamic_cast<ScalableObject*>(arg1.Get());

							if(color && arg) { color->SetColorRGB(arg->GetScale()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							ColorableObject* color = dynamic_cast<ColorableObject*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<Vector3>* arg = dynamic_cast<Variable<Vector3>*>(arg1.Get());

							if(color && arg) { color->SetColorRGB(arg->GetValue()); } 
						}
						break;
					}
					
					case List<Set>::SET_TYPE:
					{ 
						if(arg1.Get()->IsClassPropertyExist(Set::SET_CLASS))
						{
							Set* object = dynamic_cast<Set*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Set* arg = dynamic_cast<Set*>(arg1.Get());

							if(object && arg) { *object = *arg; }  
						}
						break;
					}

					case List<int32>::INTEGER_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
						{
							Variable<int32>* variable = dynamic_cast<Variable<int32>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<int32>* arg = dynamic_cast<Variable<int32>*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetValue()); }  
						}
						break;
					}

					case List<float>::FLOAT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
						{
							Variable<float>* variable = dynamic_cast<Variable<float>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<float>* arg = dynamic_cast<Variable<float>*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetValue()); }  
						}
						break;
					}

					case List<bool>::BOOLEAN_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
						{
							Variable<bool>* variable = dynamic_cast<Variable<bool>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<bool>* arg = dynamic_cast<Variable<bool>*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetValue()); }  
						}
						break;
					}

					case List<StringANSI>::STRING_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS))
						{
							Variable<StringANSI>* variable = dynamic_cast<Variable<StringANSI>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<StringANSI>* arg = dynamic_cast<Variable<StringANSI>*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetValue()); }  
						}
						else
						{  
							Variable<StringANSI>* variable = dynamic_cast<Variable<StringANSI>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							if(variable) { variable->SetValue(arg1.Get()->GetName()); }  
						}
						break;
					}

					case List<Vector2>::VECTOR_2_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS))
						{
							Variable<Vector2>* variable = dynamic_cast<Variable<Vector2>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<Vector2>* arg = dynamic_cast<Variable<Vector2>*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetValue()); }  
						}
						break;
					}

					case List<Vector3>::VECTOR_3_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							Variable<Vector3>* variable = dynamic_cast<Variable<Vector3>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<Vector3>* arg = dynamic_cast<Variable<Vector3>*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetValue()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{
							Variable<Vector3>* variable = dynamic_cast<Variable<Vector3>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							TranslatableObject* arg = dynamic_cast<TranslatableObject*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetPosition()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							Variable<Vector3>* variable = dynamic_cast<Variable<Vector3>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							ScalableObject* arg = dynamic_cast<ScalableObject*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetScale()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{ 
							Variable<Vector3>* variable = dynamic_cast<Variable<Vector3>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							ColorableObject* arg = dynamic_cast<ColorableObject*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetColorRGB()); } 
						} 
						break;
					}

					case List<Quaternion>::QUATERNION_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS))
						{
							Variable<Quaternion>* variable = dynamic_cast<Variable<Quaternion>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							Variable<Quaternion>* arg = dynamic_cast<Variable<Quaternion>*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetValue()); } 
						}
						else if(arg1.Get()->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS))
						{
							Variable<Quaternion>* variable = dynamic_cast<Variable<Quaternion>*>(target.Get()->GetItem(arg2.Get()->GetValue()));

							RotatableObject* arg = dynamic_cast<RotatableObject*>(arg1.Get());

							if(variable && arg) { variable->SetValue(arg->GetRotation()); }
						}
						break;
					}
				}
				break;
			}

			case AbstractList::COPY_IN_VARIABLE:
			{
				switch(target.Get()->GetValueType())
				{
					case List<AbstractObject>::OBJECT_TYPE:
					{
						break;
					}
					
					case List<TranslatableObject>::TRANSLATABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{ 
							TranslatableObject* arg = dynamic_cast<TranslatableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetPosition());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{
							ColorableObject* arg = dynamic_cast<ColorableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetColorRGB());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						} 
						else if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							ScalableObject* arg = dynamic_cast<ScalableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetScale());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							Variable<Vector3>* arg = dynamic_cast<Variable<Vector3>*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						break;
					}

					case List<ScalableObject>::SCALABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							ScalableObject* arg = dynamic_cast<ScalableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetScale());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{ 
							TranslatableObject* arg = dynamic_cast<TranslatableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetPosition());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{
							ColorableObject* arg = dynamic_cast<ColorableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetColorRGB());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						} 
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							Variable<Vector3>* arg = dynamic_cast<Variable<Vector3>*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							} 
						}
						break;
					}

					case List<RotatableObject>::ROTATABLE_OBJECT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS))
						{
							RotatableObject* arg = dynamic_cast<RotatableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Quaternion>* variable = new Variable<Quaternion>(arg->GetRotation());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS))
						{
							Variable<Quaternion>* arg = dynamic_cast<Variable<Quaternion>*>(arg1.Get());
							
							if(arg)
							{
								Variable<Quaternion>* variable = new Variable<Quaternion>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						break;
					}

					case List<ColorableObject>::COLORABLE_OBJECT_TYPE:
					{ 
						if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{
							ColorableObject* arg = dynamic_cast<ColorableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetColorRGB());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						} 
						else if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{ 
							TranslatableObject* arg = dynamic_cast<TranslatableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetPosition());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							ScalableObject* arg = dynamic_cast<ScalableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetScale());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							Variable<Vector3>* arg = dynamic_cast<Variable<Vector3>*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						break;
					}

					case List<Set>::SET_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Set::SET_CLASS))
						{
							Set* arg = dynamic_cast<Set*>(arg1.Get());
							
							if(arg)
							{
								Set* object = new Set(*arg);

								if(target.Get()->AddItem(object, arg2.Get()->GetValue()) == NIL)
								{
									delete object;
								}
							}
						}
						break;
					}

					case List<int32>::INTEGER_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
						{
							Variable<int32>* arg = dynamic_cast<Variable<int32>*>(arg1.Get());
							
							if(arg)
							{
								Variable<int32>* variable = new Variable<int32>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						break;
					}

					case List<float>::FLOAT_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<float>::FLOAT_VARIABLE_CLASS))
						{
							Variable<float>* arg = dynamic_cast<Variable<float>*>(arg1.Get());
							
							if(arg)
							{
								Variable<float>* variable = new Variable<float>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						break;
					}

					case List<bool>::BOOLEAN_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
						{
							Variable<bool>* arg = dynamic_cast<Variable<bool>*>(arg1.Get());
							
							if(arg)
							{
								Variable<bool>* variable = new Variable<bool>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						break;
					}

					case List<StringANSI>::STRING_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS))
						{
							Variable<StringANSI>* arg = dynamic_cast<Variable<StringANSI>*>(arg1.Get());
							
							if(arg)
							{
								Variable<StringANSI>* variable = new Variable<StringANSI>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else
						{ 
							Variable<StringANSI>* variable = new Variable<StringANSI>(arg1.Get()->GetName());

							if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
							{
								delete variable;
							}
						}
						break;
					}

					case List<Vector2>::VECTOR_2_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Vector2>::VECTOR_2_VARIABLE_CLASS))
						{
							Variable<Vector2>* arg = dynamic_cast<Variable<Vector2>*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector2>* variable = new Variable<Vector2>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						break;
					}

					case List<Vector3>::VECTOR_3_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Vector3>::VECTOR_3_VARIABLE_CLASS))
						{
							Variable<Vector3>* arg = dynamic_cast<Variable<Vector3>*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(TranslatableObject::TRANSLATABLE_OBJECT_CLASS))
						{
							TranslatableObject* arg = dynamic_cast<TranslatableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetPosition());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(ScalableObject::SCALABLE_OBJECT_CLASS))
						{
							ScalableObject* arg = dynamic_cast<ScalableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetScale());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(ColorableObject::COLORABLE_OBJECT_CLASS))
						{
							ColorableObject* arg = dynamic_cast<ColorableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Vector3>* variable = new Variable<Vector3>(arg->GetColorRGB());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						} 
						break;
					}

					case List<Quaternion>::QUATERNION_TYPE:
					{
						if(arg1.Get()->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS))
						{
							Variable<Quaternion>* arg = dynamic_cast<Variable<Quaternion>*>(arg1.Get());
							
							if(arg)
							{
								Variable<Quaternion>* variable = new Variable<Quaternion>(arg->GetValue());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						else if(arg1.Get()->IsClassPropertyExist(RotatableObject::ROTATABLE_OBJECT_CLASS))
						{
							RotatableObject* arg = dynamic_cast<RotatableObject*>(arg1.Get());
							
							if(arg)
							{
								Variable<Quaternion>* variable = new Variable<Quaternion>(arg->GetRotation());

								if(target.Get()->AddItem(variable, arg2.Get()->GetValue()) == NIL)
								{
									delete variable;
								}
							}
						}
						break;
					}
				}
				break;
			}
		}
	}
}



void AddToListAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsCreated));
	valid = false;
}



void AddToListAction::TargetIsRenamed(AbstractObject::NameInfo* _info)
{
	TargetIsLost();
}

 

void AddToListAction::Arg1IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg1IsLost();
}


void AddToListAction::Arg2IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg2IsLost();
}


void AddToListAction::Arg3IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg3IsLost();
}


void AddToListAction::ObjectIsCreated(AbstractObject* _object)
{
	if(target.IsEmpty())
	{
		if(state[TEMPLATE][TARGET])
		{
			if(_object->IsClassPropertyExist(AbstractList::ABSTRACT_LIST_CLASS)) 
			{ 
				if(templateTarget.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateTarget.GetResult() == _object->GetName())
				{
					if(target.Attach(dynamic_cast<AbstractList*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][TARGET])
		{
			if(_object->IsClassPropertyExist(AbstractList::ABSTRACT_LIST_CLASS)) 
			{
				if(targetName == _object->GetName())
				{
					if(target.Attach(dynamic_cast<AbstractList*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::TargetIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg1.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG1])
		{
			if(templateArg1.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg1.GetResult() == _object->GetName())
			{
				if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
				if(arg1.Attach(_object))
				{
					_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
					_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg1IsRenamed));
					UpdateValid();
				}
				return;
			}
		} 
		else if(state[VARIABLE][ARG1])
		{
			if(arg1Name == _object->GetName())
			{ 
				if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
				if(arg1.Attach(_object))
				{
					_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
					_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg1IsRenamed));
					UpdateValid();
				}
				return;
			}
		}
	}
	if(arg2.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG2])
		{
			if(_object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
			{
				if(templateArg2.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg2.GetResult() == _object->GetName())
				{
					if(arg2.Attach(dynamic_cast<Variable<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG2])
		{
			if(_object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
			{
				if(arg2Name == _object->GetName())
				{
					if(arg2.Attach(dynamic_cast<Variable<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg3.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG3])
		{
			if(_object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
			{
				if(templateArg3.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg3.GetResult() == _object->GetName())
				{
					if(arg3.Attach(dynamic_cast<Variable<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG3])
		{
			if(_object->IsClassPropertyExist(Variable<int32>::INTEGER_VARIABLE_CLASS))
			{
				if(arg3Name == _object->GetName())
				{
					if(arg3.Attach(dynamic_cast<Variable<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &AddToListAction::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &AddToListAction::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool AddToListAction::SaveToFile(StringANSI _path)			
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + actionsDir + GetName() + "." + actionsSuffix;
	} 

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool AddToListAction::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(type);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		for(int32 i = 0; i < AbstractAction::WIDTH; i++)
		{
			for(int32 j = 0; j < AbstractAction::HEIGHT; j++)
			{
				_file.Write(state[i][j]);
			}
		}
		_file.Write(activationLimit);
		_file.Write(loadArgsEnable);
		if(state[TEMPLATE][ARG1])
		{
			_file.Write(GetTemplateArg(ARG1).length());
			_file.WriteString(GetTemplateArg(ARG1));
		}
		else if(state[VARIABLE][ARG1])
		{
			_file.Write(GetVariableArg(ARG1).length());
			_file.WriteString(GetVariableArg(ARG1));
		}
		if(state[TEMPLATE][ARG2])
		{
			_file.Write(GetTemplateArg(ARG2).length());
			_file.WriteString(GetTemplateArg(ARG2));
		}
		else if(state[VARIABLE][ARG2])
		{
			_file.Write(GetVariableArg(ARG2).length());
			_file.WriteString(GetVariableArg(ARG2));
		}
		else if(state[KEY][ARG2])
		{
			_file.Write(GetIntegerKeyArg(ARG2));
		}
		if(state[TEMPLATE][ARG3])
		{
			_file.Write(GetTemplateArg(ARG3).length());
			_file.WriteString(GetTemplateArg(ARG3));
		}
		else if(state[VARIABLE][ARG3])
		{
			_file.Write(GetVariableArg(ARG3).length());
			_file.WriteString(GetVariableArg(ARG3));
		}
		else if(state[KEY][ARG3])
		{
			_file.Write(GetIntegerKeyArg(ARG3));
		}
		if(state[TEMPLATE][TARGET])
		{
			_file.Write(GetTemplateArg(TARGET).length());
			_file.WriteString(GetTemplateArg(TARGET));
		}
		else if(state[VARIABLE][TARGET])
		{
			_file.Write(GetVariableArg(TARGET).length());
			_file.WriteString(GetVariableArg(TARGET));
		} 
		return true;
	}
	return false;
}


AbstractAction* AddToListAction::_LoadFromFile(File& _file)
{
	AddToListAction* action = new AddToListAction(AbstractList::ADD_TO_LIST_MESSAGE);
	StringANSI name;
	int32 length;
	_file.Read(length);
	_file.ReadString(name, length);  
	action->Rename(name);
	for(int32 i = 0; i < AbstractAction::WIDTH; i++)
	{
		for(int32 j = 0; j < AbstractAction::HEIGHT; j++)
		{
			_file.Read(action->state[i][j]);
		}
	}
	int32 activationLimit;
	_file.Read(activationLimit); 
	action->SetActivationLimit(activationLimit);
	bool loadArgsEnable;
	_file.Read(loadArgsEnable); 
	action->SetLoadArgsEnable(loadArgsEnable);
	if(action->state[TEMPLATE][ARG1])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(ARG1, templateArg); 
	}
	else if(action->state[VARIABLE][ARG1])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG1, variableArg);
	}
	if(action->state[TEMPLATE][ARG2])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(ARG2, templateArg); 
	}
	else if(action->state[VARIABLE][ARG2])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG2, variableArg);
	}
	else if(action->state[KEY][ARG2])
	{
		int32 arg; 
		_file.Read(arg);
		action->SetIntegerKeyArg(ARG2, arg);
	}
	if(action->state[TEMPLATE][ARG3])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(ARG3, templateArg); 
	}
	else if(action->state[VARIABLE][ARG3])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG3, variableArg);
	}
	else if(action->state[KEY][ARG3])
	{
		int32 arg; 
		_file.Read(arg);
		action->SetIntegerKeyArg(ARG3, arg);
	}
	if(action->state[TEMPLATE][TARGET])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(TARGET, templateArg); 
	}
	else if(action->state[VARIABLE][TARGET])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(TARGET, variableArg);
	}
	return action;
}