#ifndef CHANGEFLOATLERPEVENT_H
#define CHANGEFLOATLERPEVENT_H
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Output/Graphics/Scene/GraphicsScene/GraphicsScene.h>
class ChangeFloatLerpEvent: public AbstractEvent
{
	friend class Trigger;
	friend class AbstractEvent;
	private: StringANSI argName;
    private: StringExprParserEx templateArg;
    private: MagicPointer<FloatLerp> arg;

	private: StringANSI sourceName;
	private: StringExprParserEx templateSource;
	private: MagicPointer<LerpingObject> source;
	private: ChangeFloatLerpEvent(void);
	private: ChangeFloatLerpEvent(int32 _type);
	public: virtual ~ChangeFloatLerpEvent(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	private: void EventHandler(StringANSI _name);
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