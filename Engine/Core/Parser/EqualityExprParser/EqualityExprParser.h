#ifndef EQUALITYEXPRPARSER_H
#define EQUALITYEXPRPARSER_H
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Parser/Variable/BooleanVariable/BooleanVariable.h>
#include <Engine/Core/Parser/Variable/FloatVariable/FloatVariable.h>
class EqualityExprParser
{
	private: class Operation
	{
		public: StringANSI value;
		public: int32 position;
		public: Operation(void)
		{}
		public: Operation(StringANSI _value, int32 _position)
		{
			value = _value;
			position = _position;
		}
	};
	public: class Token
	{
		public: enum TokenType
		{
			VARIABLE,
			OPERATION,
			FLOAT_NUMBER,
			INT_NUMBER
		};
		friend class EqualityExprParser;
		private: StringANSI value;
		private: TokenType type;
	    private: int32 position;
		public: Token(StringANSI _value, TokenType _type, int32 _position)
		{
			value = _value;
			type = _type;
			position = _position;
		}
		public: StringANSI GetValue(void)const
		{ 
			return value; 
		}
		public: TokenType GetType(void)const
		{
			return type; 
		}
		public: int32 GetPosition(void)const
		{
			return position;
		}
	};
	private: enum StateType
	{
		START_STATE,
		VARIABLE_RECEIVED_STATE,
		NUMBER_RECEIVED_STATE,
		OPERATION_RECEIVED_STATE,
		ERROR_STATE,
		END_STATE
	};
	public: enum ErrorType
	{  
		NO_ERRORS,
		UNKNOWN_TOKEN,
		UNKNOWN_EXPR,
		UNDEFINED_VARIABLE,
		EXPR_NOT_FOUND,
		RIGHT_BRACKET_NOT_FOUND,
		LEFT_BRACKET_NOT_FOUND,
		INVALID_OPERATION,
		INVALID_NUMBER,
		VERY_LONG_NUMBER,
		INVALID_RESULT
	};
	public: enum OperationType
	{
		PLUS = '+',
		MINUS = '-',
		DIVIDE = '/',
		MULTIPLY = '*',
		REMAINDER = '%',
		INVERSION = '@',
		ABS = '!',

		EQUAL = '=',
		NOT_EQUAL = '#',
		GREATER = '>',
		LESS = '<',
		GREATER_OR_EQUAL = ']',
		LESS_OR_EQUAL = '[',

		AND = '&',
		OR = '|',
		XOR = '^',
		NOT = '~'
	};
	private: StateType currentState;
	private: ErrorType currentError;
	private: StringANSI errorArg;
	private: int32 errorArgIndex;
	private: SimpleList<Token> tokens;
	private: StringANSI equalityExpr;
    private: bool result;
	public: EqualityExprParser(void);
	public: ~EqualityExprParser(void);
	public: static int32 _IsNameValid(StringANSI _name);
	private: int32 PriorityOf(char _operation);
	public: StringANSI GetEqualityExpr(void)const;
	public: SimpleList<Token> GetTokens(void)const;
	public: ErrorType GetCurrentError(void)const;
	public: StringANSI GetCurrentErrorString(void)const;
	public: StringANSI GetErrorArg(void)const;
	public: int32 GetErrorArgIndex(void)const;
	public: ErrorType Parse(StringANSI _equalityExpr);
	public: bool GetResult(void)const;
	public: ErrorType Calculate(void);
    public: virtual AbstractVariable* Calculate(OperationType _operation, AbstractVariable* _right);
	public: virtual AbstractVariable* Calculate(OperationType _operation, AbstractVariable* _left, AbstractVariable* _right);
	public: void Destroy(void);
	public: bool SaveToFile(File& _file);
	public: bool LoadFromFile(File& _file);
};



#endif 