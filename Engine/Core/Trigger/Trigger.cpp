#include "Trigger.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
StringANSI Trigger::triggersSuffix = "trigger";
StringANSI Trigger::triggersDir = "triggers/";
MagicList<AbstractObject> Trigger::deletionList;
bool Trigger::eventLogEnable = false;
SimpleList<Trigger::EventInfo> Trigger::eventLogList;
bool Trigger::blockEnable = false;


Trigger::Trigger(void): loadArgsEnable(true), systemTimeInMs(-1), repeatLimit(0), activationCount(0)
{
	triggerFinishAction.Attach(new TriggerFinishAction(Trigger::TRIGGER_FINISH_MESSAGE, this));

	events.trigger = this;
	actions.trigger = this;

	AddClassProperty(TRIGGER_CLASS);
}



Trigger::~Trigger(void)
{
	Destroy();
	Destroying();
}



bool Trigger::_IsEventLogEnabled(void)
{
	return eventLogEnable;
}


void Trigger::_SetEventLogEnable(bool _eventLogEnable)
{
	eventLogEnable = _eventLogEnable;
}


SimpleList<Trigger::EventInfo>& Trigger::_GetEventLogList(void)
{
	return eventLogList;
}



void Trigger::Destroy(void)
{
	actions.DisconnectFromAllMessages();
	actions.Clear();
	actionExprParser.Destroy();

	events.DisconnectFromAllMessages();
	events.Clear();
	eventExprParser.Destroy();
}


bool Trigger::IsLoadArgsEnabled(void)const
{
	return loadArgsEnable;
}


void Trigger::SetLoadArgsEnable(bool _enable)
{
	loadArgsEnable = _enable;
}



void Trigger::_SetBlockEnable(bool _enable)
{
	blockEnable = _enable;
}


bool Trigger::_IsBlockEnabled(void)
{
	return blockEnable;
}



void Trigger::CalculateBooleanExpr2(void)
{ 
	if(eventExprParser.GetCurrentError() == BooleanExprParser::NO_ERRORS)
	{
		if(eventExprParser.GetAmountOfUniqueVariables() == events.GetSize() && eventExprParser.Calculate() == BooleanExprParser::NO_ERRORS)
		{
			if(eventExprParser.GetResult())
			{
				SetValue(true);
				return;
			}
		}
	} 
	
	if(value)
	{
		SetValue(false);
	} 
}


void Trigger::CalculateBooleanExpr1(void)
{
	if(eventExprParser.GetCurrentError() == BooleanExprParser::NO_ERRORS)
	{
		if(eventExprParser.GetAmountOfUniqueVariables() == events.GetSize() && eventExprParser.Calculate() == BooleanExprParser::NO_ERRORS)
		{
			if(eventExprParser.GetResult())
			{
				if(!value)
				{
					SetValue(true);
				}
				return;
			}
		}
	} 

	if(value)
	{
		SetValue(false);
	}
}


 
StringANSI Trigger::GetEventExpr(void)const
{
	return eventExprParser.GetBooleanExpr();
}


StringANSI Trigger::GetActionExpr(void)const
{
	return actionExprParser.GetEnumerationExpr();
}
					
	
bool Trigger::SetActionExpr(StringANSI _expr)
{
	actions.DisconnectFromAllMessages();
	actions.Clear();
	
	EnumerationExprParser::ErrorType result = actionExprParser.Parse(_expr);

	if(result == EnumerationExprParser::NO_ERRORS)
	{
		for(SimpleList<StringANSI>::Node* it = actionExprParser.GetVariables().GetHead(); it != NIL; it = it->GetRight())
		{	
			AbstractObject* action = AssetLibrary::_GetAssets().FindObject(it->key);
			
			if(action)
			{
				actions.AddObject(action);
			}  
		}
			
		if(actions.GetSize() != actionExprParser.GetAmountOfUniqueVariables())
		{
			AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, &actions, Caller<>(&actions, &Trigger::ActionTree::AddAction));
		}
		return true;
	}
	else if(result == EnumerationExprParser::EXPR_NOT_FOUND)
	{
		return true;
	}
	return false;
}


