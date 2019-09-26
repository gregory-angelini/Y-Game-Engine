#ifndef STRINGEXPRPARSER_H
#define STRINGEXPRPARSER_H
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
class StringExprParser
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
			STRING,
			NUMBER
		};
		friend class StringExprParser;
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
		STRING_RECEIVED_STATE,
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
		INVALID_OPERATION,
		VERY_LONG_NUMBER,
		INVALID_RESULT
	};
	public: enum OperationType
	{
		PLUS = '+',
		CONSTANT = '$'
	};
	private: StateType currentState;
	private: ErrorType currentError;
	private: StringANSI errorArg;
	private: int32 errorArgIndex;
	private: SimpleList<Token> tokens;
	private: StringANSI stringExpr;
	private: StringANSI result;
	public: StringExprParser(void);
	public: ~StringExprParser(void);
	private: int32 PriorityOf(char _operation);
	public: StringANSI GetStringExpr(void)const;
	public: SimpleList<Token> GetTokens(void)const;
	public: ErrorType GetCurrentError(void)const;
	public: StringANSI GetCurrentErrorString(void)const;
	public: StringANSI GetErrorArg(void)const;
	public: int32 GetErrorArgIndex(void)const;
	public: ErrorType Parse(StringANSI _stringExpr);
	public: StringANSI GetResult(void)const;
	public: ErrorType Calculate(void);
	private: AbstractVariable* Calculate(OperationType _operation, AbstractVariable* _left, AbstractVariable* _right);
	public: void Destroy(void);
	public: bool SaveToFile(File& _file);
	public: bool LoadFromFile(File& _file);
};



#endif 