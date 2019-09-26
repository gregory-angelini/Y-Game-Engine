
#include "FloatLerp.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>

#ifdef SendMessage
   #undef SendMessage
#endif
StringANSI FloatLerp::floatLerpsSuffix = "f-lerp";
StringANSI FloatLerp::floatLerpsDir = "/lerps/f-lerps/";




FloatLerp::FloatLerp(void): playingSpeed(1.0f), target(1.0f), forwardMovingEnable(true), firstFrame(true)
{
	AddClassProperty(FLOAT_LERP_CLASS);

	t = delta = current = elapsedTimeInSec = 0.0f;

	moveToTarget = &FloatLerp::MoveToTargetUnitsPerSecond;
	lerpMode = UNITS_PER_SECOND;
}


FloatLerp::~FloatLerp(void)
{
	Destroying();
}



void FloatLerp::SetPlayingSpeed(float _playingSpeed)
{
	if(_playingSpeed >= 0.0f)
	{
		playingSpeed = _playingSpeed;
	}
}


float FloatLerp::GetPlayingSpeed(void)
{
	return playingSpeed;
}

	
void FloatLerp::SetLerpMode(int32 _mode)
{
	if(lerpMode != _mode)
	{
		lerpMode = _mode;

		switch(_mode)
		{
			case UNITS_PER_SECOND:
			{ 
				moveToTarget = &FloatLerp::MoveToTargetUnitsPerSecond;
				break;
			}

			case FIXED_TIME:
			{
				moveToTarget = &FloatLerp::MoveToTargetFixedTime;
				break;
			}

			case PARABOLA:
			{
				moveToTarget = &FloatLerp::MoveToTargetParabola;
				break;
			}

			case DAMPING:
			{
				moveToTarget = &FloatLerp::MoveToTargetDamping;
				break;
			}

			case MAGNETIC:
			{
				moveToTarget = &FloatLerp::MoveToTargetMagnetic;
				break;
			}
		}
	}
}

	
int32 FloatLerp::GetLerpMode(void)const
{
	return lerpMode;
} 
 

float FloatLerp::GetElapsedTimeInSec(void)const
{
	return elapsedTimeInSec;
}



void FloatLerp::Stop(void)
{  
	if(playingMode & FORWARD_PLAYING)    
	{
		forwardMovingEnable = true;

		t = delta = current = elapsedTimeInSec = 0.0f;
	}
	else
	{
		forwardMovingEnable = false;
		
		if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
		{
			current = target;
			t = 1.0f;
			delta = elapsedTimeInSec = 0.0f;
		}
		else
		{
			t = delta = current = elapsedTimeInSec = 0.0f;
		}
	}

	firstFrame = true;
	PlayingObject::Stop();
}
 

void FloatLerp::SetTarget(float _target)
{
	if(_target > 0.0f)
	{
		target = _target;
		
		if(current > target)
		{
			if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
			{
				forwardMovingEnable ? UpdateForwardPlayingModeEx() : UpdateBackwardPlayingModeEx();
			} 
			else
			{
				forwardMovingEnable ? UpdateForwardPlayingMode() : UpdateBackwardPlayingMode();
			}
		} 
	}
}

    
float FloatLerp::GetTarget(void)const
{
	return target;
}
 

float FloatLerp::GetCurrent(void)const
{
	if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
	{
		return current;
	}
	return forwardMovingEnable ? current : target - current;
}



void FloatLerp::SetT(float _t)
{
	if(_t >= 0.0f && _t <= 1.0f)
	{
		if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
		{
			t = _t;
			float previous = current;
			current = target * t;

			if(t == 1.0f)
			{  
				forwardMovingEnable ? UpdateForwardPlayingModeEx() : UpdateBackwardPlayingModeEx();
				return; 
			}
			else if(t == 0.0f)
			{
				if(playingMode & SINGLE_SHOT_PLAYING)
				{
					if(IsReceiversExist(FloatLerp::FLOAT_LERP_START_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_START_MESSAGE); }
				}
			}
			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
		}
	} 
}



