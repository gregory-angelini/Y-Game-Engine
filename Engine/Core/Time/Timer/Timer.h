#ifndef TIMER_H
#define TIMER_H
#include <Engine/Core/Object/PlayingObject/PlayingObject.h>
#include <Engine/Core/Object/LogicObject/LogicObject.h>
#include <Engine/Core/Time/HMS/HMS.h>
class Timer: public PlayingObject,
	         public LogicObject
{
	public: enum: int64 
	{
		TIMER_CLASS = 32768
	};
	public: enum
	{
		TIMER_FINISH_MESSAGE = 23,
		CREATE_TIMER_MESSAGE = 29
	};
	public: class Define
	{
		public: float targetTimeInSec;
		public: float elapsedTimeInSec;
	    public: int32 playingMode;
		public: bool playingEnable;
		public: Define(void);
	    public: Define(float _targetTimeInSec, float _elapsedTimeInSec, int32 _playingMode, bool _playingEnable);
		public: bool SaveToFile(File& _file);
		public: bool LoadFromFile(File& _file); 
	};
	private: float targetTimeInSec;
	private: float elapsedTimeInSec;
    private: bool firstFrame;
	private: float elapsedFrameTimeInSec;
	private: bool valid;
    private: bool finished;
	private: static StringANSI timersSuffix;
    private: static StringANSI timersDir;
	public: Timer(void);
    public: virtual ~Timer(void);
    public: Timer::Define GetState(void);
    private: void SetState(const Timer::Define& _define);
	private: void Reset(void);
    public: virtual void SetPlayingDurationInSec(float _duration);
    public: void SetPlayingDurationInHMS(HMS _duration);
	public: virtual float GetPlayingDurationInSec(void);
    public: HMS GetPlayingDurationInHMS(void)const;
	public: virtual float GetElapsedTimeInSec(void)const;
    public: float GetRemainedTimeInSec(void)const;
	public: virtual void Update(float _delta = 0.0f);
	public: virtual void Stop(void);
	public: void Restart(void);
	public: static StringANSI _GetDir(void);
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	protected: void SaveDerivedToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
    public: static Timer* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static Timer* _LoadFromFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
};






#endif
