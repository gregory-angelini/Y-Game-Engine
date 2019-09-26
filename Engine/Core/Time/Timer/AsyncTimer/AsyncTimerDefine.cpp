
#include "AsyncTimer.h"

AsyncTimer::Define::Define(void)
{
	durationInSec = 0.0f;
	elapsedTimeInSec = 0.0f;
	mode = AsyncTimer::EVER_SHOT; 
	paused = true;
}

AsyncTimer::Define::Define(float _durationInSec, float _elapsedTimeInSec, AsyncTimer::Mode _mode, bool _paused)
{
	durationInSec = _durationInSec;
	elapsedTimeInSec = _elapsedTimeInSec;
	mode = _mode;
	paused = _paused;
}