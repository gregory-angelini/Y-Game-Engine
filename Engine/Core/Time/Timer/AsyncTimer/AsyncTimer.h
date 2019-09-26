#ifndef ASYNCTIMER_H
#define ASYNCTIMER_H
#include <Engine/Core/Time/HMS/HMS.h>
class AsyncTimer 
{
	public: enum Mode
	{
		EVER_SHOT,
		SINGLE_SHOT,
		INTERVAL
	};
	public: class Define
	{
		public: float durationInSec;
		public: float elapsedTimeInSec;
	    public: AsyncTimer::Mode mode;
		public: bool paused;
		public: Define(void);
	    public: Define(float _durationInSec, float _elapsedTimeInSec, AsyncTimer::Mode _mode, bool _paused);
	};
	private: uint32 startTimeInMs;
	private: uint32 currentTimeInMs;
	private: uint32 durationInMs;
	private: uint32 targetTimeInMs;
	private: bool paused;
    private: AsyncTimer::Mode mode;
    private: bool disabled;
	private: bool finished;
	public: AsyncTimer(void);
    public: AsyncTimer(const AsyncTimer::Define& _define);
    public: AsyncTimer::Mode CurrentMode(void)const;
	public: float ElapsedTimeInSec(void)const;
	public: float RemainedTimeInSec(void)const;
    public: float FinishTimeInSec(void)const;
    public: AsyncTimer::Define State(void)const;
	public: bool Check(void);
	public: bool IsPaused(void)const;
    public: void Set(const AsyncTimer::Define& _define);
    public: void SetMode(AsyncTimer::Mode _mode);
	public: void StartSec(float _duration);
	public: void Start(void);
	public: void StartMs(uint32 _duration);
	public: void Start(HMS _duration);
	public: float Play(void);
	public: void Restart(void);
	public: void Reset(void);
	public: void Pause(void);
};






#endif
