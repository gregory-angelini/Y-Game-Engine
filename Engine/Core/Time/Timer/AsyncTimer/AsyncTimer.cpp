
#include "AsyncTimer.h"
#include <Engine/Core/Time/SystemTime/SystemTime.h>



AsyncTimer::AsyncTimer(void): mode(AsyncTimer::EVER_SHOT)
{
	Reset();
}


AsyncTimer::AsyncTimer(const AsyncTimer::Define& _define)
{
	Set(_define);
}




AsyncTimer::Define AsyncTimer::State(void)const
{
	return AsyncTimer::Define(FinishTimeInSec(), ElapsedTimeInSec(), CurrentMode(), IsPaused());
}



void AsyncTimer::Set(const AsyncTimer::Define& _define)
{
	StartSec(_define.durationInSec);
	SetMode(_define.mode);
	uint32 elapsedTime = (uint32)(_define.elapsedTimeInSec * HMS::MS_IN_SECOND);
	targetTimeInMs -= elapsedTime;
	startTimeInMs -= elapsedTime;
	if(currentTimeInMs >= targetTimeInMs)
	{	 
		if(mode == AsyncTimer::SINGLE_SHOT)
		{
			finished = true;
		}
	}
	if(_define.paused) { Pause(); } 
	else { Play(); }
}




void AsyncTimer::Reset(void)
{
	paused = true;
	disabled = true;
	finished = false;
	startTimeInMs = 0;
	currentTimeInMs = 0;
	durationInMs = 0;
	targetTimeInMs = 0;
} 


void AsyncTimer::StartSec(float _duration)	
{	
	disabled = finished = paused = false;
	currentTimeInMs = startTimeInMs = SystemTime::_GetSystemTimeInMs();
	targetTimeInMs = durationInMs = (uint32)(_duration * HMS::MS_IN_SECOND);
	if(Numerical<uint32>::_IsOverflowPlus(startTimeInMs, targetTimeInMs))
	{
		targetTimeInMs = Numerical<uint32>::_UpperValue();
	}
	else
	{
		targetTimeInMs += startTimeInMs;
	}
}



void AsyncTimer::StartMs(uint32 _duration)
{     
	disabled = finished = paused = false;
	currentTimeInMs = startTimeInMs = SystemTime::_GetSystemTimeInMs();
	targetTimeInMs = durationInMs = _duration;
	if(Numerical<uint32>::_IsOverflowPlus(startTimeInMs, targetTimeInMs))
	{
		targetTimeInMs = Numerical<uint32>::_UpperValue();
	}
	else
	{
		targetTimeInMs += startTimeInMs;
	}
}




void AsyncTimer::Start(HMS _duration)
{
	disabled = finished = paused = false;
	currentTimeInMs = startTimeInMs = SystemTime::_GetSystemTimeInMs();
	targetTimeInMs = durationInMs = _duration.ToMs();
	if(Numerical<uint32>::_IsOverflowPlus(startTimeInMs, targetTimeInMs))
	{
		targetTimeInMs = Numerical<uint32>::_UpperValue();
	}
	else
	{
		targetTimeInMs += startTimeInMs;
	}
}




void AsyncTimer::Start(void)	
{	
	disabled = finished = paused = false;

	currentTimeInMs = startTimeInMs = SystemTime::_GetSystemTimeInMs();
	targetTimeInMs = durationInMs = Numerical<uint32>::_UpperValue();
}


float AsyncTimer::ElapsedTimeInSec(void)const
{
	return (float)(currentTimeInMs - startTimeInMs) / HMS::MS_IN_SECOND;
}
 


	
float AsyncTimer::RemainedTimeInSec(void)const
{
	return ((float)targetTimeInMs - currentTimeInMs) / (float)HMS::MS_IN_SECOND;
}





float AsyncTimer::FinishTimeInSec(void)const
{
	return (float)(durationInMs) / HMS::MS_IN_SECOND;
}
  


bool AsyncTimer::Check(void)
{
	if(disabled) { return false; }
	if(!paused && !finished) { currentTimeInMs = SystemTime::_GetSystemTimeInMs(); }
    
	if(currentTimeInMs >= targetTimeInMs)
	{	 
		if(mode == AsyncTimer::SINGLE_SHOT)
		{
			if(finished) { return false; }
			finished = true;
		}
		else if(mode == AsyncTimer::INTERVAL)
		{
			Restart();
		}
		else if(mode == AsyncTimer::EVER_SHOT)
		{
			finished = true;
		}
		
		return true;
	}
	return false;
} 





void AsyncTimer::Pause(void)
{
	paused = true;
}


float AsyncTimer::Play(void)
{
	if(paused)
	{
		paused = false;
		 
		if(disabled || finished) { return 0.0f; }

		uint32 offset = SystemTime::_GetSystemTimeInMs() - currentTimeInMs;
		if(Numerical<uint32>::_IsOverflowPlus(targetTimeInMs, offset))
		{
			targetTimeInMs = Numerical<uint32>::_UpperValue();
			if(Numerical<uint32>::_IsOverflowPlus(startTimeInMs, offset))
			{
				startTimeInMs = Numerical<uint32>::_UpperValue();
			}
			else
			{ 
				startTimeInMs += offset;
			} 

		}
		else
		{
			targetTimeInMs += offset;
			startTimeInMs += offset;
		}

		currentTimeInMs = SystemTime::_GetSystemTimeInMs();
		
		return (float)offset / HMS::MS_IN_SECOND;
	}
	return 0.0f;
}
 


AsyncTimer::Mode AsyncTimer::CurrentMode(void)const
{
	return mode;
}


void AsyncTimer::SetMode(AsyncTimer::Mode _mode)
{
	mode = _mode;
}


bool AsyncTimer::IsPaused(void)const
{
	return paused;
}


  
void AsyncTimer::Restart(void)
{ 
	StartMs(durationInMs);
}
