
#include "RealTimeWorker.h"



RealTimeWorker::Define::Define(void)
{
	sleep = true;
	unsuspended = true;
	updateCallController.SetFPS(60);
	updateCallController.SetMaxSteps(1);
	updateCallController.SetMode(CallController::FIXED);
}

