
#include "TextureViewer.h"
#include <Editor/TextureCreator/TextureCreator.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>


TextureViewer::GraphicsLineEx::GraphicsLineEx(void): isIntersected(false)
{
	normalColor = RGBA(RGBA::GREEN_COLOR);
	intersectColor = RGBA(120, 0, 0, 255);
	SetColorRGB(normalColor.GetRGBf()); 
}


TextureViewer::GraphicsLineEx::~GraphicsLineEx(void)
{
	Destroying();
}



bool TextureViewer::GraphicsLineEx::IsIntersected(void)const
{
	return isIntersected;
}



void TextureViewer::GraphicsLineEx::RestoreColor(void)
{
	if(isIntersected) { SetColorRGB(intersectColor.GetRGBf()); }
	else { SetColorRGB(normalColor.GetRGBf()); }  
}



void TextureViewer::GraphicsLineEx::SetIntersected(bool _enable)
{
	isIntersected = _enable;
	if(isIntersected) { SetColorRGB(intersectColor.GetRGBf()); }
	else { SetColorRGB(normalColor.GetRGBf()); }  
}

