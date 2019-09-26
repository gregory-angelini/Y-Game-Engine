#include "BooleanExprParserEx.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


BooleanExprParserEx::BooleanExprParserEx(void): amountOfUniqueVariables(0)
{
	currentState = START_STATE;
	currentError = NO_ERRORS;
}


BooleanExprParserEx::~BooleanExprParserEx(void)
{
	Destroying();
	Destroy();
}



SimpleList<BooleanExprParserEx::Token> BooleanExprParserEx::GetTokens(void)const
{
	return tokens;
}



int32 BooleanExprParserEx::PriorityOf(char _operation)
{
	switch(_operation)
	{
		case '|':
		{
			return 0;
		}
			
		case '^':
		{
			return 1;
		}
			
		case '&':
		{
			return 2;
		}
			
		case '~':
		{
			return 3;
		}
	}
	return -1;
}



BooleanExprParserEx::ErrorType BooleanExprParserEx::GetCurrentError(void)const
{
	return currentError;
}



StringANSI BooleanExprParserEx::GetCurrentErrorString(void)const
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

		case RIGHT_BRACKET_NOT_FOUND:
		{
			return "RIGHT_BRACKET_NOT_FOUND";
		}

		case LEFT_BRACKET_NOT_FOUND:
		{
			return "LEFT_BRACKET_NOT_FOUND";
		}

		case INVALID_OPERATION:
		{
			return "INVALID_OPERATION";
		}

		case INVALID_RESULT:
		{
			return "INVALID_RESULT";
		}
	}
	return "";
}


StringANSI BooleanExprParserEx::GetErrorArg(void)const
{
	return errorArg;
}


int32 BooleanExprParserEx::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}


StringANSI BooleanExprParserEx::GetBooleanExpr(void)const
{
	return booleanExpr;
}

 

void BooleanExprParserEx::DisconnectVariables(void)
{
	MagicList<AbstractVariable>::AddressDerived* it = definedVariables.GetHeadAddress();

	while(it)
	{
		it->GetNode()->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
		it->GetNode()->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
		it = it->GetRight();
	}

	definedVariables.Clear();
}



void BooleanExprParserEx::VariableIsDestroyed(AbstractObject* _object)
{
	undefinedVariables.AddToTail(_object->GetName());

	currentError = UNDEFINED_VARIABLE;
	errorArg = _object->GetName();
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &BooleanExprParserEx::VariableIsCreated));
}



void BooleanExprParserEx::VariableIsRenamed(AbstractObject::NameInfo* _info)
{
	AbstractObject* object = _info->object;

	object->Disconnect(AbstractObject::DESTROY_OBJECT_MESSAGE, this);
	object->Disconnect(AbstractObject::CHANGE_NAME_MESSAGE, this);
 
	definedVariables.Remove(dynamic_cast<AbstractVariable*>(object));
	undefinedVariables.AddToTail(object->GetName());

	currentError = UNDEFINED_VARIABLE;
	errorArg = object->GetName();
	AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &BooleanExprParserEx::VariableIsCreated));
}




void BooleanExprParserEx::VariableIsCreated(AbstractObject* _object)
{
	if(IsUndefinedVariable(_object->GetName()))
	{
		if(_object->IsClassPropertyExist(AbstractVariable::ABSTRACT_VARIABLE_CLASS))
		{
			_object->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &BooleanExprParserEx::VariableIsDestroyed));
			_object->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &BooleanExprParserEx::VariableIsRenamed));
			
			definedVariables.AddToTail(dynamic_cast<AbstractVariable*>(_object));
			undefinedVariables.Remove(_object->GetName());

			if(IsAllVariablesExist())
			{
				AssetLibrary::_GetAssets().Disconnect(AssetLibrary::CREATE_OBJECT_MESSAGE, this);
			}
		}
	}
}


bool BooleanExprParserEx::IsUndefinedVariable(StringANSI _name)const
{
	return undefinedVariables.Find(_name) ? true : false;
}



bool BooleanExprParserEx::IsAllVariablesExist(void)const
{
	return undefinedVariables.IsEmpty();
}



