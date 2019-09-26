
#include "TranslateGizmo.h"



TranslateGizmo::TranslateGizmo(void): arrowCursor(NIL), unclenchedHandCursor(NIL), clenchedHandCursor(NIL), translatingEnable(false), selected(NO_SELECT), gizmo(NIL), texture(NIL)
{
	gizmoSize = 0.8f;
	dirLength = 0.585f;
	xColor = RGBA(163, 21, 21, 255);
	yColor = RGBA(118, 185, 0, 255);
	zColor = RGBA(55, 111, 194, 255);
	selectColor = RGBA(250, 249, 110, 255);
	xyColor = RGBA(235, 235, 235, 90);
	xzColor = RGBA(225, 225, 225, 90);
	yzColor = RGBA(230, 230, 230, 90);
	XY = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
		mesh->CreateQuad(Quad(Vector3(), Vector2(gizmoSize, gizmoSize)));
		XY->BindMesh(mesh);
	}
	XY->SetBackFaceCullingEnable(false); 
	XY->SetColor(xyColor);
	XY->SetPosition(Vector3(0.0f, 0.0f, -gizmoSize / 2.0f));
	XY->SetIntersectionMode(AbstractSurface::APPLY_TRANSFORMATION);
	XZ = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
		mesh->CreateQuad(Quad(Vector3(), Vector2(gizmoSize, gizmoSize)));
		XZ->BindMesh(mesh);
	}
	XZ->SetBackFaceCullingEnable(false);
	XZ->SetColor(xzColor);
	XZ->SetRotation(Quaternion(Matrix4x4::_RotationX(-Angle::HALF_PI)));
	XZ->SetPosition(Vector3(0.0f, -gizmoSize / 2.0f, 0.0f));	
	XZ->SetIntersectionMode(AbstractSurface::APPLY_TRANSFORMATION);
	YZ = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
		mesh->CreateQuad(Quad(Vector3(), Vector2(gizmoSize, gizmoSize)));
		YZ->BindMesh(mesh);
	}
	YZ->SetBackFaceCullingEnable(false);
	YZ->SetColor(yzColor);
	YZ->SetRotation(Quaternion(Matrix4x4::_RotationY(Angle::HALF_PI)));
	YZ->SetPosition(Vector3(-gizmoSize / 2.0f, 0.0f, 0.0f));
	YZ->SetIntersectionMode(AbstractSurface::APPLY_TRANSFORMATION);
	xDir = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid(0.02f);
		xDir->BindMesh(mesh);
	}
	xDir->SetColor(xColor);
	xDir->SetPosition(Vector3(dirLength, -gizmoSize / 2.0f, -gizmoSize / 2.0f));
	xDir->SetScale(Vector3(1.0f, 1.5f, 1.0f));
	xDir->SetRotation(Quaternion(Matrix4x4::_RotationZ(-Angle::HALF_PI)));
	yDir = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid(0.02f);
		yDir->BindMesh(mesh);
	}
	yDir->SetColor(yColor);
	yDir->SetPosition(Vector3(-gizmoSize / 2.0f, dirLength, -gizmoSize / 2.0f));
	yDir->SetScale(Vector3(1.0f, 1.5f, 1.0f));
	zDir = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid(0.02f);
		zDir->BindMesh(mesh);
	}
	zDir->SetColor(zColor);
	zDir->SetPosition(Vector3(-gizmoSize / 2.0f, -gizmoSize / 2.0f, dirLength));
	zDir->SetScale(Vector3(1.0f, 1.5f, 1.0f));
	zDir->SetRotation(Quaternion(Matrix4x4::_RotationX(Angle::HALF_PI)));
	xyPlane = new Plane();
	xyPlane->Set(Vector3(0.0f, 0.0f, -gizmoSize / 2.0f), Vector3::UNIT_Z);
	xyPlane->SetIntersectionMode(AbstractSurface::APPLY_TRANSFORMATION);
	xzPlane = new Plane();
	xzPlane->Set(Vector3(0.0f, -gizmoSize / 2.0f, -gizmoSize / 2.0f), Vector3::UNIT_Y);
	xzPlane->SetIntersectionMode(AbstractSurface::APPLY_TRANSFORMATION);
	yzPlane = new Plane();
	yzPlane->Set(Vector3(-gizmoSize / 2.0f, -gizmoSize / 2.0f, 0.0f), Vector3::UNIT_X);
	yzPlane->SetIntersectionMode(AbstractSurface::APPLY_TRANSFORMATION);
	texture = new Texture2D();
	PixelImage image(Renderer::_GetPicked()->GetViewportSize(), RGBA(RGBA::INVISIBLE_COLOR));
	texture->Create(Texture2D::QUAD_TEXTURE_MAP, image);
	gizmo = new TexturingSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateQuad(Quad(Vector3(), texture->GetImageSize()));
		gizmo->BindMesh(mesh);
	}

	gizmo->BindFrontTexture(texture);
	gizmo->SetPosition(Vector3(texture->GetImageSize().x / 2.0f, texture->GetImageSize().y / 2.0f, -0.3f));
	gizmo->SetShowEnable(false);
}


