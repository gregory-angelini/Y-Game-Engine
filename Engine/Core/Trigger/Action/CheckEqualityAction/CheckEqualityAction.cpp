#include "CheckEqualityAction.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Parser/List/List.h>



CheckEqualityAction::CheckEqualityAction(void)
{}


CheckEqualityAction::CheckEqualityAction(int32 _type): AbstractAction(_type)
{}



void CheckEqualityAction::UpdateValid(void)
{
	valid = false; 

	if(arg1.IsNotEmpty() || state[EXPRESSION][ARG1]) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this); // отписаться от появления объектов
		valid = true; 
	}
}



CheckEqualityAction::~CheckEqualityAction(void)
{
	Destroying();// объект начинает уничтожаться
}


void CheckEqualityAction::SetVariableArg(int32 _index, StringANSI _name)
{
	switch(_index)
	{
		case ARG1:
		{
			SetArgMode(_index, VARIABLE);

			Arg1IsLost(); 
			
			arg1Name = _name; // запомнить имя аргумента

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg1Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg1Name);// найти объект

			if(arg1.IsEmpty())// если аргумент не был загружен
			{
				if(object) // если объект с таким именем существует
				{
					if(object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS)) 
					{ 
						if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(arg1.Attach(dynamic_cast<Variable<bool>*>(object)))// захватить объект
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg1IsRenamed));// подписаться на переименование объекта
							UpdateValid();
						}
						return;// прервать функцию
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
			state[ASSET_TYPE][_index] = AssetLibrary::ACTION_ASSET;

			Arg2IsLost(); 
			
			arg2Name = _name; // запомнить имя аргумента

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg2Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg2Name);// найти объект

			if(arg2.IsEmpty())// если аргумент не был загружен
			{
				if(object) // если объект с таким именем существует
				{
					if(object->IsClassPropertyExist(AbstractAction::ACTION_CLASS)) 
					{ 
						//if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(arg2.Attach(dynamic_cast<AbstractAction*>(object)))// захватить объект
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg2IsRenamed));// подписаться на переименование объекта	
							UpdateValid();
						}
						return;// прервать функцию
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
			state[ASSET_TYPE][_index] = AssetLibrary::ACTION_ASSET;  

			Arg3IsLost(); 
			
			arg3Name = _name; // запомнить имя аргумента

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(arg3Name, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(arg3Name);// найти объект

			if(arg3.IsEmpty())// если аргумент не был загружен
			{
				if(object) // если объект с таким именем существует
				{
					if(object->IsClassPropertyExist(AbstractAction::ACTION_CLASS)) 
					{ 
						//if(loadArgsEnable) { state[ASSET_TYPE][_index] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
						if(arg3.Attach(dynamic_cast<AbstractAction*>(object)))// захватить объект
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg3IsRenamed));// подписаться на переименование объекта
							UpdateValid();
						}
						return;// прервать функцию
					}
				}
			}
			break;
		}  
	}
}


void CheckEqualityAction::SetExpressionArg(int32 _index, StringANSI _expr)
{  
	switch(_index)
	{
		case ARG1:
		{
			SetArgMode(_index, EXPRESSION);

			Arg1IsLost();

			expr.Parse(_expr);
			
			UpdateValid();
			break;
		}  
	}
}


void CheckEqualityAction::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG2:
		{ 
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CheckEqualityAction::TemplateArg2IsCompleted));// подписаться на валидацию выражения
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg2IsLost));// подписаться на потерию переменной выражения
			templateArg2.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CheckEqualityAction::TemplateArg2IsChanged));// подписаться на изменение переменной выражения
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::ACTION_ASSET;  

			Arg2IsLost(); 

			if(templateArg2.Parse(_expr) == StringExprParserEx::NO_ERRORS)// если выражение допустимо
			{ 
				TemplateArg2IsCompleted();
			}
			break;
		}  

		case ARG3:
		{ 
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &CheckEqualityAction::TemplateArg3IsCompleted));// подписаться на валидацию выражения
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg3IsLost));// подписаться на потерию переменной выражения
			templateArg3.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &CheckEqualityAction::TemplateArg3IsChanged));// подписаться на изменение переменной выражения
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::ACTION_ASSET;  

			Arg3IsLost(); 

			if(templateArg3.Parse(_expr) == StringExprParserEx::NO_ERRORS)// если выражение допустимо
			{ 
				TemplateArg3IsCompleted();
			}
			break;
		}  
	}
}



