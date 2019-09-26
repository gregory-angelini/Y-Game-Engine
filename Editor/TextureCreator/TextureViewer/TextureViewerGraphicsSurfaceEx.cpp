
#include "TextureViewer.h"
#include <Editor/TextureCreator/TextureCreator.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>


TextureViewer::GraphicsSurfaceEx::GraphicsSurfaceEx(void): isCurrent(true), frameIndex(-1)
{
	SetColor(RGBA(RGBA::GREEN_COLOR));
} 



TextureViewer::GraphicsSurfaceEx::~GraphicsSurfaceEx(void)
{
	Destroying();
}
			


void TextureViewer::GraphicsSurfaceEx::SetFrameIndex(int32 _index)
{
	frameIndex = _index;
}


int32 TextureViewer::GraphicsSurfaceEx::GetFrameIndex(void)const
{
	return frameIndex;
}



bool TextureViewer::GraphicsSurfaceEx::IsCurrent(void)const
{
	return isCurrent;
}



void TextureViewer::GraphicsSurfaceEx::SetCurrent(bool _enable)
{
	isCurrent = _enable;
	if(!isCurrent) { SetColor(RGBA(RGBA::YELLOW_COLOR)); }
	else { SetColor(RGBA(RGBA::GREEN_COLOR)); }  
}



void TextureViewer::GraphicsSurfaceEx::Draw(void)
{
	if(Renderer::_GetPicked())
	{
		SetAlpha(0.15f);
		GraphicsSurface::Draw();

		Renderer::_GetPicked()->SetWireframeEnable(true);

		SetAlpha(0.5f);
		GraphicsSurface::Draw();
		
		Renderer::_GetPicked()->SetWireframeEnable(false);
	}
}