TranslateGizmo::~TranslateGizmo(void)
{
	delete xDir;
	delete yDir;
	delete zDir;

	delete xyPlane;
	delete xzPlane; 
	delete yzPlane;

	delete XZ;
	delete XY;
	delete YZ;

	delete texture;
	delete gizmo;
}



void TranslateGizmo::Reset(void)
{
	if(gizmo->IsShowEnabled())
	{
		gizmo->SetShowEnable(false);

		targetObject.Clear(MagicContainer::REMOVE_NODE);
	}
}


MagicList<GraphicsSurface>& TranslateGizmo::GetSelectedList(void)
{
	return selectedList;
}


TexturingSurface* TranslateGizmo::GetGizmoSprite(void)const
{
	return gizmo;
}


void TranslateGizmo::SetPosition(Vector3 _position)
{
	GraphicsSurface::SetPosition(_position);
	xyPlane->SetPosition(Vector3(0.0f, 0.0f, -gizmoSize / 2.0f) + _position);
	xzPlane->SetPosition(Vector3(0.0f, -gizmoSize / 2.0f, -gizmoSize / 2.0f) + _position);
	yzPlane->SetPosition(Vector3(-gizmoSize / 2.0f, -gizmoSize / 2.0f, 0.0f) + _position);
}


void TranslateGizmo::Init(TexturingSurface* _arrowCursor, TexturingSurface* _unclenchedHandCursor, TexturingSurface* _clenchedHandCursor)
{
	arrowCursor = _arrowCursor;
	unclenchedHandCursor = _unclenchedHandCursor;
	clenchedHandCursor = _clenchedHandCursor;
}


bool TranslateGizmo::IsSelected(void)const
{
	return selected != NO_SELECT;
}

	

void TranslateGizmo::SetSelectedList(MagicList<GraphicsSurface>& _list)
{
	selectedList = _list;
}


void TranslateGizmo::ClearSelectedList(void)
{
	selectedList.Clear(MagicContainer::REMOVE_NODE);
}


GraphicsSurface* TranslateGizmo::GetTargetObject(void)const
{
	return targetObject.Get();
}


void TranslateGizmo::SetHome(void)
{
	for(int32 i = 0; i < selectedList.GetSize(); ++i)
	{
		GraphicsSurface* object = selectedList.Get(i);
		object->SetPosition(Vector3());
	}
	Reset();
}


