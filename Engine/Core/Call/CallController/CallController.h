#ifndef CALLCONTROLLER_H
#define CALLCONTROLLER_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Call/CallCounter/CallCounter.h>
#include <Engine/Core/Call/Caller/Caller.h>
class CallController
{
	public: enum CallMode
	{
		FIXED = 0,
		UNLIM = 1
	};
	private: int32 fixedFPS;
    private: float fixedDelta;
	private: float fixedDeltaAccumulator;
   
    private: int32 maxSteps;
    private: Caller<> action;
	private: CallMode mode;
	private: CallCounter updateCallCounter;
	public: CallController(void);
    public: CallController(Caller<> _function, int32 _fps, int32 _maxSteps = 5);
	public: virtual ~CallController(void);
	public: void Set(Caller<> _function, int32 _fps, int32 _maxSteps = 5);
    public: int32 Call(float _delta);
	public: void QuickCall(float _delta);
	public: void SetMode(CallMode _mode);
	public: CallMode GetMode(void)const;
	public: void SetFPS(int32 _fps);
	public: int32 GetFPS(void)const;
	public: float GetDelta(void)const;
	public: void SetMaxSteps(int32 _maxSteps);
	public: int32 GetMaxSteps(void)const;
	public: void SetAction(Caller<> _function);
	public: bool IsValid(void)const;
};





#endif