void CheckEqualityAction::Arg1IsLost(void)
{
	// отключиться от текущего аргумента...
	if(arg1.IsNotEmpty())// если аргумент определен
	{
		arg1.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);// отписаться от удаления объекта
		arg1.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);// отписаться от переименования объекта
		arg1.Clear();// отпустить объект
	}
	expr.Destroy();// очистить выражение
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsCreated));// подписаться на появление объекта
	valid = false;
}



void CheckEqualityAction::TemplateArg2IsCompleted(void)
{
	// вычислить имя аргумента и подключиться к нему если он существует...
	if(templateArg2.Calculate() == StringExprParserEx::NO_ERRORS)// если выражение вычислено
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg2.GetResult(), state[ASSET_TYPE][ARG2]) : AssetLibrary::_GetAssets().FindObject(templateArg2.GetResult());// найти объект

		if(arg2.IsEmpty())// если аргумент не был загружен
		{
			if(object) // если объект с таким именем существует
			{
				if(object->IsClassPropertyExist(AbstractAction::ACTION_CLASS)) 
				{ 
					//if(loadArgsEnable) { state[ASSET_TYPE][ARG2] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(arg2.Attach(dynamic_cast<AbstractAction*>(object)))// захватить объект
					{ 
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg2IsRenamed));// подписаться на переименование объекта	
						UpdateValid();
					}
					return;// прервать функцию
				}
			}
		}
	}
}


void CheckEqualityAction::Arg2IsLost(void)
{
	// отключиться от текущего аргумента...
	if(arg2.IsNotEmpty())// если аргумент определен
	{ 
		arg2.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);// отписаться от удаления объекта
		arg2.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);// отписаться от переименования объекта
		arg2.Clear();// отпустить объект
	}

	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsCreated));// подписаться на появление объекта
	valid = false;
}



void CheckEqualityAction::TemplateArg2IsChanged(void)
{
	Arg2IsLost();

	TemplateArg2IsCompleted(); 
}



void CheckEqualityAction::TemplateArg3IsCompleted(void)
{
	// вычислить имя аргумента и подключиться к нему если он существует...
	if(templateArg3.Calculate() == StringExprParserEx::NO_ERRORS)// если выражение вычислено
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg3.GetResult(), state[ASSET_TYPE][ARG3]) : AssetLibrary::_GetAssets().FindObject(templateArg3.GetResult());// найти объект

		if(arg3.IsEmpty())// если аргумент не был загружен
		{
			if(object) // если объект с таким именем существует
			{
				if(object->IsClassPropertyExist(AbstractAction::ACTION_CLASS)) 
				{ 
					//if(loadArgsEnable) { state[ASSET_TYPE][ARG3] = AssetLibrary::_GetAssetType(object->GetClassProperties()); }
					if(arg3.Attach(dynamic_cast<AbstractAction*>(object)))// захватить объект
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg3IsRenamed));// подписаться на переименование объекта
						UpdateValid();
					}
					return;// прервать функцию
				}
			}
		}
	}
}


void CheckEqualityAction::Arg3IsLost(void)
{
	// отключиться от текущего аргумента...
	if(arg3.IsNotEmpty())// если аргумент определен
	{ 
		arg3.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);// отписаться от удаления объекта
		arg3.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);// отписаться от переименования объекта
		arg3.Clear();// отпустить объект
	}

	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsCreated));// подписаться на появление объекта
	valid = false;
}



void CheckEqualityAction::TemplateArg3IsChanged(void)
{
	Arg3IsLost();

	TemplateArg3IsCompleted(); 
}



StringANSI CheckEqualityAction::GetExpressionArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return expr.GetEqualityExpr();
		}
	}
	return "";
}



StringANSI CheckEqualityAction::GetTemplateArg(int32 _index)
{
	switch(_index)
	{
		case ARG2:
		{
			return templateArg2.GetStringExpr();
		}

		case ARG3:
		{
			return templateArg3.GetStringExpr();
		}
	}
	return "";
}



StringANSI CheckEqualityAction::GetVariableArg(int32 _index)
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
	}
	return "";
}



