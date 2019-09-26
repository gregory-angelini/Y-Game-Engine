#include "ChangeQuaternionValueEvent.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>




ChangeQuaternionValueEvent::ChangeQuaternionValueEvent(void)
{}



ChangeQuaternionValueEvent::ChangeQuaternionValueEvent(int32 _type): AbstractEvent(_type)
{}



ChangeQuaternionValueEvent::~ChangeQuaternionValueEvent(void)
{
	Destroying();
}


void ChangeQuaternionValueEvent::UpdateValid(void)
{
	if((arg.IsNotEmpty() || state[KEY][ARG1] || state[EXPRESSION][ARG1] || state[TEMPLATE][ARG1] || state[EQUALITY][ARG1] || !argsEnable) && source.IsNotEmpty()) 
	{ 
		AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
	}
}




void ChangeQuaternionValueEvent::SetVariableArg(int32 _index, StringANSI _name)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg.Destroy();

			SetArgMode(_index, VARIABLE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			ArgIsLost(); 
			
			argName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(argName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(argName);

			if(arg.IsEmpty())
			{ 
				if(object)
				{
					if(object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
					{
						if(arg.Attach(dynamic_cast<Variable<Quaternion>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ArgIsRenamed));
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
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			SourceIsLost();
  
			sourceName = _name;

			AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(sourceName, state[ASSET_TYPE][_index]) : AssetLibrary::_GetAssets().FindObject(sourceName);

			if(source.IsEmpty())
			{
				if(object)
				{ 
					if(object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
					{
						if(source.Attach(dynamic_cast<Variable<Quaternion>*>(object)))
						{
							object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsDestroyed));
							object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::SourceIsRenamed));
							object->Connect(Variable<Quaternion>::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::EventHandler));
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


void ChangeQuaternionValueEvent::SetExpressionArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{    
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg.Destroy(); 

			SetArgMode(_index, EXPRESSION);

			ArgIsLost();  

			expr.Parse(_expr);
			
			UpdateValid();
			break;
		}   
	}
}


void ChangeQuaternionValueEvent::SetEqualityArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{    
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg.Destroy(); 

			SetArgMode(_index, EQUALITY);

			ArgIsLost();  

			equalityArg.Parse(_expr);
			
			UpdateValid();
			break;
		}   
	}
}


void ChangeQuaternionValueEvent::SetTemplateArg(int32 _index, StringANSI _expr)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::TemplateArgIsCompleted));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ArgIsLost));
			templateArg.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::TemplateArgIsChanged));
			
			SetArgMode(_index, TEMPLATE);
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET; 

			ArgIsLost(); 

			if(templateArg.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateArgIsCompleted();
			}
			break;
		}

		case SOURCE:
		{
			templateSource.Connect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::TemplateSourceIsCompleted));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::SourceIsLost));
			templateSource.Connect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::TemplateSourceIsChanged));
			
			SetArgMode(_index, TEMPLATE); 
			state[ASSET_TYPE][_index] = AssetLibrary::VARIABLE_ASSET;

			SourceIsLost(); 

			if(templateSource.Parse(_expr) == StringExprParserEx::NO_ERRORS)
			{
				TemplateSourceIsCompleted();
			}
			break;
		}
	}
}



void ChangeQuaternionValueEvent::SetQuaternionKeyArg(int32 _index, Quaternion _value)
{
	switch(_index)
	{
		case ARG1:
		{
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_COMPLETED_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_VARIABLE_LOSS_MESSAGE, this);
			templateArg.Disconnect(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE, this);
			templateArg.Destroy(); 

			SetArgMode(_index, KEY);

			ArgIsLost();

			argValue = _value;
			
			UpdateValid();
			break;
		}
	}
}



