
#include "SyncTimer.h"
#include <Engine/Core/Time/SystemTime/SystemTime.h>



SyncTimer::SyncTimer(void): playingMode(SyncTimer::EVER_SHOT)
{
	Reset();
}


SyncTimer::SyncTimer(const SyncTimer::Define& _define)
{
	SetState(_define);
}


SyncTimer::Define SyncTimer::GetState(void)
{
	return SyncTimer::Define(GetTargetTimeInSec(), GetElapsedTimeInSec(), GetPlayingMode(), IsPaused());
}


void SyncTimer::SetState(const SyncTimer::Define& _define)
{
	StartSec(_define.targetTimeInSec);
	SetPlayingMode(_define.playingMode);
  
	elapsedTimeInSec = _define.elapsedTimeInSec;
	if(elapsedTimeInSec >= targetTimeInSec)
	{	 
		if(playingMode == SyncTimer::SINGLE_SHOT)
		{
			finished = true;
		}
	}
	if(_define.paused) { Pause(); } 
	else { Play(); }
}



void SyncTimer::Reset(void)
{
	paused = true;
	disabled = true;
	finished = false;
	elapsedTimeInSec = 0.0f;
	targetTimeInSec = 0.0f;
} 


void SyncTimer::StartSec(float _duration)	
{	
	disabled = finished = paused = false;
	elapsedTimeInSec = 0.0f;
	targetTimeInSec = _duration;
}



int32 SyncTimer::GetPlayingMode(void)const
{
	return playingMode;
}


void SyncTimer::Start(HMS _duration)
{
	disabled = finished = paused = false;
	elapsedTimeInSec = 0.0f;
	targetTimeInSec = (float)_duration.ToSeconds();
}




void SyncTimer::Start(void)	
{	
	disabled = finished = paused = false;
	elapsedTimeInSec = 0;
	targetTimeInSec = Numerical<float>::_UpperValue();
}


float SyncTimer::GetElapsedTimeInSec(void)const
{
	return elapsedTimeInSec;
}


	
float SyncTimer::GetRemainedTimeInSec(void)const
{
	return targetTimeInSec - elapsedTimeInSec;
}


	
float SyncTimer::GetTargetTimeInSec(void)const
{
	return targetTimeInSec;
}



bool SyncTimer::Check(float _delta)
{
	if(disabled) { return false; }
	if(!paused && !finished) { elapsedTimeInSec += _delta; }

	if(elapsedTimeInSec >= targetTimeInSec)
	{	 
		if(playingMode == SyncTimer::SINGLE_SHOT)
		{
			if(finished) { return false; }
			finished = true;
		}
		else if(playingMode == SyncTimer::INTERVAL)
		{
			Restart();
		}
		else if(playingMode == SyncTimer::EVER_SHOT)
		{
			finished = true;
		}
		
		return true;
	}
	return false;
} 


void SyncTimer::Pause(void)
{
	paused = true;
}


void SyncTimer::Play(void)
{
	paused = false;
}
 

void SyncTimer::SetPlayingMode(int32 _playingMode)
{
	playingMode = _playingMode;
}


bool SyncTimer::IsPaused(void)const
{
	return paused;
}


  
void SyncTimer::Restart(void)
{
	StartSec(targetTimeInSec);
}