float FloatLerp::GetT(void)const
{
	if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
	{
		return t;
	}
	else if(lerpMode == PARABOLA)
	{
		return  forwardMovingEnable ? (current / target) : 1.0f - (current / target);
	}
	return forwardMovingEnable ? t : 1.0f - t;
}


float FloatLerp::GetDelta(void)const
{
	if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
	{
		return delta;
	}
	return forwardMovingEnable ? delta : -delta;
}



bool FloatLerp::IsFinish(void)const
{
	if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
	{
		if(playingMode & BACKWARD_PLAYING)
		{
			return current == 0.0f;
		}
	}
	return current == target;
}




void FloatLerp::UpdateForwardPlayingMode(void)
{
	switch(playingMode)
	{
		case FORWARD_PLAYING | ONEWAY_PLAYING | SINGLE_SHOT_PLAYING:
		{ 
			Pause();

			float previous = current;
			current = target;
			t = 1.0f;
			delta = current - previous;
			firstFrame = true;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_FINISH_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_FINISH_MESSAGE); }
			break;
		}
	
		case FORWARD_PLAYING | ONEWAY_PLAYING | LOOP_PLAYING:
		{ 
			float previous = current;
			current = target;
			t = 1.0f;
			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			
			current = t = 0.0f;
			break; 
		}  

		case FORWARD_PLAYING  | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
		case FORWARD_PLAYING  | PING_PONG_PLAYING | LOOP_PLAYING:
		case BACKWARD_PLAYING | PING_PONG_PLAYING | LOOP_PLAYING:
		{
			forwardMovingEnable = !forwardMovingEnable;

			float previous = current;
			current = target;
			t = 1.0f; 
			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			
			current = t = 0.0f;
			break;
		}
		case BACKWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
		{
			Pause();
			forwardMovingEnable = !forwardMovingEnable;

			float previous = current;
			current = target;
			t = 1.0f; 
			delta = current - previous;
			firstFrame = true;
				
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_FINISH_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_FINISH_MESSAGE); }
			break;
		}
	}
}




void FloatLerp::UpdateBackwardPlayingMode(void)
{ 
	switch(playingMode)
	{
		case BACKWARD_PLAYING | ONEWAY_PLAYING | SINGLE_SHOT_PLAYING:
		{
			Pause();

			float previous = current;
			current = target;
			t = 1.0f; 
			delta = previous - current;
			current = t = 0.0f;  
			firstFrame = true;
			
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_FINISH_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_FINISH_MESSAGE); }
			break; 
		}

		case BACKWARD_PLAYING | ONEWAY_PLAYING | LOOP_PLAYING:
		{
			float previous = current;
			current = target;
			t = 1.0f; 
			delta = previous - current;
			current = t = 0.0f;  
			
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			break;
		}

		case BACKWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
		case BACKWARD_PLAYING | PING_PONG_PLAYING | LOOP_PLAYING:
	    case FORWARD_PLAYING  | PING_PONG_PLAYING | LOOP_PLAYING:
		{
			forwardMovingEnable = !forwardMovingEnable;

			float previous = current;
			current = target;
			t = 1.0f; 
			delta = previous - current;
			current = t = 0.0f; 
						
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			break;
		}
		case FORWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
		{
			Pause();
			forwardMovingEnable = !forwardMovingEnable;

			float previous = current;
			current = target;
			t = 1.0f; 
			delta = previous - current;
			current = t = 0.0f;
			firstFrame = true;  

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_FINISH_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_FINISH_MESSAGE); }
			break;
		}   
	}
}




 
void FloatLerp::UpdateForwardPlayingModeEx(void)
{
	switch(playingMode)
	{
		case FORWARD_PLAYING | ONEWAY_PLAYING | SINGLE_SHOT_PLAYING:
		{ 
			Pause();

			float previous = current;
			current = target;
			t = 1.0f;
			delta = current - previous;
			firstFrame = true;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_FINISH_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_FINISH_MESSAGE); }
			break;
		}
	
		case FORWARD_PLAYING | ONEWAY_PLAYING | LOOP_PLAYING:
		{
			float previous = current;
			current = target;
			t = 1.0f;
			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }

			current = t = 0.0f;
			break; 
		}  

		case FORWARD_PLAYING  | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
		case FORWARD_PLAYING  | PING_PONG_PLAYING | LOOP_PLAYING:
		case BACKWARD_PLAYING | PING_PONG_PLAYING | LOOP_PLAYING:
		{
			forwardMovingEnable = !forwardMovingEnable;

			float previous = current;
			current = target;
			t = 1.0f;
			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			break;
		}
		case BACKWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
		{ 
			Pause();
			forwardMovingEnable = !forwardMovingEnable;

			float previous = current;
			current = target;
			t = 1.0f; 
			delta = current - previous;
			firstFrame = true;
			
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_FINISH_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_FINISH_MESSAGE); }
			break;
		} 
	}
}