void Trigger::SetActionExprEx(void)
{
	actions.DisconnectFromAllMessages();
	actions.Clear();
	
	EnumerationExprParser::ErrorType result = actionExprParser.GetCurrentError();

	if(result == EnumerationExprParser::NO_ERRORS)
	{
		for(SimpleList<StringANSI>::Node* it = actionExprParser.GetVariables().GetHead(); it != NIL; it = it->GetRight())
		{	
			AbstractObject* action = AssetLibrary::_GetAssets().FindObject(it->key);
			
			if(action)
			{
				actions.AddObject(action);
			}  
		}
			
		if(actions.GetSize() != actionExprParser.GetAmountOfUniqueVariables())
		{
			AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, &actions, Caller<>(&actions, &Trigger::ActionTree::AddAction));
		}
	}
}


bool Trigger::SetEventExpr(StringANSI _expr)
{ 
	events.DisconnectFromAllMessages();
	events.Clear();

	BooleanExprParser::ErrorType result = eventExprParser.Parse(_expr);

	if(result == BooleanExprParser::NO_ERRORS)
	{
		for(SimpleList<StringANSI>::Node* it = eventExprParser.GetVariables().GetHead(); it != NIL; it = it->GetRight())
		{ 
			AbstractObject* arg = AssetLibrary::_GetAssets().FindObject(it->key);
			
			if(arg)
			{
				events.AddObject(arg);
			}  
		}
 
		CalculateBooleanExpr1();
		
		if(eventExprParser.GetAmountOfUniqueVariables() != events.GetSize())
		{
			AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, &events, Caller<>(&events, &Trigger::EventTree::AddVariable));
		}
		return true;
	}
	else if(result == BooleanExprParser::EXPR_NOT_FOUND)
	{ 
		return true;
	} 
	else
	{
		if(value)
		{
			SetValue(false);
		}
	}
	return false;
}



void Trigger::CheckEventExpr(void)
{ 
	events.DisconnectFromAllMessages();
	events.Clear();

	BooleanExprParser::ErrorType result = eventExprParser.GetCurrentError();

	if(result == BooleanExprParser::NO_ERRORS)
	{
		for(SimpleList<StringANSI>::Node* it = eventExprParser.GetVariables().GetHead(); it != NIL; it = it->GetRight())
		{ 
			AbstractObject* arg = AssetLibrary::_GetAssets().FindObject(it->key);
			
			if(arg)
			{
				events.AddObject(arg);
			}  
		}

		CalculateBooleanExpr1();
		
		if(eventExprParser.GetAmountOfUniqueVariables() != events.GetSize())
		{
			AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, &events, Caller<>(&events, &Trigger::EventTree::AddVariable));
		}
	}
	else if(result == BooleanExprParser::EXPR_NOT_FOUND)
	{ 
		if(value)
		{
			SetValue(true);
		}
	} 
	else
	{
		if(value)
		{
			SetValue(false);
		}
	}
}


