#ifndef BOOLEANEXPRPARSEREX_H
#define BOOLEANEXPRPARSEREX_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Container/Stack/SimpleStack/SimpleStack.h>
#include <Engine/Core/Memory/Memory.h>
#include <Engine/Core/Container/Tree/MagicBinaryTree/MagicBinaryTree.h>
#include <Engine/Core/Parser/Variable/BooleanVariable/BooleanVariable.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
class BooleanExprParserEx: public AbstractObject
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
			OPERATION
		};
		friend class BooleanExprParserEx;
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
		UNDEFINED_VARIABLE,
		EXPR_NOT_FOUND,
		RIGHT_BRACKET_NOT_FOUND,
		LEFT_BRACKET_NOT_FOUND,
		INVALID_OPERATION,
		INVALID_RESULT
	};
	public: enum OperationType
	{
		OR = '|',
		AND = '&',
		NOT = '~',
		XOR = '^'
	};
	private: StateType currentState;
	private: ErrorType currentError;
	private: StringANSI errorArg;
	private: int32 errorArgIndex;
	private: SimpleList<Token> tokens;
	private: MagicList<AbstractVariable> definedVariables;
	private: SimpleList<StringANSI> undefinedVariables;
    private: StringANSI booleanExpr;
    private: bool result;
    private: SimpleList<StringANSI> variables;
	private: int32 amountOfUniqueVariables;
	public: BooleanExprParserEx(void);
	public: ~BooleanExprParserEx(void);
	private: void DisconnectVariables(void);
	private: void VariableIsDestroyed(AbstractObject* _object);
	private: void VariableIsRenamed(AbstractObject::NameInfo* _info);
	private: void VariableIsCreated(AbstractObject* _object);
	private: bool IsUndefinedVariable(StringANSI _name)const;
	public: bool IsAllVariablesExist(void)const;
	private: int32 PriorityOf(char _operation);
	public: StringANSI GetBooleanExpr(void)const;
	public: SimpleList<Token> GetTokens(void)const;
	public: ErrorType GetCurrentError(void)const;
	public: StringANSI GetCurrentErrorString(void)const;
	public: StringANSI GetErrorArg(void)const;
	public: int32 GetErrorArgIndex(void)const;
	public: ErrorType Parse(StringANSI _booleanExpr);
	public: bool GetResult(void)const;
	public: ErrorType Calculate(void);
    public: virtual AbstractVariable* Calculate(OperationType _operation, AbstractVariable* _right);
	public: virtual AbstractVariable* Calculate(OperationType _operation, AbstractVariable* _left, AbstractVariable* _right);
	public: void Destroy(void);
	public: static int32 _IsNameValid(StringANSI _name);
	public: bool IsVariable(StringANSI _name)const;
    public: SimpleList<StringANSI>& GetVariables(void);
	public: int32 GetAmountOfVariables(void)const;
	public: int32 GetAmountOfUniqueVariables(void)const;
	public: bool SaveToFile(File& _file);
	public: bool LoadFromFile(File& _file);
};

#endif 