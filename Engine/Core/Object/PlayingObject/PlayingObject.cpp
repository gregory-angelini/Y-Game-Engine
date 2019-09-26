#include "PlayingObject.h"




PlayingObject::PlayingObject(void): playingEnable(false)
{
	AddClassProperty(PLAYING_OBJECT_CLASS);
	playingMode = FORWARD_PLAYING | ONEWAY_PLAYING | SINGLE_SHOT_PLAYING;
}




PlayingObject::~PlayingObject(void)
{
	Destroying();
}



bool PlayingObject::IsPlaying(void)const
{
	return playingEnable;
}


bool PlayingObject::IsPaused(void)const
{
	return !playingEnable;
}



void PlayingObject::Play(void)
{
	playingEnable = true;

	if(IsReceiversExist(CHANGE_PLAYING_STATE_MESSAGE)) { SendPackage(CHANGE_PLAYING_STATE_MESSAGE, (int32)PLAY); }
}


void PlayingObject::Pause(void)
{
	playingEnable = false;

	if(IsReceiversExist(CHANGE_PLAYING_STATE_MESSAGE)) { SendPackage(CHANGE_PLAYING_STATE_MESSAGE, (int32)PAUSE); }
}



void PlayingObject::Stop(void)
{
	playingEnable = false;

	if(IsReceiversExist(CHANGE_PLAYING_STATE_MESSAGE)) { SendPackage(CHANGE_PLAYING_STATE_MESSAGE, (int32)STOP); }
}



bool PlayingObject::IsForwardPlayingEnabled(void)const
{
	if(playingMode & FORWARD_PLAYING) { return true; }
	else { return false; } 
}


bool PlayingObject::IsOnewayPlayingEnabled(void)const
{
	if(playingMode & ONEWAY_PLAYING) { return true; }
	else { return false; }
}


bool PlayingObject::IsSingleShotPlayingEnabled(void)const
{
	if(playingMode & SINGLE_SHOT_PLAYING) { return true; }
	else { return false; }
}



void PlayingObject::SetPlayingMode(int32 _playingMode)
{
	if(!(playingMode & _playingMode))
	{
		switch(_playingMode)
		{
			case FORWARD_PLAYING:
			{
				playingMode |= FORWARD_PLAYING;
				playingMode &= ~BACKWARD_PLAYING;
				if(IsReceiversExist(CHANGE_PLAYING_MODE_MESSAGE)) { SendPackage(CHANGE_PLAYING_MODE_MESSAGE, (int32)FORWARD_PLAYING); }
				break;
			}

			case BACKWARD_PLAYING:
			{
				playingMode |= BACKWARD_PLAYING;
				playingMode &= ~FORWARD_PLAYING;
				if(IsReceiversExist(CHANGE_PLAYING_MODE_MESSAGE)) { SendPackage(CHANGE_PLAYING_MODE_MESSAGE, (int32)BACKWARD_PLAYING); }
				break;
			}

			case ONEWAY_PLAYING:
			{
				playingMode |= ONEWAY_PLAYING;
				playingMode &= ~PING_PONG_PLAYING;
				if(IsReceiversExist(CHANGE_PLAYING_MODE_MESSAGE)) { SendPackage(CHANGE_PLAYING_MODE_MESSAGE, (int32)ONEWAY_PLAYING); }
				break;
			}

			case PING_PONG_PLAYING:
			{
				playingMode |= PING_PONG_PLAYING;
				playingMode &= ~ONEWAY_PLAYING;
				if(IsReceiversExist(CHANGE_PLAYING_MODE_MESSAGE)) { SendPackage(CHANGE_PLAYING_MODE_MESSAGE, (int32)PING_PONG_PLAYING); }
				break;
			}

			case SINGLE_SHOT_PLAYING:
			{
				playingMode |= SINGLE_SHOT_PLAYING;
				playingMode &= ~LOOP_PLAYING;
				if(IsReceiversExist(CHANGE_PLAYING_MODE_MESSAGE)) { SendPackage(CHANGE_PLAYING_MODE_MESSAGE, (int32)SINGLE_SHOT_PLAYING); }
				break;
			}

			case LOOP_PLAYING:
			{
				playingMode |= LOOP_PLAYING;
				playingMode &= ~SINGLE_SHOT_PLAYING;
				if(IsReceiversExist(CHANGE_PLAYING_MODE_MESSAGE)) { SendPackage(CHANGE_PLAYING_MODE_MESSAGE, (int32)LOOP_PLAYING); }
				break;
			}
		}
	}
}


int32 PlayingObject::GetPlayingMode(void)const
{
	return playingMode;
}
 

int32 PlayingObject::GetDirectionPlayingMode(void)const
{
	return playingMode & FORWARD_PLAYING ? FORWARD_PLAYING : BACKWARD_PLAYING;
}
 

int32 PlayingObject::GetReturnPlayingMode(void)const
{
	return playingMode & ONEWAY_PLAYING ? ONEWAY_PLAYING : PING_PONG_PLAYING;
}

 
int32 PlayingObject::GetCyclePlayingMode(void)const
{
	return playingMode & SINGLE_SHOT_PLAYING ? SINGLE_SHOT_PLAYING : LOOP_PLAYING;
}
 


void PlayingObject::SetPlayingDurationInSec(float _duration)
{}


float PlayingObject::GetPlayingDurationInSec(void)
{
	return 0.0f;
}

				
float PlayingObject::GetElapsedTimeInSec(void)const
{
	return 0.0f;
}


void PlayingObject::SaveDerivedToFile(File& _file)
{
	_file.Write(playingEnable);
	_file.Write(playingMode);
}



void PlayingObject::LoadDerivedFromFile(File& _file)
{
	_file.Read(playingEnable);
	_file.Read(playingMode);
}