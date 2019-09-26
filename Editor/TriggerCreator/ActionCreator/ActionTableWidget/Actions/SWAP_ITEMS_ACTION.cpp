#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>



void ActionTableWidget::SWAP_ITEMS_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "SWAP_ITEMS")
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
					if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
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
					else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
					{
						if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::ABSTRACT_LIST_EDITOR;
							return;
						}
						else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					else if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
					{
						if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::ABSTRACT_LIST_EDITOR;
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
					if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
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
					else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
					{
						if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::ANY_ASSET_EDITOR;
							return;
						}
						else if(item(ARG3_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					else if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
					{
						if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::ABSTRACT_LIST_EDITOR;
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
				
				case ARG4_PROPERTY:
				{
					if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
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
					}
					break;
				}

				case ARG5_PROPERTY:
				{
					if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
					{
						if(item(ARG5_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::ANY_ASSET_EDITOR;
							return;
						}
						else if(item(ARG5_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					break;
				}

				case TARGET_PROPERTY:
				{
					if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
					{ 
						if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
						{
							_editor = UniversalDelegate::ABSTRACT_LIST_EDITOR;
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

				case ARG4_PROPERTY:
				{
					if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
					{
						_editor = UniversalDelegate::COMBO_BOX_EDITOR;
						return;
					}
					break;
				}

				case ARG5_PROPERTY:
				{
					if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
					{
						_editor = UniversalDelegate::COMBO_BOX_EDITOR;
						return;
					}
					break;
				}

				case TARGET_PROPERTY:
				{
					if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
					{
						_editor = UniversalDelegate::COMBO_BOX_EDITOR;
						return;
					}
					break; 
				}
			}
		}
		_editor = UniversalDelegate::NO_EDIT;
	}
}




bool ActionTableWidget::SWAP_ITEMS_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "SWAP_ITEMS") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{ 
			SetDefaultState();
			SetPropertyEnable(TARGET_PROPERTY, false);
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, VALUE)), swapItemsArgsComboList);

			SetCellText(ARG1_PROPERTY, CONTENT, "Arg type");  
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultProperty(ARG2_PROPERTY);
			SetDefaultProperty(ARG3_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
			{ 
				SetPropertyEnable(TARGET_PROPERTY, true);
				SetDefaultProperty(TARGET_PROPERTY);

				SetPropertyEnable(ARG4_PROPERTY, false); 
				SetPropertyValid(ARG4_PROPERTY, true);

				SetPropertyEnable(ARG5_PROPERTY, false); 
				SetPropertyValid(ARG5_PROPERTY, true);

				delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
				SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));
								
				delegate->SetComboValuesForCell(indexFromItem(item(ARG3_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_TEMPLATE_ComboList);
				SetCellText(ARG3_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_TEMPLATE_ComboList.at(0));

				delegate->SetComboValuesForCell(indexFromItem(item(TARGET_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
				SetCellText(TARGET_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Item index1"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer"); 
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Item index1"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
				if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Item index2"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Integer"); 
				}
				else
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Item index2"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
				SetCellText(ARG4_PROPERTY, VALUE, "---");
				SetCellText(ARG5_PROPERTY, VALUE, "---");
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Abstract list"); 
			}  
			else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
			{ 
				SetPropertyEnable(ARG4_PROPERTY, true);
				SetDefaultProperty(ARG4_PROPERTY);

				SetPropertyEnable(ARG5_PROPERTY, true);
				SetDefaultProperty(ARG5_PROPERTY);

				SetPropertyEnable(TARGET_PROPERTY, false); 
				SetPropertyValid(TARGET_PROPERTY, true); 

				 
				delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
				SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));

				delegate->SetComboValuesForCell(indexFromItem(item(ARG3_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
				SetCellText(ARG3_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
	
				delegate->SetComboValuesForCell(indexFromItem(item(ARG4_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
				SetCellText(ARG4_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
	
				delegate->SetComboValuesForCell(indexFromItem(item(ARG5_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
				SetCellText(ARG5_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
				SetCellText(ARG2_PROPERTY, CONTENT, "List1"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "Abstract list");
				SetCellText(ARG3_PROPERTY, CONTENT, "Item1"); 
				SetCellText(ARG3_PROPERTY, DATA_TYPE, "Abstract object");
				SetCellText(ARG4_PROPERTY, CONTENT, "List2"); 
				SetCellText(ARG4_PROPERTY, DATA_TYPE, "Abstract list");
				SetCellText(ARG5_PROPERTY, CONTENT, "Item2"); 
				SetCellText(ARG5_PROPERTY, DATA_TYPE, "Abstract object");
				SetCellText(TARGET_PROPERTY, VALUE, "---"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
			{
				SetPropertyEnable(ARG4_PROPERTY, false); 
				SetPropertyValid(ARG4_PROPERTY, true);

				SetPropertyEnable(ARG5_PROPERTY, false); 
				SetPropertyValid(ARG5_PROPERTY, true); 
				 
				SetPropertyEnable(TARGET_PROPERTY, false); 
				SetPropertyValid(TARGET_PROPERTY, true);

				delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
				SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
								
				delegate->SetComboValuesForCell(indexFromItem(item(ARG3_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
				SetCellText(ARG3_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
				SetCellText(ARG2_PROPERTY, CONTENT, "List1"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "Abstract list");
				SetCellText(ARG3_PROPERTY, CONTENT, "List2"); 
				SetCellText(ARG3_PROPERTY, DATA_TYPE, "Abstract list");
				SetCellText(ARG4_PROPERTY, VALUE, "---");
				SetCellText(ARG5_PROPERTY, VALUE, "---");
				SetCellText(TARGET_PROPERTY, VALUE, "---"); 
			}
		}
		else if(_index.row() == ARG2_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG2_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Item index1"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer"); 
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Item index1"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer variable"); 
				} 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
			{
				SetCellText(ARG2_PROPERTY, CONTENT, "List1"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "Abstract list"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
			{
				SetCellText(ARG2_PROPERTY, CONTENT, "List1"); 
				SetCellText(ARG2_PROPERTY, DATA_TYPE, "Abstract list"); 
			} 
		}
		else if(_index.row() == ARG3_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(ARG3_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
			{
				if(item(ARG3_PROPERTY, ARG_MODE)->text() == "KEY")
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Item index2"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Integer"); 
				}
				else
				{
					SetCellText(ARG3_PROPERTY, CONTENT, "Item index2"); 
					SetCellText(ARG3_PROPERTY, DATA_TYPE, "Integer variable"); 
				} 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
			{
				SetCellText(ARG3_PROPERTY, CONTENT, "Item1"); 
				SetCellText(ARG3_PROPERTY, DATA_TYPE, "Abstract object"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
			{
				SetCellText(ARG3_PROPERTY, CONTENT, "List2"); 
				SetCellText(ARG3_PROPERTY, DATA_TYPE, "Abstract list"); 
			}
		} 
		else if(_index.row() == ARG4_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(ARG4_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
			{
				SetCellText(ARG4_PROPERTY, CONTENT, "List2"); 
				SetCellText(ARG4_PROPERTY, DATA_TYPE, "Abstract list"); 
			}
		} 
		else if(_index.row() == ARG5_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(ARG5_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
			{
				SetCellText(ARG5_PROPERTY, CONTENT, "Item2"); 
				SetCellText(ARG5_PROPERTY, DATA_TYPE, "Abstract object"); 
			}
		} 
		else if(_index.row() == TARGET_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(TARGET_PROPERTY);

			if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Abstract list"); 
			}
		}
		return true;
	}  
	return false;
}



QString ActionTableWidget::SWAP_ITEMS_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS")
	{ 
		QString name;
		
		if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
		{
			if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
			{
				name += "_" + item(TARGET_PROPERTY, VALUE)->text(); 
			}
		}
		if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG2_PROPERTY, VALUE)->text();
		}
		if(item(ARG3_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG3_PROPERTY, VALUE)->text(); 
		}
		if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
		{
			if(item(ARG4_PROPERTY, ARG_MODE)->text() == "VARIABLE")
			{
				name += "_" + item(ARG4_PROPERTY, VALUE)->text(); 
			}
		}
		if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
		{
			if(item(ARG5_PROPERTY, ARG_MODE)->text() == "VARIABLE")
			{
				name += "_" + item(ARG5_PROPERTY, VALUE)->text(); 
			}
		}
		return name;
	}
	return "";
}




bool ActionTableWidget::SWAP_ITEMS_UpdateArg1PropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS")
	{ 
		SetPropertyValid(ARG1_PROPERTY, true);
		return true;
	}
	return false;
}


bool ActionTableWidget::SWAP_ITEMS_UpdateArg2VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS")
	{
		if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
		{
			UpdateVariable(ARG2_PROPERTY, Variable<int32>::INTEGER_TYPE);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
		{
			UpdateAbstractList(ARG2_PROPERTY);
		}
		if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
		{
			UpdateAbstractList(ARG2_PROPERTY);
		}
		return true;
	}
	return false;
}

 
bool ActionTableWidget::SWAP_ITEMS_UpdateArg3VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS")
	{
		if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
		{
			UpdateVariable(ARG3_PROPERTY, Variable<int32>::INTEGER_TYPE);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
		{ 
			UpdateAnyAsset(ARG3_PROPERTY);
		}
		if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
		{
			UpdateAbstractList(ARG3_PROPERTY);
		}
		return true;
	}
	return false;
}




bool ActionTableWidget::SWAP_ITEMS_UpdateArg4VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS")
	{
		if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
		{ 
			UpdateAbstractList(ARG4_PROPERTY);
		}
		return true;
	}
	return false;
}



bool ActionTableWidget::SWAP_ITEMS_UpdateArg5VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS")
	{
		if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
		{ 
			UpdateAnyAsset(ARG5_PROPERTY);
		}
		return true;
	}
	return false;
}


bool ActionTableWidget::SWAP_ITEMS_UpdateTargetVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS")
	{
		if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
		{
			UpdateAbstractList(TARGET_PROPERTY);
		} 
		return true;
	}
	return false;
}


void ActionTableWidget::SWAP_ITEMS_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "SWAP_ITEMS")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG2_PROPERTY)
			{ 
				if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<int32>::INTEGER_TYPE);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
				{
					AbstractListDropEvent(_it, _index, _suffix, _fileName);
				}
				if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
				{
					AbstractListDropEvent(_it, _index, _suffix, _fileName);
				}
			}
			else if(_index.row() == ARG3_PROPERTY)
			{  
				if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, Variable<int32>::INTEGER_TYPE);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
				{
					AnyAssetDropEvent(_it, _index, _suffix, _fileName);
				}
				if(item(ARG1_PROPERTY, VALUE)->text() == "LIST")
				{
					AbstractListDropEvent(_it, _index, _suffix, _fileName);
				}
			}
			else if(_index.row() == ARG4_PROPERTY)
			{  
				if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
				{
					AbstractListDropEvent(_it, _index, _suffix, _fileName);
				}
			}
			else if(_index.row() == ARG5_PROPERTY)
			{  
				if(item(ARG1_PROPERTY, VALUE)->text() == "ITEM")
				{
					AnyAssetDropEvent(_it, _index, _suffix, _fileName);
				}
			}
			else if(_index.row() == TARGET_PROPERTY)
			{ 
				if(item(ARG1_PROPERTY, VALUE)->text() == "INDEX")
				{ 
					AbstractListDropEvent(_it, _index, _suffix, _fileName);
				}
			} 
		}
		else if(item(_index.row(), ARG_MODE) ->text() == "TEMPLATE")
		{ 
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}


void ActionCreator::SWAP_ITEMS_SetAction(int _actionType)
{
	if(_actionType == AbstractList::SWAP_ITEM_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "SWAP_ITEMS");

		SwapItemAction* derived = dynamic_cast<SwapItemAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "ITEM");
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG2).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG2).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG3))
			{
				actionTable->SetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG3).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG3))
			{
				actionTable->SetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG3).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG4))
			{
				actionTable->SetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG4).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG4))
			{
				actionTable->SetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG4).c_str());
			}
		}
	}
	else if(_actionType == AbstractList::SWAP_INDEX_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "SWAP_ITEMS");

		SwapIndexAction* derived = dynamic_cast<SwapIndexAction*>(action);
		
		if(derived)  
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "INDEX");
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
	else if(_actionType == AbstractList::SWAP_LIST_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "SWAP_ITEMS");

		SwapListAction* derived = dynamic_cast<SwapListAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "LIST");
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
			}
			if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG2).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG2))
			{
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG2).c_str());
			}
		}
	}
}