void ChangeQuaternionValueEvent::TemplateArgIsCompleted(void)
{
	if(templateArg.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateArg.GetResult(), state[ASSET_TYPE][ARG1]) : AssetLibrary::_GetAssets().FindObject(templateArg.GetResult());

		if(arg.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
				{
					if(arg.Attach(dynamic_cast<Variable<Quaternion>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void ChangeQuaternionValueEvent::TemplateArgIsChanged(void)
{
	ArgIsLost();

	TemplateArgIsCompleted(); 
}



void ChangeQuaternionValueEvent::ArgIsLost(void)
{
	if(arg.IsNotEmpty())
	{
		arg.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		arg.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		arg.Clear();
	}
	expr.Destroy();
	equalityArg.Destroy();
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsCreated));
}





void ChangeQuaternionValueEvent::TemplateSourceIsCompleted(void)
{
	if(templateSource.Calculate() == StringExprParserEx::NO_ERRORS)
	{
		AbstractObject* object = loadArgsEnable ? AssetLibrary::_LoadCommonAsset(templateSource.GetResult(), state[ASSET_TYPE][SOURCE]) : AssetLibrary::_GetAssets().FindObject(templateSource.GetResult());

		if(source.IsEmpty())
		{
			if(object)
			{
				if(object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
				{
					if(source.Attach(dynamic_cast<Variable<Quaternion>*>(object)))
					{
						object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsDestroyed));
						object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::SourceIsRenamed));
						object->Connect(Variable<Quaternion>::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}


void ChangeQuaternionValueEvent::TemplateSourceIsChanged(void)
{
	SourceIsLost();

	TemplateSourceIsCompleted(); 
}




void ChangeQuaternionValueEvent::SourceIsLost(void)
{
	if(source.IsNotEmpty())
	{
		source.Get()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		source.Get()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		source.Get()->Disconnect(Variable<Quaternion>::CHANGE_VALUE_MESSAGE, this);
		source.Clear();
	}
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsCreated));
}



StringANSI ChangeQuaternionValueEvent::GetExpressionArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return expr.GetArithmeticExpr();
		}
	}
	return "";
}



StringANSI ChangeQuaternionValueEvent::GetEqualityArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return equalityArg.GetEqualityExpr();
		}
	}
	return "";
}



StringANSI ChangeQuaternionValueEvent::GetTemplateArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return templateArg.GetStringExpr();
		}

		case SOURCE:
		{
			return templateSource.GetStringExpr();
		}
	}
	return "";
}


StringANSI ChangeQuaternionValueEvent::GetVariableArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return argName;
		}

		case SOURCE:
		{
			return sourceName;
		}
	}
	return "";
}


Quaternion ChangeQuaternionValueEvent::GetQuaternionKeyArg(int32 _index)
{
	switch(_index)
	{
		case ARG1:
		{
			return argValue; 
		}
	}
	return Quaternion();
}



void ChangeQuaternionValueEvent::SetValue(bool _value)
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


void ChangeQuaternionValueEvent::EventHandler(AbstractVariable* _object)
{
	if(argsEnable)
	{
		if(state[KEY][ARG1])
		{
			if(source.Get()->GetValue() == argValue)
			{ 
				SetValue(true);
			}
			else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
			{
				SetValue(false);
			}
		}
		else if(state[EXPRESSION][ARG1])
		{
			if(expr.Calculate() == ArithmeticExprParserEx<Quaternion>::NO_ERRORS)
			{  
				if(source.Get()->GetValue() == expr.GetResult())
				{
					SetValue(true);
				}
				else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
				{
					SetValue(false);
				}
			}
		}
		else if(state[EQUALITY][ARG1])
		{
			if(equalityArg.Calculate() == EqualityExprParserEx::NO_ERRORS)
			{  
				if(equalityArg.GetResult())
				{
					SetValue(true);
				}
				else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
				{
					SetValue(false);
				}
			}
		}
		else
		{
			if(arg.IsNotEmpty())
			{ 
				if(source.Get()->GetValue() == arg.Get()->GetValue())
				{
					SetValue(true);
				}
				else if(deactivationMode == AbstractEvent::OVERLAY_SWITCH_OFF)
				{
					SetValue(false);
				}
			}
		}
	}
	else
	{
		SetValue(true);
	}
}


