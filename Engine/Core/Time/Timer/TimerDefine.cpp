
#include "Timer.h"

Timer::Define::Define(void)
{
	targetTimeInSec = 0.0f;
	elapsedTimeInSec = 0.0f;
	playingMode = PlayingObject::SINGLE_SHOT_PLAYING;
	playingEnable = false;
}



Timer::Define::Define(float _targetTimeInSec, float _elapsedTimeInSec, int32 _playingMode, bool _playingEnable)
{
	targetTimeInSec = _targetTimeInSec;
	elapsedTimeInSec = _elapsedTimeInSec;
	playingMode = _playingMode;
	playingEnable = _playingEnable;
}



bool Timer::Define::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(targetTimeInSec);
		_file.Write(elapsedTimeInSec);
		_file.Write(playingMode);
		_file.Write(playingEnable);
		return true;
	}
	return false;
}



bool Timer::Define::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		_file.Read(targetTimeInSec);
		_file.Read(elapsedTimeInSec);
		_file.Read(playingMode);
		_file.Read(playingEnable);
		return true;
	}
	return false;
}