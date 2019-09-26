#ifndef TRIGGER_H
#define TRIGGER_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Trigger/Event/AbstractEvent.h>
#include <Engine/Core/Trigger/Action/AbstractAction.h>
#include <Engine/Core/Parser/BooleanExprParser/BooleanExprParser.h>
#include <Engine/Core/Object/ObjectBinaryTree/ObjectBinaryTree.h>
#include <Engine/Core/Parser/ArithmeticExprParser/ArithmeticExprParser.h>
#include <ENgine/Core/Trigger/EnumerationExprParser/EnumerationExprParser.h>
#include <Engine/Core/Trigger/StringExprParser/StringExprParser.h>
#include <Engine/Core/Parser/EqualityExprParser/EqualityExprParser.h>
#include <Engine/Core/Trigger/MagicListEx/MagicListEx.h>
#include <Engine/Core/Trigger/Action/TriggerFinishAction.h>
#include <Engine/Core/Object/ActivatableObject/ActivatableObject.h>
class Trigger: public Variable<bool>,
	           public ActivatableObject
{
	public: class EventInfo
	{
		public: enum
		{
			TRIGGER_CATEGORY,
			ACTION_CATEGORY,
			EVENT_CATEGORY
		};
		public: int32 category;
		public: int32 type;
		public: StringANSI name;
		public: uint32 time;
		public: EventInfo(void): category(-1), type(-1), time(0)
		{}
		public: EventInfo(StringANSI _name, int32 _category, int32 _type, uint32 _time): name(_name), category(_category), type(_type), time(_time)
		{}
	};
	private: class EventTree: public ObjectBinaryTree
	{
		friend class Trigger;
		private: Trigger* trigger;
		public: EventTree(void);
		public: virtual ~EventTree(void);
		public: virtual AddressDerived* AddObject(AbstractObject* _event);
		private: void AddVariable(AbstractObject* _event);
		private: void EventIsRenamed(AbstractObject::NameInfo* _info);
		public: virtual void AddNodeEvent(MagicNode* _node);
		public: virtual void RemoveNodeEvent(MagicNode* _node);
		private: void DestroyObjectEvent(AbstractObject* _object);
		private: void ChangeValue(AbstractVariable* _event);
	};
	private: class ActionTree: public ObjectBinaryTree
	{
		friend class Trigger;
	    private: Trigger* trigger;
		public: ActionTree(void);
		public: virtual ~ActionTree(void);
		private: void UpdateActionList(void);
		public: virtual AddressDerived* AddObject(AbstractObject* _action);
		private: void AddAction(AbstractObject* _action);
		private: void ActionIsRenamed(AbstractObject::NameInfo* _info);
		public: virtual void AddNodeEvent(MagicNode* _node);
		public: virtual void RemoveNodeEvent(MagicNode* _node);
		private: void DestroyObjectEvent(AbstractObject* _object);
	};
	public: enum: int64 
	{
		TRIGGER_CLASS = 134217728 
	};
	public: enum
	{
		CREATE_TRIGGER_MESSAGE = 72,
		TRIGGER_FINISH_MESSAGE = 73,
		ENABLE_TRIGGERS_BLOCK_MESSAGE = 255
	};
	public: enum
	{
		AMOUNT_OF_PRIORITYS = 6
	};
	friend class EventTree;
	friend class DestroyObjectAction;
	private: EventTree events;
	private: ActionTree actions;
    private: BooleanExprParser eventExprParser;
	private: EnumerationExprParser actionExprParser;
    private: bool loadArgsEnable;
	private: int32 repeatLimit;
	private: MagicListEx<AbstractAction> actionList;
	private: MagicPointer<TriggerFinishAction> triggerFinishAction;
	private: uint32 systemTimeInMs;
    private: int32 activationCount;
   
    private: static MagicList<AbstractObject> deletionList;
	private: static bool eventLogEnable;
	private: static SimpleList<EventInfo> eventLogList;
    private: static bool blockEnable;

	private: static StringANSI triggersSuffix;
    private: static StringANSI triggersDir;
	public: Trigger(void);
	public: virtual ~Trigger(void);
	public: virtual void Destroy(void);
	public: bool IsLoadArgsEnabled(void)const;
	public: void SetLoadArgsEnable(bool _enable);
	public: int32 GetRepeatLimit(void)const;
	public: void SetRepeatLimit(int32 _value);
	public: virtual void SetValue(bool _value);
	public: static void _ClearDeletionList(void);
	public: static void _SetBlockEnable(bool _enable);
	public: static bool _IsBlockEnabled(void);
	public: int32 GetAmountOfDefinedEvents(void)const;
	public: int32 GetAmountOfUndefinedEvents(void)const;
	public: StringANSI GetEventExpr(void)const;
	public: bool SetEventExpr(StringANSI _expr);
	private: void CheckEventExpr(void);
	private: void CalculateBooleanExpr1(void);
	private: void CalculateBooleanExpr2(void);
	public: static AbstractEvent* _CreateEvent(int32 _type);
	public: int32 GetAmountOfDefinedActions(void)const;
	public: int32 GetAmountOfUndefinedActions(void)const;
	public: StringANSI GetActionExpr(void)const;
	public: bool SetActionExpr(StringANSI _expr);
	private: void SetActionExprEx(void);
	public: static AbstractAction* _CreateAction(int32 _type);
	public: static bool _IsEventLogEnabled(void);
	public: static void _SetEventLogEnable(bool _eventLogEnable);
	public: static SimpleList<EventInfo>& _GetEventLogList(void);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	private: void SaveEventsToFile(EventTree::AddressDerived* _it, File& _file);
	private: void SaveActionsToFile(ActionTree::AddressDerived* _it);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static Trigger* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static Trigger* _LoadFromFile(File& _file);
};




#endif 