void Trigger::SetValue(bool _value)
{
	if(!blockEnable)
	{
		if(_value && Renderer::_GetPicked())
		{
			if(systemTimeInMs == Renderer::_GetPicked()->GetWorkingTimeInMs())
			{ 
				if(activationCount == 0)
				{ 
					if(eventLogEnable)
					{
						eventLogList.AddToTail(EventInfo(GetName(), EventInfo::TRIGGER_CATEGORY, -2, Renderer::_GetPicked()->GetWorkingTimeInMs()));
					}
					return; 
				} 
				else if(activationCount > 0)
				{
					activationCount--;
				}
			}
			else
			{  
				activationCount = repeatLimit;
				systemTimeInMs = Renderer::_GetPicked()->GetWorkingTimeInMs();
			}

			if(activationLimit == 0) { return; }
			else if(activationLimit > 0)
			{
				activationLimit--;
			}

			if(eventLogEnable)
			{
				eventLogList.AddToTail(EventInfo(GetName(), EventInfo::TRIGGER_CATEGORY, (repeatLimit - activationCount) + 1, Renderer::_GetPicked()->GetWorkingTimeInMs()));
			}
			Variable::SetValue(_value);
			MagicListEx<AbstractAction> activationList;

			actionList.ShareToTail(activationList);
			activationList.AddToTail(triggerFinishAction.Get()); 
			while(activationList.IsNotEmpty())
			{
				AbstractAction* action = activationList.GetHead();
				delete activationList.GetHeadAddress();

				(*action)();
			}

			if(deactivationMode == AbstractEvent::AUTO_SWITCH_OFF) 
			{
				Variable::SetValue(false);
			}
		}
		else
		{
			if(!value) { return; }
			Variable::SetValue(_value);
		}
	} 
}


int32 Trigger::GetRepeatLimit(void)const
{
	return repeatLimit;
}


void Trigger::SetRepeatLimit(int32 _value)
{
	repeatLimit = _value;
	activationCount = repeatLimit;
}


void Trigger::_ClearDeletionList(void)
{
	if(deletionList.IsNotEmpty())
	{
		deletionList.Clear(MagicContainer::DELETE_NODE);
	}
}



int32 Trigger::GetAmountOfDefinedActions(void)const
{
	return actions.GetSize();
}



int32 Trigger::GetAmountOfUndefinedActions(void)const
{
	return actionExprParser.GetAmountOfUniqueVariables() - actions.GetSize();
}



int32 Trigger::GetAmountOfDefinedEvents(void)const
{
	return events.GetSize();
}



int32 Trigger::GetAmountOfUndefinedEvents(void)const
{
	return eventExprParser.GetAmountOfUniqueVariables() - events.GetSize();
}


 
StringANSI Trigger::_GetFileSuffix(void)
{
	return triggersSuffix;
}


void Trigger::_SetFileSuffix(StringANSI _suffix)
{
	triggersSuffix = _suffix;
}

 

StringANSI Trigger::_GetDir(void)
{
	return triggersDir;
}


bool Trigger::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + triggersDir + GetName() + "." + triggersSuffix;
	}

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}

 

bool Trigger::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(activationLimit);
		_file.Write(loadArgsEnable);
		_file.Write(deactivationMode);
		_file.Write(repeatLimit);
		_file.Write(value);
		actionExprParser.SaveToFile(_file);
		SaveActionsToFile(actions.GetRootAddress());
		eventExprParser.SaveToFile(_file);
		SaveEventsToFile(events.GetRootAddress(), _file);
		return true;
	}
	return false;
}



bool Trigger::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + Trigger::_GetDir() + _name + "." + Trigger::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Trigger::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(activationLimit);
		_file.Write(loadArgsEnable);
		_file.Write(deactivationMode);
		_file.Write(repeatLimit);
		_file.Write(value);
		actionExprParser.SaveToFile(_file);
		SaveActionsToFile(actions.GetRootAddress());
		eventExprParser.SaveToFile(_file);
		SaveEventsToFile(events.GetRootAddress(), _file);
		return true;
	}
	return false;
}


