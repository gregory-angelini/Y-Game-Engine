#ifndef ABSTRACTEVENT_H
#define ABSTRACTEVENT_H
#include <Engine/Core/Parser/Variable/BooleanVariable/BooleanVariable.h>
#include <Engine/Core/Object/ActivatableObject/ActivatableObject.h>
class AbstractEvent: public Variable<bool>,
                 	 public ActivatableObject
{
	public: enum: int64
	{
		EVENT_CLASS = 4096
	};
	public: enum
	{
		SOURCE = 0,
		ARG1,
		ARG2,
		ARG3,
		ARG4,
		ARG5
	};
	public: enum
	{
		ASSET_TYPE = 0,
		VARIABLE,
		TEMPLATE,
		KEY,
		EXPRESSION,
		EQUALITY
	};
	public: enum
	{
		WIDTH = 6,
		HEIGHT = 6
	};
	friend class Trigger;
	friend class EventBinaryTree;
    protected: int32 type;
	protected: bool loadArgsEnable;
	protected: bool argsEnable;
	protected: int32 state[WIDTH][HEIGHT];
    protected: static StringANSI eventsSuffix;
    protected: static StringANSI eventsDir;
	protected: AbstractEvent(void);
	protected: AbstractEvent(int32 _type);
	public: virtual ~AbstractEvent(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetExpressionArg(int32 _index, StringANSI _expr);
	public: virtual void SetEqualityArg(int32 _index, StringANSI _expr);
	public: virtual void SetStringKeyArg(int32 _index, StringANSI _value);
	public: virtual void SetIntegerKeyArg(int32 _index, int32 _value);
	public: virtual void SetFloatKeyArg(int32 _index, float _value);
	public: virtual void SetBooleanKeyArg(int32 _index, bool _value);
	public: virtual void SetVector2KeyArg(int32 _index, Vector2 _value);
	public: virtual void SetVector3KeyArg(int32 _index, Vector3 _value);
	public: virtual void SetQuaternionKeyArg(int32 _index, Quaternion _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetExpressionArg(int32 _index);
	public: virtual StringANSI GetEqualityArg(int32 _index);
	public: virtual StringANSI GetVariableArg(int32 _index);
	public: virtual StringANSI GetStringKeyArg(int32 _index);
	public: virtual int32 GetIntegerKeyArg(int32 _index);
	public: virtual float GetFloatKeyArg(int32 _index);
	public: virtual bool GetBooleanKeyArg(int32 _index);
	public: virtual Vector2 GetVector2KeyArg(int32 _index);
	public: virtual Vector3 GetVector3KeyArg(int32 _index);
	public: virtual Quaternion GetQuaternionKeyArg(int32 _index);
	public: bool IsLoadArgsEnabled(void)const;
	public: void SetLoadArgsEnable(bool _enable);
	public: void SetArgsEnable(bool _enable);
	public: bool IsArgsEnabled(void)const;
	public: int32 GetType(void)const;
	public: static StringANSI _GetStringType(int32 _type);
	public: virtual void SetValue(bool _value);
    public: void SetArgAssetType(int32 _index, int32 _type);
	public: void SetArgMode(int32 _index, int32 _mode);
    public: int32 GetArgState(int32 _property, int32 _index);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
    public: static AbstractEvent* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static AbstractEvent* _LoadFromFile(File& _file);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
};


#endif 