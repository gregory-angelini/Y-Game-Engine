#include "AbstractAnimation.h"
#include <Engine/Core/Scene/LogicScene/LogicScene.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
StringANSI AbstractAnimation::animationsDir = "animations/";


#ifdef SendMessage
   #undef SendMessage
#endif




AbstractAnimation::AbstractAnimation(void): frameDurationInSec(0.0f), amountOfFrames(0), currentFrame(0), firstFrame(true), elapsedFrameTimeInSec(0.0f), elapsedTimeInSec(0.0f)
{
	AddClassProperty(ABSTRACT_ANIMATION_CLASS);
	playingDirection = 1;
}



AbstractAnimation::~AbstractAnimation(void)
{
	Destroying();
}


void AbstractAnimation::Stop(void)
{
	elapsedTimeInSec = 0.0f;

	if(playingMode & BACKWARD_PLAYING)
	{
		SetCurrentFrame(amountOfFrames - 1);
		playingDirection = -1;
	}
	else if(playingMode & FORWARD_PLAYING)
	{
		SetCurrentFrame(0);
		playingDirection = 1;
	}

	firstFrame = true;
	PlayingObject::Stop();
}



void AbstractAnimation::SetPlayingMode(int32 _playingMode)
{
	if(_playingMode == FORWARD_PLAYING)
	{
		playingDirection = 1;
	}
	else if(_playingMode == BACKWARD_PLAYING)
	{
		playingDirection = -1;
	}
	if(playingMode & PING_PONG_PLAYING)
	{
		if(_playingMode == ONEWAY_PLAYING)
		{ 
			if(playingMode & FORWARD_PLAYING)
			{
				playingDirection = 1;
			}
			else
			{
				playingDirection = -1;
			}
		}
	}
	
	PlayingObject::SetPlayingMode(_playingMode);
}



bool AbstractAnimation::IsLastFrame(void)
{
	if(playingMode & FORWARD_PLAYING)
	{
		if(currentFrame == amountOfFrames - 1)
		{
			return true;
		}
	}
	else
	{
		if(currentFrame == 0)
		{
			return true;
		}
	}
	return false;
}




float AbstractAnimation::GetPlayingFrameDurationInSec(void)const
{
	return frameDurationInSec;
}


void AbstractAnimation::SetPlayingFrameDurationInSec(float _duration)
{
	if(_duration > 0.0f)
	{
		frameDurationInSec = _duration;
	}
	else
	{
		frameDurationInSec = 0.0f;
	}
}


float AbstractAnimation::GetPlayingDurationInSec(void)
{
	return frameDurationInSec * amountOfFrames;
}


void AbstractAnimation::SetPlayingDurationInSec(float _duration)
{
	if(_duration > 0.0f)
	{
		if(amountOfFrames > 0)
		{
			frameDurationInSec = _duration / amountOfFrames;
		}
	}
	else
	{
		frameDurationInSec = 0.0f;
	}
}


void AbstractAnimation::SetFPS(float _fps) 
{ 
	if(_fps > 0.0f)
	{
		frameDurationInSec = 1.0f / _fps; 
	}
	else
	{
		frameDurationInSec = 0.0f;
	}
}


float AbstractAnimation::GetElapsedTimeInSec(void)const
{
	return elapsedTimeInSec;
}



float AbstractAnimation::GetFPS(void)const
{ 
	if(frameDurationInSec > 0.0f)
	{
		return 1.0f / frameDurationInSec; 
	}
	return 0.0f;
}



void AbstractAnimation::SetCurrentFrame(StringANSI _name)
{}



StringANSI AbstractAnimation::GetNameOfCurrentFrame(void)const
{
	return "";
}



int32 AbstractAnimation::GetCurrentFrame(void)const
{
	return currentFrame;
}
	


void AbstractAnimation::SetCurrentFrame1(float _t)
{
	if(_t >= 0.0f && _t <= 1.0f)
	{
		if(amountOfFrames > 0)
		{
			currentFrame = _t * (float)(amountOfFrames - 1);
			firstFrame = true; 
			CurrentFrameIsChanged();
		}
	}
}


