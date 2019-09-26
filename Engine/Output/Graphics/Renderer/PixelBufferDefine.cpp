#include "PixelBuffer.h"


PixelBuffer::Define::Define(void)
{
	colorDepth = 32;

	zBufferDepth = 24;
	zBufferEnable = true;
	zBufferClearValue = 1.0f;

	stencilDepth = 8;

	frontFaceClockwise = false;
	wireframeEnable = false;

	mode = PixelBuffer::RENDER_TO_TEXTURE;

	width = height = 256;

	filteringMode = Texture2D::POINT_FILTERING;
}