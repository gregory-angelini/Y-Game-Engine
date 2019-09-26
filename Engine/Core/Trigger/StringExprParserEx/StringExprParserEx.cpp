#include "StringExprParserEx.h"
#include <Engine/Core/Container/Stack/SimpleStack/SimpleStack.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>


#ifdef SendMessage
   #undef SendMessage
#endif


StringExprParserEx::StringExprParserEx(void)
{
	currentState = START_STATE;
	currentError = NO_ERRORS;
}


StringExprParserEx::~StringExprParserEx(void)
{
	Destroying();
	Destroy();
}


int32 StringExprParserEx::PriorityOf(char _operation)
{
	switch(_operation)
	{
	    case '+':
		{
			return 1;
		}
	}
	return -1;
}



SimpleList<StringExprParserEx::Token> StringExprParserEx::GetTokens(void)const
{
	return tokens;
}


StringANSI StringExprParserEx::GetStringExpr(void)const
{
	return stringExpr;
}


StringExprParserEx::ErrorType StringExprParserEx::GetCurrentError(void)const
{
	return currentError;
}

			
StringANSI StringExprParserEx::GetCurrentErrorString(void)const
{
	switch(currentError)
	{
		case NO_ERRORS:
		{
			return "NO_ERRORS";
		}

		case UNKNOWN_TOKEN:
		{
			return "UNKNOWN_TOKEN";
		}

		case EXPR_NOT_FOUND:
		{
			return "EXPR_NOT_FOUND";
		}

		case UNDEFINED_VARIABLE:
		{
			return "UNDEFINED_VARIABLE";
		}

		case UNKNOWN_EXPR:
		{
			return "UNKNOWN_EXPR";
		}

		case INVALID_OPERATION:
		{
			return "INVALID_OPERATION";
		}

		case VERY_LONG_NUMBER:
		{
			return "VERY_LONG_NUMBER";
		}

		case INVALID_RESULT:
		{
			return "INVALID_RESULT";
		}
	}
	return "";
}



StringANSI StringExprParserEx::GetErrorArg(void)const
{
	return errorArg;
}



int32 StringExprParserEx::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}


void StringExprParserEx::DisconnectVariables(void)
{
	MagicList<AbstractVariable>::AddressDerived* it = definedVariables.GetHeadAddress();

	while(it)
	{
		it->GetNode()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		it->GetNode()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		it->GetNode()->Disconnect(AbstractVariable::CHANGE_VALUE_MESSAGE, this);
		it = it->GetRight();
	}

	definedVariables.Clear();
}

 
void StringExprParserEx::VariableIsChanged(AbstractVariable* _variable)
{
	if(IsReceiversExist(STRING_EXPR_CHANGE_VALUE_MESSAGE)) { SendMessage(StringExprParserEx::STRING_EXPR_CHANGE_VALUE_MESSAGE); }
}



void StringExprParserEx::VariableIsDestroyed(AbstractObject* _object)
{
	undefinedVariables.AddToTail(_object->GetName());

	currentError = UNDEFINED_VARIABLE;
	errorArg = _object->GetName();
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsCreated));

	if(IsReceiversExist(STRING_EXPR_VARIABLE_LOSS_MESSAGE)) { SendMessage(STRING_EXPR_VARIABLE_LOSS_MESSAGE); }
}



void StringExprParserEx::VariableIsRenamed(AbstractObject::NameInfo* _info)
{
	AbstractObject* object = _info->object;

	object->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
	object->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
	object->Disconnect(AbstractVariable::CHANGE_VALUE_MESSAGE, this);

	definedVariables.Remove(dynamic_cast<AbstractVariable*>(object));
	undefinedVariables.AddToTail(object->GetName());

	currentError = UNDEFINED_VARIABLE;
	errorArg = object->GetName();
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsCreated));

	if(IsReceiversExist(STRING_EXPR_VARIABLE_LOSS_MESSAGE)) { SendMessage(STRING_EXPR_VARIABLE_LOSS_MESSAGE); }
}




void StringExprParserEx::VariableIsCreated(AbstractObject* _object)
{
	if(IsUndefinedVariable(_object->GetName()))
	{
		if(_object->IsClassPropertyExist(AbstractVariable::ABSTRACT_VARIABLE_CLASS))
		{
			_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsDestroyed));
			_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsRenamed));
			_object->Connect(AbstractVariable::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsChanged));
			
			definedVariables.AddToTail(dynamic_cast<AbstractVariable*>(_object));
			undefinedVariables.Remove(_object->GetName());

			if(IsAllVariablesExist())
			{
				AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
					
				if(IsReceiversExist(STRING_EXPR_COMPLETED_MESSAGE)) { SendMessage(STRING_EXPR_COMPLETED_MESSAGE); }
			}
		}
	}
}


bool StringExprParserEx::IsUndefinedVariable(StringANSI _name)const
{
	return undefinedVariables.Find(_name) ? true : false;
}
 

bool StringExprParserEx::IsAllVariablesExist(void)const
{
	return undefinedVariables.IsEmpty();
}