BooleanExprParserEx::ErrorType BooleanExprParserEx::Parse(StringANSI _booleanExpr)
{ 

	currentState = START_STATE;
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
	tokens.Clear();
	booleanExpr = _booleanExpr;
	undefinedVariables.Clear();
	DisconnectVariables();
	variables.Clear();
	amountOfUniqueVariables = 0;

	if(_booleanExpr.length() > 0)
	{
		SimpleStack<Operation> stack;
		int32 lostBrackets = 0;
		char lastOperation = ' ';
		int32 lastOperationIndex = -1;
		char character;
		for(int32 i = 0; i < _booleanExpr.length(); ++i)
		{
			character = _booleanExpr[i];

			if(character != ' ')
			{ 
				switch(character)
				{
					case '(':
					{
						if(currentState == VARIABLE_RECEIVED_STATE)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						} 
						lostBrackets++;
						stack.Push(Operation(StringANSI(1, character), i));
						break;
					}

					case ')':
					{
						if(currentState == OPERATION_RECEIVED_STATE)
						{	
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						else if(lostBrackets == 0)
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = '(';
							return currentError = LEFT_BRACKET_NOT_FOUND;
						} 
						lostBrackets--;
						
						Operation operation = stack.Pop();

						while(operation.value[0] != '(')
						{
							tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							operation = stack.Pop();
						} 
						break;
					}

					case '~':
					{
						if(currentState == START_STATE)
						{ 
							currentState = OPERATION_RECEIVED_STATE;
							lastOperation = character;
							lastOperationIndex = i;

							stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						}
						else if(currentState == OPERATION_RECEIVED_STATE)
						{ 
							if(stack.Peek().value[0] == character)
							{
								tokens.Clear();
								currentState = ERROR_STATE;
								errorArg = character;
								errorArgIndex = i;
								return currentError = UNKNOWN_EXPR;
							}
							lastOperation = character;
							lastOperationIndex = i;

							while(stack.IsNotEmpty() && PriorityOf(lastOperation) < PriorityOf(stack.Peek().value[0]))
							{
								Operation operation = stack.Pop();
								tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
							} 
							stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						} 
						else
						{
							tokens.Clear();
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						break;
					}

					case '|':
					case '&':
					case '^':
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
						currentState = OPERATION_RECEIVED_STATE;
						lastOperation = character;
						lastOperationIndex = i;

						while(stack.IsNotEmpty() && PriorityOf(lastOperation) <= PriorityOf(stack.Peek().value[0]))
						{
							Operation operation = stack.Pop();
							tokens.AddToTail(Token(operation.value, Token::OPERATION, i));
						} 
						stack.Push(Operation(StringANSI(1, lastOperation), lastOperationIndex));
						break;
					}

					default:
					{
						if(character >= 65 && character <= 90 ||
						   character >= 97 && character <= 122 ||
						   character >= 48 && character <= 57 ||
						   character == '_')
						{
							if(currentState == VARIABLE_RECEIVED_STATE)
							{
								if(_booleanExpr[i - 1] == ' ' ||
								   _booleanExpr[i - 1] == ')')
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
								currentState = VARIABLE_RECEIVED_STATE;
								tokens.AddToTail(Token(StringANSI(1, character), Token::VARIABLE, i));
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

		if(lostBrackets)
		{
			tokens.Clear();
			currentState = ERROR_STATE;
			errorArg = ')';
			return currentError = RIGHT_BRACKET_NOT_FOUND;
		}
	} 
	else
	{
		currentState = ERROR_STATE;
		return currentError = EXPR_NOT_FOUND;
	}
	for(SimpleList<Token>::Node* it = tokens.GetHead(); it != NIL; it = it->GetRight())
	{
		if(it->key.type == Token::VARIABLE)
		{
			if(variables.Find(it->key.value) == NIL)
			{
				amountOfUniqueVariables++;
			}
			variables.AddToTail(it->key.value);
		}
	}
	currentState = END_STATE;
	for(SimpleList<Token>::Node* token = tokens.GetHead(); token != NIL; token = token->GetRight())
	{
		if(token->key.type == Token::VARIABLE)
		{ 
			AbstractVariable* it = dynamic_cast<AbstractVariable*>(AssetLibrary::_GetAssets().FindObject(token->key.value));
			
			if(it)
			{
				it->Connect(AbstractObject::DESTROY_OBJECT_MESSAGE, this, Caller<>(this, &BooleanExprParserEx::VariableIsDestroyed));
				it->Connect(AbstractObject::CHANGE_NAME_MESSAGE, this, Caller<>(this, &BooleanExprParserEx::VariableIsRenamed));
				definedVariables.AddToTail(it);
			} 
			else
			{
				if(undefinedVariables.IsEmpty())
				{
					AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &BooleanExprParserEx::VariableIsCreated));
				}
				undefinedVariables.AddToTail(token->key.value);
			}
		}
	}

	return currentError;
}


bool BooleanExprParserEx::GetResult(void)const
{
	return result;
}


bool BooleanExprParserEx::IsVariable(StringANSI _name)const
{
	return variables.Find(_name) ? true : false;
}



int32 BooleanExprParserEx::GetAmountOfVariables(void)const
{
	return variables.GetSize();
}


int32 BooleanExprParserEx::GetAmountOfUniqueVariables(void)const
{
	return amountOfUniqueVariables;
}


SimpleList<StringANSI>& BooleanExprParserEx::GetVariables(void)
{
	return variables;
}


BooleanExprParserEx::ErrorType BooleanExprParserEx::Calculate(void)
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
			else if(token->key.type == Token::OPERATION)
			{
				char operation = token->key.value[0];
				
				if(operation == '~')
				{
					rightOperand = result.Pop();
					
					if(it = Calculate((OperationType)operation, rightOperand))
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
				else
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
		
		if(it->GetValueType() != Variable<bool>::BOOLEAN_TYPE)
		{
			return currentError = INVALID_RESULT;
		}
		else
		{
			BooleanExprParserEx::result = dynamic_cast<Variable<bool>*>(it)->GetValue();
			currentError = NO_ERRORS;
		}
	}
	return currentError;
} 


AbstractVariable* BooleanExprParserEx::Calculate(OperationType _operation, AbstractVariable* _right)
{
	switch(_operation)
	{
		case NOT:
		{		
			if(_right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(!dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}
	}
	return NIL;
}


AbstractVariable* BooleanExprParserEx::Calculate(OperationType _operation, AbstractVariable* _left, AbstractVariable* _right)
{
	switch(_operation)
	{				
		case OR:
		{
			if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() | dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}
						
		case AND:
		{
			if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() & dynamic_cast<Variable<bool>*>(_right)->GetValue());
			}
			break;
		}

		case XOR:
		{
			if(_left->GetValueType() == Variable<bool>::BOOLEAN_TYPE && _right->GetValueType() == Variable<bool>::BOOLEAN_TYPE)
			{
				return new Variable<bool>(dynamic_cast<Variable<bool>*>(_left)->GetValue() ^ dynamic_cast<Variable<bool>*>(_right)->GetValue());			
			}
			break;
		}
	}
	return NIL;
}



void BooleanExprParserEx::Destroy(void)
{
	currentState = START_STATE;
	variables.Clear();
	tokens.Clear();
	amountOfUniqueVariables = 0;
	booleanExpr.clear();
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;

	undefinedVariables.Clear();
	DisconnectVariables();
}


int32 BooleanExprParserEx::_IsNameValid(StringANSI _name)
{
	if(!_name.empty())
	{
		for(char character, i = 0; i < _name.length(); ++i)
		{
			 character = _name[i];

			if(character >= 48 && character <= 57 ||
			   character >= 65 && character <= 90 ||
			   character >= 97 && character <= 122 ||
			   character == '_')
			{}
			else
			{ 
				return i; 
			}
		}
	}
	return -1;
}


bool BooleanExprParserEx::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(booleanExpr.length());
		_file.WriteString(booleanExpr);
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


bool BooleanExprParserEx::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		Destroy();
		int32 length;
		_file.Read(length);
		_file.ReadString(booleanExpr, length);
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

			if(type == Token::VARIABLE)
			{
				if(variables.Find(value) == NIL)
				{
					amountOfUniqueVariables++;
				}
				variables.AddToTail(value);
			}
		}
		return true;
	}
	return false;
}