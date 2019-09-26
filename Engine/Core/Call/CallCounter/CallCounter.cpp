
#include "CallCounter.h"
#include <Engine/Core/Time/SystemTime/SystemTime.h>


CallCounter::CallCounter(void)
{
	Restart();
}


CallCounter::~CallCounter(void)
{}



void CallCounter::Restart(void)
{
	elapsedCallsPerSecond = 0;
	callTotal = 0;
	callPerSecond = 0;

	elapsedTimePerSecond = 0;
	timeTotal = 0;
	timeDelta = 0;

	currentTime = lastTime = 0;
	start = true; 
}


void CallCounter::Update(void)
{
	if(start)
	{
		lastTime = SystemTime::_GetSystemTimeInMs();
		start = false;
	}
	else
	{
		currentTime = SystemTime::_GetSystemTimeInMs();

		++elapsedCallsPerSecond;
		++callTotal;

		timeDelta = (int32)(currentTime - lastTime);
		
		elapsedTimePerSecond += timeDelta;
		timeTotal += timeDelta;
		if(elapsedTimePerSecond >= HMS::MS_IN_SECOND)
		{
			callPerSecond = elapsedCallsPerSecond;
			elapsedCallsPerSecond = 0;
			elapsedTimePerSecond = 0;
		}

		lastTime = currentTime;
	}
}






int32 CallCounter::ElapsedCallsPerSecond(void)const
{
	return elapsedCallsPerSecond;
}

 
int64 CallCounter::TotalCalls(void)const 
{
	return callTotal;
} 


int32 CallCounter::CallsPerSecond(void)const
{
	return callPerSecond;
}


int64 CallCounter::TotalTime(void)const
{ 
	return timeTotal;
}

    
int32 CallCounter::DeltaTime(void)const
{
	return timeDelta;
}

	
int32 CallCounter::ElapsedTimePerSecond(void)const
{
	return elapsedTimePerSecond;
} 