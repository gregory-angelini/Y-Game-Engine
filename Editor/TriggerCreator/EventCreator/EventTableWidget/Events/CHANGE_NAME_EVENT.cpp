#include "../EventTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Event/Events.h>



void EventTableWidget::CHANGE_NAME_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor)
{
	if(_eventType == "CHANGE_NAME")
	{
		if(_index.column() == VALUE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
				    if(item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						_editor = UniversalDelegate::NAME_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::STRING_VARIABLE_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
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



bool EventTableWidget::CHANGE_NAME_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "CHANGE_NAME") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			SetPropertyEnable(ARG2_PROPERTY, false);
			SetPropertyEnable(ARG3_PROPERTY, false);
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG1_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
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
				SetCellText(ARG1_PROPERTY, CONTENT, "New name"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
			}
			else
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "New name"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "String variable"); 
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



QString EventTableWidget::CHANGE_NAME_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_NAME")
	{ 
		QString name;

		if(item(SOURCE_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(SOURCE_PROPERTY, VALUE)->text(); 
		}
		if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE" ||
		   item(ARG1_PROPERTY, ARG_MODE)->text() == "KEY")
		{
			name += "_" + item(ARG1_PROPERTY, VALUE)->text();
		}
		return name;
	}
	return "";
}



bool EventTableWidget::CHANGE_NAME_UpdateSourceVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_NAME")
	{ 
		UpdateAnyAsset(SOURCE_PROPERTY);
		return true;
	}
	return false;
}




bool EventTableWidget::CHANGE_NAME_UpdateArg1KeyPropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_NAME")
	{ 
		UpdateName(ARG1_PROPERTY);
		return true;
	}
	return false;
}



bool EventTableWidget::CHANGE_NAME_UpdateArg1VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_NAME")
	{
		UpdateVariable(ARG1_PROPERTY, Variable<StringANSI>::STRING_TYPE);
		return true;
	}
	return false;
}


void EventTableWidget::CHANGE_NAME_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_NAME")
	{
		if(item(_index.row(), ARG_MODE)->text() == "KEY")
		{
			if(_index.row() == ARG1_PROPERTY)
			{
				AnyAssetDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG1_PROPERTY)
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<StringANSI>::STRING_TYPE);
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


void EventCreator::CHANGE_NAME_SetEvent(int _eventType)
{
	if(_eventType == AbstractObject::CHANGE_NAME_MESSAGE)
	{
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "CHANGE_NAME");

		ChangeNameEvent* derived = dynamic_cast<ChangeNameEvent*>(event);
		
		if(derived)
		{
			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, derived->GetStringKeyArg(AbstractEvent::ARG1).c_str());
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


void EventCreator::CHANGE_NAME_Create(void)
{
	if(eventTable->GetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE) == "CHANGE_NAME")
	{
		event = Trigger::_CreateEvent(AbstractObject::CHANGE_NAME_MESSAGE);

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
					event->SetStringKeyArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE).toStdString());
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
		}
	} 
}