void CheckEqualityAction::operator () (void)
{
	if(IsValid()) 
	{
		if(state[VARIABLE][ARG1])
		{
			if(arg1.Get()->GetValue())
			{
				if(arg2.IsNotEmpty()) { (*arg2.Get())(); }
			}
			else 
			{
				if(arg3.IsNotEmpty()) { (*arg3.Get())(); }
			}
		}
		else if(state[EXPRESSION][ARG1])
		{
			if(expr.Calculate() == EqualityExprParserEx::NO_ERRORS)// если выражение успешно вычислено
			{  
				if(expr.GetResult())// если выражение истинно
				{
					if(arg2.IsNotEmpty()) { (*arg2.Get())(); } 
				}
				else 
				{
					if(arg3.IsNotEmpty()) { (*arg3.Get())(); }
				}
			}
		}
	} 
}



void CheckEqualityAction::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsCreated));// подписаться на появление объекта
	valid = false;
}


 

void CheckEqualityAction::Arg1IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg1IsLost();
}



void CheckEqualityAction::Arg2IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg2IsLost();
}



void CheckEqualityAction::Arg3IsRenamed(AbstractObject::NameInfo* _info)
{
	Arg3IsLost();
}


void CheckEqualityAction::ObjectIsCreated(AbstractObject* _object)
{
	// ARG1...
	if(arg1.IsEmpty())// если аргумент не определен 
	{ 
		if(state[VARIABLE][ARG1])
		{
			if(_object->IsClassPropertyExist(Variable<bool>::BOOLEAN_VARIABLE_CLASS))
			{
				if(arg1Name == _object->GetName())// если имена совпадают
				{ 
					if(loadArgsEnable) { state[ASSET_TYPE][ARG1] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg1.Attach(dynamic_cast<Variable<bool>*>(_object)))// захватить объект
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg1IsRenamed));// подписаться на переименование объекта		
						UpdateValid();
					}
					return;
				}
			}
		}
	}

	// ARG2...
	if(arg2.IsEmpty())// если аргумент не определен 
	{ 
		if(state[TEMPLATE][ARG2])
		{
			if(_object->IsClassPropertyExist(AbstractAction::ACTION_CLASS))
			{
				if(templateArg2.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg2.GetResult() == _object->GetName()) //  если имена совпадают
				{
					//if(loadArgsEnable) { state[ASSET_TYPE][ARG2] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg2.Attach(dynamic_cast<AbstractAction*>(_object)))// захватить объект
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg2IsRenamed));// подписаться на переименование объекта
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG2])
		{
			if(_object->IsClassPropertyExist(AbstractAction::ACTION_CLASS))
			{
				if(arg2Name == _object->GetName())// если имена совпадают
				{ 
					//if(loadArgsEnable) { state[ASSET_TYPE][ARG2] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg2.Attach(dynamic_cast<AbstractAction*>(_object)))// захватить объект
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg2IsRenamed));// подписаться на переименование объекта		
						UpdateValid();
					}
					return;
				}
			}
		}
	}

	// ARG3...
	if(arg3.IsEmpty())// если аргумент не определен 
	{  
		if(state[TEMPLATE][ARG3])
		{
			if(_object->IsClassPropertyExist(AbstractAction::ACTION_CLASS))
			{
				if(templateArg3.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg3.GetResult() == _object->GetName()) // если имена совпадают
				{
					//if(loadArgsEnable) { state[ASSET_TYPE][ARG3] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg3.Attach(dynamic_cast<AbstractAction*>(_object)))// захватить объект
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg3IsRenamed));// подписаться на переименование объекта
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG3])
		{
			if(_object->IsClassPropertyExist(AbstractAction::ACTION_CLASS))
			{
				if(arg3Name == _object->GetName())// если имена совпадают
				{ 
					//if(loadArgsEnable) { state[ASSET_TYPE][ARG3] = AssetLibrary::_GetAssetType(_object->GetClassProperties()); }
					if(arg3.Attach(dynamic_cast<AbstractAction*>(_object)))// захватить объект
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &CheckEqualityAction::ObjectIsDestroyed));// подписаться на уничтожение объекта
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &CheckEqualityAction::Arg3IsRenamed));// подписаться на переименование объекта
						UpdateValid();
					}
					return;
				}
			}
		}
	} 
}



