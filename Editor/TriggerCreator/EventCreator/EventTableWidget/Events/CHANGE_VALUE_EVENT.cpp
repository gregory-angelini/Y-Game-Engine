#include "../EventTableWidget.h"
#include <Editor/TriggerCreator/TriggerCreator.h>
#include <Engine/Core/Trigger/Event/Events.h>



void EventTableWidget::CHANGE_VALUE_GetEditorTypeForCell(const QModelIndex& _index, const QString& _eventType, int& _editor)
{
	if(_eventType == "CHANGE_VALUE")
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
						else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
						{
							_editor = UniversalDelegate::BOOLEAN_EXPR_EDITOR;
							return;
						}
						else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
						{
							_editor = UniversalDelegate::STRING_EXPR_EDITOR;
							return;
						}
					}
					else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
					{
						_editor = UniversalDelegate::EQUALITY_EXPR_EDITOR;
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




bool EventTableWidget::CHANGE_VALUE_UpdateRow(const QModelIndex& _index, const QString& _type)
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
			delegate->SetComboValuesForCell(indexFromItem(item(ARG2_PROPERTY, ARG_MODE)), argMode_KEY_VARIABLE_EXPRESSION_EQUALITY_TEMPLATE_ComboList);
			SetCellText(ARG2_PROPERTY, ARG_MODE, argMode_KEY_VARIABLE_EXPRESSION_EQUALITY_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(SOURCE_PROPERTY, ARG_MODE)), argMode_VARIABLE_TEMPLATE_ComboList);
			SetCellText(SOURCE_PROPERTY, ARG_MODE, argMode_VARIABLE_TEMPLATE_ComboList.at(0));
			delegate->SetComboValuesForCell(indexFromItem(item(DEACTIVATION_MODE_PROPERTY, VALUE)), deactivationMode_EVER_AUTO_OVERLAY_ComboList);
			if(item(DEACTIVATION_MODE_PROPERTY, VALUE)->text().isEmpty()) { SetCellText(DEACTIVATION_MODE_PROPERTY, VALUE, deactivationMode_EVER_AUTO_OVERLAY_ComboList.at(0)); }
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean variable"); 
				}
			}
			
			SetDefaultProperty(SOURCE_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Integer variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Float variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Vector2 variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Vector3 variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Quaternion variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "String variable");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Boolean variable");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
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
				else if(item(ARG2_PROPERTY, ARG_MODE)->text() == "EQUALITY")
				{ 
					SetCellText(ARG1_PROPERTY, CONTENT, "Equality expression"); 
					SetCellText(ARG1_PROPERTY, DATA_TYPE, "String");
				}
				else
				{
					SetCellText(ARG2_PROPERTY, CONTENT, "Value"); 
					SetCellText(ARG2_PROPERTY, DATA_TYPE, "Boolean variable"); 
				}
			} 
		}
		else if(_index.row() == SOURCE_PROPERTY && _index.column() == ARG_MODE)
		{
			SetDefaultProperty(SOURCE_PROPERTY);
			if(item(ARG1_PROPERTY, VALUE)->text() == "INTEGER")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Integer variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "FLOAT")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Float variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_2")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Vector2 variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "VECTOR_3")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Vector3 variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "QUATERNION")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Quaternion variable"); 
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "String variable");
			}
			else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
			{
				SetCellText(SOURCE_PROPERTY, CONTENT, "Name"); 
				SetCellText(SOURCE_PROPERTY, DATA_TYPE, "Boolean variable");
			}
		}
		return true;
	}  
	return false;
}
 


QString EventTableWidget::CHANGE_VALUE_CreateAutoName(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
	{ 
		QString name;

		if(item(SOURCE_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(SOURCE_PROPERTY, VALUE)->text(); 
		}
		if(item(ARG2_PROPERTY, ARG_MODE)->text() == "VARIABLE")
		{
			name += "_" + item(ARG2_PROPERTY, VALUE)->text();
		}
		return name;
	}
	return "";
}



bool EventTableWidget::CHANGE_VALUE_UpdateSourceVariablePropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
	{ 
		UpdateVariable(SOURCE_PROPERTY, GetVariableType(item(ARG1_PROPERTY, VALUE)->text()));
		return true;
	}
	return false;
}



bool EventTableWidget::CHANGE_VALUE_UpdateArg1PropertyStatus(void)
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


bool EventTableWidget::CHANGE_VALUE_UpdateArg2VariablePropertyStatus(void)
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


 
bool EventTableWidget::CHANGE_VALUE_UpdateArg2ExpressionPropertyStatus(void)
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
		else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
		{ 
			UpdateStringExpr(ARG2_PROPERTY);
		}
		else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
		{
			UpdateBooleanExpr(ARG2_PROPERTY);
		}
		return true;
	}
	return false;
}

 
 
bool EventTableWidget::CHANGE_VALUE_UpdateArg2EqualityPropertyStatus(void)
{
	if(item(TYPE_PROPERTY, VALUE)->text() == "CHANGE_VALUE")
	{ 
		UpdateEqualityExpr(ARG2_PROPERTY);
		return true;
	}
	return false;
}