void FloatLerp::UpdateBackwardPlayingModeEx(void)
{
	switch(playingMode)
	{
		case BACKWARD_PLAYING | ONEWAY_PLAYING | SINGLE_SHOT_PLAYING:
		{    
			Pause();

			float previous = current;
			current = t = 0.0f;
			delta = current - previous;
			firstFrame = true;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_FINISH_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_FINISH_MESSAGE); }
			break;
		}

		case BACKWARD_PLAYING | ONEWAY_PLAYING | LOOP_PLAYING:
		{
			float previous = current;
			current = t = 0.0f;
			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			
			current = target;
			t = 1.0f;
			break;
		}

		case BACKWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
		case BACKWARD_PLAYING | PING_PONG_PLAYING | LOOP_PLAYING:
		case FORWARD_PLAYING  | PING_PONG_PLAYING | LOOP_PLAYING:
		{
			forwardMovingEnable = !forwardMovingEnable;

			float previous = current;
			current = t = 0.0f;
			delta = current - previous;
				
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			break;
		}
		case FORWARD_PLAYING | PING_PONG_PLAYING | SINGLE_SHOT_PLAYING:
		{
			Pause();
			forwardMovingEnable = !forwardMovingEnable;

			float previous = current;
			current = t = 0.0f; 
			delta = current - previous;
			firstFrame = true;
					
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_FINISH_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_FINISH_MESSAGE); }
			break;
		}   
	}
}



void FloatLerp::SetPlayingMode(int32 _playingMode)
{
	if(!(playingMode & _playingMode))
	{
		if(_playingMode == FORWARD_PLAYING)
		{
			forwardMovingEnable = true;

			if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
			{
			}
			else
			{
				t = delta = current = elapsedTimeInSec = 0.0f;
			}
		}
		else if(_playingMode == BACKWARD_PLAYING)
		{
			forwardMovingEnable = false;

			if(lerpMode == FIXED_TIME || lerpMode == UNITS_PER_SECOND)
			{
			}
			else
			{
				t = delta = current = elapsedTimeInSec = 0.0f;
			}
		}
		if(playingMode & PING_PONG_PLAYING)
		{
			if(_playingMode == ONEWAY_PLAYING)
			{ 
				if(playingMode & FORWARD_PLAYING)
				{
					forwardMovingEnable = true;
				}
				else
				{
					forwardMovingEnable = false;
				}
			}
		}
		PlayingObject::SetPlayingMode(_playingMode);
	}
}



 
void FloatLerp::MoveToTargetMagnetic(float _delta)
{ 
	if(_delta != 0.0f)
	{
		if(firstFrame)
		{ 
			elapsedTimeInSec = 0.0f;
			firstFrame = false;

			if(IsReceiversExist(FloatLerp::FLOAT_LERP_START_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_START_MESSAGE); }
		}
		else
		{
			elapsedTimeInSec += _delta;
		}
		float steps = (target - current) / (playingSpeed * _delta);
		t += 1.0f / steps;
		if(t > 1.0f)
		{  
			forwardMovingEnable ? UpdateForwardPlayingMode() : UpdateBackwardPlayingMode();
		}
		else
		{ 
			float previous = current;

			current = target * t;
		
			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, forwardMovingEnable ? t : 1.0f - t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, forwardMovingEnable ? current : target - current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, forwardMovingEnable ? delta : -delta); }
		}
	}
}



