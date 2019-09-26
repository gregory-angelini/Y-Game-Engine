#ifndef ENABLEPROPERTYEVENT_H
#define ENABLEPROPERTYEVENT_H
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Output/Graphics/Surface/TexturingSurface/TexturingSurface.h>
class EnablePropertyEvent: public AbstractEvent
{
	friend class Trigger;
	friend class AbstractEvent;
	private: StringANSI arg1Name;
    private: StringExprParserEx templateArg1;
    private: MagicPointer<Variable<int32>> arg1;

	private: StringANSI arg2Name;
    private: StringExprParserEx templateArg2;
    private: MagicPointer<Variable<bool>> arg2;

	private: StringANSI sourceName;
	private: StringExprParserEx templateSource;
	private: MagicPointer<AbstractObject> source;
	private: EnablePropertyEvent(void);
	private: EnablePropertyEvent(int32 _type);
	public: virtual ~EnablePropertyEvent(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetIntegerKeyArg(int32 _index, int32 _value);
	public: virtual void SetBooleanKeyArg(int32 _index, bool _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual int32 GetIntegerKeyArg(int32 _index);
	public: virtual bool GetBooleanKeyArg(int32 _index);
	private: void EventHandler(AbstractObject::PropertyInfo* _info);
	public: virtual void SetValue(bool _value);
	private: void TemplateArg1IsCompleted(void);
	private: void Arg1IsLost(void);
	private: void TemplateArg1IsChanged(void);
	private: void TemplateArg2IsCompleted(void);
	private: void Arg2IsLost(void);
	private: void TemplateArg2IsChanged(void);
	private: void TemplateSourceIsCompleted(void);
	private: void SourceIsLost(void);
	private: void TemplateSourceIsChanged(void);
	private: void ObjectIsDestroyed(AbstractObject* _object);
	private: void SourceIsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg1IsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg2IsRenamed(AbstractObject::NameInfo* _info);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void UpdateValid(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	private: static AbstractEvent* _LoadFromFile(File& _file);
};


#endif 