void EventTableWidget::CHANGE_VALUE_DropEvent(QTableWidgetItem* _it, const QModelIndex& _index, const QString& _suffix, QString& _fileName)
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
			else if(_index.row() == SOURCE_PROPERTY)
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
				else if(item(ARG1_PROPERTY, VALUE)->text() == "STRING")
				{
					StringExprDropEvent(_it, _index, _suffix, _fileName);
				}
				else if(item(ARG1_PROPERTY, VALUE)->text() == "BOOLEAN")
				{
					BooleanExprDropEvent(_it, _index, _suffix, _fileName);
				}
			}
		}
		else if(item(_index.row(), ARG_MODE)->text() == "EQUALITY")
		{ 
			if(_index.row() == ARG2_PROPERTY)
			{
				EqualityExprDropEvent(_it, _index, _suffix, _fileName);
			}
		}
		else if(item(_index.row(), ARG_MODE)->text() == "TEMPLATE")
		{
			StringExprDropEvent(_it, _index, _suffix, _fileName);
		}
	}
}



void EventCreator::CHANGE_VALUE_SetEvent(int _eventType)
{
	if(_eventType == AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE)
	{
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "CHANGE_VALUE");

		ChangeIntegerValueEvent* derived = dynamic_cast<ChangeIntegerValueEvent*>(event);
		
		if(derived)
		{
			eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, "INTEGER");

			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, QString::number(derived->GetIntegerKeyArg(AbstractEvent::ARG1)));
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EXPRESSION, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EXPRESSION");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetExpressionArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EQUALITY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EQUALITY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetEqualityArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
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
	else if(_eventType == AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE)
	{
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "CHANGE_VALUE");

		ChangeFloatValueEvent* derived = dynamic_cast<ChangeFloatValueEvent*>(event);
		
		if(derived)
		{
			eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, "FLOAT");

			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, QString::number(derived->GetFloatKeyArg(AbstractEvent::ARG1)));
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EXPRESSION, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EXPRESSION");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetExpressionArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EQUALITY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EQUALITY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetEqualityArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
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
	else if(_eventType == AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE)
	{
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "CHANGE_VALUE");

		ChangeBooleanValueEvent* derived = dynamic_cast<ChangeBooleanValueEvent*>(event);
		
		if(derived)
		{
			eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, "BOOLEAN");

			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetBooleanKeyArg(AbstractEvent::ARG1) ? "true" : "false");
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EXPRESSION, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EXPRESSION");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetExpressionArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EQUALITY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EQUALITY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetEqualityArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
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
	else if(_eventType == AbstractVariable::CHANGE_STRING_VALUE_MESSAGE)
	{ 
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "CHANGE_VALUE");

		ChangeStringValueEvent* derived = dynamic_cast<ChangeStringValueEvent*>(event);
		
		if(derived)
		{
			eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, "STRING");

			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetStringKeyArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EXPRESSION, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EXPRESSION");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetExpressionArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EQUALITY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EQUALITY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetEqualityArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
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
	else if(_eventType == AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE)
	{ 
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "CHANGE_VALUE");

		ChangeVector2ValueEvent* derived = dynamic_cast<ChangeVector2ValueEvent*>(event);
		
		if(derived)
		{
			eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, "VECTOR_2");

			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, Vector2ToString(derived->GetVector2KeyArg(AbstractEvent::ARG1)));
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EXPRESSION, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EXPRESSION");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetExpressionArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EQUALITY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EQUALITY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetEqualityArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
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
	else if(_eventType == AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE)
	{ 
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "CHANGE_VALUE");

		ChangeVector3ValueEvent* derived = dynamic_cast<ChangeVector3ValueEvent*>(event);
		
		if(derived)
		{
			eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, "VECTOR_3");

			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, Vector3ToString(derived->GetVector3KeyArg(AbstractEvent::ARG1)));
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EXPRESSION, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EXPRESSION");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetExpressionArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EQUALITY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EQUALITY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetEqualityArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
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
	else if(_eventType == AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE)
	{
		eventTable->SetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE, "CHANGE_VALUE");

		ChangeQuaternionValueEvent* derived = dynamic_cast<ChangeQuaternionValueEvent*>(event);
		
		if(derived)
		{
			eventTable->SetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE, "QUATERNION");

			if(derived->IsArgsEnabled())
			{
				if(derived->GetArgState(AbstractEvent::KEY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "KEY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, QuaternionToString(derived->GetQuaternionKeyArg(AbstractEvent::ARG1)));
				}
				else if(derived->GetArgState(AbstractEvent::VARIABLE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "VARIABLE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetVariableArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EXPRESSION, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EXPRESSION");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetExpressionArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::EQUALITY, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "EQUALITY");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetEqualityArg(AbstractEvent::ARG1).c_str());
				}
				else if(derived->GetArgState(AbstractEvent::TEMPLATE, AbstractEvent::ARG1))
				{
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE, "TEMPLATE");
					eventTable->SetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE, derived->GetTemplateArg(AbstractEvent::ARG1).c_str());
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


