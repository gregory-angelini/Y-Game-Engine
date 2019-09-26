#include "../EventTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Event/Events.h>



void EventTableWidget::COLLISION_BETWEEN_OBJECTS_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor)
{
	if(_eventType == "COLLISION_BETWEEN_OBJECTS")
	{
		if(_index.column() == VALUE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
				    if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::OBJECT_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}
				
				case ARG2_PROPERTY:
				{
				    if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						_editor = UniversalDelegate::NAME_EDITOR;
						return;
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::STRING_VARIABLE_EDITOR;
						return;
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case ARG3_PROPERTY:
				{
					if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						_editor = UniversalDelegate::NAME_EDITOR;
						return;
					}
					else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::STRING_VARIABLE_EDITOR;
						return;
					}
					else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case ARG4_PROPERTY:
				{
				    if(item(ARG4_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::ABSTRACT_LIST_EDITOR;
						return;
					}
					else if(item(ARG4_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case ARG5_PROPERTY:
				{
				    if(item(ARG5_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{ 
						_editor = UniversalDelegate::ABSTRACT_LIST_EDITOR;
						return;
					}
					else if(item(ARG5_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}

				case SOURCE_PROPERTY:
				{
					if(item(SOURCE_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::OBJECT_EDITOR;
						return;
					}
					else if(item(SOURCE_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
				}
								
				case DEACTIVATION_MODE_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}
			}
		}
		else if(_index.column() == ARG_MODE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case ARG2_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case ARG3_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case ARG4_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case ARG5_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}

				case SOURCE_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}
			}
		}
		_editor = UniversalDelegate::NO_EDIT;
	}
}




bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "COLLISION_BETWEEN_OBJECTS") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG1_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG3_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG3_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG4_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG4_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG5_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG5_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(SOURCE_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(SOURCE_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(DEACTIVATION_MODE_PROPERTY, VALUE)), deactivationMode_EVER_AUTO_OVERLAY_ComboList);
			if(item(DEACTIVATION_MODE_PROPERTY, VALUE)->text().isEmpty()) { SetCellText(DEACTIVATION_MODE_PROPERTY, VALUE, deactivationMode_EVER_AUTO_OVERLAY_ComboList.at(0)); }
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG1_PROPERTY);
			SetCellText(ARG1_PROPERTY, CONTENT, "Target"); 
			SetCellText(ARG1_PROPERTY, DATA_TYPE, "Polygonal surface"); 
		}
		else if(_index.row() == ARG2_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG2_PROPERTY);
			if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				SetCellText(ARG2_PROPERTY, CONTENT, "Source shape"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
			}
			else
			{
				SetCellText(ARG2_PROPERTY, CONTENT, "Source shape"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "String variable"); 
			}
		}
		else if(_index.row() == ARG3_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG3_PROPERTY);
			if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				SetCellText(ARG3_PROPERTY, CONTENT, "Target shape"); 
				SetCellText(ARG3_PROPERTY, DATA_TYPE, "String");
			} 
			else
			{
				SetCellText(ARG3_PROPERTY, CONTENT, "Target shape"); 
				SetCellText(ARG3_PROPERTY, DATA_TYPE, "String variable"); 
			}
		}
		else if(_index.row() == ARG4_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG4_PROPERTY);
			SetCellText(ARG4_PROPERTY, CONTENT, "Source polygons"); 
			SetCellText(ARG4_PROPERTY, DATA_TYPE, "Integer list"); 
		}
		else if(_index.row() == ARG5_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG5_PROPERTY);
			SetCellText(ARG5_PROPERTY, CONTENT, "Target polygons"); 
			SetCellText(ARG5_PROPERTY, DATA_TYPE, "Integer list"); 
		}
		else if(_index.row() == SOURCE_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(SOURCE_PROPERTY);
			SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
			SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Polygonal surface"); 
		}
		return true;
	}  
	return false;
}



QString EventTableWidget::COLLISION_BETWEEN_OBJECTS_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{ 
		QString name;

		if(item(SOURCE_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(SOURCE_PROPERTY, VALUE)->text(); 
		}
		if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG1_PROPERTY, VALUE)->text();
		}
		if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE" ||
		   item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
		{
			name += "_" + item(ARG2_PROPERTY, VALUE)->text();
		}
		if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE" ||
		   item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
		{
			name += "_" + item(ARG3_PROPERTY, VALUE)->text();
		}
		if(item(ARG4_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{ 
			name += "_" + item(ARG4_PROPERTY, VALUE)->text();
		}
		if(item(ARG5_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG5_PROPERTY, VALUE)->text();
		}
		return name;
	}
	return "";
}


bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateSourceVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{ 
		UpdateObject(SOURCE_PROPERTY);
		return true;
	}
	return false;
}



bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateArg2KeyPropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{ 
		UpdateName(ARG2_PROPERTY);
		return true;
	}
	return false;
}


bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateArg3KeyPropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{ 
		UpdateName(ARG3_PROPERTY);
		return true;
	}
	return false;
}


bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateArg1VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{
		UpdateObject(ARG1_PROPERTY);
		return true;
	}
	return false;
} 


bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateArg2VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{
		UpdateVariable(ARG2_PROPERTY, Variable<StringANSI>::STRING_TYPE);
		return true;
	}
	return false;
}


bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateArg3VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{
		UpdateVariable(ARG3_PROPERTY, Variable<StringANSI>::STRING_TYPE);
		return true;
	}
	return false;
}



bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateArg4VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{
		UpdateList(ARG4_PROPERTY, List<int32>::INTEGER_TYPE); 
		return true;
	}
	return false;
}


bool EventTableWidget::COLLISION_BETWEEN_OBJECTS_UpdateArg5VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{
		UpdateList(ARG5_PROPERTY, List<int32>::INTEGER_TYPE); 
		return true;
	}
	return false;
}


