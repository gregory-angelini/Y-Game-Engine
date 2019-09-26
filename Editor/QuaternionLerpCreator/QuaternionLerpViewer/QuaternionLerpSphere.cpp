
#include "QuaternionLerpViewer.h"
#include <Editor/QuaternionLerpCreator/QuaternionLerpCreator.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsStrip/GraphicsStrip.h>


QuaternionLerpSphere::QuaternionLerpSphere(void): segments(360 / 5), scale(1.01f), selected(NO_SELECTED), marker(NIL), startLerpValid(false), targetLerpValid(false), showLocalSpins(true)
{
	marker = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateCircle(Circle(Vector3(), 0.07f), 16);
		marker->BindMesh(mesh);
	}
	marker->SetColor(RGBA(200, 200, 200, 255));
	for(int32 i = 1; i < marker->GetMesh()->GetAmountOfVertices(); i++)
	{
		vList.AddToTail(marker->GetMesh()->GetVertex(i));
	}
	vList.AddToTail(marker->GetMesh()->GetVertex(1));

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

	{
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid(0.08f);
		face->BindMesh(mesh);
	}

	face->SetPosition(zAxis);
	face->SetRotation(Matrix4x4::_RotationX(Angle::HALF_PI));
	face->SetColor(faceColor);
}


QuaternionLerpSphere::~QuaternionLerpSphere(void)
{
	delete face;
	delete marker;
}


void QuaternionLerpSphere::ShowLocalSpins(int _state)
{
	showLocalSpins = _state;
}


bool QuaternionLerpSphere::IsStartLerpValid(void)const
{
	return startLerpValid;
}



bool QuaternionLerpSphere::IsTargetLerpValid(void)const
{
	return targetLerpValid;
}



void QuaternionLerpSphere::SetStartQ(void)
{
	startLerp = GetRotation() * Vector3::UNIT_Z;
	startLerpValid = true;
}


void QuaternionLerpSphere::SetTargetQ(void)
{
	targetLerp = GetRotation() * Vector3::UNIT_Z;
	targetLerpValid = true;
}


void QuaternionLerpSphere::SetStartQ(Quaternion _quaternion)
{
	startLerp = _quaternion * Vector3::UNIT_Z;
	startLerpValid = true;
}



void QuaternionLerpSphere::SetTargetQ(Quaternion _quaternion)
{
	targetLerp = _quaternion * Vector3::UNIT_Z;
	targetLerpValid = true;
}


SimpleList<Vector3>& QuaternionLerpSphere::GetPathList(void)
{
	return pathList;
}


void QuaternionLerpSphere::Update(float _delta)
{
	face->SetColor(RGBA::_GetRandom());
}


void QuaternionLerpSphere::SetQLerp(QuaternionLerp* _qLerp)
{
	qLerp = _qLerp;
}


void QuaternionLerpSphere::Draw(void)
{
	if(IsShowEnabled())
	{
		GraphicsSurface::Draw();

		BeginTransform();
		if(showLocalSpins)
		{
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
		}
		Renderer::_SetLineWidth(3.0f);
		face->BeginTransform();
			
		GraphicsLine::_Draw(Vector3() + Vector3(0, 0.04f, 0), (xAxis / 4.0f) + Vector3(0, 0.04f, 0), xColor + RGBA(1, 0, 0, 0));
		GraphicsLine::_Draw(Vector3() + Vector3(0, 0.04f, 0), (-zAxis / 4.0f) + Vector3(0, 0.04f, 0), yColor + RGBA(0, 1, 0, 0));
			
		face->EndTransform();
		Renderer::_SetLineWidth(6.0f);

		face->Draw();

		EndTransform();
		if(startLerpValid)
		{
			Vector3 v = Vector3::_FromTo(Vector3(), startLerp);
			v.Normalize();
			v.z *= 1.005f;

			marker->SetPosition(v);
			marker->SetRotation(Quaternion(Vector3::UNIT_Z, startLerp));


			Renderer::_SetLineWidth(3.0f);
			marker->BeginTransform();
			GraphicsStrip::_Draw(vList, marker->GetColorRGBA());
			marker->EndTransform();


			marker->SetRotation(Quaternion());
			marker->BeginTransform();
			Vector3 v1(0, 0.07f, 0);
			Vector3 v2(0, -0.07f, 0);
			GraphicsLine::_Draw(Quaternion::_GetNormalize(qLerp->GetFrom())*v1, Quaternion::_GetNormalize(qLerp->GetFrom())*v2, marker->GetColorRGBA());

			Vector3 v3(-0.07f, 0, 0);
			Vector3 v4(0.07f, 0, 0);
			GraphicsLine::_Draw(Quaternion::_GetNormalize(qLerp->GetFrom())*v3, Quaternion::_GetNormalize(qLerp->GetFrom())*v4, marker->GetColorRGBA());
				
			marker->EndTransform();
		}

		if(targetLerpValid)
		{
			Vector3 v = Vector3::_FromTo(Vector3(), targetLerp);
			v.Normalize();
			v.z *= 1.005f;

			marker->SetPosition(v);
			marker->SetRotation(Quaternion(Vector3::UNIT_Z, targetLerp));

			Renderer::_SetLineWidth(3.0f);
			marker->BeginTransform();
			GraphicsStrip::_Draw(vList, marker->GetColorRGBA());
			marker->EndTransform();
				
			
			marker->SetRotation(Quaternion());
			marker->BeginTransform();
			Vector3 v1(0, 0.07f, 0);
			Vector3 v2(0, -0.07f, 0);
			GraphicsLine::_Draw(Quaternion::_GetNormalize(qLerp->GetTo())*v1, Quaternion::_GetNormalize(qLerp->GetTo())*v2, marker->GetColorRGBA());

			Vector3 v3(-0.07f, 0, 0);
			Vector3 v4(0.07f, 0, 0);
			GraphicsLine::_Draw(Quaternion::_GetNormalize(qLerp->GetTo())*v3, Quaternion::_GetNormalize(qLerp->GetTo())*v4, marker->GetColorRGBA());
				
			marker->EndTransform();
		}
		{
			Renderer::_SetLineWidth(4.0f);
			GraphicsStrip::_Draw(pathList, marker->GetColorRGBA());
		}
	}
}
 
		
RGBA QuaternionLerpSphere::GetXColor(void)const
{
	return xColor;
}

RGBA QuaternionLerpSphere::GetYColor(void)const
{
	return yColor;
}


RGBA QuaternionLerpSphere::GetZColor(void)const
{
	return zColor;
}


void QuaternionLerpSphere::SelectX(void)
{
	selected = CIRCLE_X;
}


void QuaternionLerpSphere::SelectY(void)
{
	selected = CIRCLE_Y;
}


void QuaternionLerpSphere::SelectZ(void)
{
	selected = CIRCLE_Z;
}


int32 QuaternionLerpSphere::GetSelected(void)const
{
	return selected;
}

void QuaternionLerpSphere::Unselect(void)
{
	selected = NO_SELECTED;
}

 
bool QuaternionLerpSphere::SaveToFile(File& _file)
{
	return false;
}



bool QuaternionLerpSphere::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}


	
bool QuaternionLerpSphere::LoadFromFile(File& _file)
{
	return false;
}
