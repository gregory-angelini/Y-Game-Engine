
#include "CallController.h"
#include <Engine/Core/Common/Common.h>


CallController::CallController(void)
{
	mode = FIXED;
	Set(Caller<>(), 0, 0);
}


CallController::~CallController(void)
{}


CallController::CallController(Caller<> _action, int32 _fps, int32 _maxSteps)
{
	mode = FIXED;
	Set(_action, _fps, _maxSteps);
}


void CallController::SetAction(Caller<> _action)
{
	action = _action;
}


bool CallController::IsValid(void)const
{
	return action.IsValid();
}


CallController::CallMode CallController::GetMode(void)const
{
	return mode;
}


void CallController::SetMode(CallMode _mode)
{
	mode = _mode;
}


void CallController::Set(Caller<> _action, int32 _fps, int32 _maxSteps)
{
	action = _action;
	fixedFPS = _fps;
	maxSteps = _maxSteps;

	fixedDeltaAccumulator = 0.0f;

	if(fixedFPS > 0)
	{
		fixedDelta = 1.0f / (float)fixedFPS;
	}
	else
	{
		fixedDelta = 0.0f;
	}
}


void CallController::QuickCall(float _delta)
{
	if(action.IsValid())
	{
		action(_delta);
	}
}



int32 CallController::Call(float _delta)
{ 

	if(action.IsValid())
	{
		if(mode)
		{ 
			action(_delta);
			updateCallCounter.Update();
			return 1;
		}  
		else
		{  
			if(fixedDelta > 0.0f)
			{
				fixedDeltaAccumulator += _delta;
				int32 steps = (int32)(floorf(fixedDeltaAccumulator / fixedDelta));
				
				if(steps)
				{
					fixedDeltaAccumulator -= steps * fixedDelta;
					int32 clampedSteps = Numerical<int32>::_GetMin(steps, maxSteps);
			
					for(int32 i = 0; i < clampedSteps; ++i)
					{
						action(fixedDelta);
					}

					return clampedSteps;
				}
			}
		} 
	}
	return 0;
}



int32 CallController::GetFPS(void)const
{
	if(mode)
	{
		return updateCallCounter.CallsPerSecond();
	}
	return fixedFPS;
}


float CallController::GetDelta(void)const
{
	if(mode)
	{
		return updateCallCounter.DeltaTime() * 0.001f;
	}
	return fixedDelta;
}

 
int32 CallController::GetMaxSteps(void)const
{
	return maxSteps;
}

void CallController::SetMaxSteps(int32 _maxSteps)
{
	maxSteps = _maxSteps;
}
				

void CallController::SetFPS(int32 _fps)
{
	fixedFPS = _fps;
	if(fixedFPS > 0)
	{
		fixedDelta = 1.0f / (float)fixedFPS;
	}
	else
	{
		fixedDelta = 0.0f;
	}
	fixedDeltaAccumulator = 0.0f;
}