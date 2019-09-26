
#include "QuaternionViewer.h"
#include <Editor/QuaternionCreator/QuaternionCreator.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsStrip/GraphicsStrip.h>



QuaternionSphere::QuaternionSphere(void): segments(360 / 5), scale(1.01f), selected(NO_SELECTED)
{
	step = Angle::DOUBLE_PI / (float)segments;
	qX = Quaternion(Matrix4x4::_RotationX(step));
	qY = Quaternion(Matrix4x4::_RotationY(step));
	qZ = Quaternion(Matrix4x4::_RotationZ(step));

	xAxis = Vector3::UNIT_X;
	yAxis = Vector3::UNIT_Y;
	zAxis = Vector3::UNIT_Z;

	xColor = RGBA(163, 21, 21, 255);
	yColor = RGBA(118, 185, 0, 255);
	zColor = RGBA(55, 111, 194, 255);

	faceColor = RGBA(RGBA::BLACK_COLOR);
	face = new GraphicsSurface();
	
	Mesh* mesh = new Mesh();
	mesh->CreatePyramid(0.08f);
	face->BindMesh(mesh);
	
	face->SetPosition(zAxis);
	face->SetRotation(Matrix4x4::_RotationX(Angle::HALF_PI));
	face->SetColor(faceColor);
}


QuaternionSphere::~QuaternionSphere(void)
{
	delete face;
}



void QuaternionSphere::Update(float _delta)
{
	face->SetColor(RGBA::_GetRandom());
}


void QuaternionSphere::Draw(void)
{
	if(IsShowEnabled())
	{
		GraphicsSurface::Draw();

		BeginTransform();
		point1 = yAxis * scale;
		SimpleList<Vector3> xList;
		xList.AddToTail(point1);

		for(int32 i = 0; i < segments; i++)
		{  
			point2 = qX * point1;
				
			xList.AddToTail(point2);
			point1 = point2;
		}
		GraphicsStrip::_Draw(xList, selected == CIRCLE_X ? RGBA(RGBA::YELLOW_COLOR) : xColor);
		point1 = xAxis * scale;
		SimpleList<Vector3> yList;
		yList.AddToTail(point1);

		for(int32 i = 0; i < segments; i++)
		{
			point2 = qY * point1;
			yList.AddToTail(point2);
			point1 = point2;
		}
		GraphicsStrip::_Draw(yList, selected == CIRCLE_Y ? RGBA(RGBA::YELLOW_COLOR) : yColor);
		point1 = xAxis * scale;
		SimpleList<Vector3> zList;
		zList.AddToTail(point1);

		for(int32 i = 0; i < segments; i++)
		{
			point2 = qZ * point1;
			zList.AddToTail(point2);
			point1 = point2;
		}
		GraphicsStrip::_Draw(zList, selected == CIRCLE_Z ? RGBA(RGBA::YELLOW_COLOR) : zColor);
		Renderer::_SetLineWidth(3.0f);
		face->BeginTransform();
			
		GraphicsLine::_Draw(Vector3() + Vector3(0, 0.04f, 0), (xAxis / 4.0f) + Vector3(0, 0.04f, 0), xColor + RGBA(1, 0, 0, 0));
		GraphicsLine::_Draw(Vector3() + Vector3(0, 0.04f, 0), (-zAxis / 4.0f) + Vector3(0, 0.04f, 0), yColor + RGBA(0, 1, 0, 0));
			
		face->EndTransform();
		Renderer::_SetLineWidth(6.0f);

		face->Draw();

		EndTransform();
	}
}
 
		
RGBA QuaternionSphere::GetXColor(void)const
{
	return xColor;
}

RGBA QuaternionSphere::GetYColor(void)const
{
	return yColor;
}


RGBA QuaternionSphere::GetZColor(void)const
{
	return zColor;
}


void QuaternionSphere::SelectX(void)
{
	selected = CIRCLE_X;
}


void QuaternionSphere::SelectY(void)
{
	selected = CIRCLE_Y;
}


void QuaternionSphere::SelectZ(void)
{
	selected = CIRCLE_Z;
}


int32 QuaternionSphere::GetSelected(void)const
{
	return selected;
}

void QuaternionSphere::Unselect(void)
{
	selected = NO_SELECTED;
}



bool QuaternionSphere::SaveToFile(File& _file)
{
	return false;
}



bool QuaternionSphere::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}

	
bool QuaternionSphere::LoadFromFile(File& _file)
{
	return false;
}
