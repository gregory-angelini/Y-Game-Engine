
#include "SyncTimer.h"
SyncTimer::Define::Define(void)
{
	targetTimeInSec = 0.0f;
	elapsedTimeInSec = 0.0f;
	playingMode = SyncTimer::EVER_SHOT; 
	paused = true;
}


SyncTimer::Define::Define(float _targetTimeInSec, float _elapsedTimeInSec, int32 _playingMode, bool _paused)
{
	targetTimeInSec = _targetTimeInSec;
	elapsedTimeInSec = _elapsedTimeInSec;
	playingMode = _playingMode;
	paused = _paused;
}