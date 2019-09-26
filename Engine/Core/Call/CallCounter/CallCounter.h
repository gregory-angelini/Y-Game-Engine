#ifndef CALLCOUNTER_H
#define CALLCOUNTER_H
#include <Engine/Core/Numerical/Numerical.h>
class CallCounter
{
    private: int32 elapsedCallsPerSecond;
    private: int64 callTotal;
	private: int32 callPerSecond;

	private: int32 elapsedTimePerSecond;
	private: int64 timeTotal;
    private: int32 timeDelta;

    private: bool start;
    private: int64 currentTime;
	private: int64 lastTime;
    public: CallCounter(void);
    public: virtual ~CallCounter(void);
    public: void Restart(void);
	public: void Update(void);
	public: int32 ElapsedCallsPerSecond(void)const;
    public: int64 TotalCalls(void)const;
	public: int32 CallsPerSecond(void)const;
    public: int64 TotalTime(void)const;
    public: int32 DeltaTime(void)const;
	public: int32 ElapsedTimePerSecond(void)const;

};





#endif
