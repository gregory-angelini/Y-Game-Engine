#ifndef QUATERNIONLERP_H
#define QUATERNIONLERP_H
#include <Engine/Math/Quaternion/Quaternion.h>
#include <Engine/Core/Object/LerpingObject/LerpingObject.h>
class QuaternionLerp: public LerpingObject
{
	public: enum: int64 
	{
		QUATERNION_LERP_CLASS = 1099511627776
	};
	public: enum
	{
			STREAMING_QUATERNION_CURRENT_MESSAGE = 47,
			STREAMING_ROTATABLE_CURRENT_MESSAGE = 168,
			STREAMING_QUATERNION_DELTA_MESSAGE = 98,
			STREAMING_ROTATABLE_DELTA_MESSAGE = 169,
		CREATE_QUATERNION_LERP_MESSAGE = 90,
			CHANGE_CONTROL_QUATERNIONS_MESSAGE = 95,
			CHANGE_CONTROL_ROTATABLES_MESSAGE = 178,
			SET_TWO_QUATERNIONS_MESSAGE = 56,
			SET_TWO_ROTATABLES_MESSAGE =  179,
			GET_DATA_FROM_CONTROL_QUATERNION_MESSAGE = 231,
			GET_DATA_FROM_CONTROL_ROTATABLE_MESSAGE = 232,
			GET_QUATERNION_LERP_T_MESSAGE = 245,
			GET_ROTATABLE_LERP_T_MESSAGE = 246,
		GET_QUATERNION_LERP_PATH_MESSAGE = 247,
		GET_CONTROL_QUATERNIONS_MESSAGE = 252,
			SET_DATA_TO_CONTROL_QUATERNION_MESSAGE = 260,
			SET_DATA_TO_CONTROL_ROTATABLE_MESSAGE = 261
	};
	private: int32 segments;
	private: SimpleList<Quaternion> controlQuaternions;
	private: Quaternion delta;
	private: Quaternion current;
	private: Quaternion qFrom, qTo;
	private: int32 fromIndex, toIndex;
    private: static StringANSI quaternionLerpsSuffix;
    private: static StringANSI quaternionLerpsDir;
	public: QuaternionLerp(void);
	public: virtual ~QuaternionLerp(void);
	public: Quaternion GetCurrent(void)const;
	public: Quaternion GetDelta(void)const;
	public: virtual void Destroy(void);
	public: bool IsExist(void)const;
	public: virtual void BindFloatLerp(FloatLerp* _floatLerp);
	private: void FloatLerpStartEvent(void);
	private: void FloatLerpPlayingStateIsChanged(int32 _state);
	public: void SetAmountOfSegments(int32 _segments);
    public: int32 GetAmountOfSegments(void)const;
	public: SimpleList<Quaternion>& GetControlQuaternions(void);
    public: void Set(Quaternion _from, Quaternion _to);
    public: void SetFrom(Quaternion _quaternion);
    public: void SetTo(Quaternion _quaternion);
	public: Quaternion GetFrom(void)const;
	public: Quaternion GetTo(void)const;
	public: SimpleList<Quaternion> GetPath(void);
	public: void CalculateNQuaternion(float _t);
	public: Quaternion GetNQuaternion(float _t);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static QuaternionLerp* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static QuaternionLerp* _LoadFromFile(File& _file);
};



#endif