void FloatLerp::MoveToTargetParabola(float _delta)
{
	if(_delta != 0.0f)
	{
		if(firstFrame)
		{ 
			elapsedTimeInSec = 0.0f;
			firstFrame = false;
			 
			if(IsReceiversExist(FloatLerp::FLOAT_LERP_START_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_START_MESSAGE); }
		}
		else
		{
			elapsedTimeInSec += _delta;
		}
		float steps = (target - current) / (playingSpeed * _delta);
		t += 1.0f / steps;
		if(t > 1.0f)
		{
			forwardMovingEnable ? UpdateForwardPlayingMode() : UpdateBackwardPlayingMode();
		}
		else
		{ 
			float previous = current;

			current = current * (1.0f - t) + target * t;

			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, forwardMovingEnable ? (current / target) : 1.0f - (current / target)); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) {SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, forwardMovingEnable ? current : target - current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) {SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, forwardMovingEnable ? delta : -delta); }
		}
	}
}



void FloatLerp::MoveToTargetFixedTime(float _delta)
{
	if(_delta != 0.0f)
	{
		if(firstFrame)
		{ 
			elapsedTimeInSec = 0.0f;
			firstFrame = false;

			if(IsReceiversExist(FloatLerp::FLOAT_LERP_START_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_START_MESSAGE); }
		}
		else 
		{
			elapsedTimeInSec += _delta;
		} 

		float step = 1.0f / playingSpeed * _delta;
		
		forwardMovingEnable ? t += step : t -= step;
		if(t > 1.0f || t < 0.0f)
		{ 
			forwardMovingEnable ? UpdateForwardPlayingModeEx() : UpdateBackwardPlayingModeEx();
		}
		else
		{ 
			float previous = current;

			current = target * t;

			delta = current - previous;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
		}
	}
}



void FloatLerp::MoveToTargetUnitsPerSecond(float _delta)
{
	if(_delta != 0.0f)
	{ 
		if(firstFrame)
		{ 
			elapsedTimeInSec = 0.0f;
			firstFrame = false;

			if(IsReceiversExist(FloatLerp::FLOAT_LERP_START_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_START_MESSAGE); }
		}
		else 
		{
			elapsedTimeInSec += _delta;
		}
		float steps = forwardMovingEnable ? (target - current) / (playingSpeed * _delta) : current / (playingSpeed * _delta);
		if(steps < 1.0f)
		{
			forwardMovingEnable ? UpdateForwardPlayingModeEx() : UpdateBackwardPlayingModeEx();
		} 
		else
		{ 
			float previous = current;

			float step = 1.0f / steps;
			
			current = forwardMovingEnable ? current * (1.0f - step) + target * step : current * (1.0f - step);

			delta = current - previous;
			t = current / target;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, delta); }
		}
	}
}




void FloatLerp::MoveToTargetDamping(float _delta)
{
	if(_delta != 0.0f)
	{
		if(firstFrame)
		{ 
			elapsedTimeInSec = 0.0f;
			firstFrame = false;

			if(IsReceiversExist(FloatLerp::FLOAT_LERP_START_MESSAGE)) { SendMessage(FloatLerp::FLOAT_LERP_START_MESSAGE); }
		}
		else 
		{
			elapsedTimeInSec += _delta;
		}

		float previous = current;
  
		float step = 1.0f / playingSpeed * _delta;
		
		current = current * (1.0f - step) + target * step;
			
		if(current >= target ||
		  (current - previous) <= Numerical<float>::TINY)
		{ 
			forwardMovingEnable ? UpdateForwardPlayingMode() : UpdateBackwardPlayingMode();
		}  
		else
		{
			delta = current - previous;
			t = current / target;

			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_T_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_T_MESSAGE, forwardMovingEnable ? t : 1.0f - t); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, forwardMovingEnable ? current : target - current); }
			if(IsReceiversExist(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE)) { SendPackage(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, forwardMovingEnable ? delta : -delta); }
		}
	}
}        



