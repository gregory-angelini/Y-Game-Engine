#include "Renderer.h"

Renderer::Define::Define(void)
{ 
	realTimeWorkerDef.updateCallController.SetMaxSteps(5);
	
	drawCallController.SetFPS(60);
	drawCallController.SetMaxSteps(1);
	drawCallController.SetMode(CallController::FIXED);
	vSync = true;

	backgroundColor = RGBA(RGBA::BLACK_COLOR).GetRGBAf();
	colorDepth = 32; 
	colorFormat = RGBA_FORMAT;

	zBufferEnable = true;
	zBufferClearValue = 1.0f;
	zBufferDepth = 24;

	frontFaceClockwise = false;
	wireframeEnable = false;
	viewportAutoEnable = true;

}