void EventTableWidget::COLLISION_BETWEEN_OBJECTS_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "COLLISION_BETWEEN_OBJECTS")
	{
		if(item(_index.row(), ARG_MODE)->text() == "KEY")
		{
			if(_index.row() == ARG2_PROPERTY)
			{
				AnyAssetDropEvent(_it, _index, _suffix, _fileName);
			}
			else if(_index.row() == ARG3_PROPERTY)
			{
				AnyAssetDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG1_PROPERTY)
			{
				ObjectDropEvent(_it, _index, _suffix, _fileName);
			}
			else if(_index.row() == ARG2_PROPERTY)
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<StringANSI>::STRING_TYPE);
			}
			else if(_index.row() == ARG3_PROPERTY)
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<StringANSI>::STRING_TYPE);
			}
			else if(_index.row() == ARG4_PROPERTY)
			{
				ListDropEvent(_it, _index, _suffix, _fileName, List<int32>::INTEGER_TYPE);
			}
			else if(_index.row() == ARG5_PROPERTY)
			{
				ListDropEvent(_it, _index, _suffix, _fileName, List<int32>::INTEGER_TYPE);
			}
			else if(_index.row() == SOURCE_PROPERTY)
			{
				ObjectDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void EventCreator::COLLISION_BETWEEN_OBJECTS_SetEvent(int _eventType)
{
	if(_eventType == PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE)
	{
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "COLLISION_BETWEEN_OBJECTS");

		CollisionBetweenObjectsEvent* derived = dynamic_cast<CollisionBetweenObjectsEvent*>(event);
		
		if(derived)
		{
			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
				}
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG2))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetStringKeyArg(AbstractEvent::ARG2).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG2))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG2).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG2))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG2).c_str());
				}
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG3))
				{
					eventTable->SetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE, derived->GetStringKeyArg(AbstractEvent::ARG3).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG3))
				{
					eventTable->SetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG3).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG3))
				{
					eventTable->SetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG3).c_str());
				}
				if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG4))
				{
					eventTable->SetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG4).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG4))
				{
					eventTable->SetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG4).c_str());
				}
				if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG5))
				{
					eventTable->SetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG5).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG5))
				{
					eventTable->SetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG5).c_str());
				}
			}
			if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::SOURCE))
			{
				eventTable->SetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
				eventTable->SetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::SOURCE).c_str());
			}
			else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::SOURCE))
			{
				eventTable->SetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
				eventTable->SetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::SOURCE).c_str());
			}
		}
	}
}


void EventCreator::COLLISION_BETWEEN_OBJECTS_Create(void)
{
	if(eventTable->GetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE) == "COLLISION_BETWEEN_OBJECTS")
	{
		event = Trigger::_CreateEvent(PolygonalSurface::COLLISION_BETWEEN_OBJECTS_MESSAGE);

		if(event)
		{
			if(eventTable->GetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
			{
				event->SetVariableArg(AbstractEvent::SOURCE, eventTable->GetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::VALUE).toStdString());
				if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::SOURCE) == AssetLibrary::UNKNOWN_ASSET)
				{
					int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::VALUE).toStdString());

					event->SetArgAssetType(AbstractEvent::SOURCE, assetType);
				}
			}
			else if(eventTable->GetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
			{
				event->SetTemplateArg(AbstractEvent::SOURCE, eventTable->GetCellText(EventTableWidget::SOURCE_PROPERTY, EventTableWidget::VALUE).toStdString());
			}
			if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).toStdString());
					if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).toStdString());

						event->SetArgAssetType(AbstractEvent::ARG1, assetType);
					}
				}
			}
			else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}
			if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetStringKeyArg(AbstractEvent::ARG2, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}
			else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetVariableArg(AbstractEvent::ARG2, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG2) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

						event->SetArgAssetType(AbstractEvent::ARG2, assetType);
					}
				}
			}
			else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetTemplateArg(AbstractEvent::ARG2, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}
			if(eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetStringKeyArg(AbstractEvent::ARG3, eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}
			else if(eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetVariableArg(AbstractEvent::ARG3, eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE).toStdString());
					if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG3) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE).toStdString());

						event->SetArgAssetType(AbstractEvent::ARG3, assetType);
					}
				}
			}
			else if(eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetTemplateArg(AbstractEvent::ARG3, eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}
			if(eventTable->GetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetVariableArg(AbstractEvent::ARG4, eventTable->GetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::VALUE).toStdString());
					if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG4) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::VALUE).toStdString());

						event->SetArgAssetType(AbstractEvent::ARG4, assetType);
					}
				}
			}
			else if(eventTable->GetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetTemplateArg(AbstractEvent::ARG4, eventTable->GetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}
			if(eventTable->GetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetVariableArg(AbstractEvent::ARG5, eventTable->GetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::VALUE).toStdString());
					if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG5) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::VALUE).toStdString());

						event->SetArgAssetType(AbstractEvent::ARG5, assetType);
					}
				}
			}
			else if(eventTable->GetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
			{
				if(!eventTable->GetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetTemplateArg(AbstractEvent::ARG5, eventTable->GetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}

			if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).isEmpty() &&
			   eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty() &&
			   eventTable->GetCellText(EventTableWidget::ARG3_PROPERTY, EventTableWidget::VALUE).isEmpty() &&
			   eventTable->GetCellText(EventTableWidget::ARG4_PROPERTY, EventTableWidget::VALUE).isEmpty() &&
			   eventTable->GetCellText(EventTableWidget::ARG5_PROPERTY, EventTableWidget::VALUE).isEmpty())
			{
				event->SetArgsEnable(false);
			}
		}
	} 
}