StringExprParserEx::ErrorType StringExprParserEx::Parse(StringANSI _stringExpr)
{ 

	currentState = START_STATE;
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
	tokens.Clear();
	stringExpr = _stringExpr;
	result.clear();
	undefinedVariables.Clear();
	DisconnectVariables();
  
	if(_stringExpr.length() > 0)
	{
		SimpleStack<Operation> stack;
		int32 lostBrackets = 0;
		char lastOperation = ' ';
		int32 lastOperationIndex = -1;
		char character;
		for(int32 i = 0; i < _stringExpr.length(); ++i)
		{
			character = _stringExpr[i];

			if(character != ' ')
			{ 
				switch(character)
				{
					case '$':
					{
						if(currentState == VARIABLE_RECEIVED_STATE ||
						   lastOperation == '$')
						{	 
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						currentState = OPERATION_RECEIVED_STATE;
						lastOperation = character;
						lastOperationIndex = i;
						break;
					}

					case '+':
					{
						if(currentState == START_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{	 
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						else
						{
							currentState = OPERATION_RECEIVED_STATE;
						}
						lastOperation = character;
						lastOperationIndex = i;

						while(stack.IsNotEmpty() && PriorityOf(lastOperation) <= PriorityOf(stack.Peek().value[0]))
						{
							Operation operation = stack.Pop();
							tokens.AddToTail(Token(operation.value, Token::OPERATION, operation.position));
						} 
						stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						break;
					}
					
					default:
					{
						if(character >= 65 && character <= 90 ||
						   character >= 97 && character <= 122 ||
						   character == '_')
						{
							if(currentState == VARIABLE_RECEIVED_STATE)
							{
								if(_stringExpr[i - 1] == ' ')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else if(currentState == NUMBER_RECEIVED_STATE)
							{ 
								if(_stringExpr[i - 1] == ' ')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character; 
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								currentState = VARIABLE_RECEIVED_STATE;
								tokens.GetTail()->key.type = Token::VARIABLE;
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else if(currentState == STRING_RECEIVED_STATE)
							{ 
								if(_stringExpr[i - 1] == ' ')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character; 
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else
							{
								if(lastOperation == '$')
								{
									currentState = STRING_RECEIVED_STATE;
									tokens.AddToTail(Token(StringANSI(1, character), Token::STRING, i));
								}
								else
								{
									currentState = VARIABLE_RECEIVED_STATE;
									tokens.AddToTail(Token(StringANSI(1, character), Token::VARIABLE, i));
								}
							}
						}
						else if(character >= 48 && character <= 57)
						{
							if(currentState == VARIABLE_RECEIVED_STATE)
							{
								if(_stringExpr[i - 1] == ' ')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else if(currentState == NUMBER_RECEIVED_STATE)
							{
								if(_stringExpr[i - 1] == ' ')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								if(tokens.GetTail()->key.value.length() > 7)
								{
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									tokens.Clear();
									return currentError = VERY_LONG_NUMBER;
								}
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else if(currentState == STRING_RECEIVED_STATE)
							{
								if(_stringExpr[i - 1] == ' ')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								tokens.GetTail()->key.value += StringANSI(1, character);
							}
							else
							{ 
								if(lastOperation == '$')
								{
									tokens.Clear();
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								else
								{
									currentState = NUMBER_RECEIVED_STATE;
									tokens.AddToTail(Token(StringANSI(1, character), Token::NUMBER, i));
								}
							}
						}
						else
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_TOKEN;
						}
						break;
					}
				}
			}
		}
		while(stack.IsNotEmpty())
		{ 
			Operation operation = stack.Pop();
			tokens.AddToTail(Token(operation.value, Token::OPERATION, operation.position));
		}
			
		if(tokens.IsEmpty())
		{
			currentState = ERROR_STATE;
			return currentError = EXPR_NOT_FOUND;
		}

		if(currentState == OPERATION_RECEIVED_STATE)
		{
			tokens.Clear();
			currentState = ERROR_STATE;
			errorArg = lastOperation;
			errorArgIndex = lastOperationIndex;
			return currentError = UNKNOWN_EXPR;
		}
	} 
	else
	{
		currentState = ERROR_STATE;
		return currentError = EXPR_NOT_FOUND;
	}
	currentState = END_STATE;
	for(SimpleList<Token>::Node* token = tokens.GetHead(); token != NIL; token = token->GetRight())
	{
		if(token->key.type == Token::VARIABLE)
		{ 
			AbstractVariable* it = dynamic_cast<AbstractVariable*>(AssetLibrary::_GetAssets().FindObject(token->key.value));
				 
			if(it)
			{
				it->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsDestroyed));
				it->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsRenamed));
				it->Connect(AbstractVariable::CHANGE_VALUE_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsChanged));
				
				definedVariables.AddToTail(it);
			} 
			else
			{
				if(undefinedVariables.IsEmpty())
				{
					AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &StringExprParserEx::VariableIsCreated));
				}
				undefinedVariables.AddToTail(token->key.value);
			}
		}
	}
	
	return currentError;
}




StringANSI StringExprParserEx::GetResult(void)const
{
	return result;
}
	

StringExprParserEx::ErrorType StringExprParserEx::Calculate(void)
{
	if(tokens.IsNotEmpty())
	{
		if(undefinedVariables.IsNotEmpty())
		{
			errorArg = undefinedVariables.GetHead()->key;
			return currentError = UNDEFINED_VARIABLE;
		}

		SimpleStack<AbstractVariable*> result;
		MagicList<AbstractVariable> deletionList;
		AbstractVariable* leftOperand;
		AbstractVariable* rightOperand;
		AbstractVariable* it;
		for(SimpleList<Token>::Node* token = tokens.GetHead(); token != NIL; token = token->GetRight())
		{
			if(token->key.type == Token::VARIABLE)
			{ 
				it = definedVariables.Find(token->key.value, &AbstractObject::GetName);
				 
				if(it)
				{
					result.Push(it);
				}
				else
				{
					errorArg = token->key.value;
					return currentError = UNDEFINED_VARIABLE;
				}
			}
			else if(token->key.type == Token::NUMBER)
			{ 
				it = new Variable<int32>(StringToInt(token->key.value));
				deletionList.AddToTail(it);
				result.Push(it);
			}
			else if(token->key.type == Token::STRING)
			{ 
				it = new Variable<StringANSI>(token->key.value);
				deletionList.AddToTail(it);
				result.Push(it);
			}
			else if(token->key.type == Token::OPERATION)
			{
				char operation = token->key.value[0];
				{
					rightOperand = result.Pop();
					leftOperand = result.Pop();

					if(it = Calculate((OperationType)operation, leftOperand, rightOperand))
					{
						deletionList.AddToTail(it);
						result.Push(it);
					}
					else
					{ 
						errorArg = token->key.value;
						return currentError = INVALID_OPERATION;
					}
				}
			}
		} 
		it = result.Pop();
		
		if(it->GetValueType() != Variable<StringANSI>::STRING_TYPE)
		{
			StringExprParserEx::result.clear();
			return currentError = INVALID_RESULT;
		}
		else
		{
			StringExprParserEx::result = dynamic_cast<Variable<StringANSI>*>(it)->GetValue();
			currentError = NO_ERRORS;
		}
	}
	return currentError;
}



AbstractVariable* StringExprParserEx::Calculate(OperationType _operation, AbstractVariable* _left, AbstractVariable* _right)
{
	switch(_operation)
	{			
		case PLUS:
		{
			if(_left->GetValueType() == Variable<StringANSI>::STRING_TYPE)
			{
				if(_right->GetValueType() == Variable<StringANSI>::STRING_TYPE)
				{
					return new Variable<StringANSI>(dynamic_cast<Variable<StringANSI>*>(_left)->GetValue() + dynamic_cast<Variable<StringANSI>*>(_right)->GetValue());			
				}
				else if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<StringANSI>(dynamic_cast<Variable<StringANSI>*>(_left)->GetValue() + NumberToString(dynamic_cast<Variable<int32>*>(_right)->GetValue()));			
				}
			}
			else if(_left->GetValueType() == Variable<int32>::INTEGER_TYPE)
			{
				if(_right->GetValueType() == Variable<int32>::INTEGER_TYPE)
				{
					return new Variable<int32>(dynamic_cast<Variable<int32>*>(_left)->GetValue() + dynamic_cast<Variable<int32>*>(_right)->GetValue());			
				}
			}
			break;
		}
	}
	return NIL;
}




