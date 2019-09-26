
#include "RealTimeWorker.h"
#include <Engine/Output/File/File.h>

  

RealTimeWorker::RealTimeWorker(void)
{
	RealTimeWorker::Define define;
	SetState(define);
}


RealTimeWorker::~RealTimeWorker(void)
{
}


void RealTimeWorker::SetState(RealTimeWorker::Define _define)
{
	Window::SetState(_define.windowDef);
	sleep = _define.sleep;
	unsuspended = _define.unsuspended;
	updateCallController = _define.updateCallController;
}



RealTimeWorker::Define RealTimeWorker::GetState(void)const
{
	RealTimeWorker::Define define;
	define.windowDef = Window::GetState();
	
	define.sleep = sleep;
	define.unsuspended = unsuspended;
	define.updateCallController = updateCallController;
	return define;
}


bool RealTimeWorker::Create(RealTimeWorker::Define _define)
{
	if(Window::Create(_define.windowDef))
	{
		sleep = _define.sleep;
		unsuspended = _define.unsuspended;
		updateCallController = _define.updateCallController;
		if(!updateCallController.IsValid()) 
		{ 
			updateCallController.SetAction(Caller<>(this, &RealTimeWorker::Update));
		}
		return true;
	}

	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(GetTitle() + ".RealTimeWorker::Create - FAILED\r\n");
	}
	return false;
}



CallController& RealTimeWorker::GetUpdateCallController(void)
{
	return updateCallController;
}

	
bool RealTimeWorker::GetSleep(void)const
{
	return sleep;
}

void RealTimeWorker::SetSleep(bool _sleep)
{
	sleep = _sleep;
}


void RealTimeWorker::Loop(void)
{
	while(Frame() != Window::CLOSE);
}


bool RealTimeWorker::IsUnsuspended(void)const
{
	return unsuspended; 
}


void RealTimeWorker::SetUnsuspended(bool _unsuspended)
{
	unsuspended = _unsuspended;
}


int32 RealTimeWorker::Frame(void)
{
	if(Window::Frame())
	{
		if((IsKeyboardFocus() && !IsMinimized()) || unsuspended)
		{
			if(updateCallController.Call(GetDeltaTimeInSec()))
			{} 
		    else
			{
				if(sleep)
				{
					::SleepEx(1, false);
				}
			}
		}
		else
		{
			::SleepEx(1, false);
		}
	}

	return result;
}

 
void RealTimeWorker::Update(float _delta)
{}

