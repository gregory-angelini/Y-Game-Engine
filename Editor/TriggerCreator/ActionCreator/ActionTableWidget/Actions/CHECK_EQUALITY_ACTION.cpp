#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>


void ActionTableWidget::CHECK_EQUALITY_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "CHECK_EQUALITY")
	{
		if(_index.column() == VALUE)
		{
			switch(_index.row())
			{
				case ARG1_PROPERTY:
				{
					if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::BOOLEAN_VARIABLE_EDITOR;
						return;
					}
					else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "EQUALITY")
					{
						_editor = UniversalDelegate::EQUALITY_EXPR_EDITOR;
						return;
					}
					break;
				}
				
				case ARG2_PROPERTY:
				{
					if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::ACTION_EDITOR;
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
					if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						_editor = UniversalDelegate::ACTION_EDITOR;
						return;
					}
					else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;
					}
					break;
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
			}
		}
		_editor = UniversalDelegate::NO_EDIT;
	}
}




bool ActionTableWidget::CHECK_EQUALITY_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "CHECK_EQUALITY") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();// задать значение ячейкам таблицы по умолчанию

			// заблокированные свойства
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			SetPropertyEnable(TARGET_PROPERTY, false);

			// ARG1
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, ARG_MODE)), argMode_VARIABLE_EQUALITY_ComboList);// задать значения для Arg1
			SetCellText(ARG1_PROPERTY, ARG_MODE, argMode_VARIABLE_EQUALITY_ComboList.at(0)); // значение по умолчанию

			// ARG2
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);// задать значения для Arg1
			SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0)); // значение по умолчанию

			// ARG3
			delegate->SetComboValuesForCell(indexFromItem(item(ARG3_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);// задать значения для Arg1
			SetCellText(ARG3_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0)); // значение по умолчанию
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG1_PROPERTY);// сбросить состояние

			// ARG1
			if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE") 
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "Enable"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "Boolean variable"); 
			}
			else if(item(ARG1_PROPERTY, ARG_MODE)->text() == "EQUALITY")
			{
				SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
				SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
			}
		}
		else if(_index.row() == ARG2_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(ARG2_PROPERTY);// сбросить состояние

			// ARG2
			SetCellText(ARG2_PROPERTY, CONTENT, "If"); 
			SetCellText(ARG2_PROPERTY, DATA_TYPE, "Action"); 
		}
		else if(_index.row() == ARG3_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(ARG3_PROPERTY);// сбросить состояние

			// ARG3
			SetCellText(ARG3_PROPERTY, CONTENT, "Else"); 
			SetCellText(ARG3_PROPERTY, DATA_TYPE, "Action"); 
		}
		return true;
	}  
	return false;
}



QString ActionTableWidget::CHECK_EQUALITY_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_EQUALITY")
	{ 
		QString name;

		if(item(ARG1_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG1_PROPERTY, VALUE)->text();
		}
		if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG2_PROPERTY, VALUE)->text(); 
		}
		if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG3_PROPERTY, VALUE)->text(); 
		}
		return name;
	}
	return "";
}


 
bool ActionTableWidget::CHECK_EQUALITY_UpdateArg1EqualityPropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_EQUALITY")
	{ 
		UpdateEqualityExpr(ARG1_PROPERTY);
		return true;
	}
	return false;
}


bool ActionTableWidget::CHECK_EQUALITY_UpdateArg1VariablePropertyStatus(void)
{ 
	// если аргумент существует...
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_EQUALITY")
	{
		UpdateVariable(ARG1_PROPERTY, Variable<bool>::BOOLEAN_TYPE);
		return true;
	}
	return false;
}


bool ActionTableWidget::CHECK_EQUALITY_UpdateArg2VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_EQUALITY")
	{ 
		UpdateAction(ARG2_PROPERTY);
		return true;
	}
	return false;
}


bool ActionTableWidget::CHECK_EQUALITY_UpdateArg3VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_EQUALITY")
	{ 
		UpdateAction(ARG3_PROPERTY);
		return true;
	}
	return false;
}



void ActionTableWidget::CHECK_EQUALITY_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHECK_EQUALITY")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG1_PROPERTY)// ARG1
			{
				VariableDropEvent(_it, _index, _suffix, _fileName, Variable<bool>::BOOLEAN_TYPE);
			}
			else if(_index.row() == ARG2_PROPERTY)// ARG2
			{
				ActionDropEvent(_it, _index, _suffix, _fileName);
			}
			else if(_index.row() == ARG3_PROPERTY)// ARG3
			{
				ActionDropEvent(_it, _index, _suffix, _fileName);
			}
		} 
		else if(item(_index.row(), ARG_MODE) ->text() == "EQUALITY")
		{
			if(_index.row() == ARG1_PROPERTY)// ARG1
			{
				EqualityExprDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void ActionCreator::CHECK_EQUALITY_SetAction(int _actionType)
{
	if(_actionType == AbstractVariable::CHECK_EQUALITY_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHECK_EQUALITY");

		CheckEqualityAction* derived = dynamic_cast<CheckEqualityAction*>(action);
		
		if(derived)
		{
			// ARG1
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE, "EQUALITY");
				actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}

			// ARG2
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG2).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG2).c_str());
			}

			// ARG3
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG3))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG3).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG3))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG3).c_str());
			}
		}
	}
}


void ActionCreator::CHECK_EQUALITY_Create(void)
{
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "CHECK_EQUALITY")
	{
		action = Trigger::_CreateAction(AbstractVariable::CHECK_EQUALITY_MESSAGE);// создать действие

		if(action)
		{
			// ARG1
			if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
			{
				action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());

				// тип ассета...
				if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)// если тип ассета не определен
				{
					int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());// найти ассет с таким именем на диске

					action->SetArgAssetType(AbstractAction::ARG1, assetType);// задать тип ассета
				}
			}
			else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::ARG_MODE) == "EQUALITY")
			{
				action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE).toStdString());
			}

			// ARG2
			if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
			{
				action->SetVariableArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

				// тип ассета...
				if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG2) == AssetLibrary::UNKNOWN_ASSET)// если тип ассета не определен
				{
					int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());// найти ассет с таким именем на диске

					action->SetArgAssetType(AbstractAction::ARG2, assetType);// задать тип ассета
				}
			}
			else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
			{
				action->SetTemplateArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
			}

			// ARG3
			if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
			{
				action->SetVariableArg(AbstractAction::ARG3, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());

				// тип ассета...
				if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG3) == AssetLibrary::UNKNOWN_ASSET)// если тип ассета не определен
				{
					int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());// найти ассет с таким именем на диске

					action->SetArgAssetType(AbstractAction::ARG3, assetType);// задать тип ассета
				}
			}
			else if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
			{
				action->SetTemplateArg(AbstractAction::ARG3, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
			}
		}
	} 
}
