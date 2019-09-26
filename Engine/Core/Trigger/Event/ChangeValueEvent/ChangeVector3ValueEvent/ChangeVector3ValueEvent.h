#ifndef CHANGEVECTOR3VALUEEVENT_H
#define CHANGEVECTOR3VALUEEVENT_H
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Core/Trigger/ArithmeticExprParserEx/ArithmeticExprParserEx.h>
#include <Engine/Core/Trigger/EqualityExprParserEx/EqualityExprParserEx.h>
class ChangeVector3ValueEvent: public AbstractEvent
{
	friend class Trigger;
	friend class AbstractEvent;
	private: Vector3 argValue;
	private: StringANSI argName;
    private: MagicPointer<Variable<Vector3>> arg;
    private: ArithmeticExprParserEx<Vector3> expr;
	private: StringExprParserEx templateArg;
	private: EqualityExprParserEx equalityArg;

	private: StringANSI sourceName;
	private: StringExprParserEx templateSource;
	private: MagicPointer<Variable<Vector3>> source;
	private: ChangeVector3ValueEvent(void);
	private: ChangeVector3ValueEvent(int32 _type);
	public: virtual ~ChangeVector3ValueEvent(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetExpressionArg(int32 _index, StringANSI _expr);
	public: virtual void SetEqualityArg(int32 _index, StringANSI _expr);
	public: virtual void SetVector3KeyArg(int32 _index, Vector3 _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetExpressionArg(int32 _index);
	public: virtual StringANSI GetEqualityArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual Vector3 GetVector3KeyArg(int32 _index);
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