bool CheckEqualityAction::SaveToFile(StringANSI _path)			
{
	if(_path.empty())// если пустая строка 
	{
		_path = AssetLibrary::_GetDir() + actionsDir + GetName() + "." + actionsSuffix;// вычислить путь к файлу 
	} 

	File file(_path, File::REWRITE);// открыть файл на перезапись; если файл существует, то он будет перезаписан; если файл отсутствует, то он будет создан

	if(!SaveToFile(file))// если не удалось сохранить объект 
	{
		file.Remove();// удалить файл
		return false;
	}
	// иначе объект успешно сохранен...
	return true; 
}


bool CheckEqualityAction::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))// если файл существует и открыт на запись
	{
		// тип действия...
		_file.Write(type);
	
		// имя действия...
		_file.Write(GetName().length());
		_file.WriteString(GetName());

		// состояние...
		for(int32 i = 0; i < AbstractAction::WIDTH; i++)
		{
			for(int32 j = 0; j < AbstractAction::HEIGHT; j++)
			{
				_file.Write(state[i][j]);
			}
		}

		// повторения...
		_file.Write(activationLimit); 

		// загрузка аргументов...
		_file.Write(loadArgsEnable); 

		// ARG1...
		if(state[EXPRESSION][ARG1])
		{
			// аргумент...
			_file.Write(GetExpressionArg(ARG1).length());
			_file.WriteString(GetExpressionArg(ARG1));
		}
		else if(state[VARIABLE][ARG1])
		{
			// аргумент...
			_file.Write(GetVariableArg(ARG1).length());
			_file.WriteString(GetVariableArg(ARG1));
		}

		// ARG2...
		if(state[TEMPLATE][ARG2])
		{
			// аргумент...
			_file.Write(GetTemplateArg(ARG2).length());
			_file.WriteString(GetTemplateArg(ARG2));
		}
		else if(state[VARIABLE][ARG2])
		{
			// аргумент...
			_file.Write(GetVariableArg(ARG2).length());
			_file.WriteString(GetVariableArg(ARG2));
		} 

		// ARG3...
		if(state[TEMPLATE][ARG3])
		{
			// аргумент...
			_file.Write(GetTemplateArg(ARG3).length());
			_file.WriteString(GetTemplateArg(ARG3));
		}
		else if(state[VARIABLE][ARG3])
		{
			// аргумент...
			_file.Write(GetVariableArg(ARG3).length());
			_file.WriteString(GetVariableArg(ARG3));
		}
		return true;
	}
	return false;
}


AbstractAction* CheckEqualityAction::_LoadFromFile(File& _file)
{
	// создать действие...
	CheckEqualityAction* action = new CheckEqualityAction(AbstractVariable::CHECK_EQUALITY_MESSAGE);

	// имя действия...
	StringANSI name;
	int32 length;
	_file.Read(length);
	_file.ReadString(name, length);  
	action->Rename(name);// задать имя

	// состояние...
	for(int32 i = 0; i < AbstractAction::WIDTH; i++)
	{
		for(int32 j = 0; j < AbstractAction::HEIGHT; j++)
		{
			_file.Read(action->state[i][j]);
		}
	}

	// повторения...
	int32 activationLimit;
	_file.Read(activationLimit); 
	action->SetActivationLimit(activationLimit);// количество повторений

	// загрузка аргументов...
	bool loadArgsEnable;
	_file.Read(loadArgsEnable); 
	action->SetLoadArgsEnable(loadArgsEnable);

	// ARG1...
	if(action->state[EXPRESSION][ARG1])
	{
		// аргумент...
		StringANSI expression;
		_file.Read(length);
		_file.ReadString(expression, length);
		action->SetExpressionArg(ARG1, expression); 
	}
	else if(action->state[VARIABLE][ARG1])
	{
		// аргумент...
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG1, variableArg);
	}

	// ARG2...
	if(action->state[TEMPLATE][ARG2])
	{
		// аргумент...
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(ARG2, templateArg); 
	}
	else if(action->state[VARIABLE][ARG2])
	{
		// аргумент...
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG2, variableArg);
	}

	// ARG3...
	if(action->state[TEMPLATE][ARG3])
	{
		// аргумент...
		StringANSI templateArg;
		_file.Read(length);
		_file.ReadString(templateArg, length);
		action->SetTemplateArg(ARG3, templateArg); 
	}
	else if(action->state[VARIABLE][ARG3])
	{
		// аргумент...
		StringANSI variableArg;
		_file.Read(length);
		_file.ReadString(variableArg, length);
		action->SetVariableArg(ARG3, variableArg);
	}
	action->UpdateValid();
	return action;
}