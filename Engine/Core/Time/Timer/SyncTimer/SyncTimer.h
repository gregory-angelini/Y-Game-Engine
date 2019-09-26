#ifndef SYNCTIMER_H
#define SYNCTIMER_H
#include <Engine/Core/Time/HMS/HMS.h>
class SyncTimer 
{
	public: enum
	{
		EVER_SHOT,
		SINGLE_SHOT,
		INTERVAL
	};
	public: class Define
	{
		public: float targetTimeInSec;
		public: float elapsedTimeInSec;
	    public: int32 playingMode;
		public: bool paused;
		public: Define(void);
	    public: Define(float _targetTimeInSec, float _elapsedTimeInSec, int32 _playingMode, bool _paused);
	};
	private: float elapsedTimeInSec;
	private: float targetTimeInSec;
	private: bool paused;
    private: int32 playingMode;
    private: bool disabled;
    private: bool finished;
	public: SyncTimer(void);
    public: SyncTimer(const SyncTimer::Define& _define);
    public: SyncTimer::Define GetState(void);
    public: void SetState(const SyncTimer::Define& _define);
    public: int32 GetPlayingMode(void)const;
    public: void SetPlayingMode(int32 _playingMode);
	public: float GetElapsedTimeInSec(void)const;
    public: float GetRemainedTimeInSec(void)const;
    public: float GetTargetTimeInSec(void)const;
	public: bool Check(float _delta);
	public: void StartSec(float _duration);
	public: void Start(HMS _duration);
	public: void Start(void);
	public: void Restart(void);
	public: void Reset(void);
	public: void Pause(void);
	public: void Play(void);
	public: bool IsPaused(void)const;
};






#endif
