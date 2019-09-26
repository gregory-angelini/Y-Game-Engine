#ifndef ENUMERATIONEXPRPARSER_H
#define ENUMERATIONEXPRPARSER_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
class EnumerationExprParser
{
	public: class Token
	{
		public: enum TokenType
		{
			VARIABLE,
			OPERATION
		};
		friend class EnumerationExprParser;
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
		OPERATION_RECEIVED_STATE,
		ERROR_STATE,
		END_STATE
	};
	public: enum ErrorType
	{
		NO_ERRORS,
		UNKNOWN_TOKEN,
		UNKNOWN_EXPR,
		EXPR_NOT_FOUND
	};
	private: StateType currentState;
	private: ErrorType currentError;
	private: StringANSI errorArg;
	private: int32 errorArgIndex;
    private: SimpleList<Token> tokens;
	private: StringANSI enumerationExpr;
	private: SimpleList<StringANSI> sequence;
	private: int32 amountOfUniqueVariables;
	public: EnumerationExprParser(void);
	public: ~EnumerationExprParser(void);
	public: ErrorType Parse(StringANSI _enumerationExpr);
	public: StringANSI GetEnumerationExpr(void)const;
	public: SimpleList<Token> GetTokens(void)const;
	public: ErrorType GetCurrentError(void)const;
	public: StringANSI GetCurrentErrorString(void)const;
	public: StringANSI GetErrorArg(void)const;
	public: int32 GetErrorArgIndex(void)const;
	public: void Destroy(void);
	public: bool IsVariable(StringANSI _name)const;
	public: int32 GetAmountOfVariables(void)const;
	public: int32 GetAmountOfUniqueVariables(void)const;
	public: SimpleList<StringANSI>& GetVariables(void);
	public: bool SaveToFile(File& _file);
	public: bool LoadFromFile(File& _file);
};





#endif 