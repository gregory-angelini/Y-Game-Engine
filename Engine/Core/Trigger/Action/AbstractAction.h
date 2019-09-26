#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Quaternion/Quaternion.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
class Trigger;
class AbstractAction: public AbstractObject
{
    public: enum: int64
	{
		ACTION_CLASS = 268435456
	};
	public: enum
	{
		TARGET = 0,
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
		EXPRESSION
	};
	public: enum
	{
		WIDTH = 5,
		HEIGHT = 6
	};
	friend class Trigger;
	friend class ActionBinaryTree;
    protected: int32 type;
	protected: int32 activationLimit;
	protected: bool loadArgsEnable;
    protected: bool valid;

	protected: int32 state[WIDTH][HEIGHT];
    protected: static StringANSI actionsSuffix;
    protected: static StringANSI actionsDir;
	protected: AbstractAction(void);
	protected: AbstractAction(int32 _type);
	public: virtual ~AbstractAction(void);
	public: virtual void SetVariableArg(int32 _index, StringANSI _name);
	public: virtual void SetTemplateArg(int32 _index, StringANSI _expr);
	public: virtual void SetExpressionArg(int32 _index, StringANSI _expr);
	public: virtual void SetStringKeyArg(int32 _index, StringANSI _value);
	public: virtual void SetIntegerKeyArg(int32 _index, int32 _value);
	public: virtual void SetFloatKeyArg(int32 _index, float _value);
	public: virtual void SetBooleanKeyArg(int32 _index, bool _value);
	public: virtual void SetVector2KeyArg(int32 _index, Vector2 _value);
	public: virtual void SetVector3KeyArg(int32 _index, Vector3 _value);
	public: virtual void SetQuaternionKeyArg(int32 _index, Quaternion _value);
	public: virtual StringANSI GetTemplateArg(int32 _index);
	public: virtual StringANSI GetExpressionArg(int32 _index);
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
	public: int32 GetType(void)const;
	public: static StringANSI _GetStringType(int32 _type);
	public: virtual void operator () (void) = 0;
	protected: bool IsValid(void);
	public: int32 GetActivationLimit(void)const;
	public: void SetActivationLimit(int32 _activationLimit);
    public: void SetArgAssetType(int32 _index, int32 _type);
	public: void SetArgMode(int32 _index, int32 _mode);
    public: int32 GetArgState(int32 _property, int32 _index);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: static AbstractAction* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static AbstractAction* _LoadFromFile(File& _file);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
};


#endif 