void ChangeQuaternionValueEvent::ObjectIsDestroyed(AbstractObject* _object)
{
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsCreated));
}


void ChangeQuaternionValueEvent::SourceIsRenamed(AbstractObject::NameInfo* _info)
{
	SourceIsLost();
}
 


void ChangeQuaternionValueEvent::ArgIsRenamed(AbstractObject::NameInfo* _info)
{
	ArgIsLost();
}


void ChangeQuaternionValueEvent::ObjectIsCreated(AbstractObject* _object)
{
	if(source.IsEmpty())
	{
		if(state[TEMPLATE][SOURCE])
		{
			if(_object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
			{
				if(templateSource.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateSource.GetResult() == _object->GetName())
				{
					if(source.Attach(dynamic_cast<Variable<Quaternion>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::SourceIsRenamed));
						_object->Connect(Variable<Quaternion>::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
		else if(state[VARIABLE][SOURCE])
		{
			if(_object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
			{
				if(sourceName == _object->GetName())
				{
					if(source.Attach(dynamic_cast<Variable<Quaternion>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::SourceIsRenamed));
						_object->Connect(Variable<Quaternion>::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::EventHandler));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
	if(arg.IsEmpty())
	{ 
		if(state[TEMPLATE][ARG1])
		{   
			if(_object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
			{
				if(templateArg.GetCurrentError() == StringExprParserEx::NO_ERRORS && templateArg.GetResult() == _object->GetName())
				{
					if(arg.Attach(dynamic_cast<Variable<Quaternion>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		} 
		else if(state[VARIABLE][ARG1])
		{
			if(_object->IsClassPropertyExist(Variable<Quaternion>::QUATERNION_VARIABLE_CLASS)) 
			{
				if(argName == _object->GetName())
				{
					if(arg.Attach(dynamic_cast<Variable<Quaternion>*>(_object)))
					{
						_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ObjectIsDestroyed));
						_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &ChangeQuaternionValueEvent::ArgIsRenamed));
						UpdateValid();
					}
					return;
				}
			}
		}
	}
}



bool ChangeQuaternionValueEvent::SaveToFile(StringANSI _path)			
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


bool ChangeQuaternionValueEvent::SaveToFile(File& _file)
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
		if(state[EXPRESSION][ARG1])
		{
			_file.Write(GetExpressionArg(ARG1).length());
			_file.WriteString(GetExpressionArg(ARG1));
		}
		else if(state[EQUALITY][ARG1])
		{
			_file.Write(GetEqualityArg(ARG1).length());
			_file.WriteString(GetEqualityArg(ARG1));
		}
		else if(state[TEMPLATE][ARG1])
		{
			_file.Write(GetTemplateArg(ARG1).length());
			_file.WriteString(GetTemplateArg(ARG1));
		}
		else if(state[VARIABLE][ARG1])
		{
			_file.Write(GetVariableArg(ARG1).length());
			_file.WriteString(GetVariableArg(ARG1));
		}
		else if(state[KEY][ARG1])
		{
			_file.Write(GetQuaternionKeyArg(ARG1));
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


AbstractEvent* ChangeQuaternionValueEvent::_LoadFromFile(File& _file)
{
	ChangeQuaternionValueEvent* event = new ChangeQuaternionValueEvent(Variable<Quaternion>::CHANGE_QUATERNION_VALUE_MESSAGE);
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
	if(event->state[EXPRESSION][ARG1])
	{
		StringANSI expression;
		_file.Read(length);
		_file.ReadString(expression, length);
		event->SetExpressionArg(ARG1, expression); 
	}
	else if(event->state[EQUALITY][ARG1])
	{
		StringANSI equality;
		_file.Read(length);
		_file.ReadString(equality, length);
		event->SetEqualityArg(ARG1, equality); 
	}
	else if(event->state[TEMPLATE][ARG1])
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
	else if(event->state[KEY][ARG1])
	{
		Quaternion arg; 
		_file.Read(arg); 
		event->SetQuaternionKeyArg(ARG1, arg);
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