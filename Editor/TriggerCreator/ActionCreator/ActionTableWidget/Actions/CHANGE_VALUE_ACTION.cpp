#include "../ActionTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Action/Actions.h>



void ActionTableWidget::CHANGE_VALUE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _actionType, int& _editor)
{
	if(_actionType == "CHANGE_VALUE")
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
					if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
					{
						if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
						{
							_editor = UniversalDelegate::INT_EX_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
						{
							_editor = UniversalDelegate::FLOAT_EX_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
						{
							_editor = UniversalDelegate::VECTOR_2_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
						{
							_editor = UniversalDelegate::VECTOR_3_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
						{
							_editor = UniversalDelegate::QUATERNION_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOL_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{ 
							_editor = UniversalDelegate::TEXT_EDITOR;
							return;
						}
					}
				    else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
						{
							_editor = UniversalDelegate::INTEGER_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
						{
							_editor = UniversalDelegate::FLOAT_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
						{
							_editor = UniversalDelegate::VECTOR_2_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
						{
							_editor = UniversalDelegate::VECTOR_3_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
						{
							_editor = UniversalDelegate::QUATERNION_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOLEAN_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{
							_editor = UniversalDelegate::STRING_VARIABLE_EDITOR;
							return;
						}
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
					{
						if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
						{
							_editor = UniversalDelegate::INTEGER_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
						{
							_editor = UniversalDelegate::FLOAT_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
						{
							_editor = UniversalDelegate::VECTOR_2_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
						{
							_editor = UniversalDelegate::VECTOR_3_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
						{
							_editor = UniversalDelegate::QUATERNION_ARITHMETIC_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOLEAN_EXPR_EDITOR;
							return;
						} 
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
					{
						_editor = UniversalDelegate::STRING_EXPR_EDITOR;
						return;  
					}
					break;
				}

				case TARGET_PROPERTY:
				{
					if(item(TARGET_PROPERTY, ARG_MODE)->text() == "VARIABLE")
					{
						if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
						{
							_editor = UniversalDelegate::INTEGER_VARIABLE_EDITOR;
							return;
						}
					    else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
						{
							_editor = UniversalDelegate::FLOAT_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
						{
							_editor = UniversalDelegate::VECTOR_2_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
						{
							_editor = UniversalDelegate::VECTOR_3_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
						{
							_editor = UniversalDelegate::QUATERNION_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{
							_editor = UniversalDelegate::STRING_VARIABLE_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOLEAN_VARIABLE_EDITOR;
							return;
						}
					} 
					else if(item(TARGET_PROPERTY, ARG_MODE)->text() == "TEMPLATE")
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
				case ARG2_PROPERTY:
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




bool ActionTableWidget::CHANGE_VALUE_UpdateRow(const QModelIndex& _index, const QString& _type)
{
	if(_type == "CHANGE_VALUE") 
	{
		if(_index.row() == TYPE_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultState();
			SetPropertyEnable(ARG3_PROPERTY, false);
			SetPropertyEnable(ARG4_PROPERTY, false);
			SetPropertyEnable(ARG5_PROPERTY, false);
			delegate->SetComboValuesForCell(indexFromItem(item(ARG1_PROPERTY, VALUE)), createVariableArgsComboList);

			SetCellText(ARG1_PROPERTY, CONTENT, "Variable type");
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList);
			SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_EXPRESSION_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(TARGET_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(TARGET_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
		}
		else if(_index.row() == ARG1_PROPERTY && _index.column() == VALUE)
		{
			SetDefaultProperty(ARG2_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector2");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector2 variable"); 
				} 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3 variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "String expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				} 
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{ 
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Boolean expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean variable"); 
				}
			} 
			
			SetDefaultProperty(TARGET_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Integer variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Float variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Vector2 variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Vector3 variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Quaternion variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "String variable");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Boolean variable");
			}
		}
		else if(_index.row() == ARG2_PROPERTY && _index.column() == ARG_MODE)
		{ 
			SetDefaultProperty(ARG2_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Integer variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Float variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector2");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector2 variable"); 
				} 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Vector3 variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Arithmetic expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Quaternion variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "String expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String variable"); 
				}
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				if(item(ARG2_PROPERTY, ARG_MODE)->text() == "KEY")
				{ 
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean");
				}
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EXPRESSION")
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Boolean expression"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean variable"); 
				}
			}
		}
		else if(_index.row() == TARGET_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(TARGET_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Integer variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Float variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Vector2 variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Vector3 variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Quaternion variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "String variable");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				SetCellText(TARGET_PROPERTY, CONTENT, "Name"); 
				SetCellText(TARGET_PROPERTY, DATA_TYPE, "Boolean variable");
			}
		}
		return true;
	}  
	return false;
}
 


QString ActionTableWidget::CHANGE_VALUE_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
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
		return name;
	}
	return "";
}



bool ActionTableWidget::CHANGE_VALUE_UpdateTargetVariablePropertyStatus(void)
{ 
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
	{ 
		UpdateVariable(TARGET_PROPERTY, GetVariableType(item(ARG1_PROPERTY, VALUE)->text()));
		return true;
	}
	return false;
}



bool ActionTableWidget::CHANGE_VALUE_UpdateArg1PropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
	{ 
		SetPropertyValid(ARG1_PROPERTY, true); 
		if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
		{
			SetPropertyValid(ARG2_PROPERTY, true);
		}
		return true;
	}
	return false;
}


bool ActionTableWidget::CHANGE_VALUE_UpdateArg2VariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
	{

		if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "STRING" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
		{
			UpdateVariable(ARG2_PROPERTY, GetVariableType(item(ARG1_PROPERTY, VALUE)->text())); 
		}
		return true;
	}
	return false;
}


bool ActionTableWidget::CHANGE_VALUE_UpdateArg2ExpressionPropertyStatus(void)
{ 
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
	{ 
		if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
		   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
		{ 
			UpdateArithmeticExpr(ARG2_PROPERTY, GetVariableType(item(ARG1_PROPERTY, VALUE)->text()));
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
		{
			UpdateBooleanExpr(ARG2_PROPERTY);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
		{
			UpdateStringExpr(ARG2_PROPERTY);
		}
		return true;
	}
	return false; 
}

 

void ActionTableWidget::CHANGE_VALUE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
	{
		if(item(_index.row(), ARG_MODE)->text() == "VARIABLE")
		{
			if(_index.row() == ARG2_PROPERTY)
			{ 
				if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "STRING" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
				{ 
					VariableDropEvent(_it, _index, _suffix, _fileName, GetVariableType(item(ARG1_PROPERTY, VALUE)->text()));
				}
			}
			else if(_index.row() == TARGET_PROPERTY)
			{
				if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "STRING" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
				{
					VariableDropEvent(_it, _index, _suffix, _fileName, GetVariableType(item(ARG1_PROPERTY, VALUE)->text()));
				}
			}
		}
		else if(item(_index.row(), ARG_MODE)->text() == "EXPRESSION")
		{
			if(_index.row() == ARG2_PROPERTY)
			{
				if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "FLOAT" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3" ||
				   item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
				{
					ArithmeticExprDropEvent(_it, _index, _suffix, _fileName, item(ARG1_PROPERTY, VALUE)->text());
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
				{
					BooleanExprDropEvent(_it, _index, _suffix, _fileName);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
				{
					StringExprDropEvent(_it, _index, _suffix, _fileName);
				}
			}
		} 
		else if(item(_index.row(), ARG_MODE)->text() == "TEMPLATE")
		{
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}



void ActionCreator::CHANGE_VALUE_SetAction(int _actionType)
{
	if(_actionType == AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE");

		ChangeIntegerValueAction* derived = dynamic_cast<ChangeIntegerValueAction*>(action);
		
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
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE");

		ChangeFloatValueAction* derived = dynamic_cast<ChangeFloatValueAction*>(action);
		
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
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			} 
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE");

		ChangeBooleanValueAction* derived = dynamic_cast<ChangeBooleanValueAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "BOOLEAN");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetBooleanKeyArg(AbstractAction::ARG1) ? "true" : "false");
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractVariable::CHANGE_STRING_VALUE_MESSAGE)
	{   
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE");

		ChangeStringValueAction* derived = dynamic_cast<ChangeStringValueAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "STRING");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetStringKeyArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE)
	{ 
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE");

		ChangeVector2ValueAction* derived = dynamic_cast<ChangeVector2ValueAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "VECTOR_2");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, Vector2ToString(derived->GetVector2KeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE)
	{ 
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE");

		ChangeVector3ValueAction* derived = dynamic_cast<ChangeVector3ValueAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "VECTOR_3");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, Vector3ToString(derived->GetVector3KeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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
	else if(_actionType == AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE)
	{
		actionTable->SetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE, "CHANGE_VALUE");

		ChangeQuaternionValueAction* derived = dynamic_cast<ChangeQuaternionValueAction*>(action);
		
		if(derived)
		{
			actionTable->SetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE, "QUATERNION");
			if(derived->GetArgState(AbstractAction::KEY, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "KEY");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, QuaternionToString(derived->GetQuaternionKeyArg(AbstractAction::ARG1)));
			}
			else if(derived->GetArgState(AbstractAction::VARIABLE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "VARIABLE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetVariableArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::EXPRESSION, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "EXPRESSION");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetExpressionArg(AbstractAction::ARG1).c_str());
			}
			else if(derived->GetArgState(AbstractAction::TEMPLATE, AbstractAction::ARG1))
			{
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE, "TEMPLATE");
				actionTable->SetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE, derived->GetTemplateArg(AbstractAction::ARG1).c_str());
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


void ActionCreator::CHANGE_VALUE_Create(void)
{
	if(actionTable->GetCellText(ActionTableWidget::TYPE_PROPERTY, ActionTableWidget::VALUE) == "CHANGE_VALUE")
	{
		if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "INTEGER")
		{
			action = Trigger::_CreateAction(AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE);

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
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				} 
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "FLOAT")
		{
			action = Trigger::_CreateAction(AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE);

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
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "BOOLEAN")
		{
			action = Trigger::_CreateAction(AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE);

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
					action->SetBooleanKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE) == "true" ? true : false);
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
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				} 
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "STRING")
		{
			action = Trigger::_CreateAction(AbstractVariable::CHANGE_STRING_VALUE_MESSAGE);

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
					action->SetStringKeyArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
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
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "VECTOR_2")
		{
			action = Trigger::_CreateAction(AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE);

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
					action->SetVector2KeyArg(AbstractAction::ARG1, StringToVector2(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
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
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "VECTOR_3")
		{
			action = Trigger::_CreateAction(AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE);

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
					action->SetVector3KeyArg(AbstractAction::ARG1, StringToVector3(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
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
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
		else if(actionTable->GetCellText(ActionTableWidget::ARG1_PROPERTY, ActionTableWidget::VALUE) == "QUATERNION")
		{
			action = Trigger::_CreateAction(AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE);

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
					action->SetQuaternionKeyArg(AbstractAction::ARG1, StringToQuaternion(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE)));
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
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "EXPRESSION")
				{
					action->SetExpressionArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
				else if(actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::ARG_MODE) == "TEMPLATE")
				{
					action->SetTemplateArg(AbstractAction::ARG1, actionTable->GetCellText(ActionTableWidget::ARG2_PROPERTY, ActionTableWidget::VALUE).toStdString());
				}
			}
		}
	} 
}