Mouse::Event* TranslateGizmo::MouseEventFilter(Mouse::Event* _event)
{
	if(_event->GetEventType() == Mouse::KEY_DOWN)
	{
		if(_event->GetKey() == Mouse::LEFT_BUTTON)
		{
			bool picked = false;
			for(int32 i = 0; i < selectedList.GetSize(); ++i)
			{
				GraphicsSurface* object = selectedList.Get(i);
							
				GraphicsContainer* scene = dynamic_cast<GraphicsContainer*>(object->GetGraphicsContainer());

				if(scene) 
				{
					Camera3D* camera = scene->GetCamera();

					if(camera)
					{
						if(object->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
						{
							targetObject.Clear(MagicContainer::REMOVE_NODE);
							targetObject.Attach(object);
							picked = true;

							Vector3 parentScenePosition = scene ? scene->GetPosition() : Vector3();

							SetPosition(parentScenePosition + object->GetPosition());
							gizmo->SetShowEnable(true);
							float divisor = 3.0f;
							float xScale = fabsf(object->GetScale().x);
							float yScale = fabsf(object->GetScale().y);
							float zScale = fabsf(object->GetScale().z);
							float width = targetObject.Get()->GetMesh()->GetBoundingBox().GetWidth();
							float height = targetObject.Get()->GetMesh()->GetBoundingBox().GetHeight();
							float length = targetObject.Get()->GetMesh()->GetBoundingBox().GetLength();
							float size = (width + height + length) / divisor;
						
							float maxScale = (xScale + yScale + zScale) / divisor;
							Vector3 scale(size * maxScale, size * maxScale, size * maxScale);
							SetScale(scale);
							break;
						}
					}
				}
			}

			if(!picked && selected == NO_SELECT)
			{
				Reset();
			}
			else if(selected != NO_SELECT)
			{
				clenchedHandCursor->SetShowEnable(true);
				arrowCursor->SetShowEnable(false);
				unclenchedHandCursor->SetShowEnable(false);

				translatingEnable = true;
				if(Camera3D::_GetPicked())
				{
					selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));

					if(selected == X_AXIS)
					{
						if(xyPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
						{
							previousIntersectedPoint = intersectedPoint;
						}
					}
					else if(selected == Y_AXIS)
					{
						if(xyPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
						{
							previousIntersectedPoint = intersectedPoint;
						}
					}
					else if(selected == Z_AXIS)
					{
						if(xzPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
						{
							previousIntersectedPoint = intersectedPoint;
						}
					}
					else if(selected == XY_PLANE)
					{
						if(xyPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
						{
							previousIntersectedPoint = intersectedPoint;
						}
					}
					else if(selected == XZ_PLANE)
					{
						if(xzPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
						{
							previousIntersectedPoint = intersectedPoint;
						}
					}
					else if(selected == YZ_PLANE)
					{
						if(yzPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
						{
							previousIntersectedPoint = intersectedPoint;
						}
					}
				}
			}
		}
	}
	else if(_event->GetEventType() == Mouse::KEY_UP)
	{
		if(_event->GetKey() == Mouse::LEFT_BUTTON)
		{
			if(selected != NO_SELECT)
			{
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);

				translatingEnable = false;
						
				CheckSelect();
			}
		}
	} 
	else if(_event->GetEventType() == Mouse::MOVE)
	{
		Vector2 cursorPosition = Mouse::_GetCursorPosition(Renderer::_GetPicked());
		cursorPosition.y = Renderer::_GetPicked()->GetViewportSize().y - cursorPosition.y;
		arrowCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - (arrowCursor->GetFrontTexture()->GetImageSize().y - 1.0f), 0.0f));
		unclenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((unclenchedHandCursor->GetFrontTexture()->GetImageSize().y - 1.0f) / 2.0f), 0.0f));
		clenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((clenchedHandCursor->GetFrontTexture()->GetImageSize().y - 1.0f) / 2.0f), 0.0f));

		if(gizmo->IsShowEnabled())
		{
			if(!translatingEnable)
			{
				CheckSelect();
			}
			else
			{
				if(selected != NO_SELECT)
				{
					if(Camera3D::_GetPicked())
					{
						selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));
						for(int32 i = 0; i < selectedList.GetSize(); ++i)
						{
							GraphicsSurface* object = selectedList.Get(i);
							
							switch(selected)
							{
								case X_AXIS:
								{
									if(xyPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
									{
										object->SetPosition(object->GetPosition() + Vector3(intersectedPoint.x - previousIntersectedPoint.x, 0.0f, 0.0f)); 
											
										GraphicsObject* scene = targetObject.Get()->GetGraphicsContainer();
										
										Vector3 parentScenePosition = scene ? scene->GetPosition() : Vector3();

										SetPosition(parentScenePosition + targetObject.Get()->GetPosition());
									}
									break;
								}

								case Y_AXIS:
								{
									if(xyPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
									{
										object->SetPosition(object->GetPosition() + Vector3(0.0f, intersectedPoint.y - previousIntersectedPoint.y, 0.0f)); 
											
										GraphicsObject* scene = targetObject.Get()->GetGraphicsContainer();
										
										Vector3 parentScenePosition = scene ? scene->GetPosition() : Vector3();

										SetPosition(parentScenePosition + targetObject.Get()->GetPosition());
									}
									break;
								}

								case Z_AXIS:
								{
									if(xzPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
									{
										object->SetPosition(object->GetPosition() + Vector3(0.0f, 0.0f, intersectedPoint.z - previousIntersectedPoint.z));
												
										GraphicsObject* scene = targetObject.Get()->GetGraphicsContainer();
										
										Vector3 parentScenePosition = scene ? scene->GetPosition() : Vector3();

										SetPosition(parentScenePosition + targetObject.Get()->GetPosition());
									}
									break;
								}

								case XY_PLANE:
								{
									if(xyPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
									{
										object->SetPosition(object->GetPosition() + Vector3(intersectedPoint.x - previousIntersectedPoint.x, intersectedPoint.y - previousIntersectedPoint.y, 0.0f));
												
										GraphicsObject* scene = targetObject.Get()->GetGraphicsContainer();
										
										Vector3 parentScenePosition = scene ? scene->GetPosition() : Vector3();

										SetPosition(parentScenePosition + targetObject.Get()->GetPosition());
									}
									break;
								}

								case XZ_PLANE:
								{
									if(xzPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
									{
										object->SetPosition(object->GetPosition() + Vector3(intersectedPoint.x - previousIntersectedPoint.x, 0.0f, intersectedPoint.z - previousIntersectedPoint.z));
												
										GraphicsObject* scene = targetObject.Get()->GetGraphicsContainer();
										
										Vector3 parentScenePosition = scene ? scene->GetPosition() : Vector3();

										SetPosition(parentScenePosition + targetObject.Get()->GetPosition());
									}
									break;
								}

								case YZ_PLANE:
								{
									if(yzPlane->IsIntersected(selectRay, &intersectedPoint) != -1)
									{
										object->SetPosition(object->GetPosition() + Vector3(0.0f, intersectedPoint.y - previousIntersectedPoint.y, intersectedPoint.z - previousIntersectedPoint.z));
												
										GraphicsObject* scene = targetObject.Get()->GetGraphicsContainer();
										
										Vector3 parentScenePosition = scene ? scene->GetPosition() : Vector3();

										SetPosition(parentScenePosition + targetObject.Get()->GetPosition());
									}
									break;
								}
							}
						}
						previousIntersectedPoint = intersectedPoint;
					}
				}
			}
		}
	}
	return _event;
}



void TranslateGizmo::CheckSelect(void)
{
	RGBA pixelColor;
	PixelImage image;

	if(texture->GetImage(image))
	{ 
		Vector2 cursorPosition = Mouse::_GetCursorPosition(Renderer::_GetPicked());
		cursorPosition.y = Renderer::_GetPicked()->GetViewportSize().y - cursorPosition.y;
			
		if(!image.GetPixel(cursorPosition, pixelColor))
		{
			return; 
		}
	}
	else
	{ 
		return; 
	}
			
	if(xColor == pixelColor)
	{
		selected = X_AXIS;
		unclenchedHandCursor->SetShowEnable(true);
		clenchedHandCursor->SetShowEnable(false);
		arrowCursor->SetShowEnable(false);
	}
	else if(yColor == pixelColor)
	{
		selected = Y_AXIS;
		unclenchedHandCursor->SetShowEnable(true);
		clenchedHandCursor->SetShowEnable(false);
		arrowCursor->SetShowEnable(false);
	}
	else if(zColor == pixelColor)
	{
		selected = Z_AXIS;
		unclenchedHandCursor->SetShowEnable(true);
		clenchedHandCursor->SetShowEnable(false);
		arrowCursor->SetShowEnable(false);
	}
	else if(selectColor != pixelColor)
	{ 
		if(Camera3D::_GetPicked())
		{
			selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));
			Vector3 xyPos = XY->GetPosition();
			Vector3 xzPos = XZ->GetPosition();
			Vector3 yzPos = YZ->GetPosition();
			Vector3 xyScale = XY->GetScale();
			Vector3 xzScale = XZ->GetScale();
			Vector3 yzScale = YZ->GetScale();
			Matrix4x4 scaleMatrix;
			scaleMatrix.Scaling(GetScale());
			XY->SetScale(GetScale());
			XZ->SetScale(GetScale()); 
			YZ->SetScale(GetScale());
			XY->SetPosition(scaleMatrix * Vector3(0.0f, 0.0f, -gizmoSize / 2.0f) + GetPosition());
			XZ->SetPosition(scaleMatrix * Vector3(0.0f, -gizmoSize / 2.0f, 0.0f) + GetPosition());
			YZ->SetPosition(scaleMatrix * Vector3(-gizmoSize / 2.0f, 0.0f, 0.0f) + GetPosition());

			if(XY->IsIntersected(selectRay, &intersectedPoint) != -1)
			{
				selected = XY_PLANE;
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(XZ->IsIntersected(selectRay, &intersectedPoint) != -1)
			{
				selected = XZ_PLANE;	
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(YZ->IsIntersected(selectRay, &intersectedPoint) != -1)
			{
				selected = YZ_PLANE;
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else 
			{
				selected = NO_SELECT;
				arrowCursor->SetShowEnable(true);
				unclenchedHandCursor->SetShowEnable(false);
				clenchedHandCursor->SetShowEnable(false);
			}
			XY->SetPosition(xyPos);
			XZ->SetPosition(xzPos);
			YZ->SetPosition(yzPos);
			XY->SetScale(xyScale);
			XZ->SetScale(xzScale);
			YZ->SetScale(yzScale);
		}
	} 
}



void TranslateGizmo::Draw(void)
{
	if(IsShowEnabled() && targetObject.IsNotEmpty())
	{
		bool zBufferEnable = Renderer::_GetPicked()->IsZBufferEnabled();
		Renderer::_GetPicked()->SetZBufferEnable(false);
		float lineWidth = Renderer::_GetLineWidth();
		Renderer::_SetLineWidth(4.0f);
		Camera3D::_GetPicked()->BeginTransform(Renderer::PROJECTION);
		Camera3D::_GetPicked()->BeginTransform(Renderer::MODELVIEW);

		BeginTransform();
		XZ->SetColor(selected == XZ_PLANE ? RGBA(250, 249, 110, 120) : xzColor);
		XZ->Draw();  

		XY->SetColor(selected == XY_PLANE ? RGBA(250, 249, 110, 120) : xyColor);
		XY->Draw();
			
		YZ->SetColor(selected == YZ_PLANE ? RGBA(250, 249, 110, 120) : yzColor);
		YZ->Draw();
		GraphicsLine::_Draw(Vector3(-gizmoSize / 2.0f, -gizmoSize / 2.0f, -gizmoSize / 2.0f), Vector3(dirLength, -gizmoSize / 2.0f, -gizmoSize / 2.0f), selected == X_AXIS ? selectColor : xColor);
		xDir->Draw();

		GraphicsLine::_Draw(Vector3(-gizmoSize / 2.0f, -gizmoSize / 2.0f, -gizmoSize / 2.0f), Vector3(-gizmoSize / 2.0f, dirLength, -gizmoSize / 2.0f), selected == Y_AXIS ? selectColor : yColor);
		yDir->Draw();
			
		GraphicsLine::_Draw(Vector3(-gizmoSize / 2.0f, -gizmoSize / 2.0f, -gizmoSize / 2.0f), Vector3(-gizmoSize / 2.0f, -gizmoSize / 2.0f, dirLength), selected == Z_AXIS ? selectColor : zColor);
		zDir->Draw();

		EndTransform();
		Camera3D::_GetPicked()->EndTransform(Renderer::MODELVIEW);
		Camera3D::_GetPicked()->EndTransform(Renderer::PROJECTION);
		Renderer::_SetLineWidth(lineWidth);
		Renderer::_GetPicked()->SetZBufferEnable(zBufferEnable);
	}
}