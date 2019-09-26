
#include "TextureViewer.h"
#include <Editor/TextureCreator/TextureCreator.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>


TextureViewer::GraphicsPointEx::GraphicsPointEx(void): isSelected(false), isConvex(false)
{
	normalColor = RGBA(0, 120, 0, 255);
	selectColor = RGBA(234, 170, 68, 255);
	convexColor = RGBA(120, 0, 0, 255);

	SetColorRGB(normalColor.GetRGBf());
}



TextureViewer::GraphicsPointEx::~GraphicsPointEx(void)
{
	Destroying();
}



bool TextureViewer::GraphicsPointEx::IsSelected(void)const
{
	return isSelected;
}



void TextureViewer::GraphicsPointEx::SetSelected(bool _enable)
{
	isSelected = _enable;

	if(isConvex) { SetColorRGB(convexColor.GetRGBf()); } 
	else { SetColorRGB(normalColor.GetRGBf()); } 
}
			


bool TextureViewer::GraphicsPointEx::IsConvex(void)const
{
	return isConvex;
}



void TextureViewer::GraphicsPointEx::RestoreColor(void)
{
	if(isConvex) { SetColorRGB(convexColor.GetRGBf()); } 
	else { SetColorRGB(normalColor.GetRGBf()); } 
}


void TextureViewer::GraphicsPointEx::SetConvex(bool _enable)
{
	isConvex = _enable;

	if(isConvex) { SetColorRGB(convexColor.GetRGBf()); } 
	else { SetColorRGB(normalColor.GetRGBf()); } 
}
				


void TextureViewer::GraphicsPointEx::Draw(void)
{
	GraphicsPoint::Draw();

	float scale = TextureViewer::_GetCamera()->GetDistanceToViewingPoint();
	if(isSelected) { GraphicsCircle::_Draw(GetPosition(), TextureViewer::_GetPointSizeInUnits() * scale, 36, selectColor); } 
}