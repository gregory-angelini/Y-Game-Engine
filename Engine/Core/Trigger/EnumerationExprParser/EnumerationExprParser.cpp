#include "EnumerationExprParser.h"


EnumerationExprParser::EnumerationExprParser(void): amountOfUniqueVariables(0)
{
	currentState = START_STATE;
	currentError = NO_ERRORS;
}


EnumerationExprParser::~EnumerationExprParser(void)
{
	Destroy();
}


EnumerationExprParser::ErrorType EnumerationExprParser::Parse(StringANSI _enumerationExpr)
{
	currentState = START_STATE;
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
	enumerationExpr = _enumerationExpr;
	tokens.Clear();
	sequence.Clear();
	amountOfUniqueVariables = 0;

	if(_enumerationExpr.length() > 0)
	{
		StringANSI lastVariable;
		int32 lastVariableIndex;
		char character;
		char lastOperation = ' ';
		int32 lastOperationIndex = -1;
		for(int32 i = 0; i < _enumerationExpr.length(); ++i)
		{
			character = _enumerationExpr[i];

			if(character != ' ')
			{
				switch(character)
				{
					case ',':
					{
						if(currentState == OPERATION_RECEIVED_STATE || currentState == START_STATE)
						{
							sequence.Clear();
							tokens.Clear();
							amountOfUniqueVariables = 0;
							currentState = ERROR_STATE;
							errorArg = character;
							errorArgIndex = i;
							return currentError = UNKNOWN_EXPR;
						}
						if(sequence.Find(lastVariable) == NIL)
						{
							amountOfUniqueVariables++;
						}
						lastOperation = character;
						lastOperationIndex = i;

						tokens.AddToTail(Token(lastVariable, Token::VARIABLE, lastVariableIndex));
						tokens.AddToTail(Token(StringANSI(1, lastOperation), Token::OPERATION, lastOperationIndex)); 

						sequence.AddToTail(lastVariable);
						currentState = OPERATION_RECEIVED_STATE;
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
								if(_enumerationExpr[i - 1] == ' ')
								{
									sequence.Clear();
									tokens.Clear();
									amountOfUniqueVariables = 0;
									currentState = ERROR_STATE;
									errorArg = character;
									errorArgIndex = i;
									return currentError = UNKNOWN_EXPR;
								}
								lastVariable += character;
							}
							else
							{
								currentState = VARIABLE_RECEIVED_STATE;
								lastVariable = character;
								lastVariableIndex = i;
							}
						}
						else
						{
							sequence.Clear();
							tokens.Clear();
							amountOfUniqueVariables = 0;
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
		if(!lastVariable.empty())
		{
			if(sequence.Find(lastVariable) == NIL)
			{
				amountOfUniqueVariables++;
			}
			tokens.AddToTail(Token(lastVariable, Token::VARIABLE, lastVariableIndex));

			sequence.AddToTail(lastVariable);
		}

		if(sequence.IsEmpty())
		{
			currentState = ERROR_STATE;
			return currentError = EXPR_NOT_FOUND;
		}
		if(currentState == OPERATION_RECEIVED_STATE)
		{
			sequence.Clear();
			tokens.Clear();
			amountOfUniqueVariables = 0;
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


void EnumerationExprParser::Destroy(void)
{
	currentState = START_STATE;
	sequence.Clear();
	tokens.Clear();
	amountOfUniqueVariables = 0;
	enumerationExpr.clear();
	currentError = NO_ERRORS;
	errorArg.clear();
	errorArgIndex = -1;
}


SimpleList<StringANSI>& EnumerationExprParser::GetVariables(void)
{
	return sequence;
}



SimpleList<EnumerationExprParser::Token> EnumerationExprParser::GetTokens(void)const
{
	return tokens;
}

 
StringANSI EnumerationExprParser::GetEnumerationExpr(void)const
{
	return enumerationExpr;
}



EnumerationExprParser::ErrorType EnumerationExprParser::GetCurrentError(void)const
{
	return currentError;
}



StringANSI EnumerationExprParser::GetErrorArg(void)const
{
	return errorArg;
}


int32 EnumerationExprParser::GetErrorArgIndex(void)const
{
	return errorArgIndex;
}



StringANSI EnumerationExprParser::GetCurrentErrorString(void)const
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

		case UNKNOWN_EXPR:
		{
			return "UNKNOWN_EXPR";
		}
	}
	return "";
}


bool EnumerationExprParser::IsVariable(StringANSI _name)const
{
	return sequence.Find(_name) ? true : false;
}


int32 EnumerationExprParser::GetAmountOfVariables(void)const
{
	return sequence.GetSize();
}


int32 EnumerationExprParser::GetAmountOfUniqueVariables(void)const
{
	return amountOfUniqueVariables;
}


bool EnumerationExprParser::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(enumerationExpr.length());
		_file.WriteString(enumerationExpr);
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


bool EnumerationExprParser::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		Destroy();
		int32 length;
		_file.Read(length);
		_file.ReadString(enumerationExpr, length);
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
				if(sequence.Find(value) == NIL)
				{
					amountOfUniqueVariables++;
				}
				sequence.AddToTail(value);
			}
		}
		return true;
	}
	return false;
}