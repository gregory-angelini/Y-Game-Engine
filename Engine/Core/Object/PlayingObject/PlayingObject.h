#ifndef PLAYINGOBJECT_H
#define PLAYINGOBJECT_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
class PlayingObject: public virtual AbstractObject
{
	public: enum: int64 
	{
		PLAYING_OBJECT_CLASS = 4194304
	};
	public: enum 
	{
		PLAY,
		PAUSE,
		STOP
	};
	public: enum 
	{
		FORWARD_PLAYING = 1,
		BACKWARD_PLAYING = 2,
		PING_PONG_PLAYING = 4,
		ONEWAY_PLAYING = 8,
		LOOP_PLAYING = 16,
		SINGLE_SHOT_PLAYING = 32
	};
	public: enum
	{
		CHANGE_PLAYING_STATE_MESSAGE = 27,
		CHANGE_PLAYING_MODE_MESSAGE = 17,
		CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE = 39,
		REPLAY_MESSAGE = 321
	};
	protected: bool playingEnable;
	protected: int32 playingMode;
	public: PlayingObject(void);
	public: virtual ~PlayingObject(void);
	public: bool IsPlaying(void)const;
	public: bool IsPaused(void)const;
    public: virtual void Stop(void);
	public: virtual void Play(void);
	public: virtual void Pause(void);
	public: virtual void SetPlayingMode(int32 _playingMode);
	public: int32 GetPlayingMode(void)const;
	public: int32 GetDirectionPlayingMode(void)const;
	public: int32 GetReturnPlayingMode(void)const;
	public: int32 GetCyclePlayingMode(void)const;
	public: bool IsForwardPlayingEnabled(void)const;
	public: bool IsOnewayPlayingEnabled(void)const;
	public: bool IsSingleShotPlayingEnabled(void)const;
	public: virtual void SetPlayingDurationInSec(float _duration);
	public: virtual float GetPlayingDurationInSec(void);
	public: virtual float GetElapsedTimeInSec(void)const;
	protected: void SaveDerivedToFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
};


#endif 