void EventCreator::CHANGE_VALUE_Create(void)
{
	if(eventTable->GetCellText(EventTableWidget::TYPE_PROPERTY, EventTableWidget::VALUE) == "CHANGE_VALUE")
	{
		if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE) == "INTEGER")
		{
			event = Trigger::_CreateEvent(AbstractVariable::CHANGE_INTEGER_VALUE_MESSAGE);

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
				if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetArgsEnable(false);
				}
				else
				{
					if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
					{
						event->SetIntegerKeyArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toInt());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
					{
						event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
						if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
						{
							int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

							event->SetArgAssetType(AbstractEvent::ARG1, assetType);
						}
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EXPRESSION")
					{
						event->SetExpressionArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EQUALITY")
					{
						event->SetEqualityArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
					{
						event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
				}
			}
		}
		else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE) == "FLOAT")
		{
			event = Trigger::_CreateEvent(AbstractVariable::CHANGE_FLOAT_VALUE_MESSAGE);

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
				if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetArgsEnable(false);
				}
				else
				{
					if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
					{
						event->SetFloatKeyArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toFloat());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
					{
						event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
						if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
						{
							int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

							event->SetArgAssetType(AbstractEvent::ARG1, assetType);
						}
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EXPRESSION")
					{
						event->SetExpressionArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EQUALITY")
					{
						event->SetEqualityArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
					{
						event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
				}
			}
		}
		else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE) == "BOOLEAN")
		{
			event = Trigger::_CreateEvent(AbstractVariable::CHANGE_BOOLEAN_VALUE_MESSAGE);

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
				if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetArgsEnable(false);
				}
				else
				{
					if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
					{
						event->SetBooleanKeyArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE) == "true" ? true : false);
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
					{
						event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
						if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
						{
							int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

							event->SetArgAssetType(AbstractEvent::ARG1, assetType);
						}
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EXPRESSION")
					{
						event->SetExpressionArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EQUALITY")
					{
						event->SetEqualityArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
					{
						event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
				}
			}
		}
		else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE) == "STRING")
		{
			event = Trigger::_CreateEvent(AbstractVariable::CHANGE_STRING_VALUE_MESSAGE);

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
				if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetArgsEnable(false);
				}
				else
				{
					if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
					{
						event->SetStringKeyArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
					{
						event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
						if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
						{
							int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

							event->SetArgAssetType(AbstractEvent::ARG1, assetType);
						}
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EXPRESSION")
					{
						event->SetExpressionArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EQUALITY")
					{
						event->SetEqualityArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
					{
						event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
				}
			}
		}
		else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE) == "VECTOR_2")
		{
			event = Trigger::_CreateEvent(AbstractVariable::CHANGE_VECTOR_2_VALUE_MESSAGE);

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
				if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetArgsEnable(false);
				}
				else
				{
					if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
					{
						event->SetVector2KeyArg(AbstractEvent::ARG1, StringToVector2(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE)));
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
					{
						event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
						if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
						{
							int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

							event->SetArgAssetType(AbstractEvent::ARG1, assetType);
						}
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EXPRESSION")
					{
						event->SetExpressionArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EQUALITY")
					{
						event->SetEqualityArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
					{
						event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
				}
			}
		}
		else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE) == "VECTOR_3")
		{
			event = Trigger::_CreateEvent(AbstractVariable::CHANGE_VECTOR_3_VALUE_MESSAGE);

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
				if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetArgsEnable(false);
				}
				else
				{
					if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
					{
						event->SetVector3KeyArg(AbstractEvent::ARG1, StringToVector3(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE)));
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
					{
						event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
						if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
						{
							int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

							event->SetArgAssetType(AbstractEvent::ARG1, assetType);
						}
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EXPRESSION")
					{
						event->SetExpressionArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EQUALITY")
					{
						event->SetEqualityArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
					{
						event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
				}
			}
		}
		else if(eventTable->GetCellText(EventTableWidget::ARG1_PROPERTY, EventTableWidget::VALUE) == "QUATERNION")
		{
			event = Trigger::_CreateEvent(AbstractVariable::CHANGE_QUATERNION_VALUE_MESSAGE);

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
				if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).isEmpty())
				{
					event->SetArgsEnable(false);
				}
				else
				{
					if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "KEY")
					{
						event->SetQuaternionKeyArg(AbstractEvent::ARG1, StringToQuaternion(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE)));
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "VARIABLE")
					{
						event->SetVariableArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
						if(event->GetArgState(AbstractEvent::ASSET_TYPE, AbstractEvent::ARG1) == AssetLibrary::UNKNOWN_ASSET)
						{
							int32 assetType = AssetLibrary::_IsAssetExist(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());

							event->SetArgAssetType(AbstractEvent::ARG1, assetType);
						}
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EXPRESSION")
					{
						event->SetExpressionArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "EQUALITY")
					{
						event->SetEqualityArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
					else if(eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::ARG_MODE) == "TEMPLATE")
					{
						event->SetTemplateArg(AbstractEvent::ARG1, eventTable->GetCellText(EventTableWidget::ARG2_PROPERTY, EventTableWidget::VALUE).toStdString());
					}
				}
			}
		}
	} 
}