float FloatLerp::GetTimeToTarget(void)
{
	if(lerpMode == FIXED_TIME) 
	{ 
		return playingSpeed - (playingSpeed * t); 
	} 
	else if(lerpMode == UNITS_PER_SECOND) 
	{ 
		return (target - current) / playingSpeed; 
	} 
	return -1.0f;
}


float FloatLerp::GetPlayingDurationInSec(void)
{
	switch(lerpMode)
	{
		case UNITS_PER_SECOND:
		{
			return target / playingSpeed;
		}

		case FIXED_TIME:
		{ 
			return playingSpeed;
		}
	}
	return -1.0f;
}





void FloatLerp::SetPlayingDurationInSec(float _duration)
{
	if(_duration >= 0.001f) 
	{
		switch(lerpMode)
		{
			case UNITS_PER_SECOND:
			{ 
				playingSpeed = target / _duration;
				break;
			}

			case FIXED_TIME:
			{ 
				playingSpeed = _duration;
				break;
			}

			case MAGNETIC:
			case PARABOLA:
			case DAMPING:
			{
				break;
			}
		}
	}
}


void FloatLerp::Update(float _delta)
{
	if(!freezeEnable && playingEnable)
	{
		(this->*moveToTarget)(GetDeltaScale() * _delta);
	}
}



StringANSI FloatLerp::_GetDir(void)
{
	return floatLerpsDir;
}


void FloatLerp::_SetFileSuffix(StringANSI _suffix)
{
	floatLerpsSuffix = _suffix;
}


StringANSI FloatLerp::_GetFileSuffix(void)
{
	return floatLerpsSuffix;
}



bool FloatLerp::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + floatLerpsDir + GetName() + "." + floatLerpsSuffix;
	}

	File file(_path, File::REWRITE);
		
	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}




bool FloatLerp::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
			
		LogicObject::SaveDerivedToFile(_file);
		PlayingObject::SaveDerivedToFile(_file);
		_file.Write(target);
		_file.Write(current);
		_file.Write(delta);
		_file.Write(t);
		_file.Write(forwardMovingEnable);
		_file.Write(playingSpeed);
		_file.Write(lerpMode);
		_file.Write(elapsedTimeInSec);
		_file.Write(firstFrame);
		return true;
	}
	return false;
}


bool FloatLerp::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + FloatLerp::_GetDir() + _name + "." + FloatLerp::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool FloatLerp::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
			
		LogicObject::SaveDerivedToFile(_file);
		PlayingObject::SaveDerivedToFile(_file);
		_file.Write(target);
		_file.Write(current);
		_file.Write(delta);
		_file.Write(t);
		_file.Write(forwardMovingEnable);
		_file.Write(playingSpeed);
		_file.Write(lerpMode);
		_file.Write(elapsedTimeInSec);
		_file.Write(firstFrame);
		return true;
	}
	return false;
}


bool FloatLerp::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + floatLerpsDir + _path + "." + floatLerpsSuffix;
	}
	
	File file(_path, File::READ);

	return LoadFromFile(file);
}



bool FloatLerp::LoadFromFile(File& _file)
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
		
		LogicObject::LoadDerivedFromFile(_file);
	    PlayingObject::LoadDerivedFromFile(_file);
		_file.Read(target);
		_file.Read(current);
		_file.Read(delta);
		_file.Read(t);
		_file.Read(forwardMovingEnable);
		_file.Read(playingSpeed);
		int32 newLerpMode; 
		_file.Read(newLerpMode);  
		SetLerpMode(newLerpMode);
		_file.Read(elapsedTimeInSec);
		_file.Read(firstFrame);
		return true;
	}
	return false;
}



FloatLerp* FloatLerp::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + floatLerpsDir + _path + "." + floatLerpsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


FloatLerp* FloatLerp::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		FloatLerp* floatLerp = new FloatLerp();

		if(floatLerp->LoadFromFile(_file))
		{
			return floatLerp;
		}
		delete floatLerp;
	}
	return NIL;
}
