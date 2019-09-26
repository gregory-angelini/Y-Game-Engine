

#include "SystemTime.h"
#include <Engine/Core/Numerical/Numerical.h>

SystemTime::SystemTime(void)
{}


HMS SystemTime::_GetTimeToOverflow(void)
{
	return HMS::_MsToHMS(Numerical<int32>::_UpperValue() - _GetSystemTimeInMs());
}


HMS SystemTime::_GetSystemTimeInHMS(void)
{
	return HMS::_MsToHMS(_GetSystemTimeInMs());
}


bool SystemTime::_SetQuantum(int32 _period)
{
	return accuracy.SetQuantum(_period);
}


void SystemTime::_SetMinQuantum(void)
{
	accuracy.SetMinQuantum();
}


int32 SystemTime::_GetQuantum(void)
{
	return accuracy.GetQuantum();
}






