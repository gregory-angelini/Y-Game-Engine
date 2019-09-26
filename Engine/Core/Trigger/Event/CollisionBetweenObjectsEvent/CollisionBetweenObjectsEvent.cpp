#include "CollisionBetweenObjectsEvent.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>


CollisionBetweenObjectsEvent::CollisionBetweenObjectsEvent(void)
{}



CollisionBetweenObjectsEvent::CollisionBetweenObjectsEvent(int32 _type): AbstractEvent(_type)
{}



CollisionBetweenObjectsEvent::~CollisionBetweenObjectsEvent(void)
{
	Destroying();
}


void CollisionBetweenObjectsEvent::UpdateValid(void)
{
	if(((arg1.IsNotEmpty() && arg2.IsNotEmpty() && arg3.IsNotEmpty() && arg4.IsNotEmpty() && arg5.IsNotEmpty()) || !argsEnable) && source.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
	}
}



void CollisionBetweenObjectsEvent::SetVariableArg(int32 _index, StringANSI _name)
{
	if(_name.empty()) { return; } 

	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg1.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg1.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::OBJECT_ASSET;

			Arg1IsLost(); 
			
			arg1Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg1Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg1Name);

			if(arg1.IsEmpty())
			{ 
				if(object)
				{
					if(object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
					{
						if(arg1.Attach(dynamic_cast<PolygonalSurface*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg1IsRenamed));
							UpdateValid();
						}
						return;
					}
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
					if(object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
					{
						if(arg2.Attach(dynamic_cast<Variable<StringANSI>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg2IsRenamed));
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
					if(object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
					{
						if(arg3.Attach(dynamic_cast<Variable<StringANSI>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg3IsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}
 
		case ARG4:
		{ 
			templateArg4.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg4.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg4.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg4.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::LIST_ASSET;

			Arg4IsLost(); 
			
			arg4Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg4Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg4Name);

			if(arg4.IsEmpty())
			{ 
				if(object)
				{
					if(object->IsClassPropertyExist(List<int32>::INTEGER_LIST_CLASS)) 
					{
						if(arg4.Attach(dynamic_cast<List<int32>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg4IsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}

		case ARG5:
		{ 
			templateArg5.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg5.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg5.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg5.Destroy();
			
			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::LIST_ASSET;

			Arg5IsLost(); 
			
			arg5Name = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg5Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg5Name);

			if(arg5.IsEmpty())
			{ 
				if(object)
				{
					if(object->IsClassPropertyExist(List<int32>::INTEGER_LIST_CLASS)) 
					{
						if(arg5.Attach(dynamic_cast<List<int32>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg5IsRenamed));
							UpdateValid();
						}
						return;
					}
				}
			}
			break;
		}

		case SOURCE:
		{
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateSource.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateSource.Destroy();
			
			SetArgMode(_index, VARIABLE); 
			state[ASSET_TYPE][_index] = AssetLibrary::OBJECT_ASSET;

			SourceIsLost();
  
			sourceName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(sourceName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(sourceName);

			if(source.IsEmpty())
			{
				if(object)
				{
					if(object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
					{
						if(source.Attach(dynamic_cast<PolygonalSurface*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::SourceIsRenamed));
							object->Connect(PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::EventHandler));
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


void CollisionBetweenObjectsEvent::SetTemplateArg(int32 _index, StringANSI _expr)
{
	if(_expr.empty()) { return; }

	switch(_index)
	{
		case ARG1:
		{
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg1IsCompleted));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg1IsLost));
			templateArg1.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg1IsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::OBJECT_ASSET;

			Arg1IsLost(); 

			if(templateArg1.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg1IsCompleted();
			}
			break;
		}  

		case ARG2:
		{ 
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg2IsCompleted));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg2IsLost));
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg2IsChanged));
			
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
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg3IsCompleted));
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg3IsLost));
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg3IsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			Arg3IsLost(); 

			if(templateArg3.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg3IsCompleted();
			}
			break;
		}

		case ARG4:
		{ 
			templateArg4.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg4IsCompleted));
			templateArg4.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg4IsLost));
			templateArg4.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg4IsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::LIST_ASSET;

			Arg4IsLost(); 

			if(templateArg4.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{ 
				TemplateArg4IsCompleted();
			}
			break;
		}  
 
		case ARG5:
		{ 
			templateArg5.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg5IsCompleted));
			templateArg5.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg5IsLost));
			templateArg5.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateArg5IsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::LIST_ASSET;

			Arg5IsLost(); 

			if(templateArg5.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateArg5IsCompleted();
			}
			break;
		}  

		case SOURCE:
		{
			templateSource.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateSourceIsCompleted));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::SourceIsLost));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::TemplateSourceIsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::OBJECT_ASSET;

			SourceIsLost(); 

			if(templateSource.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateSourceIsCompleted();
			}
			break;
		}
	}
}