void AbstractAnimation::SetCurrentFrame(int32 _index)
{
	if(_index >= 0 && _index < amountOfFrames)
	{
		currentFrame = _index;
		firstFrame = true; 
		CurrentFrameIsChanged();
	} 
	else if(_index == -1 && amountOfFrames > 0)
	{ 
		currentFrame = playingMode & FORWARD_PLAYING ? amountOfFrames - 1 : 0;
		firstFrame = true;
		CurrentFrameIsChanged();
	}
}



void AbstractAnimation::CurrentFrameIsChanged(void)
{
	if(IsReceiversExist(CHANGE_ANIMATION_FRAME_MESSAGE)) { SendPackage(CHANGE_ANIMATION_FRAME_MESSAGE, currentFrame); }
}


int32 AbstractAnimation::GetAmountOfFrames(void)const
{
	return amountOfFrames;
}


void AbstractAnimation::SetAmountOfFrames(int32 _frames)
{
	amountOfFrames = _frames;
}


void AbstractAnimation::Update(float _delta)
{
	if(!freezeEnable)
	{
		if(playingEnable && amountOfFrames > 1 && frameDurationInSec > 0.0f)
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
			while(elapsedFrameTimeInSec >= frameDurationInSec)
			{

				elapsedFrameTimeInSec -= frameDurationInSec;

				if(currentFrame + playingDirection == amountOfFrames)
				{   
					switch(playingMode)
					{
						case FORWARD_PLAYING | ONEWAY_PLAYING | SINGLE_SHOT_PLAYING:
						{ 
							Pause();
							firstFrame = true;
							break;
						}
					
						case FORWARD_PLAYING | ONEWAY_PLAYING | LOOP_PLAYING:
						{
							currentFrame = -1;
							break; 
						}  

						case FORWARD_PLAYING  | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
						case FORWARD_PLAYING  | PING_PONG_PLAYING | LOOP_PLAYING:
						case BACKWARD_PLAYING | PING_PONG_PLAYING | LOOP_PLAYING:
						{
							playingDirection = -playingDirection;
							break;
						}
						case BACKWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
						{
							Pause();
							firstFrame = true;
							playingDirection = -1;
							break;
						}
					}
				}
				else if(currentFrame + playingDirection < 0)
				{
					switch(playingMode)
					{
						case BACKWARD_PLAYING | ONEWAY_PLAYING | SINGLE_SHOT_PLAYING:
						{
							Pause();
							firstFrame = true; 
							break;
						}

						case BACKWARD_PLAYING | ONEWAY_PLAYING | LOOP_PLAYING:
						{
							currentFrame = amountOfFrames;
							break;
						}

						case BACKWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
						case BACKWARD_PLAYING | PING_PONG_PLAYING | LOOP_PLAYING:
						case FORWARD_PLAYING  | PING_PONG_PLAYING | LOOP_PLAYING:
						{ 
							playingDirection = -playingDirection;
							break;
						}
						case FORWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
						{
							Pause();
							firstFrame = true;
							playingDirection = 1;
							break;
						}   
					}
				}
				if(playingEnable)
				{
					currentFrame = currentFrame + playingDirection;

					CurrentFrameIsChanged();
				}
			}
		}
	}
}


StringANSI AbstractAnimation::_GetDir(void)
{
	return animationsDir;
}



void AbstractAnimation::SaveDerivedToFile(File& _file)
{
	LogicObject::SaveDerivedToFile(_file);
	PlayingObject::SaveDerivedToFile(_file);
	_file.Write(frameDurationInSec);
	_file.Write(amountOfFrames);
	_file.Write(currentFrame);
	_file.Write(elapsedTimeInSec);
	_file.Write(firstFrame);
	_file.Write(elapsedFrameTimeInSec);
	_file.Write(playingDirection);
}



void AbstractAnimation::LoadDerivedFromFile(File& _file)
{
	LogicObject::LoadDerivedFromFile(_file);
	PlayingObject::LoadDerivedFromFile(_file);
	_file.Read(frameDurationInSec);
	_file.Read(amountOfFrames);
	_file.Read(currentFrame);
	_file.Read(elapsedTimeInSec);
	_file.Read(firstFrame);
	_file.Read(elapsedFrameTimeInSec);
	_file.Read(playingDirection);
}