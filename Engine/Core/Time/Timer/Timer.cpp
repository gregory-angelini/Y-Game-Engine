
#include "Timer.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
StringANSI Timer::timersSuffix = "timer";
StringANSI Timer::timersDir = "timers/";



#ifdef SendMessage
   #undef SendMessage
#endif



Timer::Timer(void): firstFrame(true)
{
	AddClassProperty(TIMER_CLASS);
	Reset();
}



Timer::~Timer(void)
{
	Destroying();
}


Timer::Define Timer::GetState(void)
{
	return Timer::Define(GetPlayingDurationInSec(), GetElapsedTimeInSec(), GetCyclePlayingMode(), IsPlaying());
} 



void Timer::SetState(const Timer::Define& _define)
{
	finished = false;
	firstFrame = true;
 
	SetPlayingDurationInSec(_define.targetTimeInSec);
	 
	elapsedTimeInSec = _define.elapsedTimeInSec;
  
	SetPlayingMode(_define.playingMode);
	if(targetTimeInSec > 0.0f && elapsedFrameTimeInSec >= targetTimeInSec)
	{ 
		if(playingMode & PlayingObject::SINGLE_SHOT_PLAYING)
		{
			finished = true;
		}
	}
	if(_define.playingEnable) 
	{ 
		Play(); 
	} 
	else 
	{ 
		Pause(); 
	} 
}



void Timer::Reset(void)
{
	targetTimeInSec = elapsedTimeInSec = 0.0f;

	playingEnable = false;
	finished = false;
	firstFrame = true;
	valid = false;
} 


void Timer::Stop(void)
{ 
	elapsedTimeInSec = 0.0f;

	finished = false;
	firstFrame = true;

	PlayingObject::Stop(); 
}


  
void Timer::Restart(void)
{
	elapsedTimeInSec = 0.0f;

	playingEnable = true;
	finished = false;
	firstFrame = true;
}
 


float Timer::GetElapsedTimeInSec(void)const
{
	return elapsedTimeInSec;
}



float Timer::GetRemainedTimeInSec(void)const
{
	if(elapsedTimeInSec > targetTimeInSec)
	{
		return targetTimeInSec - (elapsedTimeInSec - (targetTimeInSec * floorf(elapsedTimeInSec / targetTimeInSec)));
	}

	return targetTimeInSec - elapsedTimeInSec;
}



float Timer::GetPlayingDurationInSec(void)
{
	return targetTimeInSec;
}



HMS Timer::GetPlayingDurationInHMS(void)const
{
	return HMS::_SecondsToHMS(targetTimeInSec);
}



void Timer::SetPlayingDurationInSec(float _duration)
{
	if(_duration > 0.0f)
	{
		targetTimeInSec = _duration;

		valid = true;
	}
}



void Timer::SetPlayingDurationInHMS(HMS _duration)
{
	uint32 ms = _duration.ToMs();
	
	if(ms > 0)
	{
		targetTimeInSec = (float)ms / HMS::MS_IN_SECOND;

		valid = true;
	}
}



void Timer::Update(float _delta)
{
	if(!freezeEnable)
	{ 
		if(valid && playingEnable && !finished)
		{
			_delta *= GetDeltaScale();
			if(firstFrame)
			{
				elapsedTimeInSec = elapsedFrameTimeInSec = 0.0f;
				firstFrame = false;
			}
			else
			{
				elapsedTimeInSec += _delta;
				elapsedFrameTimeInSec += _delta;
			}

			while(elapsedFrameTimeInSec >= targetTimeInSec)
			{

				elapsedFrameTimeInSec -= targetTimeInSec;

				if(playingMode & SINGLE_SHOT_PLAYING)
				{
					finished = true;
					firstFrame = true;
				}
				if(IsReceiversExist(Timer::TIMER_FINISH_MESSAGE)) { SendMessage(Timer::TIMER_FINISH_MESSAGE); }
			}
		}
	}
}


StringANSI Timer::_GetDir(void)
{
	return timersDir;
}



void Timer::_SetFileSuffix(StringANSI _suffix)
{
	timersSuffix = _suffix;
}


StringANSI Timer::_GetFileSuffix(void)
{
	return timersSuffix;
}



bool Timer::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + timersDir + GetName() + "." + timersSuffix;
	}

	File file(_path, File::REWRITE);
		
	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool Timer::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
			
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}


bool Timer::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + Timer::_GetDir() + _name + "." + Timer::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Timer::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
			
		SaveDerivedToFile(_file);  
		return true;
	}
	return false;
}



void Timer::SaveDerivedToFile(File& _file)
{
	LogicObject::SaveDerivedToFile(_file);
	_file.Write(firstFrame);
	_file.Write(elapsedFrameTimeInSec);
	GetState().SaveToFile(_file);
}



bool Timer::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + timersDir + _path + "." + timersSuffix;
	}
	
	File file(_path, File::READ);

	return LoadFromFile(file);
}


bool Timer::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		int32 version;
		_file.Read(version);
		_file.Read(properties);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		Rename(name);
		
		LoadDerivedFromFile(_file);
		return true;
	}
	return false;
} 


Timer* Timer::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + timersDir + _path + "." + timersSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


Timer* Timer::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Timer* timer = new Timer();

		if(timer->LoadFromFile(_file))
		{
			return timer;
		}
		delete timer;
	}
	return NIL;
}


void Timer::LoadDerivedFromFile(File& _file)
{
	LogicObject::LoadDerivedFromFile(_file);
	_file.Read(firstFrame);
	_file.Read(elapsedFrameTimeInSec);
	Define define;
	define.LoadFromFile(_file);
	SetState(define);
}