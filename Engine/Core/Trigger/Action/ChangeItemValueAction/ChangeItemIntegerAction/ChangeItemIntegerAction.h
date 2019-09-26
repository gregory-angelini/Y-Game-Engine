#ifndef CHANGEITEMINTEGERACTION_H
#define CHANGEITEMINTEGERACTION_H
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Parser/Variable/FloatVariable/FloatVariable.h>
#include <Engine/Core/Parser/Variable/BooleanVariable/BooleanVariable.h>
#include <Engine/Core/Parser/Variable/StringVariable/StringVariable.h>
#include <Engine/Core/Parser/Variable/Vector2Variable/Vector2Variable.h>
#include <Engine/Core/Parser/Variable/Vector3Variable/Vector3Variable.h>
#include <Engine/Core/Parser/Variable/QuaternionVariable/QuaternionVariable.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/ArithmeticExprParserEx/ArithmeticExprParserEx.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h>
#include <Engine/Core/Parser/List/FloatList/FloatList.h>
#include <Engine/Core/Parser/List/BooleanList/BooleanList.h>
#include <Engine/Core/Parser/List/StringList/StringList.h>
#include <Engine/Core/Parser/List/Vector2List/Vector2List.h>
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h>
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h>
#include <Engine/Core/Trigger/BooleanExprParserEx/BooleanExprParserEx.h>
class ChangeItemIntegerAction: public AbstractAction
{
	friend class Trigger;
	friend class AbstractAction;
	private: StringANSI arg1Name;
    private: StringExprParserEx templateArg1;
    private: MagicPointer<Variable<int32>> arg1;

	private: int32 arg2Value;
	private: StringANSI arg2Name;
	private: MagicPointer<Variable<int32>> arg2;
    private: ArithmeticExprParserEx<int32> expr;
	private: StringExprParserEx templateArg2;
	
	private: StringANSI targetName;
	private: StringExprParserEx templateTarget;
	private: MagicPointer<List<int32>> target;
	private: ChangeItemIntegerAction(void);
	private: ChangeItemIntegerAction(int32 _type);
	public: virtual ~ChangeItemIntegerAction(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetExpressionArg(int32 _index, StringANSI _expr);
	public: virtual void SetIntegerKeyArg(int32 _index, int32 _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetExpressionArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual int32 GetIntegerKeyArg(int32 _index);
	public: virtual void operator () (void);
	private: void TemplateArg1IsCompleted(void);
	private: void Arg1IsLost(void);
	private: void TemplateArg1IsChanged(void);
	private: void TemplateArg2IsCompleted(void);
	private: void Arg2IsLost(void);
	private: void TemplateArg2IsChanged(void);
	private: void TemplateTargetIsCompleted(void);
	private: void TargetIsLost(void);
	private: void TemplateTargetIsChanged(void);
	private: void ObjectIsDestroyed(AbstractObject* _object);
	private: void TargetIsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg1IsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg2IsRenamed(AbstractObject::NameInfo* _info);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void UpdateValid(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	private: static AbstractAction* _LoadFromFile(File& _file);
};





#endif 