void ActionCreator::SWAP_ITEMS_Create(void)
{
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "SWAP_ITEMS")
	{
		if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "ITEM")
		{ 
			action = Trigger::_CreateAction(AbstractList::SWAP_ITEM_MESSAGE);

			if(action)
			{
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
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
				if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
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
				if(actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG3, actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG3) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG3, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG3, actionTable->GetCellText(ActionTableWidget::ARG4_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				if(actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
				{
					action->SetVariableArg(AbstractAction::ARG4, actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE).toStdString());
					if(action->GetArgState(AbstractAction::ASSET_TYPE, AbstractAction::ARG4) == AssetLibrary::UNKNOWN_ASSET)
					{
						int32 assetType = AssetLibrary::_IsAssetExist(actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE).toStdString());

						action->SetArgAssetType(AbstractAction::ARG4, assetType);
					}
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG4, actionTable->GetCellText(ActionTableWidget::ARG5_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		} 
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "INDEX")
		{  
			action = Trigger::_CreateAction(AbstractList::SWAP_INDEX_MESSAGE);

			if(action)
			{
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
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "LIST")
		{  
			action = Trigger::_CreateAction(AbstractList::SWAP_LIST_MESSAGE);

			if(action)
			{
				if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
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
				if(actionTable->GetCellText(ActionTableWidget::ARG3_PROPERTY, ActionTableWidget::ARG_MODE) == "VARIABLE")
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
