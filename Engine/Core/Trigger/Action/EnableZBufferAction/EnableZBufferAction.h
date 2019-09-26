#ifndef ENABLEZBUFFERACTION_H
#define ENABLEZBUFFERACTION_H
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Parser/Variable/IntegerVariable/IntegerVariable.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Animation/AbstractAnimation/AbstractAnimation.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h>
class EnableZBufferAction: public AbstractAction
{
	friend class Trigger;
	friend class AbstractAction;
	private: StringANSI argName;
    private: StringExprParserEx templateArg;
    private: MagicPointer<Variable<bool>> arg;
	private: EnableZBufferAction(void);
	private: EnableZBufferAction(int32 _type);
	public: virtual ~EnableZBufferAction(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetBooleanKeyArg(int32 _index, bool _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual bool GetBooleanKeyArg(int32 _index);
	public: virtual void operator () (void);
	private: void TemplateArgIsCompleted(void);
	private: void ArgIsLost(void);
	private: void TemplateArgIsChanged(void);
	private: void ObjectIsDestroyed(AbstractObject* _object);
	private: void ArgIsRenamed(AbstractObject::NameInfo* _info);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void UpdateValid(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	private: static AbstractAction* _LoadFromFile(File& _file);
};


#endif 