void CollisionBetweenObjectsEvent::TemplateArg1IsCompleted(void)
{
	if(templateArg1.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg1.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg1.GetResult());

		if(arg1.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
				{
					if(arg1.Attach(dynamic_cast<PolygonalSurface*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void CollisionBetweenObjectsEvent::Arg1IsLost(void)
{
	if(arg1.IsNotEmpty())
	{
		arg1.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg1.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg1.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsCreated));
}




void CollisionBetweenObjectsEvent::TemplateArg1IsChanged(void)
{
	Arg1IsLost();

	TemplateArg1IsCompleted(); 
}



void CollisionBetweenObjectsEvent::TemplateArg2IsCompleted(void)
{
	if(templateArg2.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg2.GetResult(), state[ASSET_TYPE][ARG2]) : AssetLibrary::_GetAssets().FindObject(templateArg2.GetResult());

		if(arg2.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
				{
					if(arg2.Attach(dynamic_cast<Variable<StringANSI>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void CollisionBetweenObjectsEvent::Arg2IsLost(void)
{
	if(arg2.IsNotEmpty())
	{
		arg2.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg2.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg2.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsCreated));
}




void CollisionBetweenObjectsEvent::TemplateArg2IsChanged(void)
{
	Arg2IsLost();

	TemplateArg2IsCompleted(); 
}


void CollisionBetweenObjectsEvent::TemplateArg3IsCompleted(void)
{
	if(templateArg3.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg3.GetResult(), state[ASSET_TYPE][ARG3]) : AssetLibrary::_GetAssets().FindObject(templateArg3.GetResult());

		if(arg3.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
				{
					if(arg3.Attach(dynamic_cast<Variable<StringANSI>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



void CollisionBetweenObjectsEvent::Arg3IsLost(void)
{
	if(arg3.IsNotEmpty())
	{
		arg3.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg3.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg3.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsCreated));
}




void CollisionBetweenObjectsEvent::TemplateArg3IsChanged(void)
{
	Arg3IsLost();

	TemplateArg3IsCompleted(); 
}



void CollisionBetweenObjectsEvent::TemplateArg4IsCompleted(void)
{
	if(templateArg4.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg4.GetResult(), state[ASSET_TYPE][ARG4]) : AssetLibrary::_GetAssets().FindObject(templateArg4.GetResult());

		if(arg4.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(List<int32>::INTEGER_LIST_CLASS)) 
				{
					if(arg4.Attach(dynamic_cast<List<int32>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg4IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void CollisionBetweenObjectsEvent::Arg4IsLost(void)
{
	if(arg4.IsNotEmpty())
	{
		arg4.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg4.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg4.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsCreated));
}




void CollisionBetweenObjectsEvent::TemplateArg4IsChanged(void)
{
	Arg4IsLost();

	TemplateArg4IsCompleted(); 
}



void CollisionBetweenObjectsEvent::TemplateArg5IsCompleted(void)
{
	if(templateArg5.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg5.GetResult(), state[ASSET_TYPE][ARG5]) : AssetLibrary::_GetAssets().FindObject(templateArg5.GetResult());

		if(arg5.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(List<int32>::INTEGER_LIST_CLASS)) 
				{
					if(arg5.Attach(dynamic_cast<List<int32>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg5IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void CollisionBetweenObjectsEvent::Arg5IsLost(void)
{
	if(arg5.IsNotEmpty())
	{
		arg5.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg5.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg5.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsCreated));
}




void CollisionBetweenObjectsEvent::TemplateArg5IsChanged(void)
{
	Arg5IsLost();

	TemplateArg5IsCompleted(); 
}


 
void CollisionBetweenObjectsEvent::TemplateSourceIsCompleted(void)
{
	if(templateSource.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateSource.GetResult(), state[ASSET_TYPE][SOURCE]) : AssetLibrary::_GetAssets().FindObject(templateSource.GetResult());

		if(source.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
				{
					if(source.Attach(dynamic_cast<PolygonalSurface*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::SourceIsRenamed));
						object->Connect(PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void CollisionBetweenObjectsEvent::SourceIsLost(void)
{
	if(source.IsNotEmpty())
	{
		source.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		source.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		source.Get()->Disconnect(PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE, this);
		source.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsCreated));
}



void CollisionBetweenObjectsEvent::TemplateSourceIsChanged(void)
{
	SourceIsLost();

	TemplateSourceIsCompleted(); 
}





StringANSI CollisionBetweenObjectsEvent::GetTemplateArg(int32 _index)
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

		case ARG4:
		{
			return templateArg4.GetStringExpr();
		}

		case ARG5:
		{
			return templateArg5.GetStringExpr();
		}

		case SOURCE:
		{
			return templateSource.GetStringExpr();
		}
	}
	return "";
}



StringANSI CollisionBetweenObjectsEvent::GetVariableArg(int32 _index)
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

		case ARG4:
		{
			return arg4Name;
		}

		case ARG5:
		{
			return arg5Name;
		}

		case SOURCE:
		{
			return sourceName;
		}
	}
	return "";
}



StringANSI CollisionBetweenObjectsEvent::GetStringKeyArg(int32 _index)
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
	return "";
}



void CollisionBetweenObjectsEvent::SetStringKeyArg(int32 _index, StringANSI _value)
{
	if(_value.empty()) { return; }

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
			Variable<StringANSI>* object = new Variable<StringANSI>();
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
			Variable<StringANSI>* object = new Variable<StringANSI>();
			object->SetValue(_value);
			
			if(arg3.Attach(object))
			{
				UpdateValid();
			}
			break;
		}
	}
}


void CollisionBetweenObjectsEvent::SetValue(bool _value)
{
	AbstractEvent::SetValue(_value);

	if(_value)
	{
		if(deactivationMode == AbstractEvent::AUTO_SWITCH_OFF) 
		{ 
			SetValue(false); 
		}
	}
}


void CollisionBetweenObjectsEvent::EventHandler(PolygonalSurface::CollisionInfo* _collisionInfo)
{
	if(argsEnable)
	{ 

		arg1Exist = false; arg2Exist = false; arg3Exist = false;
		arg1IsTrue = false; arg2IsTrue = false; arg3IsTrue = false;

		if(arg1.IsNotEmpty())
		{
			arg1Exist = true;
			
			if(arg1.Get()->GetName() == _collisionInfo->targetObject->GetName())
			{ 
				arg1IsTrue = true;
			}	
		}
		
		if(arg2.IsNotEmpty())
		{
			arg2Exist = true;

			if(arg2.Get()->GetValue() == _collisionInfo->sourceMeshName)
			{ 
				arg2IsTrue = true;
			}	
		}
			
		if(arg3.IsNotEmpty())
		{
			arg3Exist = true;
			
			if(arg3.Get()->GetValue() == _collisionInfo->targetMeshName)
			{ 
				arg3IsTrue = true;
			}	
		}
		
		if(arg1Exist == arg1IsTrue && arg2Exist == arg2IsTrue && arg3Exist == arg3IsTrue)
		{
			if(arg2Exist && arg4.IsNotEmpty() && _collisionInfo->sourceMeshIntersectedPolygons.IsNotEmpty()) 
			{
				for(SimpleList<int32>::Node* it = _collisionInfo->sourceMeshIntersectedPolygons.GetHead(); it != NIL; it = it->GetRight())
				{
					arg4.Get()->AddToTail(new Variable<int32>(it->key));
				}
			}
			if(arg3Exist && arg5.IsNotEmpty() && _collisionInfo->targetMeshIntersectedPolygons.IsNotEmpty()) 
			{
				for(SimpleList<int32>::Node* it = _collisionInfo->targetMeshIntersectedPolygons.GetHead(); it != NIL; it = it->GetRight())
				{
					arg5.Get()->AddToTail(new Variable<int32>(it->key));
				}
			}

			SetValue(true);
		}
		else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
		{
			SetValue(false);
		}
	}
	else
	{
		SetValue(true);
	}
}


void CollisionBetweenObjectsEvent::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsCreated));
}


void CollisionBetweenObjectsEvent::SourceIsRenamed(AbstractObject::NameInfo* _info)
{
	SourceIsLost();
}
 


void CollisionBetweenObjectsEvent::Arg1IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg1IsLost();
}



void CollisionBetweenObjectsEvent::Arg2IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg2IsLost();
}

 
void CollisionBetweenObjectsEvent::Arg3IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg3IsLost();
}



void CollisionBetweenObjectsEvent::Arg4IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg4IsLost();
}
 


void CollisionBetweenObjectsEvent::Arg5IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg5IsLost();
}


void CollisionBetweenObjectsEvent::ObjectIsCreated(AbstractObject* _object)
{
	if(source.IsEmpty())
	{
		if(state[TEMPLATE][SOURCE])
		{
			if(_object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
			{
				if(templateSource.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateSource.GetResult() == _object->GetName())
				{
					if(source.Attach(dynamic_cast<PolygonalSurface*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::SourceIsRenamed));
						_object->Connect(PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][SOURCE])
		{
			if(_object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
			{
				if(sourceName == _object->GetName())
				{
					if(source.Attach(dynamic_cast<PolygonalSurface*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::SourceIsRenamed));
						_object->Connect(PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::EventHandler));
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
			if(_object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
			{
				if(templateArg1.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg1.GetResult() == _object->GetName())
				{
					if(arg1.Attach(dynamic_cast<PolygonalSurface*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG1])
		{
			if(_object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS)) 
			{
				if(arg1Name == _object->GetName())
				{
					if(arg1.Attach(dynamic_cast<PolygonalSurface*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg1IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg2.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG2])
		{
			if(_object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
			{
				if(templateArg2.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg2.GetResult() == _object->GetName())
				{
					if(arg2.Attach(dynamic_cast<Variable<StringANSI>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg2IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG2])
		{
			if(_object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
			{
				if(arg2Name == _object->GetName())
				{
					if(arg2.Attach(dynamic_cast<Variable<StringANSI>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg2IsRenamed));
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
			if(_object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
			{
				if(templateArg3.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg3.GetResult() == _object->GetName())
				{
					if(arg3.Attach(dynamic_cast<Variable<StringANSI>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG3])
		{
			if(_object->IsClassPropertyExist(Variable<StringANSI>::STRING_VARIABLE_CLASS)) 
			{
				if(arg3Name == _object->GetName())
				{
					if(arg3.Attach(dynamic_cast<Variable<StringANSI>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg3IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg4.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG4])
		{
			if(_object->IsClassPropertyExist(List<int32>::INTEGER_LIST_CLASS)) 
			{
				if(templateArg4.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg4.GetResult() == _object->GetName())
				{
					if(arg4.Attach(dynamic_cast<List<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg4IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG4])
		{
			if(_object->IsClassPropertyExist(List<int32>::INTEGER_LIST_CLASS)) 
			{
				if(arg4Name == _object->GetName())
				{
					if(arg4.Attach(dynamic_cast<List<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg4IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg5.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG5])
		{
			if(_object->IsClassPropertyExist(List<int32>::INTEGER_LIST_CLASS)) 
			{
				if(templateArg5.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg5.GetResult() == _object->GetName())
				{
					if(arg5.Attach(dynamic_cast<List<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg5IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG5])
		{
			if(_object->IsClassPropertyExist(List<int32>::INTEGER_LIST_CLASS)) 
			{
				if(arg5Name == _object->GetName())
				{
					if(arg5.Attach(dynamic_cast<List<int32>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CollisionBetweenObjectsEvent::Arg5IsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool CollisionBetweenObjectsEvent::SaveToFile(StringANSI _path)			
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + eventsDir + GetName() + "." + eventsSuffix;
	} 
 
	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool CollisionBetweenObjectsEvent::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(type);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		for(int32 i = 0; i < AbstractEvent::WIDTH; i++)
		{
			for(int32 j = 0; j < AbstractEvent::HEIGHT; j++)
			{
				_file.Write(state[i][j]);
			}
		}
		_file.Write(activationLimit);
		_file.Write(loadArgsEnable);
		_file.Write(deactivationMode);
		_file.Write(argsEnable);
		_file.Write(value);
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
			_file.Write(GetStringKeyArg(ARG2).length());
			_file.WriteString(GetStringKeyArg(ARG2));
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
			_file.Write(GetStringKeyArg(ARG3).length());
			_file.WriteString(GetStringKeyArg(ARG3));
		}
		if(state[TEMPLATE][ARG4])
		{
			_file.Write(GetTemplateArg(ARG4).length());
			_file.WriteString(GetTemplateArg(ARG4));
		}
		else if(state[VARIABLE][ARG4])
		{
			_file.Write(GetVariableArg(ARG4).length());
			_file.WriteString(GetVariableArg(ARG4));
		}
		if(state[TEMPLATE][ARG5])
		{
			_file.Write(GetTemplateArg(ARG5).length());
			_file.WriteString(GetTemplateArg(ARG5));
		}
		else if(state[VARIABLE][ARG5])
		{
			_file.Write(GetVariableArg(ARG5).length());
			_file.WriteString(GetVariableArg(ARG5));
		}
		if(state[TEMPLATE][SOURCE])
		{
			_file.Write(GetTemplateArg(SOURCE).length());
			_file.WriteString(GetTemplateArg(SOURCE));
		}
		else if(state[VARIABLE][SOURCE])
		{
			_file.Write(GetVariableArg(SOURCE).length());
			_file.WriteString(GetVariableArg(SOURCE));
		} 
		return true;
	}
	return false;
}


AbstractEvent* CollisionBetweenObjectsEvent::_LoadFromFile(File& _file)
{
	CollisionBetweenObjectsEvent* event = new CollisionBetweenObjectsEvent(PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE);
	StringANSI name;
	int32 length;
	_file.Read(length);
	_file.ReadString(name, length);
	event->Rename(name);
	for(int32 i = 0; i < AbstractEvent::WIDTH; i++)
	{
		for(int32 j = 0; j < AbstractEvent::HEIGHT; j++)
		{
			_file.Read(event->state[i][j]);
		}
	}
	int32 activationLimit;
	_file.Read(activationLimit); 
	event->SetActivationLimit(activationLimit);
	bool loadArgsEnable;
	_file.Read(loadArgsEnable); 
	event->SetLoadArgsEnable(loadArgsEnable);
	int32 deactivationMode;
	_file.Read(deactivationMode);
	event->SetDeactivationMode(deactivationMode);
	bool argsEnable;
	_file.Read(argsEnable);
	event->SetArgsEnable(argsEnable);
	bool value;
	_file.Read(value);
	if(event->state[TEMPLATE][ARG1])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(ARG1, templateArg); 
	}
	else if(event->state[VARIABLE][ARG1])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(ARG1, variableArg);
	}
	if(event->state[TEMPLATE][ARG2])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(ARG2, templateArg); 
	}
	else if(event->state[VARIABLE][ARG2])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(ARG2, variableArg);
	}
	else if(event->state[KEY][ARG2])
	{
		StringANSI arg; 
		_file.Read(length);
		_file.ReadString(arg, length);
		event->SetStringKeyArg(ARG2, arg);
	}
	if(event->state[TEMPLATE][ARG3])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(ARG3, templateArg); 
	}
	else if(event->state[VARIABLE][ARG3])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(ARG3, variableArg);
	}
	else if(event->state[KEY][ARG3])
	{
		StringANSI arg; 
		_file.Read(length);
		_file.ReadString(arg, length);
		event->SetStringKeyArg(ARG3, arg);
	}
	if(event->state[TEMPLATE][ARG4])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(ARG4, templateArg); 
	}
	else if(event->state[VARIABLE][ARG4])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(ARG4, variableArg);
	}
	if(event->state[TEMPLATE][ARG5])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(ARG5, templateArg); 
	}
	else if(event->state[VARIABLE][ARG5])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(ARG5, variableArg);
	}
	if(event->state[TEMPLATE][SOURCE])
	{
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		event->SetTemplateArg(SOURCE, templateArg); 
	}
	else if(event->state[VARIABLE][SOURCE])
	{
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		event->SetVariableArg(SOURCE, variableArg);
	}

	if(value) { event->value = value; }
	return event;
}