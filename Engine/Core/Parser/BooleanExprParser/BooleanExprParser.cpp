#include "BooleanExprParser.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


BooleanExprParser::BooleanExprParser(void): amountOfUniqueVariables(0)
{
	currentState = START_STATE;
	currentError = NO_ERRORS;
}


BooleanExprParser::~BooleanExprParser(void)
{
	Destroy();
}



SimpleList<BooleanExprParser::Token> BooleanExprParser::GetTokens(void)const
{
	return tokens;
}



int32 BooleanExprParser::PriorityOf(char _operation)
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



BooleanExprParser::ErrorType BooleanExprParser::GetCurrentError(void)const
{
	return currentError;
}



StringANSI BooleanExprParser::GetCurrentErrorString(void)const
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


StringANSI BooleanExprParser::GetErrorArg(void)const
{
	return errorArg;
}


int32 BooleanExprParser::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}


StringANSI BooleanExprParser::GetBooleanExpr(void)const
{
	return booleanExpr;
}



BooleanExprParser::ErrorType BooleanExprParser::Parse(StringANSI _booleanExpr)
{ 

	currentState = START_STATE;
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
	booleanExpr = _booleanExpr;
	tokens.Clear();
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
	return currentError;
}


bool BooleanExprParser::GetResult(void)const
{
	return result;
}


bool BooleanExprParser::IsVariable(StringANSI _name)const
{
	return variables.Find(_name) ? true : false;
}



int32 BooleanExprParser::GetAmountOfVariables(void)const
{
	return variables.GetSize();
}


int32 BooleanExprParser::GetAmountOfUniqueVariables(void)const
{
	return amountOfUniqueVariables;
}


SimpleList<StringANSI>& BooleanExprParser::GetVariables(void)
{
	return variables;
}


BooleanExprParser::ErrorType BooleanExprParser::Calculate(void)
{
	if(tokens.IsNotEmpty())
	{
		SimpleStack<AbstractVariable*> result;
		MagicList<AbstractVariable> deletionList;
		AbstractVariable* leftOperand;
		AbstractVariable* rightOperand;
		AbstractVariable* it;
		for(SimpleList<Token>::Node* token = tokens.GetHead(); token != NIL; token = token->GetRight())
		{
			if(token->key.type == Token::VARIABLE)
			{
				it = dynamic_cast<AbstractVariable*>(AssetLibrary::_GetAssets().FindObject(token->key.value));
				 
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
			BooleanExprParser::result = dynamic_cast<Variable<bool>*>(it)->GetValue();
			currentError = NO_ERRORS;
		}
	}
	return currentError;
} 


AbstractVariable* BooleanExprParser::Calculate(OperationType _operation, AbstractVariable* _right)
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


AbstractVariable* BooleanExprParser::Calculate(OperationType _operation, AbstractVariable* _left, AbstractVariable* _right)
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



void BooleanExprParser::Destroy(void)
{
	currentState = START_STATE;
	variables.Clear();
	tokens.Clear();
	amountOfUniqueVariables = 0;
	booleanExpr.clear();
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
}


int32 BooleanExprParser::_IsNameValid(StringANSI _name)
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


bool BooleanExprParser::SaveToFile(File& _file)
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


bool BooleanExprParser::LoadFromFile(File& _file)
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