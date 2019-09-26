#include "StringExprParser.h"
#include <Engine/Core/Container/Stack/SimpleStack/SimpleStack.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>


StringExprParser::StringExprParser(void)
{
	currentState = START_STATE;
	currentError = NO_ERRORS;
}


StringExprParser::~StringExprParser(void)
{
	Destroy();
}


int32 StringExprParser::PriorityOf(char _operation)
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


StringANSI StringExprParser::GetStringExpr(void)const
{
	return stringExpr;
}


StringExprParser::ErrorType StringExprParser::GetCurrentError(void)const
{
	return currentError;
}

			
StringANSI StringExprParser::GetCurrentErrorString(void)const
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



StringANSI StringExprParser::GetErrorArg(void)const
{
	return errorArg;
}



int32 StringExprParser::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}


SimpleList<StringExprParser::Token> StringExprParser::GetTokens(void)const
{
	return tokens;
}


StringExprParser::ErrorType StringExprParser::Parse(StringANSI _stringExpr)
{ 

	currentState = START_STATE;
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
	stringExpr = _stringExpr;
	tokens.Clear();
	result.clear();
 
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
	return currentError;
}



StringANSI StringExprParser::GetResult(void)const
{
	return result;
}
	

StringExprParser::ErrorType StringExprParser::Calculate(void)
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
			StringExprParser::result.clear();
			return currentError = INVALID_RESULT;
		}
		else
		{
			StringExprParser::result = dynamic_cast<Variable<StringANSI>*>(it)->GetValue();
			currentError = NO_ERRORS;
		}
	}
	return currentError;
}



AbstractVariable* StringExprParser::Calculate(OperationType _operation, AbstractVariable* _left, AbstractVariable* _right)
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




void StringExprParser::Destroy(void)
{
	currentState = START_STATE;
	tokens.Clear();
	stringExpr.clear();
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
	result.clear(); 
}



bool StringExprParser::SaveToFile(File& _file)
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


bool StringExprParser::LoadFromFile(File& _file)
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