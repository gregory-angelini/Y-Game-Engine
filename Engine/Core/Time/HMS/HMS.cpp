
#include "HMS.h"


HMS::HMS(void): hours(0), minutes(0), seconds(0), milliseconds(0)
{}



HMS::HMS(uint32 _hours, uint32 _minutes, uint32 _seconds, uint32 _milliseconds)
{
	if(_minutes > MINUTE_IN_HOUR - 1)
	{
		_minutes = MINUTE_IN_HOUR - 1;
	}
	if(_seconds > SECOND_IN_MINUTE - 1)
	{
		_seconds = SECOND_IN_MINUTE - 1;
	}
	if(_milliseconds > MS_IN_SECOND - 1)
	{
		_milliseconds = MS_IN_SECOND - 1;
	}
	hours = _hours;
	minutes = _minutes;
	seconds = _seconds;
	milliseconds = _milliseconds;
}


uint32 HMS::GetHours(void)const
{
	return hours;
}
			
uint32 HMS::GetMinutes(void)const
{
	return minutes;
}
	

uint32 HMS::GetSeconds(void)const
{
	return seconds;
}

uint32 HMS::GetMs(void)const
{
	return milliseconds;
}


StringANSI HMS::ToString(bool _showMs)
{
	StringANSI hmsString;
	if(hours < 10) { hmsString += "0"; }
	hmsString += NumberToString(hours);
	hmsString += ":";
	if(minutes < 10) { hmsString += "0"; }
	hmsString += NumberToString(minutes);
	hmsString += ":";
	if(seconds < 10) { hmsString += "0"; }
	hmsString += NumberToString(seconds);

	if(_showMs)
	{
		hmsString += ":";
		if(milliseconds < 100) { hmsString += "0"; }
		if(milliseconds < 10) { hmsString += "0"; }
		hmsString += NumberToString(milliseconds);
	}

	return hmsString;
}


uint32 HMS::ToMs(void)
{
	uint32 ms = _HoursToMs(hours);
	ms += _MinutesToMs(minutes);
	ms += _SecondsToMs(seconds);
	ms += milliseconds;
	return ms;
}


 
uint32 HMS::ToSeconds(void)
{
	uint32 sec = _HoursToSeconds(hours);
	sec += _MinutesToSeconds(minutes);
	sec += seconds;
	return sec;
}



uint32 HMS::ToMinutes(void)
{
	uint32 min = _HoursToMinutes(hours);
	min += minutes;
	return min;
}


uint32 HMS::ToHours(void)
{
	return hours;
}


bool HMS::operator > (const HMS& _hms)const
{
	if(this == &_hms)
	{
		return false;
	}

	if(hours > _hms.hours)
	{
		return true;
	}

	if(minutes > _hms.minutes)
	{
		return true;
	}

	if(seconds > _hms.seconds)
	{
		return true;
	}

	if(milliseconds > _hms.milliseconds)
	{
		return true;
	}
	return false;
}

bool HMS::operator < (const HMS& _hms)const
{
	if(this == &_hms)
	{
		return false;
	}

	if(hours < _hms.hours)
	{
		return true;
	}

	if(minutes < _hms.minutes)
	{
		return true;
	}

	if(seconds < _hms.seconds)
	{
		return true;
	}

	if(milliseconds < _hms.milliseconds)
	{
		return true;
	}
	return false;
}


bool HMS::operator == (const HMS& _hms)const
{
	if(this == &_hms)
	{
		return true;
	}

	if(hours == _hms.hours)
	{
		if(minutes == _hms.minutes)
		{
			if(seconds == _hms.seconds)
			{
				if(milliseconds == _hms.milliseconds)
				{
					return true;
				}
			}
		}
	}
	return false;
}


float HMS::_MsToSeconds(uint32 _milliseconds)
{
	return (float)(_milliseconds / MS_IN_SECOND);
}


float HMS::_MsToMinutes(uint32 _milliseconds)
{
	return _MsToSeconds(_milliseconds) / SECOND_IN_MINUTE;
}


