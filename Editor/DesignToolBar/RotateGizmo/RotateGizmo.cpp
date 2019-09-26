
#include "RotateGizmo.h"
#include <Engine/Output/Graphics/Shape/Line/GraphicsStrip/GraphicsStrip.h>



RotateGizmo::RotateGizmo(void): selected(NO_SELECT), arrowCursor(NIL), unclenchedHandCursor(NIL), clenchedHandCursor(NIL), scale(1.01f), startRotation(false)
{
	sphere = new GraphicsSurface();
	sphere->SetColor(RGBA(230,230,230,99));

	{
		Mesh* mesh = new Mesh();
		mesh->CreateIcosphere(4);
		sphere->BindMesh(mesh);
	}

	sphere->SetScale(Vector3(0.5,0.5,0.5));

	segments = 360 / 5;
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
	selectColor = RGBA(250, 249, 110, 255);
	texture = new Texture2D();
	PixelImage img(Vector2(800, 600), RGBA(RGBA::WHITE_COLOR));
	texture->Create(Texture2D::QUAD_TEXTURE_MAP, img);
	gizmo = new TexturingSurface();
		
	{
		Mesh* mesh = new Mesh();
		mesh->CreateQuad(Quad(Vector3(), texture->GetImageSize()));
		gizmo->BindMesh(mesh);
	}

	gizmo->BindFrontTexture(texture);
		
	gizmo->SetPosition(Vector3(texture->GetImageSize().x / 2, texture->GetImageSize().y / 2, -0.3));
	gizmo->SetShowEnable(false);
}


RotateGizmo::~RotateGizmo(void)
{
	delete texture;
	delete gizmo;

	delete sphere;
}



MagicList<GraphicsSurface>& RotateGizmo::GetSelectedList(void)
{
	return selectedList;
}



void RotateGizmo::Reset(void)
{
	if(gizmo->IsShowEnabled())
	{
		gizmo->SetShowEnable(false);

		targetObject.Clear(MagicContainer::REMOVE_NODE);
	}
}


TexturingSurface* RotateGizmo::GetGizmoSprite(void)const
{
	return gizmo;
}



void RotateGizmo::Init(TexturingSurface* _arrowCursor, TexturingSurface* _unclenchedHandCursor, TexturingSurface* _clenchedHandCursor)
{
	arrowCursor = _arrowCursor;
	unclenchedHandCursor = _unclenchedHandCursor;
	clenchedHandCursor = _clenchedHandCursor;
}


void RotateGizmo::SetSelectedList(MagicList<GraphicsSurface>& _list)
{
	selectedList = _list;
}


void RotateGizmo::ClearSelectedList(void)
{
	selectedList.Clear(MagicContainer::REMOVE_NODE);
}


void RotateGizmo::Draw(void)
{
	if(IsShowEnabled() && targetObject.IsNotEmpty())
	{
		Camera3D::_GetPicked()->BeginTransform(Renderer::PROJECTION);
		Camera3D::_GetPicked()->BeginTransform(Renderer::MODELVIEW);
		sphere->Draw();
		float preLineWidth = Renderer::_GetLineWidth();
		Renderer::_SetLineWidth(4.0f);

		sphere->BeginTransform();
		point1 = yAxis * scale;
		SimpleList<Vector3> xList;
		xList.AddToTail(point1);

		for(int32 i = 0; i < segments; i++)
		{  
			point2 = qX * point1;
				
			xList.AddToTail(point2);
			point1 = point2;
		}
		GraphicsStrip::_Draw(xList, selected == X_AXIS ? selectColor : xColor);
		point1 = xAxis * scale;
		SimpleList<Vector3> yList;
		yList.AddToTail(point1);

		for(int32 i = 0; i < segments; i++)
		{
			point2 = qY * point1;
				
			yList.AddToTail(point2);
			point1 = point2;
		}
		GraphicsStrip::_Draw(yList, selected == Y_AXIS ? selectColor : yColor);
		point1 = xAxis * scale;
		SimpleList<Vector3> zList;
		zList.AddToTail(point1);

		for(int32 i = 0; i < segments; i++)
		{
			point2 = qZ * point1;
				
			zList.AddToTail(point2);
			point1 = point2;
		}
		GraphicsStrip::_Draw(zList, selected == Z_AXIS ? selectColor : zColor);


		GraphicsSurface::Draw();

		sphere->EndTransform();
		Camera3D::_GetPicked()->EndTransform(Renderer::MODELVIEW);
		Camera3D::_GetPicked()->EndTransform(Renderer::PROJECTION);

		Renderer::_SetLineWidth(preLineWidth);
	}
}

void RotateGizmo::SetHome(void)
{
	for(int32 i = 0; i < selectedList.GetSize(); ++i)
	{
		GraphicsSurface* object = selectedList.Get(i);
		object->SetRotation(Quaternion());
	}
	Reset();
}


GraphicsSurface* RotateGizmo::GetTargetObject(void)const
{
	return targetObject.Get();
}


void RotateGizmo::SetPosition(Vector3 _position)
{
	GraphicsSurface::SetPosition(_position);
			
	sphere->SetPosition(GetPosition());
}


