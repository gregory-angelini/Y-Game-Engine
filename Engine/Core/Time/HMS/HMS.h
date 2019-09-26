#ifndef HMS_H
#define HMS_H
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Numerical/Numerical.h>
class HMS
{
	public: enum 
	{
		MS_IN_SECOND = 1000,
		SECOND_IN_MINUTE = 60,
		MINUTE_IN_HOUR = 60
	};
	private: uint32 hours;
	private: uint32 minutes;
	private: uint32 seconds;
	private: uint32 milliseconds;
	public: HMS(void);
	public: HMS(uint32 _hours, uint32 _minutes, uint32 _seconds, uint32 _milliseconds = 0);
	public: uint32 GetHours(void)const;
	public: uint32 GetMinutes(void)const;
	public: uint32 GetSeconds(void)const;
	public: uint32 GetMs(void)const;
	public: StringANSI ToString(bool _showMs = false);
	public: uint32 ToMs(void);
	public: uint32 ToSeconds(void);
	public: uint32 ToMinutes(void);
	public: uint32 ToHours(void);
	public: static float _MsToSeconds(uint32 _milliseconds);
	public: static float _MsToMinutes(uint32 _milliseconds);
	public: static float _MsToHours(uint32 _milliseconds);
	public: static HMS _MsToHMS(uint32 _milliseconds);
	public: static uint32 _SecondsToMs(uint32 _seconds);
	public: static uint32 _SecondsToMs(float _seconds);
	public: static uint32 _SecondsToMinutes(uint32 _seconds);
	public: static uint32 _SecondsToMinutes(float _seconds);
	public: static uint32 _SecondsToHours(uint32 _seconds);
	public: static uint32 _SecondsToHours(float _seconds);
	public: static HMS _SecondsToHMS(uint32 _seconds);
	public: static HMS _SecondsToHMS(float _seconds);
	public: static uint32 _MinutesToMs(uint32 _minutes);
	public: static uint32 _MinutesToSeconds(uint32 _minutes);
	public: static uint32 _MinutesToHours(uint32 _minutes);
	public: static HMS _MinutesToHMS(uint32 _minutes);
	public: static uint32 _HoursToMs(uint32 _hours);
	public: static uint32 _HoursToSeconds(uint32 _hours);
	public: static uint32 _HoursToMinutes(uint32 _hours);
	public: static HMS _HoursToHMS(uint32 _hours);
	public: uint32 static _HMSToMs(HMS _hms);
	public: bool operator < (const HMS& _hms)const;
	public: bool operator > (const HMS& _hms)const;
	public: bool operator == (const HMS& _hms)const;
};







#endif
