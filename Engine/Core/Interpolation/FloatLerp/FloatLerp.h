#ifndef FLOATLERP_H
#define FLOATLERP_H
#include <Engine/Core/Object/LogicObject/LogicObject.h>
#include <Engine/Core/Object/PlayingObject/PlayingObject.h>
class FloatLerp: public PlayingObject,
	             public LogicObject
{
	public: enum: int64 
	{
		FLOAT_LERP_CLASS = 549755813888
	};
	public: enum
	{
		FLOAT_LERP_START_MESSAGE = 99,
		STREAMING_FLOAT_CURRENT_MESSAGE = 46,
			STREAMING_FLOAT_T_MESSAGE = 28,
			STREAMING_FLOAT_T_ALPHA_MESSAGE = 133,
			STREAMING_FLOAT_T_HUE_MESSAGE = 163,
			STREAMING_FLOAT_T_SATURATION_MESSAGE = 151,
			STREAMING_FLOAT_T_LIGHTNESS_MESSAGE = 84,
			STREAMING_FLOAT_T_RED_MESSAGE = 191,
			STREAMING_FLOAT_T_GREEN_MESSAGE = 192,
			STREAMING_FLOAT_T_BLUE_MESSAGE = 193,
			STREAMING_FLOAT_T_ANIMATION_FRAME_MESSAGE = 194,
			STREAMING_FLOAT_T_DELTA_SCALE_MESSAGE = 164,
		STREAMING_FLOAT_DELTA_MESSAGE = 12,
		CREATE_FLOAT_LERP_MESSAGE = 91,
		CHANGE_FLOAT_LERP_SPEED_MESSAGE = 93,
			CHANGE_FLOAT_LERP_FLOAT_TARGET_MESSAGE = 94,
			CHANGE_FLOAT_LERP_INTEGER_TARGET_MESSAGE = 166,
		FLOAT_LERP_FINISH_MESSAGE = 101,
		CHANGE_FLOAT_LERP_MODE_MESSAGE = 126,
		CHANGE_FLOAT_LERP_T_MESSAGE = 214,
		GET_FLOAT_LERP_T_MESSAGE = 215,
		GET_FLOAT_LERP_CURRENT_MESSAGE = 268,
		GET_FLOAT_LERP_DELTA_MESSAGE = 269
	};
	public: enum 
	{
		UNITS_PER_SECOND = 0,
		FIXED_TIME,
		PARABOLA,
		DAMPING,
		MAGNETIC
	};
	friend class VectorLerp;
	friend class QuaternionLerp;
	private: float target;
	private: float elapsedTimeInSec;
    private: float playingSpeed;
	private: int32 lerpMode;

	private: float current;
	private: float delta;
	private: float t;
    private: bool firstFrame;
    private: bool forwardMovingEnable;
	private: void(FloatLerp::* moveToTarget)(float);
    private: static StringANSI floatLerpsSuffix;
    private: static StringANSI floatLerpsDir;
	public: FloatLerp(void);
	public: virtual ~FloatLerp(void);
	public: void SetPlayingSpeed(float _playingSpeed);
	public: float GetPlayingSpeed(void);
	public: virtual void SetPlayingDurationInSec(float _duration);
	public: virtual float GetPlayingDurationInSec(void);
	public: virtual float GetElapsedTimeInSec(void)const;
    public: virtual void Stop(void);
	public: bool IsFinish(void)const;
	public: virtual void SetPlayingMode(int32 _playingMode);
	private: void UpdateForwardPlayingMode(void);
	private: void UpdateBackwardPlayingMode(void);
	private: void UpdateBackwardPlayingModeEx(void);
	private: void UpdateForwardPlayingModeEx(void);
	public: void SetLerpMode(int32 _mode);
	public: int32 GetLerpMode(void)const;
	public: float GetTimeToTarget(void);
	public: void SetTarget(float _target);
	public: float GetTarget(void)const;
	public: float GetCurrent(void)const;
	public: void SetT(float _t);
	public: float GetT(void)const;
	public: float GetDelta(void)const;
	public: void MoveToTargetMagnetic(float _delta);
	public: void MoveToTargetDamping(float _delta);
	public: void MoveToTargetUnitsPerSecond(float _delta);
	public: void MoveToTargetParabola(float _delta);
	public: void MoveToTargetFixedTime(float _delta);
	public: virtual void Update(float _delta);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static FloatLerp* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static FloatLerp* _LoadFromFile(File& _file);
};



#endif