Mouse::Event* RotateGizmo::MouseEventFilter(Mouse::Event* _event)
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
							sphere->SetRotation(object->GetRotation());
							float divisor = 3.0f; 
							float xScale = fabsf(object->GetScale().x);
							float yScale = fabsf(object->GetScale().y);
							float zScale = fabsf(object->GetScale().z);
							float size = 1.0f;

							if(targetObject.IsNotEmpty() && targetObject.Get()->GetMesh())
							{
								divisor = 3.0f;

								float width = targetObject.Get()->GetMesh()->GetBoundingBox().GetWidth();
								float height = targetObject.Get()->GetMesh()->GetBoundingBox().GetHeight();
								float length = targetObject.Get()->GetMesh()->GetBoundingBox().GetLength();

								size = (width + height + length) / divisor;
							}

							float max = (xScale + yScale + zScale) / divisor;
							sphere->SetScale(Vector3(size * (max / 2.0f), size * (max / 2.0f),  size * (max / 2.0f)));
							break;
						}
					}
				}
			}

			if(!picked && selected == NO_SELECT)
			{
				Reset();
			}

			if(selected != NO_SELECT)
			{
				clenchedHandCursor->SetShowEnable(true);
				arrowCursor->SetShowEnable(false);
				unclenchedHandCursor->SetShowEnable(false);
				startRotation = true;

				if(Camera3D::_GetPicked())
				{
					if(sphere->IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						startCursorPosition = intersectedPoint;
						previousCursorPosition = intersectedPoint;
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
				startRotation = false;
							
				CheckSelect();
			}
		}
	}
	else if(_event->GetEventType() == Mouse::MOVE)
	{
		Vector2 cursorPosition = Mouse::_GetCursorPosition(Renderer::_GetPicked());
		cursorPosition.y = Renderer::_GetPicked()->GetViewportSize().y - cursorPosition.y;

		arrowCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - (arrowCursor->GetFrontTexture()->GetImageSize().y-1), 0.0f));
		unclenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((unclenchedHandCursor->GetFrontTexture()->GetImageSize().y-1)/2), 0.0f));
		clenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((clenchedHandCursor->GetFrontTexture()->GetImageSize().y-1)/2), 0.0f));


		if(gizmo->IsShowEnabled())
		{
			if(!startRotation)
			{
				CheckSelect();
			}
			else if(selected != NO_SELECT)
			{
				if(Camera3D::_GetPicked())
				{
					if(sphere->IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						Vector3 currentCursorPosition = intersectedPoint;
	 
						Vector3 v1 = Vector3::_FromTo(sphere->GetPosition(), previousCursorPosition);
						v1.Normalize();
						v1 = Quaternion::_GetInverse(sphere->GetRotation()) * v1;

						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), currentCursorPosition);
						v2.Normalize();
						v2 = Quaternion::_GetInverse(sphere->GetRotation()) * v2;

						Vector3 v3 = Vector3::_FromTo(sphere->GetPosition(), startCursorPosition);
						v3.Normalize();
						v3 = Quaternion::_GetInverse(sphere->GetRotation()) * v3;
						for(int32 i = 0; i < selectedList.GetSize(); ++i)
						{
							GraphicsSurface* object = selectedList.Get(i);

							switch(selected)
							{
								case X_AXIS:
								{
									float x1 = v1.GetAngleX();
									float x2 = v2.GetAngleX();
										
									object->SetRotation(object->GetRotation() * Quaternion(Matrix4x4::_RotationX(x2 - x1)));
									sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationX(x2 - x1)));
									previousCursorPosition = currentCursorPosition;
											
									float x3 = v3.GetAngleX();
									break;
								}

								case Y_AXIS:
								{
									float y1 = v1.GetAngleY();
									float y2 = v2.GetAngleY();

									object->SetRotation(object->GetRotation() * Quaternion(Matrix4x4::_RotationY(y2 - y1)));
									sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationY(y2 - y1)));
									previousCursorPosition = currentCursorPosition;
											
									float y3 = v3.GetAngleY();
									break;
								}

								case Z_AXIS:
								{
									float z1 = v1.GetAngleZ();
									float z2 = v2.GetAngleZ();

									object->SetRotation(object->GetRotation() * Quaternion(Matrix4x4::_RotationZ(z2 - z1)));
									sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationZ(z2 - z1)));
									previousCursorPosition = currentCursorPosition;
											
									float z3 = v3.GetAngleZ();
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	return _event;
}


void RotateGizmo::CheckSelect(void)
{
	RGBA pixelColor;
	PixelImage img;

	if(texture->GetImage(img))
	{ 
		Vector2 cursor = Mouse::_GetCursorPosition(Renderer::_GetPicked());
		cursor.y = Renderer::_GetPicked()->GetViewportSize().y - cursor.y;

		if(!img.GetPixel(cursor, pixelColor))
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
	else
	{
		if(selectColor != pixelColor)
		{
			selected = NO_SELECT;
			arrowCursor->SetShowEnable(true);
			unclenchedHandCursor->SetShowEnable(false);
			clenchedHandCursor->SetShowEnable(false);
		}
	}
}