void StringExprParserEx::Destroy(void)
{
	currentState = START_STATE;
	tokens.Clear();
	stringExpr.clear();
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
	result.clear(); 

	undefinedVariables.Clear();
	DisconnectVariables();
}



bool StringExprParserEx::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(stringExpr.length());
		_file.WriteString(stringExpr);
		_file.Write(currentState);
		_file.Write(currentError);
		_file.Write(errorArg.length());
		_file.WriteString(errorArg);
		_file.Write(errorArgIndex);
		_file.Write(tokens.GetSize());
		for(SimpleList<Token>::Node* it = tokens.GetHead(); it != NIL; it = it->GetRight())
		{
			_file.Write(it->key.value.length());
			_file.WriteString(it->key.value);
			_file.Write(it->key.type);
			_file.Write(it->key.position);
		}
		return true;
	}
	return false;
}


bool StringExprParserEx::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		Destroy();
		int32 length;
		_file.Read(length);
		_file.ReadString(stringExpr, length);
		_file.Read(currentState);
		_file.Read(currentError);
		_file.Read(length);
		_file.ReadString(errorArg, length);
		_file.Read(errorArgIndex);
		StringANSI value;
		Token::TokenType type;
		int32 position;
		int32 count;
		_file.Read(count);
		for(int32 i = 0; i < count; ++i)
		{
			_file.Read(length);
			_file.ReadString(value, length);
			_file.Read(type);
			_file.Read(position);

			tokens.AddToTail(Token(value, type, position));
		}
		return true;
	}
	return false;
}