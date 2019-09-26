#ifndef SYSTEMTIME_H
#define SYSTEMTIME_H
#include <Engine/Core/Time/HMS/HMS.h>
class SystemTime
{
	public: enum
	{
		GET_SYSTEM_TIME_MESSAGE = 116
	};
	private: class Accuracy
	{
		public: int32 periodInMs;
		public: Accuracy(void);
		public: ~Accuracy(void);
		public: void SetMinQuantum(void);
		public: bool SetQuantum(int32 _period);
		public: int32 GetQuantum(void)const;
	};
	private: static Accuracy accuracy;
	private: SystemTime(void);
	public: static bool _SetQuantum(int32 _period);
	public: static int32 _GetQuantum(void);
	public: static void _SetMinQuantum(void);
	public: static int32 _GetSystemTimeInMs(void);
	public: static float _GetSystemTimeInSec(void);
	public: static HMS _GetSystemTimeInHMS(void);
	public: static HMS _GetTimeToOverflow(void);

};






#endif
