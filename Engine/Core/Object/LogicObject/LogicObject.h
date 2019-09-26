#ifndef LOGICOBJECT_H
#define LOGICOBJECT_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
#include <Engine/Core/Container/MagicPointer/MagicPointer.h>
class LogicObject: public virtual AbstractObject
{
	public: enum: int64
	{
		LOGIC_OBJECT_CLASS = 1
	};
	public: enum
	{
		ENABLE_FREEZE_MESSAGE = 233,
		CHANGE_DELTA_SCALE_MESSAGE = 234
	};
	friend class UpdateList;
	friend class LogicScene;
	friend class AbstractAnimation;
	friend class Camera3D;
	friend class Timer;
	friend class FloatLerp;
	friend class Graph;
	friend class VectorLerp;
	private: MagicPointer<LogicObject> logicContainer;
	protected: bool freezeEnable;
	protected: float deltaScale;
	private: float totalDeltaScale;
	public: LogicObject(void);
	public: virtual ~LogicObject(void);
    public: float GetDeltaScale(void)const;
    public: virtual void SetDeltaScale(float _scale);
	private: void ResetDeltaScale(void);
	private: void UpdateDeltaScale(void);
	public: void SetFreezeEnable(bool _enable);
	public: bool IsFreezeEnabled(void)const;
	public: LogicObject* GetLogicContainer(void)const;
	public: virtual void Update(float _delta = 0.0f) = 0;
	protected: void SaveDerivedToFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
};


#endif 