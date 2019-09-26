#ifndef VECTORLERP_H
#define VECTORLERP_H
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Core/Object/LerpingObject/LerpingObject.h>
class VectorLerp: public LerpingObject
{
	public: enum: int64 
	{
		VECTOR_LERP_CLASS = 2199023255552
	};
	public: enum
	{
			STREAMING_VECTOR_CURRENT_MESSAGE = 22,
			STREAMING_TRANSLATABLE_CURRENT_MESSAGE = 172,
			STREAMING_SCALABLE_CURRENT_MESSAGE = 173,
			STREAMING_COLORABLE_CURRENT_MESSAGE = 142,
			STREAMING_VECTOR_DELTA_MESSAGE = 97,
			STREAMING_TRANSLATABLE_DELTA_MESSAGE = 170,
			STREAMING_SCALABLE_DELTA_MESSAGE = 171,
			STREAMING_COLORABLE_DELTA_MESSAGE = 148,
		CREATE_VECTOR_LERP_MESSAGE = 92,
			CHANGE_CONTROL_VECTORS_MESSAGE = 100,
			CHANGE_CONTROL_TRANSLATABLES_MESSAGE = 176,
			CHANGE_CONTROL_SCALABLES_MESSAGE = 177,
			CHANGE_CONTROL_COLORABLES_MESSAGE = 213,
			SET_TWO_VECTORS_MESSAGE = 62,
			SET_TWO_TRANSLATABLES_MESSAGE = 174,
			SET_TWO_SCALABLES_MESSAGE = 175,
			SET_TWO_COLORABLES_MESSAGE = 212,
		CHANGE_VECTOR_LERP_MODE_MESSAGE = 127,
			GET_DATA_FROM_CONTROL_VECTOR_MESSAGE = 208,
			GET_DATA_FROM_CONTROL_TRANSLATABLE_MESSAGE = 209,
			GET_DATA_FROM_CONTROL_SCALABLE_MESSAGE = 210,
			GET_DATA_FROM_CONTROL_COLORABLE_MESSAGE = 211,
		GET_VECTOR_LERP_PATH_MESSAGE = 180,
			GET_VECTOR_LERP_T_MESSAGE = 241,
			GET_TRANSLATABLE_LERP_T_MESSAGE = 242,
			GET_SCALABLE_LERP_T_MESSAGE = 243,
			GET_COLORABLE_LERP_T_MESSAGE = 244,
		GET_CONTROL_VECTORS_MESSAGE = 251,
			SET_DATA_TO_CONTROL_VECTOR_MESSAGE = 256,
			SET_DATA_TO_CONTROL_TRANSLATABLE_MESSAGE = 257,
			SET_DATA_TO_CONTROL_SCALABLE_MESSAGE = 258,
			SET_DATA_TO_CONTROL_COLORABLE_MESSAGE = 259
	};
	public: enum
	{
		BAZIER,
		LAGRANGE,
		RING
	};
	private: int32 segments;
	private: SimpleList<Vector3> controlVectors;
    private: int32 lerpMode;

	private: Vector3 delta;
	private: Vector3 current;
	private: void(VectorLerp::* calculateNVertex)(float);
    private: static StringANSI vectorLerpsSuffix;
    private: static StringANSI vectorLerpsDir;
	public: VectorLerp(void);
	public: virtual ~VectorLerp(void);
	public: virtual void Destroy(void);
	public: bool IsExist(void)const;
	public: Vector3 GetCurrent(void)const;
	public: Vector3 GetDelta(void)const;
	public: virtual void BindFloatLerp(FloatLerp* _floatLerp);
	private: void FloatLerpStartEvent(void);
	private: void FloatLerpPlayingStateIsChanged(int32 _state);
	public: void SetAmountOfSegments(int32 _segments);
    public: int32 GetAmountOfSegments(void)const;
	public: SimpleList<Vector3>& GetControlVectors(void);
	public: void SetLerpMode(int32 _mode);
	public: int32 GetLerpMode(void)const;
    public: void Set(Vector3 _from, Vector3 _to);
    public: void SetFrom(Vector3 _vector);
    public: void SetTo(Vector3 _vector);
	public: Vector3 GetFrom(void)const;
	public: Vector3 GetTo(void)const;
	public: SimpleList<Vector3> GetPath(void);
	private: SimpleList<Vector3> BuildRing(void);
	private: SimpleList<Vector3> BuildBazier(void);
	private: SimpleList<Vector3> BuildLagrange(void);
	private: void CalculateRingNVertex(float _t);
	private: void CalculateLagrangeNVertex(float _t);
	private: double LagrangePolynomial(float _x, int32 _index, float _xi);
	private: void CalculateBazierNVertex(float _t);
	public: void CalculateNVertex(float _t);
	private: Vector3 GetRingNVertex(float _t);
	private: Vector3 GetLagrangeNVertex(float _t);
	private: Vector3 GetBazierNVertex(float _t);
	public: Vector3 GetNVertex(float _t);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static VectorLerp* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static VectorLerp* _LoadFromFile(File& _file);
};



#endif