void Trigger::SaveEventsToFile(EventTree::AddressDerived* _it, File& _file)
{
	if(_it) 
	{
		int64 properties = _it->GetNode()->GetClassProperties();
		int32 assetType;

		if(properties & Trigger::TRIGGER_CLASS)
		{
			assetType = AssetLibrary::TRIGGER_ASSET;
		}
		else if(properties & AbstractEvent::EVENT_CLASS)
		{
			assetType = AssetLibrary::EVENT_ASSET;
		}
		else
		{
			assetType = AssetLibrary::VARIABLE_ASSET;
		}

		if(!AssetLibrary::_IsAssetExist(_it->GetNode()->GetName(), assetType))
		{
			if(assetType == AssetLibrary::TRIGGER_ASSET && _it->GetNode()->GetName() == GetName())
			{}
			else 
			{
				_it->GetNode()->SaveToFile();
			}
		}

		SaveEventsToFile(_it->GetChild(MagicBinaryTree<AbstractEvent>::LEFT_CHILD), _file);
		SaveEventsToFile(_it->GetChild(MagicBinaryTree<AbstractEvent>::RIGHT_CHILD), _file);
	}
}



void Trigger::SaveActionsToFile(ActionTree::AddressDerived* _it)
{
	if(_it)
	{
		if(!AssetLibrary::_IsAssetExist(_it->GetNode()->GetName(), AssetLibrary::ACTION_ASSET))
		{
			_it->GetNode()->SaveToFile();
		}

		SaveActionsToFile(_it->GetChild(MagicBinaryTree<AbstractAction>::LEFT_CHILD));
		SaveActionsToFile(_it->GetChild(MagicBinaryTree<AbstractAction>::RIGHT_CHILD));
	}
}


bool Trigger::LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + triggersDir + _path + "." + triggersSuffix;
	}

	File file(_path, File::READ);

	return LoadFromFile(file);
} 


bool Trigger::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		Destroy();
		int32 version;
		_file.Read(version);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		Rename(name);
		_file.Read(activationLimit);
		bool loadArgsEnable;
		_file.Read(loadArgsEnable); 
		SetLoadArgsEnable(loadArgsEnable);
		int32 deactivationMode;
		_file.Read(deactivationMode);
		SetDeactivationMode(deactivationMode);
		_file.Read(repeatLimit);
		_file.Read(value);
		actionExprParser.LoadFromFile(_file);
		SetActionExprEx();
		if(loadArgsEnable && actionExprParser.GetCurrentError() == EnumerationExprParser::NO_ERRORS)
		{
			if(actions.GetSize() != actionExprParser.GetAmountOfUniqueVariables())
			{
				AbstractObject* argument;
				for(SimpleList<StringANSI>::Node* it = actionExprParser.GetVariables().GetHead(); it != NIL; it = it->GetRight())
				{ 
					if(actions.FindObject(it->key) == NIL)
					{ 
						argument = AbstractAction::_LoadFromFile(it->key);

						if(argument)
						{
							AssetLibrary::_GetAssets().AddObject(argument);
						}
					} 
				} 
			}
		}
		eventExprParser.LoadFromFile(_file);
		CheckEventExpr();
		if(loadArgsEnable && eventExprParser.GetCurrentError() == BooleanExprParser::NO_ERRORS)
		{
			if(events.GetSize() != eventExprParser.GetAmountOfUniqueVariables())
			{
				AbstractObject* argument;
				for(SimpleList<StringANSI>::Node* it = eventExprParser.GetVariables().GetHead(); it != NIL; it = it->GetRight())
				{ 
					if(events.FindObject(it->key) == NIL)
					{ 
						if((argument = AbstractEvent::_LoadFromFile(it->key)) == NIL)
						{ 
							if((argument = AbstractVariable::_LoadFromFile(it->key)) == NIL)
							{
								if(it->key != GetName())
								{		
									argument = Trigger::_LoadFromFile(it->key);
								}
								else { argument = this; } 
							}
						}
										
						if(argument)
						{
							AssetLibrary::_GetAssets().AddObject(argument);
						}
					} 
				}
			}
		}
		return true;
	}
	return false;
}



Trigger* Trigger::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + triggersDir + _path + "." + triggersSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


Trigger* Trigger::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Trigger* trigger = new Trigger();

		if(trigger->LoadFromFile(_file))
		{
			return trigger;
		}
		delete trigger;
	}
	return NIL;
}
