
#include "QuaternionLerpViewer.h"
#include <Editor/QuaternionLerpCreator/QuaternionLerpCreator.h>


QuaternionLerpGizmo::QuaternionLerpGizmo(void): segments(360), scale(1.51f), xAxisArrow(NIL), yAxisArrow(NIL), zAxisArrow(NIL), font(NIL), showGlobalSpins(true), showGlobalAxises(true)
{
	rotationMode = EULER_SCROLLBAR;

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
	Font2D::Define def;
	def.fontName = "Arial";
	def.fontWeight = Font2D::WEIGHT_BOLD;
	def.fontSize = 18;
	font = new Font2D();
	font->Create(def);
	xAxisBegin = -xAxis * (scale * 1.5f);
	xAxisEnd = xAxis * (scale * 1.5f);
	
	xAxisArrow = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid(0.04f);
		xAxisArrow->BindMesh(mesh);
	}
	xAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));
	xAxisArrow->SetColor(xColor + RGBA(1, 0, 0, 0));
	xAxisArrow->SetPosition(xAxisEnd);
	xAxisArrow->SetRotation(Matrix4x4::_RotationZ(-Angle::HALF_PI));
	yAxisBegin = -yAxis * (scale * 1.5f);
	yAxisEnd = yAxis * (scale * 1.5f);
	
	yAxisArrow = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid(0.04f);
		yAxisArrow->BindMesh(mesh);
	}

	yAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));
	yAxisArrow->SetColor(yColor + RGBA(0, 1, 0, 0));
	yAxisArrow->SetPosition(yAxisEnd);
	zAxisBegin = -zAxis * (scale * 1.5f);
	zAxisEnd = zAxis * (scale * 1.5f);
	
	zAxisArrow = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid(0.04f);
		zAxisArrow->BindMesh(mesh);
	}
	zAxisArrow->SetScale(Vector3(0.5f, 1.0f, 0.5f));
	zAxisArrow->SetColor(zColor + RGBA(0, 0, 1, 0));
	zAxisArrow->SetPosition(zAxisEnd);
	zAxisArrow->SetRotation(Matrix4x4::_RotationX(Angle::HALF_PI));
}


void QuaternionLerpGizmo::ShowGlobalSpins(int _state)
{
	showGlobalSpins = _state;
}



void QuaternionLerpGizmo::ShowGlobalAxises(int _state)
{
	showGlobalAxises = _state;
}



QuaternionLerpGizmo::~QuaternionLerpGizmo(void)
{
	delete font; 

	delete xAxisArrow;
	delete yAxisArrow;
	delete zAxisArrow;
}


RGBA QuaternionLerpGizmo::GetXColor(void)const
{
	return xColor;
}


RGBA QuaternionLerpGizmo::GetYColor(void)const
{
	return yColor;
}


RGBA QuaternionLerpGizmo::GetZColor(void)const
{
	return zColor;
}


void QuaternionLerpGizmo::SetRotationMode(RotationMode _mode)
{
	rotationMode = _mode;
}


QuaternionLerpGizmo::RotationMode QuaternionLerpGizmo::GetRotationMode(void)const
{
	return rotationMode;
}


StringANSI QuaternionLerpGizmo::GetRotationModeString(void)const
{
	switch(rotationMode)
	{
		case EULER_SCROLLBAR:
		{
			return "EULER_SCROLLBAR";
		}
		
		case SPHERICAL_SWING_TWIST:
		{
			return "SPHERICAL_SWING_TWIST";
		}

		case SPHERICAL_FROM_TO:
		{
			return "SPHERICAL_FROM_TO";
		}

		case SPHERICAL_TWO_POLE:
		{
			return "SPHERICAL_TWO_POLE";
		}
	}
	return "";
}


Quaternion QuaternionLerpGizmo::GetQuaternion(void)const
{ 
	return quaternion;
}



void QuaternionLerpGizmo::DrawXAxis(void)
{
	GraphicsLine::_Draw(xAxisBegin, xAxisEnd, xColor + RGBA(1, 0, 0, 0));
					
	xAxisArrow->Draw();

	font->SetColor(xColor + RGBA(1, 0, 0, 0));
	font->DrawWorldText(xAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "X");
}


void QuaternionLerpGizmo::DrawYAxis(void)
{
	GraphicsLine::_Draw(yAxisBegin, yAxisEnd, yColor + RGBA(0, 1, 0, 0));
					
	yAxisArrow->Draw();

	font->SetColor(yColor + RGBA(0, 1, 0, 0));
	font->DrawWorldText(yAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "Y");
}


void QuaternionLerpGizmo::DrawZAxis(void)
{
	GraphicsLine::_Draw(zAxisBegin, zAxisEnd, zColor + RGBA(0, 0, 1, 0));
		
	zAxisArrow->Draw();

	font->SetColor(zColor + RGBA(0, 0, 1, 0));
	font->DrawWorldText(zAxisEnd + Vector3(0.0f, +0.10f, 0.0f), "Z");
}


void QuaternionLerpGizmo::Draw(void)
{
	if(IsShowEnabled())
	{
		BeginTransform();

		Renderer::_SetLineWidth(3.0f);

		if(showGlobalSpins)
		{
			point1 = yAxis * scale;
			for(int32 i = 0; i < segments; i++)
			{
				point2 = qX * point1;
				GraphicsLine::_Draw(point1, point2, xColor + RGBA(1, 0, 0, 0));
				point1 = point2;
			}
		}

		if(showGlobalAxises) 
		{ 
			DrawXAxis();
		}


		if(showGlobalSpins)
		{
			point1 = xAxis * scale;
			for(int32 i = 0; i < segments; i++)
			{
				point2 = qY * point1;
				GraphicsLine::_Draw(point1, point2, yColor + RGBA(0, 1, 0, 0));
				point1 = point2;
			}
		}


		if(showGlobalAxises) 
		{
			DrawYAxis();
		}	

		
		if(showGlobalSpins)
		{
			point1 = xAxis * scale;
			for(int32 i = 0; i < segments; i++)
			{
				point2 = qZ * point1;
				GraphicsLine::_Draw(point1, point2, zColor + RGBA(0, 0, 1, 0));
				point1 = point2;
			}
		}

		if(showGlobalAxises) 
		{
			DrawZAxis();
		}
			
		Renderer::_SetLineWidth(6.0f);


		EndTransform();
	}
}



bool QuaternionLerpGizmo::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}


bool QuaternionLerpGizmo::LoadFromFile(File& _file)
{
	return false;
}