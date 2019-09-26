#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>



void ActionTableWidget::CHANGE_VALUE_RANGE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "CHANGE_VALUE_RANGE")
	{
		if(_index.column() == VALUE)
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
				    if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
					{
						if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
						{
							_editor = UniversalDelegate::FLOAT_EX_EDITOR;
							return;
						}
						else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::FLOAT_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
					{
						if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
						{
							_editor = UniversalDelegate::INT_EX_EDITOR;
							return;
						}
						else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::INTEGER_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					break;
				}
				
				case ARG3_PROPERTY:
				{
				    if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
					{
						if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
						{
							_editor = UniversalDelegate::FLOAT_EX_EDITOR;
							return;
						}
						else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::FLOAT_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
					{
						if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
						{
							_editor = UniversalDelegate::INT_EX_EDITOR;
							return;
						}
						else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::INTEGER_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					break;
				}

				case TARGET_PROPERTY:
				{
					if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
					{
						if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::FLOAT_VARIABLE_EDITOR;
							return;
						}
						else if(item(TARGET_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
					{
						if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::INTEGER_VARIABLE_EDITOR;
							return;
						}
						else if(item(TARGET_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
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

				case TARGET_PROPERTY:
				{
					_editor = UniversalDelegate::COMBO_BOX_EDITOR;
					return;
				}
			}
		}
		_editor = UniversalDelegate::NO_EDIT;
	}
}




bool ActionTableWidget::CHANGE_VALUE_RANGE_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "CHANGE_VALUE_RANGE") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, VALUE)), changeValueRangeArgsComboList);

			SetCellText(ARG1_PROPERTY, CONTENT, "Arg type");
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(ARG3_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
			SetCellText(ARG3_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(TARGET_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(TARGET_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultProperty(TARGET_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Float variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Integer variable"); 
			}

			SetDefaultProperty(ARG2_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Lower bound"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float"); 
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Lower bound"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Lower bound"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer"); 
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Lower bound"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
			}

			SetDefaultProperty(ARG3_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Upper bound"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Float"); 
				}
				else
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Upper bound"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Float variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Upper bound"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Integer"); 
				}
				else
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Upper bound"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
			}
		} 
		else if(_index.row() == ARG2_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(ARG2_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Lower bound"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float"); 
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Lower bound"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Lower bound"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer"); 
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Lower bound"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
			}
		}
		else if(_index.row() == ARG3_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(ARG3_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Upper bound"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Float"); 
				}
				else
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Upper bound"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Float variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Upper bound"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Integer"); 
				}
				else
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Upper bound"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
			} 
		}
		else if(_index.row() == TARGET_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(TARGET_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Float variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Integer variable"); 
			}
		}
		return true;
	}  
	return false;
}



QString ActionTableWidget::CHANGE_VALUE_RANGE_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE_RANGE")
	{ 
		QString name;

		if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(TARGET_PROPERTY, VALUE)->text(); 
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


bool ActionTableWidget::CHANGE_VALUE_RANGE_UpdateTargetVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE_RANGE")
	{  
		if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
		{
			UpdateVariable(TARGET_PROPERTY, Variable<float>::FLOAT_TYPE);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
		{
			UpdateVariable(TARGET_PROPERTY, Variable<int32>::INTEGER_TYPE);
		}
		return true;
	}
	return false;
}



bool ActionTableWidget::CHANGE_VALUE_RANGE_UpdateArg1PropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE_RANGE")
	{ 
		SetPropertyValid(ARG1_PROPERTY, true);
		return true;
	}
	return false;
}



bool ActionTableWidget::CHANGE_VALUE_RANGE_UpdateArg2VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE_RANGE")
	{
		if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
		{
			UpdateVariable(ARG2_PROPERTY, Variable<float>::FLOAT_TYPE);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
		{
			UpdateVariable(ARG2_PROPERTY, Variable<int32>::INTEGER_TYPE);
		}
		return true;
	}
	return false;
}


bool ActionTableWidget::CHANGE_VALUE_RANGE_UpdateArg3VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE_RANGE")
	{
		if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
		{
			UpdateVariable(ARG3_PROPERTY, Variable<float>::FLOAT_TYPE);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
		{ 
			UpdateVariable(ARG3_PROPERTY, Variable<int32>::INTEGER_TYPE);
		}
		return true;
	}
	return false;
}



void ActionTableWidget::CHANGE_VALUE_RANGE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE_RANGE")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG2_PROPERTY)
			{
				if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<float>::FLOAT_TYPE);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<int32>::INTEGER_TYPE);
				}
			}
			else if(_index.row() == ARG3_PROPERTY)
			{
				if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<float>::FLOAT_TYPE);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<int32>::INTEGER_TYPE);
				}
			}
			else if(_index.row() == TARGET_PROPERTY)
			{
				if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<float>::FLOAT_TYPE);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<int32>::INTEGER_TYPE);
				}
			}
		} 
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void ActionCreator::CHANGE_VALUE_RANGE_SetAction(int _actionType)
{
	if(_actionType == Variable<float>::CHANGE_FLOAT_RANGE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE_RANGE");

		ChangeFloatValueRangeAction* derived = dynamic_cast<ChangeFloatValueRangeAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "FLOAT");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, QString::number(derived->GetFloatKeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
			}
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, QString::number(derived->GetFloatKeyArg(AbstractAction::ARG2)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG2).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG2).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::TARGET))
			{
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::TARGET).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::TARGET))
			{
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::TARGET).c_str());
			}
		}
	}
	else if(_actionType == Variable<int32>::CHANGE_INTEGER_RANGE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE_RANGE");

		ChangeIntegerValueRangeAction* derived = dynamic_cast<ChangeIntegerValueRangeAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "INTEGER");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, QString::number(derived->GetIntegerKeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
			}
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, QString::number(derived->GetIntegerKeyArg(AbstractAction::ARG2)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG2).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG2).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::TARGET))
			{
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::TARGET).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::TARGET))
			{
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::TARGET).c_str());
			}
		}
	}
}


void ActionCreator::CHANGE_VALUE_RANGE_Create(void)
{
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "CHANGE_VALUE_RANGE")
	{
		if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "FLOAT")
		{
			action = Trigger::_CreateAction(Variable<float>::CHANGE_FLOAT_RANGE_MESSAGE);

			if(action)
			{
				if(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::TARGET, actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::TARGET) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::TARGET, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::TARGET, actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetFloatKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toFloat());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetFloatKeyArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toFloat());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG2) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG2, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		} 
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "INTEGER")
		{
			action = Trigger::_CreateAction(Variable<int32>::CHANGE_INTEGER_RANGE_MESSAGE);

			if(action)  
			{
				if(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::TARGET, actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::TARGET) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::TARGET, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::TARGET, actionTable->GetCellText(ActionTableWidget::TARGET_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetIntegerKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toInt());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG1) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG1, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "KEY")
				{
					action->SetIntegerKeyArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toInt());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG2) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG2, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG2, actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		} 
	}
}
