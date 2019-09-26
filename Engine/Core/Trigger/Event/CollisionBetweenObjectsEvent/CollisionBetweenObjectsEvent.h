#ifndef COLLISIONBETWEENOBJECTSEVENT_H
#define COLLISIONBETWEENOBJECTSEVENT_H
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
#include <Engine/Core/Trigger/StringExprParserEx/StringExprParserEx.h>
#include <Engine/Core/Surface/PolygonalSurface/PolygonalSurface.h>
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h>
class CollisionBetweenObjectsEvent: public AbstractEvent
{
	friend class Trigger;
	friend class AbstractEvent;
	private: StringANSI arg1Name;
    private: StringExprParserEx templateArg1;
    private: MagicPointer<PolygonalSurface> arg1;

	private: StringANSI arg2Name;
    private: StringExprParserEx templateArg2;
    private: MagicPointer<Variable<StringANSI>> arg2;

	private: StringANSI arg3Name;
    private: StringExprParserEx templateArg3;
    private: MagicPointer<Variable<StringANSI>> arg3;

	private: StringANSI arg4Name;
    private: StringExprParserEx templateArg4;
    private: MagicPointer<List<int32>> arg4;

	private: StringANSI arg5Name;
    private: StringExprParserEx templateArg5;
    private: MagicPointer<List<int32>> arg5;

	private: StringANSI sourceName;
	private: StringExprParserEx templateSource;
	private: MagicPointer<PolygonalSurface> source;
	private: bool arg1Exist, arg2Exist, arg3Exist, arg1IsTrue, arg2IsTrue, arg3IsTrue;
	private: CollisionBetweenObjectsEvent(void);
	private: CollisionBetweenObjectsEvent(int32 _type);
	public: virtual ~CollisionBetweenObjectsEvent(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetStringKeyArg(int32 _index, StringANSI _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual StringANSI GetStringKeyArg(int32 _index);
	private: void EventHandler(PolygonalSurface::CollisionInfo* _collisionInfo);
	public: virtual void SetValue(bool _value);
	private: void TemplateArg1IsCompleted(void);
	private: void Arg1IsLost(void);
	private: void TemplateArg1IsChanged(void);
	private: void TemplateArg2IsCompleted(void);
	private: void Arg2IsLost(void);
	private: void TemplateArg2IsChanged(void);
	private: void TemplateArg3IsCompleted(void);
	private: void Arg3IsLost(void);
	private: void TemplateArg3IsChanged(void);
	private: void TemplateArg4IsCompleted(void);
	private: void Arg4IsLost(void);
	private: void TemplateArg4IsChanged(void);
	private: void TemplateArg5IsCompleted(void);
	private: void Arg5IsLost(void);
	private: void TemplateArg5IsChanged(void);
	private: void TemplateSourceIsCompleted(void);
	private: void SourceIsLost(void);
	private: void TemplateSourceIsChanged(void);
	private: void ObjectIsDestroyed(AbstractObject* _object);
	private: void SourceIsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg1IsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg2IsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg3IsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg4IsRenamed(AbstractObject::NameInfo* _info);
	private: void Arg5IsRenamed(AbstractObject::NameInfo* _info);
	private: void ObjectIsCreated(AbstractObject* _object);
	private: void UpdateValid(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	private: static AbstractEvent* _LoadFromFile(File& _file);
};


#endif 