float HMS::_MsToHours(uint32 _milliseconds)
{
	return _MsToMinutes(_milliseconds) / MINUTE_IN_HOUR;
}



HMS HMS::_MsToHMS(uint32 _milliseconds)
{
	uint32 ms = _milliseconds % MS_IN_SECOND;

	uint32 seconds = _milliseconds / MS_IN_SECOND;
	  
	uint32 minutes = _SecondsToMinutes(seconds);
 
	uint32 hours = _MinutesToHours(minutes);

	seconds -= _MinutesToSeconds(minutes);

	minutes -= _HoursToMinutes(hours);
	return HMS(hours, minutes, seconds, ms); 
}



uint32 HMS::_SecondsToMinutes(float _seconds)
{
	if(_seconds > 0.0f)
	{
		return (uint32)(_seconds / SECOND_IN_MINUTE);
	}
	return 0;
}



uint32 HMS::_SecondsToMinutes(uint32 _seconds)
{
	return _seconds / SECOND_IN_MINUTE;
}


uint32 HMS::_SecondsToHours(uint32 _seconds)
{
	return _SecondsToMinutes(_seconds) / MINUTE_IN_HOUR;
}



uint32 HMS::_SecondsToHours(float _seconds)
{
	return _SecondsToMinutes(_seconds) / MINUTE_IN_HOUR;
}



uint32 HMS::_SecondsToMs(uint32 _seconds)
{
	return _seconds * MS_IN_SECOND; 
}


uint32 HMS::_SecondsToMs(float _seconds)
{
	if(_seconds > 0.0f)
	{
		return (uint32)(_seconds * MS_IN_SECOND);
	}
	return 0;
}


HMS HMS::_SecondsToHMS(float _seconds)
{ 
	if(_seconds > 0.0f)
	{
		uint32 minutes = _SecondsToMinutes(_seconds);
	 
		uint32 hours = _MinutesToHours(minutes);

		_seconds -= _MinutesToSeconds(minutes);
	 
		minutes -= _HoursToMinutes(hours);
		return HMS(hours, minutes, _seconds); 
	}
	return HMS();
}



HMS HMS::_SecondsToHMS(uint32 _seconds)
{ 
	uint32 minutes = _SecondsToMinutes(_seconds);
 
	uint32 hours = _MinutesToHours(minutes);

	_seconds -= _MinutesToSeconds(minutes);
 
	minutes -= _HoursToMinutes(hours);
	return HMS(hours, minutes, _seconds); 
}



uint32 HMS::_MinutesToHours(uint32 _minutes)
{
	return _minutes / MINUTE_IN_HOUR;
}


uint32 HMS::_MinutesToSeconds(uint32 _minutes)
{
	return _minutes * SECOND_IN_MINUTE;
}


uint32 HMS::_MinutesToMs(uint32 _minutes)
{
	return _MinutesToSeconds(_minutes) * MS_IN_SECOND;
}

HMS HMS::_MinutesToHMS(uint32 _minutes)
{
	uint32 hours = _MinutesToHours(_minutes);

	_minutes -= _HoursToMinutes(hours);
	return HMS(hours, _minutes, 0); 
}



uint32 HMS::_HoursToMinutes(uint32 _hours)
{
	return _hours * MINUTE_IN_HOUR;
}


HMS HMS::_HoursToHMS(uint32 _hours)
{
	return HMS(_hours, 0, 0);
}



uint32 HMS::_HoursToSeconds(uint32 _hours)
{
	return _HoursToMinutes(_hours) * SECOND_IN_MINUTE;
}



uint32 HMS::_HoursToMs(uint32 _hours)
{
	return _HoursToSeconds(_hours) * MS_IN_SECOND;
}


uint32 HMS::_HMSToMs(HMS _hms)
{
	uint32 ms = _HoursToMs(_hms.hours);
	ms += _MinutesToMs(_hms.minutes);
	ms += _SecondsToMs(_hms.seconds);
	ms += _hms.milliseconds;
	return ms;
}