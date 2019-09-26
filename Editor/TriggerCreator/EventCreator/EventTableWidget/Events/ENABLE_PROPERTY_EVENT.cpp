#include "../EventTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Event/Events.h>



void EventTableWidget::ENABLE_PROPERTY_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor)
{
	if(_eventType == "ENABLE_PROPERTY")
	{
		if(_index.column() == VALUE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
				    if(item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						_editor = UniversalDelegate::INT_EX_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::INTEGER_VARIABLE_EDITOR;
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
						_editor = UniversalDelegate::BOOL_EDITOR;
						return;
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::BOOLEAN_VARIABLE_EDITOR;
						return;
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
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
						_editor = UniversalDelegate::ANY_ASSET_EDITOR;
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



bool EventTableWidget::ENABLE_PROPERTY_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "ENABLE_PROPERTY") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			SetPropertyEnable(ARG3_PROPERTY, false);
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG1_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(SOURCE_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(SOURCE_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(DEACTIVATION_MODE_PROPERTY, VALUE)), deactivationMode_EVER_AUTO_OVERLAY_ComboList);
			if(item(DEACTIVATION_MODE_PROPERTY, VALUE)->text().isEmpty()) { SetCellText(DEACTIVATION_MODE_PROPERTY, VALUE, deactivationMode_EVER_AUTO_OVERLAY_ComboList.at(0)); }
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG1_PROPERTY);
			if(item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "Property index"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "Integer");
			}
			else
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "Property index"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "Integer variable"); 
			}
		}
		else if(_index.row() == ARG2_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG2_PROPERTY);
			if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
			{
				SetCellText(ARG2_PROPERTY, CONTENT, "Enable"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean");
			}
			else
			{
				SetCellText(ARG2_PROPERTY, CONTENT, "Enable"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean variable"); 
			}
		}
		else if(_index.row() == SOURCE_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(SOURCE_PROPERTY);
			SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
			SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Abstract object"); 
		}
		return true;
	}  
	return false;
}



QString EventTableWidget::ENABLE_PROPERTY_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "ENABLE_PROPERTY")
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
		if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG2_PROPERTY, VALUE)->text();
		}
		return name;
	}
	return "";
}



bool EventTableWidget::ENABLE_PROPERTY_UpdateSourceVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "ENABLE_PROPERTY")
	{ 
		UpdateAnyAsset(SOURCE_PROPERTY);
		return true;
	}
	return false;
}



bool EventTableWidget::ENABLE_PROPERTY_UpdateArg1VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "ENABLE_PROPERTY")
	{
		UpdateVariable(ARG1_PROPERTY, Variable<int32>::INTEGER_TYPE);
		return true;
	}
	return false;
}


bool EventTableWidget::ENABLE_PROPERTY_UpdateArg2VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "ENABLE_PROPERTY")
	{
		UpdateVariable(ARG2_PROPERTY, Variable<bool>::BOOLEAN_TYPE);
		return true;
	}
	return false;
}


void EventTableWidget::ENABLE_PROPERTY_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "ENABLE_PROPERTY")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG1_PROPERTY)
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<int32>::INTEGER_TYPE);
			}
			else if(_index.row() == ARG2_PROPERTY)
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<bool>::BOOLEAN_TYPE);
			}
			else if(_index.row() == SOURCE_PROPERTY)
			{
				AnyAssetDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void EventCreator::ENABLE_PROPERTY_SetEvent(int _eventType)
{
	if(_eventType == AbstractObject::ENABLE_PROPERTY_MESSAGE)
	{
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "ENABLE_PROPERTY");

		EnablePropertyEvent* derived = dynamic_cast<EnablePropertyEvent*>(event);
		
		if(derived)
		{
			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, QString::number(derived->GetIntegerKeyArg(AbstractEvent::ARG1)));
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
				}
			}
			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG2))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetBooleanKeyArg(AbstractEvent::ARG2) ? "true" : "false");
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


void EventCreator::ENABLE_PROPERTY_Create(void)
{
	if(eventTable->GetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE) == "ENABLE_PROPERTY")
	{
		event = Trigger::_CreateEvent(AbstractObject::ENABLE_PROPERTY_MESSAGE);

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
			if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).isEmpty())
			{
				event->SetArgsEnable(false);
			}
			else
			{
				if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
				{
					event->SetIntegerKeyArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).toInt());
				}
				else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
				{
					event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).toStdString());
					if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).toStdString());

						event->SetArgAssetType(AbstractEvent::ARG1, assetType);
					}
				}
				else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
				{
					event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}
			if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
			{
				event->SetArgsEnable(false);
			}
			else
			{
				if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
				{
					event->SetBooleanKeyArg(AbstractEvent::ARG2, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE) == "true" ? true : false);
				}
				else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
				{
					event->SetVariableArg(AbstractEvent::ARG2, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG2) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

						event->SetArgAssetType(AbstractEvent::ARG2, assetType);
					}
				}
				else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
				{
					event->SetTemplateArg(AbstractEvent::ARG2, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
				}
			}
		}
	} 
}
