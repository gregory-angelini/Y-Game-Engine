
#include "SystemTime.h"
#include <windows.h>
SystemTime::Accuracy SystemTime::accuracy;


SystemTime::Accuracy::Accuracy(void)
{			
	periodInMs = 1;
	while(::timeBeginPeriod(periodInMs) == TIMERR_NOCANDO)
	{
		++periodInMs;
	}
}

void SystemTime::Accuracy::SetMinQuantum(void)
{
	periodInMs = 1;

	::timeEndPeriod(periodInMs);
	while(::timeBeginPeriod(periodInMs) == TIMERR_NOCANDO)
	{
		++periodInMs;
	}
}


int32 SystemTime::Accuracy::GetQuantum(void)const
{
	return periodInMs;
}

bool SystemTime::Accuracy::SetQuantum(int32 _period)
{
	if(_period == periodInMs) { return true; }

	if(_period > 0)
	{
		::timeEndPeriod(periodInMs);

		if(::timeBeginPeriod(_period) == TIMERR_NOCANDO)
		{
			return false;
		}
		periodInMs = _period;
		return true;
	}
	return false;
}


SystemTime::Accuracy::~Accuracy(void)
{
	::timeEndPeriod(periodInMs);
}



int32 SystemTime::_GetSystemTimeInMs(void)
{ 
	return ::timeGetTime();
}




float SystemTime::_GetSystemTimeInSec(void)
{
	return HMS::_MsToSeconds(_GetSystemTimeInMs());
}
