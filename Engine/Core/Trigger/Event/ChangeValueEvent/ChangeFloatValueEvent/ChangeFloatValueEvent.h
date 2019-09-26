#ifndef CHANGEFLOATVALUEEVENT_H
#define CHANGEFLOATVALUEEVENT_H
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Core/Trigger/ArithmeticExprParserEx/ArithmeticExprParserEx.h>
#include <Engine/Core/Trigger/EqualityExprParserEx/EqualityExprParserEx.h>
class ChangeFloatValueEvent: public AbstractEvent
{
	friend class Trigger;
	friend class AbstractEvent;
	private: float argValue;
	private: StringANSI argName;
    private: MagicPointer<Variable<float>> arg;
    private: ArithmeticExprParserEx<float> expr;
	private: StringExprParserEx templateArg;
	private: EqualityExprParserEx equalityArg;

	private: StringANSI sourceName;
	private: StringExprParserEx templateSource;
	private: MagicPointer<Variable<float>> source;
	private: ChangeFloatValueEvent(void);
	private: ChangeFloatValueEvent(int32 _type);
	public: virtual ~ChangeFloatValueEvent(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetExpressionArg(int32 _index, StringANSI _expr);
	public: virtual void SetEqualityArg(int32 _index, StringANSI _expr);
	public: virtual void SetFloatKeyArg(int32 _index, float _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetExpressionArg(int32 _index);
	public: virtual StringANSI GetEqualityArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual float GetFloatKeyArg(int32 _index);
	private: void EventHandler(AbstractVariable* _object);
	public: virtual void SetValue(bool _value);
	private: void TemplateArgIsCompleted(void);
	private: void ArgIsLost(void);
	private: void TemplateArgIsChanged(void);
	private: void TemplateSourceIsCompleted(void);
	private: void SourceIsLost(void);
	private: void TemplateSourceIsChanged(void);
	private: void ObjectIsDestroyed(AbstractObject* _object);
	private: void SourceIsRenamed(AbstractObject::NameInfo* _info);
	private: void ArgIsRenamed(AbstractObject::NameInfo* _info);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void UpdateValid(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	private: static AbstractEvent* _LoadFromFile(File& _file);
};


#endif 