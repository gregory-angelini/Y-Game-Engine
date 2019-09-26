
#include "ScaleGizmo.h"


ScaleGizmo::ScaleGizmo(void): selected(NO_SELECT), arrowCursor(NIL), unclenchedHandCursor(NIL), clenchedHandCursor(NIL), scale(1.01f), startScaling(false), aabbScale(1.1f), selectedFace(NO_SELECT), mesh(NIL)
{
	{
		mesh = new Mesh(); 
		BindMesh(mesh);
	}

	xColor = RGBA(163, 21, 21, 255);
	yColor = RGBA(118, 185, 0, 255);
	zColor = RGBA(55, 111, 194, 255);
	selectColor = RGBA(250, 249, 110, 255);
	cornerPointColor = RGBA(221, 95, 155, 255);
		
	scaleTiny = 0.004f;
	texture = new Texture2D();
	PixelImage img(Vector2(800, 600), RGBA(RGBA::WHITE_COLOR));
	texture->Create(Texture2D::QUAD_TEXTURE_MAP, img);
	gizmo = new TexturingSurface();
		
	Mesh* mesh = new Mesh();
	mesh->CreateQuad(Quad(Vector3(), texture->GetImageSize()));
	gizmo->BindMesh(mesh);

	gizmo->BindFrontTexture(texture);
		
	gizmo->SetPosition(Vector3(texture->GetImageSize().x / 2, texture->GetImageSize().y / 2, -0.3));
	gizmo->SetShowEnable(false);
	SetScale(Vector3(aabbScale, aabbScale, aabbScale));
	SetColor(RGBA(230,230,230,99));


	vertexSize = 0.05f;
	vFace1 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vFace1->BindMesh(mesh);
	}

	vFace2 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vFace2->BindMesh(mesh);
	}

	vFace3 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vFace3->BindMesh(mesh);
	}

	vFace4 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vFace4->BindMesh(mesh);
	}
	vBack1 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vBack1->BindMesh(mesh);
	}

	vBack2 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vBack2->BindMesh(mesh);
	}

	vBack3 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vBack3->BindMesh(mesh);
	}

	vBack4 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vBack4->BindMesh(mesh);
	}
	vX1 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vX1->BindMesh(mesh);
	}

	vX2 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vX2->BindMesh(mesh);
	}

	vX3 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vX3->BindMesh(mesh);
	}

	vX4 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vX4->BindMesh(mesh);
	}
	vY1 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vY1->BindMesh(mesh);
	}

	vY2 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vY2->BindMesh(mesh);
	}

	vY3 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vY3->BindMesh(mesh);
	}

	vY4 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vY4->BindMesh(mesh);
	}
	vZ1 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vZ1->BindMesh(mesh);
	}

	vZ2 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vZ2->BindMesh(mesh);
	}

	vZ3 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vZ3->BindMesh(mesh);
	}

	vZ4 = new GraphicsSurface();
	{
		Mesh* mesh = new Mesh();
		mesh->CreateBox(Box(Vector3(), Vector3(vertexSize, vertexSize, vertexSize)));
		vZ4->BindMesh(mesh);
	}
}


ScaleGizmo::~ScaleGizmo(void)
{
	delete texture;
	delete gizmo;

	delete vFace1;
	delete vFace2;
	delete vFace3;
	delete vFace4;

	delete vBack1;
	delete vBack2;
	delete vBack3;
	delete vBack4;

	delete vY1;
	delete vY2;
	delete vY3;
	delete vY4;

	delete vX1;
	delete vX2;
	delete vX3;
	delete vX4;

	delete vZ1;
	delete vZ2;
	delete vZ3;
	delete vZ4;
}



void ScaleGizmo::Reset(void)
{
	if(gizmo->IsShowEnabled())
	{
		gizmo->SetShowEnable(false);
		if(targetObject.IsNotEmpty()) { targetObject.Get()->SetBoundingVolumeShowMode(GraphicsSurface::SHOW_SHORTEST_BOUNDING_VOLUME | GraphicsSurface::SHOW_TRANSFORMED_BOUNDING_VOLUME | GraphicsSurface::SHOW_BOUNDING_BOX); }

		targetObject.Clear(MagicContainer::REMOVE_NODE);
	}
}


MagicList<GraphicsSurface>& ScaleGizmo::GetSelectedList(void)
{
	return selectedList;
}


TexturingSurface* ScaleGizmo::GetGizmoSprite(void)const
{
	return gizmo;
}


void ScaleGizmo::ClampScale(GraphicsSurface* _object)
{
	Vector3 scale = _object->GetScale();
	if(scale.x < scaleTiny) 
	{ 
		scale.x = scaleTiny;
		{
		}
	}

	if(scale.y < scaleTiny) 
	{ 
		scale.y = scaleTiny;
		{
		}
	}

	if(scale.z < scaleTiny) 
	{ 
		scale.z = scaleTiny;
		{
		}
	}

	_object->SetScale(scale);
}



void ScaleGizmo::Init(TexturingSurface* _arrowCursor, TexturingSurface* _unclenchedHandCursor, TexturingSurface* _clenchedHandCursor)
{
	arrowCursor = _arrowCursor;
	unclenchedHandCursor = _unclenchedHandCursor;
	clenchedHandCursor = _clenchedHandCursor;
}


void ScaleGizmo::SetSelectedList(MagicList<GraphicsSurface>& _list)
{
	selectedList = _list;
}


void ScaleGizmo::ClearSelectedList(void)
{
	selectedList.Clear(MagicContainer::REMOVE_NODE);
}


void ScaleGizmo::Draw(void)
{
	if(IsShowEnabled() && targetObject.IsNotEmpty())
	{ 
		if(targetObject.IsNotEmpty())
		{
			Camera3D::_GetPicked()->BeginTransform(Renderer::PROJECTION);
			Camera3D::_GetPicked()->BeginTransform(Renderer::MODELVIEW);

			BeginTransform();

			float prePointWidth = Renderer::_GetPointSize();
			Renderer::_SetPointSize(10.0f);
			Renderer::_SetCurrentStack(Renderer::MODELVIEW);
			Renderer::_PushMatrix();
			Renderer::_MultMatrix(targetObject.Get()->GetRotation());
			Triangle tr1, tr2;
			switch(selectedFace)
			{
				case FRONT_FACE:
				{
					mesh->GetTriangle(0, tr1);
					mesh->GetTriangle(1, tr2);
					GraphicsSurface::_DrawCulledTriangle(tr1, RGBA(230,230,230,99));
					GraphicsSurface::_DrawCulledTriangle(tr2, RGBA(230,230,230,99));
					vX1->Draw();
					vX2->Draw();
					vY1->Draw();
					vY2->Draw();
					vFace1->Draw();
					vFace2->Draw();
					vFace3->Draw();
					vFace4->Draw();
					break;
				}

				case BACK_FACE:
				{
					mesh->GetTriangle(2, tr1);
					mesh->GetTriangle(3, tr2);
					GraphicsSurface::_DrawCulledTriangle(tr1, RGBA(230,230,230,99));
					GraphicsSurface::_DrawCulledTriangle(tr2, RGBA(230,230,230,99));
					vX3->Draw();
					vX4->Draw();
					vY3->Draw();
					vY4->Draw();
					vBack1->Draw();
					vBack2->Draw();
					vBack3->Draw();
					vBack4->Draw();
					break;
				}

				case LEFT_FACE:
				{
					mesh->GetTriangle(4, tr1);
					mesh->GetTriangle(5, tr2);
					GraphicsSurface::_DrawCulledTriangle(tr1, RGBA(230,230,230,99));
					GraphicsSurface::_DrawCulledTriangle(tr2, RGBA(230,230,230,99));
					vZ1->Draw();
					vZ2->Draw();
					vY1->Draw();
					vY3->Draw();
					vFace1->Draw();
					vFace4->Draw();
					vBack1->Draw();
					vBack4->Draw();
					break;
				}

				case RIGHT_FACE:
				{
					mesh->GetTriangle(6, tr1);
					mesh->GetTriangle(7, tr2);
					GraphicsSurface::_DrawCulledTriangle(tr1, RGBA(230,230,230,99));
					GraphicsSurface::_DrawCulledTriangle(tr2, RGBA(230,230,230,99));
					vZ3->Draw();
					vZ4->Draw();
					vY2->Draw();
					vY4->Draw();
					vFace2->Draw();
					vFace3->Draw();
					vBack2->Draw();
					vBack3->Draw();
					break;
				}

				case TOP_FACE:
				{
					mesh->GetTriangle(8, tr1);
					mesh->GetTriangle(9, tr2);
					GraphicsSurface::_DrawCulledTriangle(tr1, RGBA(230,230,230,99));
					GraphicsSurface::_DrawCulledTriangle(tr2, RGBA(230,230,230,99));
					vX1->Draw();
					vX3->Draw();
					vZ1->Draw();
					vZ3->Draw();
					vFace1->Draw();
					vFace2->Draw();
					vBack1->Draw();
					vBack2->Draw();
					break;
				}

				case BOTTOM_FACE:
				{
					mesh->GetTriangle(10, tr1);
					mesh->GetTriangle(11, tr2);
					GraphicsSurface::_DrawCulledTriangle(tr1, RGBA(230,230,230,99));
					GraphicsSurface::_DrawCulledTriangle(tr2, RGBA(230,230,230,99));
					vX2->Draw();
					vX4->Draw();
					vZ2->Draw();
					vZ4->Draw();
					vFace3->Draw();
					vFace4->Draw();
					vBack3->Draw();
					vBack4->Draw();
					break;
				}
			}

			Renderer::_SetCurrentStack(Renderer::MODELVIEW);
			Renderer::_PopMatrix();

			EndTransform();
			Camera3D::_GetPicked()->EndTransform(Renderer::MODELVIEW);
			Camera3D::_GetPicked()->EndTransform(Renderer::PROJECTION);

			Renderer::_SetPointSize(prePointWidth);
		}
	}
}


void ScaleGizmo::UpdateAABB(GraphicsSurface* _object)
{
	if(_object && _object->GetMesh())
	{
		aabb = _object->GetMesh()->GetBoundingBox();
		Matrix4x4 scaleM;
		scaleM.Scaling(_object->GetScale());
		aabb.leftBottom = scaleM * aabb.leftBottom;
		aabb.rightTop = scaleM * aabb.rightTop;

		vFace1->SetPosition(aabb.GetVertex1());
		vFace2->SetPosition(aabb.GetVertex2());
		vFace3->SetPosition(aabb.GetVertex3());
		vFace4->SetPosition(aabb.GetVertex4());

		vBack1->SetPosition(aabb.GetVertex5());
		vBack2->SetPosition(aabb.GetVertex6());
		vBack3->SetPosition(aabb.GetVertex7());
		vBack4->SetPosition(aabb.GetVertex8());

		Vector3 width = Vector3(aabb.GetWidth() / 2.0f, 0.0f, 0.0f);
		Vector3 height = Vector3(0.0f, aabb.GetHeight() / 2.0f, 0.0f);
		Vector3 length = Vector3(0.0f, 0.0f, aabb.GetLength() / 2.0f);
		if(_object->GetScale().x > 0.0f)
		{
			vX1->SetPosition(aabb.GetVertex1() + width);
			vX2->SetPosition(aabb.GetVertex4() + width);
			vX3->SetPosition(aabb.GetVertex1() + width - (length * 2));
			vX4->SetPosition(aabb.GetVertex4() + width - (length * 2));
		}
		else
		{
			vX1->SetPosition(aabb.GetVertex1() - width);
			vX2->SetPosition(aabb.GetVertex4() - width);
			vX3->SetPosition(aabb.GetVertex1() - width - (length * 2));
			vX4->SetPosition(aabb.GetVertex4() - width - (length * 2));
		}

		if(_object->GetScale().y > 0.0f)
		{
			vY1->SetPosition(aabb.GetVertex1() - height);
			vY2->SetPosition(aabb.GetVertex2() - height);
			vY3->SetPosition(aabb.GetVertex1() - height - (length * 2));
			vY4->SetPosition(aabb.GetVertex2() - height - (length * 2));
		}
		else
		{
			vY1->SetPosition(aabb.GetVertex1() + height);
			vY2->SetPosition(aabb.GetVertex2() + height);
			vY3->SetPosition(aabb.GetVertex1() + height - (length * 2));
			vY4->SetPosition(aabb.GetVertex2() + height - (length * 2));
		}

		if(_object->GetScale().z > 0.0f)
		{
			vZ1->SetPosition(aabb.GetVertex1() - length);
			vZ2->SetPosition(aabb.GetVertex4() - length);
			vZ3->SetPosition(aabb.GetVertex2() - length);
			vZ4->SetPosition(aabb.GetVertex3() - length);
		}
		else
		{
			vZ1->SetPosition(aabb.GetVertex1() + length);
			vZ2->SetPosition(aabb.GetVertex4() + length);
			vZ3->SetPosition(aabb.GetVertex2() + length);
			vZ4->SetPosition(aabb.GetVertex3() + length);
		}
		float divisor = 3.0f;
		float xScale = fabsf(_object->GetScale().x);
		float yScale = fabsf(_object->GetScale().y);
		float zScale = fabsf(_object->GetScale().z);
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
		maxScale = Vector3(size * max, size * max, size * max);

		vFace1->SetScale(maxScale);
		vFace2->SetScale(maxScale); 
		vFace3->SetScale(maxScale); 
		vFace4->SetScale(maxScale);
		vBack1->SetScale(maxScale); 
		vBack2->SetScale(maxScale); 
		vBack3->SetScale(maxScale); 
		vBack4->SetScale(maxScale);
		vY1->SetScale(maxScale); 
		vY2->SetScale(maxScale); 
		vY3->SetScale(maxScale); 
		vY4->SetScale(maxScale);
		vX1->SetScale(maxScale); 
		vX2->SetScale(maxScale); 
		vX3->SetScale(maxScale); 
		vX4->SetScale(maxScale);
		vZ1->SetScale(maxScale); 
		vZ2->SetScale(maxScale); 
		vZ3->SetScale(maxScale); 
		vZ4->SetScale(maxScale);

		mesh->CreateBox(aabb);
	}
}



Mouse::Event* ScaleGizmo::MouseEventFilter(Mouse::Event* _event)
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
								
							object->SetBoundingVolumeShowMode(GraphicsSurface::SHOW_WIREFRAME_BOUNDING_VOLUME | GraphicsSurface::SHOW_TRANSFORMED_BOUNDING_VOLUME | GraphicsSurface::SHOW_BOUNDING_BOX);

							object->SetBoundingVolumeColor(selectColor);
							
							UpdateAABB(object);
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
				startScaling = true;
				switch(selectedFace)
				{
					case FRONT_FACE:
					{
						if(selected == Y_POINT1 ||
						   selected == Y_POINT2 ||
						   selected == X_POINT1 ||
						   selected == X_POINT2 ||
						   selected == XY_POINT1 ||
						   selected == XY_POINT2 ||
						   selected == XY_POINT3 ||
						   selected == XY_POINT4)
						{
							plane.Set(targetObject.Get()->GetTransformation() * vY1->GetPosition(), targetObject.Get()->GetTransformation() * vX2->GetPosition(), targetObject.Get()->GetTransformation() * vX1->GetPosition());

							if(Camera3D::_GetPicked())
							{
								if(plane.IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
								{
									startCursorPosition = intersectedPoint;
									previousCursorPosition = intersectedPoint;
								}
							}
						}
						break;
					}

					case BACK_FACE:
					{
						if(selected == Y_POINT1 ||
						   selected == Y_POINT2 ||
						   selected == X_POINT1 ||
						   selected == X_POINT2 ||
						   selected == XY_POINT1 ||
						   selected == XY_POINT2 ||
						   selected == XY_POINT3 ||
						   selected == XY_POINT4)
						{
							plane.Set(targetObject.Get()->GetTransformation() * vY3->GetPosition(), targetObject.Get()->GetTransformation() * vX3->GetPosition(), targetObject.Get()->GetTransformation() * vX4->GetPosition());

							if(Camera3D::_GetPicked())
							{
								if(plane.IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
								{
									startCursorPosition = intersectedPoint;
									previousCursorPosition = intersectedPoint;
								}
							}
						}
						break;
					}

					case TOP_FACE:
					{
						if(selected == Z_POINT1 ||
					       selected == Z_POINT2 ||
						   selected == X_POINT1 ||
						   selected == X_POINT2 ||
						   selected == XZ_POINT1 ||
						   selected == XZ_POINT2 ||
						   selected == XZ_POINT3 ||
						   selected == XZ_POINT4)
						{
							plane.Set(targetObject.Get()->GetTransformation() * vX1->GetPosition(), targetObject.Get()->GetTransformation() * vX3->GetPosition(), targetObject.Get()->GetTransformation() * vZ1->GetPosition());

							if(Camera3D::_GetPicked())
							{
								if(plane.IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
								{
									startCursorPosition = intersectedPoint;
									previousCursorPosition = intersectedPoint;
								}
							}
						}
						break;
					}

					case BOTTOM_FACE:
					{
						if(selected == Z_POINT1 ||
						   selected == Z_POINT2 ||
						   selected == X_POINT1 ||
						   selected == X_POINT2 ||
						   selected == XZ_POINT1 ||
						   selected == XZ_POINT2 ||
						   selected == XZ_POINT3 ||
						   selected == XZ_POINT4)
						{
							plane.Set(targetObject.Get()->GetTransformation() * vX4->GetPosition(), targetObject.Get()->GetTransformation() * vX2->GetPosition(), targetObject.Get()->GetTransformation() * vZ2->GetPosition());

							if(Camera3D::_GetPicked())
							{
								if(plane.IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
								{
									startCursorPosition = intersectedPoint;
									previousCursorPosition = intersectedPoint;
								}
							}
						}
						break;
					}

					case LEFT_FACE:
					{
						if(selected == Z_POINT1 ||
						   selected == Z_POINT2 ||
						   selected == Y_POINT1 ||
						   selected == Y_POINT2 ||
						   selected == YZ_POINT1 ||
						   selected == YZ_POINT2 ||
						   selected == YZ_POINT3 ||
						   selected == YZ_POINT4)
						{
							plane.Set(targetObject.Get()->GetTransformation() * vZ1->GetPosition(), targetObject.Get()->GetTransformation() * vZ2->GetPosition(), targetObject.Get()->GetTransformation() * vY1->GetPosition());

							if(Camera3D::_GetPicked())
							{
								if(plane.IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
								{
									startCursorPosition = intersectedPoint;
									previousCursorPosition = intersectedPoint;
								}
							}
						}
						break;
					}

					case RIGHT_FACE:
					{
						if(selected == Z_POINT1 ||
						   selected == Z_POINT2 ||
						   selected == Y_POINT1 ||
						   selected == Y_POINT2 ||
						   selected == YZ_POINT1 ||
						   selected == YZ_POINT2 ||
						   selected == YZ_POINT3 ||
						   selected == YZ_POINT4)
						{
							plane.Set(targetObject.Get()->GetTransformation() * vZ4->GetPosition(), targetObject.Get()->GetTransformation() * vZ3->GetPosition(), targetObject.Get()->GetTransformation() * vY2->GetPosition());

							if(Camera3D::_GetPicked())
							{
								if(plane.IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
								{
									startCursorPosition = intersectedPoint;
									previousCursorPosition = intersectedPoint;
								}
							}
						}
						break;
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
				startScaling = false;
							
				CheckSelect();
			}
		}
	}
	else if(_event->GetEventType() == Mouse::MOVE)
	{ 
		Update();
	}
	return _event;
}


void ScaleGizmo::SetHome(void)
{
	for(int32 i = 0; i < selectedList.GetSize(); ++i)
	{
		GraphicsSurface* object = selectedList.Get(i);
		object->SetScale(Vector3(1,1,1));
	}
	Reset();
}



GraphicsSurface* ScaleGizmo::GetTargetObject(void)const
{
	return targetObject.Get();
}


void ScaleGizmo::Update(void)
{   
	Vector2 cursorPosition = Mouse::_GetCursorPosition(Renderer::_GetPicked());
	cursorPosition.y = Renderer::_GetPicked()->GetViewportSize().y - cursorPosition.y;

	arrowCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - (arrowCursor->GetFrontTexture()->GetImageSize().y-1), 0.0f));
	unclenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((unclenchedHandCursor->GetFrontTexture()->GetImageSize().y-1)/2), 0.0f));
	clenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((clenchedHandCursor->GetFrontTexture()->GetImageSize().y-1)/2), 0.0f));

	if(gizmo->IsShowEnabled())
	{
		if(Camera3D::_GetPicked())
		{
			Line line = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));
			Quaternion previousRotation = GetRotation();
			SetRotation(targetObject.Get()->GetRotation());
			Vector3 previousScale = GetScale();
			SetScale(previousScale * 1.1);
			int32 intersectedTriangle = IsIntersected(line, &intersectedPoint);
			SetRotation(previousRotation);
			SetScale(previousScale);
			if(!startScaling)
			{
				switch(intersectedTriangle)
				{
					case 0:
					case 1:
					{
						if(!startScaling)
						{
							selectedFace = FRONT_FACE;
						}

						if(selected == NO_SELECT)
						{
							vX1->SetColor(yColor + RGBA(1,0,0,0));
							vX2->SetColor(yColor + RGBA(2,0,0,0));
							vY1->SetColor(xColor + RGBA(1,0,0,0));
							vY2->SetColor(xColor + RGBA(2,0,0,0));

							vFace1->SetColor(cornerPointColor + RGBA(1,0,0,0));
							vFace2->SetColor(cornerPointColor + RGBA(2,0,0,0));
							vFace3->SetColor(cornerPointColor + RGBA(3,0,0,0));
							vFace4->SetColor(cornerPointColor + RGBA(4,0,0,0));
						}
						break;
					}

					case 2:
					case 3:
					{
						if(!startScaling)
						{
							selectedFace = BACK_FACE;
						}

						if(selected == NO_SELECT)
						{
							vX3->SetColor(yColor + RGBA(1,0,0,0));
							vX4->SetColor(yColor + RGBA(2,0,0,0));
							vY3->SetColor(xColor + RGBA(1,0,0,0));
							vY4->SetColor(xColor + RGBA(2,0,0,0));

							vBack1->SetColor(cornerPointColor + RGBA(1,0,0,0));
							vBack2->SetColor(cornerPointColor + RGBA(2,0,0,0));
							vBack3->SetColor(cornerPointColor + RGBA(3,0,0,0));
							vBack4->SetColor(cornerPointColor + RGBA(4,0,0,0));
						}
						break;
					}

					case 4:
					case 5:
					{
						if(!startScaling)
						{
							selectedFace = LEFT_FACE;
						}
									
						if(selected == NO_SELECT)
						{
							vZ1->SetColor(yColor + RGBA(1,0,0,0));
							vZ2->SetColor(yColor + RGBA(2,0,0,0));
							vY1->SetColor(zColor + RGBA(1,0,0,0));
							vY3->SetColor(zColor + RGBA(2,0,0,0));
										
							vFace1->SetColor(cornerPointColor + RGBA(1,0,0,0));
							vFace4->SetColor(cornerPointColor + RGBA(2,0,0,0));
							vBack1->SetColor(cornerPointColor + RGBA(3,0,0,0));
							vBack4->SetColor(cornerPointColor + RGBA(4,0,0,0));
						}
						break;
					}

					case 6:
					case 7:
					{
						if(!startScaling)
						{
							selectedFace = RIGHT_FACE;
						}

						if(selected == NO_SELECT)
						{
							vZ3->SetColor(yColor + RGBA(1,0,0,0));
							vZ4->SetColor(yColor + RGBA(2,0,0,0));
							vY2->SetColor(zColor + RGBA(1,0,0,0));
							vY4->SetColor(zColor + RGBA(2,0,0,0));

							vFace2->SetColor(cornerPointColor + RGBA(1,0,0,0));
							vFace3->SetColor(cornerPointColor + RGBA(2,0,0,0));
							vBack2->SetColor(cornerPointColor + RGBA(3,0,0,0));
							vBack3->SetColor(cornerPointColor + RGBA(4,0,0,0));
						}
						break;
					}

					case 8:
					case 9:
					{
						if(!startScaling)
						{
							selectedFace = TOP_FACE;
						}

						if(selected == NO_SELECT)
						{
							vX1->SetColor(zColor + RGBA(1,0,0,0)); 
							vX3->SetColor(zColor + RGBA(2,0,0,0));
							vZ1->SetColor(xColor + RGBA(1,0,0,0));
							vZ3->SetColor(xColor + RGBA(2,0,0,0));

							vFace1->SetColor(cornerPointColor + RGBA(1,0,0,0));
							vFace2->SetColor(cornerPointColor + RGBA(2,0,0,0));
							vBack1->SetColor(cornerPointColor + RGBA(3,0,0,0));
							vBack2->SetColor(cornerPointColor + RGBA(4,0,0,0));
						}
						break;
					}

					case 10:
					case 11:
					{
						if(!startScaling)
						{
							selectedFace = BOTTOM_FACE;
						}

						if(selected == NO_SELECT)
						{
							vX2->SetColor(zColor + RGBA(1,0,0,0));
							vX4->SetColor(zColor + RGBA(2,0,0,0));
							vZ2->SetColor(xColor + RGBA(1,0,0,0));
							vZ4->SetColor(xColor + RGBA(2,0,0,0));

							vFace3->SetColor(cornerPointColor + RGBA(1,0,0,0));
							vFace4->SetColor(cornerPointColor + RGBA(2,0,0,0));
							vBack3->SetColor(cornerPointColor + RGBA(3,0,0,0));
							vBack4->SetColor(cornerPointColor + RGBA(4,0,0,0));
						}
						break;
					}

					default:
					{
						selectedFace = NO_SELECT;

						vX1->SetColor(RGBA(RGBA::WHITE_COLOR));
						vX2->SetColor(RGBA(RGBA::WHITE_COLOR));
						vX3->SetColor(RGBA(RGBA::WHITE_COLOR));
						vX4->SetColor(RGBA(RGBA::WHITE_COLOR));

						vY1->SetColor(RGBA(RGBA::WHITE_COLOR));
						vY2->SetColor(RGBA(RGBA::WHITE_COLOR));
						vY3->SetColor(RGBA(RGBA::WHITE_COLOR));
						vY4->SetColor(RGBA(RGBA::WHITE_COLOR));

						vZ1->SetColor(RGBA(RGBA::WHITE_COLOR));
						vZ2->SetColor(RGBA(RGBA::WHITE_COLOR));
						vZ3->SetColor(RGBA(RGBA::WHITE_COLOR));
						vZ4->SetColor(RGBA(RGBA::WHITE_COLOR));

						vFace1->SetColor(RGBA(RGBA::WHITE_COLOR));
						vFace2->SetColor(RGBA(RGBA::WHITE_COLOR));
						vFace3->SetColor(RGBA(RGBA::WHITE_COLOR));
						vFace4->SetColor(RGBA(RGBA::WHITE_COLOR));

						vBack1->SetColor(RGBA(RGBA::WHITE_COLOR));
						vBack2->SetColor(RGBA(RGBA::WHITE_COLOR));
						vBack3->SetColor(RGBA(RGBA::WHITE_COLOR));
						vBack4->SetColor(RGBA(RGBA::WHITE_COLOR));
						break;
					}
				}

				if(selectedFace != NO_SELECT)
				{
					CheckSelect(); 
				}
			}
			else if(selected != NO_SELECT)
			{
				{
				}

				if(Camera3D::_GetPicked())
				{ 
					if(plane.IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						Vector3 currentCursorPosition = intersectedPoint;
						for(int32 i = 0; i < selectedList.GetSize(); ++i)
						{
							GraphicsSurface* object = selectedList.Get(i);
							float midScale = 1.0f;

							if(targetObject.IsNotEmpty() && targetObject.Get()->GetMesh())
							{
								float divisor = 3.0f;

								float width = targetObject.Get()->GetMesh()->GetBoundingBox().GetWidth();
								float height = targetObject.Get()->GetMesh()->GetBoundingBox().GetHeight();
								float length = targetObject.Get()->GetMesh()->GetBoundingBox().GetLength();

								midScale = (width + height + length) / divisor;
							}

							float modifier = 1.5f / midScale; 
							Matrix4x4 m = targetObject.Get()->GetTransformation();
							m.Inverse();

							switch(selected)
							{
								case Y_POINT1:
								{
									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									object->SetScale(object->GetScale() + Vector3(0, (y2 - y1) * (modifier * object->GetScale().y), 0));
									ClampScale(object);

									UpdateAABB(object);
									break;
								}

								case Y_POINT2:
								{
									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
											
									object->SetScale(object->GetScale() + Vector3(0, (y1 - y2) * (modifier * object->GetScale().y), 0));
									ClampScale(object);

									UpdateAABB(object);
									break;
								}

								case X_POINT1:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;
											
									object->SetScale(object->GetScale() + Vector3((x1 - x2) * (modifier * object->GetScale().x), 0, 0));
									ClampScale(object);

									UpdateAABB(object);
									break;
								}

								case X_POINT2:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;
											
									object->SetScale(object->GetScale() + Vector3((x2 - x1) * (modifier * object->GetScale().x), 0, 0));
									ClampScale(object);

									UpdateAABB(object);
									break;
								}

								case Z_POINT1:
								{
									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;
											
									object->SetScale(object->GetScale() + Vector3(0, 0, (z2 - z1) * (modifier * object->GetScale().z)));
									ClampScale(object);

									UpdateAABB(object);
									break;
								}

								case Z_POINT2:
								{
									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;
											
									object->SetScale(object->GetScale() + Vector3(0, 0, (z1 - z2) * (modifier * object->GetScale().z)));
									ClampScale(object);

									UpdateAABB(object);
									break;
								}

								case XY_POINT1:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;

									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										float x = (x1 - x2) * (modifier * object->GetScale().x);
										float y = (y2 - y1) * (modifier * object->GetScale().y);
										float scale = 0;

										if(x > 0 || y > 0)
										{
											scale = Numerical<float>::_GetMax(x, y);
										}
										else if(x < 0 || y < 0)
										{
											scale = Numerical<float>::_GetMin(x, y);
										}

										object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
										ClampScale(object);
									}
									else
									{
										object->SetScale(object->GetScale() + Vector3((x1 - x2) * (modifier * object->GetScale().x), (y2 - y1) * (modifier * object->GetScale().y), 0));
										ClampScale(object);
									}

									UpdateAABB(object);
									break;
								}

								case XY_POINT2:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;

									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										float x = (x2 - x1) * (modifier * object->GetScale().x);
										float y = (y2 - y1) * (modifier * object->GetScale().y);
										float scale = 0;
										
										if(x > 0 || y > 0)
										{
											scale = Numerical<float>::_GetMax(x, y);
										}
										else if(x < 0 || y < 0)
										{
											scale = Numerical<float>::_GetMin(x, y);
										}

										object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
										ClampScale(object);
									}
									else
									{
										object->SetScale(object->GetScale() + Vector3((x2 - x1) * (modifier * object->GetScale().x), (y2 - y1) * (modifier * object->GetScale().y), 0));
										ClampScale(object);
									}

									UpdateAABB(object);
									break;
								}

								case XY_POINT3:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;

									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										float x = (x2 - x1) * (modifier * object->GetScale().x);
										float y = (y1 - y2) * (modifier * object->GetScale().y);
										float scale = 0;
										
										if(x > 0 || y > 0)
										{
											scale = Numerical<float>::_GetMax(x, y);
										}
										else if(x < 0 || y < 0)
										{
											scale = Numerical<float>::_GetMin(x, y);
										}

										object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
										ClampScale(object);
									}
									else
									{
										object->SetScale(object->GetScale() + Vector3((x2 - x1) * (modifier * object->GetScale().x), (y1 - y2) * (modifier * object->GetScale().y), 0));
										ClampScale(object);
									}

									UpdateAABB(object);
									break;
								}

								case XY_POINT4:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;

									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										float x = (x1 - x2) * (modifier * object->GetScale().x);
										float y = (y1 - y2) * (modifier * object->GetScale().y);
										float scale = 0;
										
										if(x > 0 || y > 0)
										{
											scale = Numerical<float>::_GetMax(x, y);
										}
										else if(x < 0 || y < 0)
										{
											scale = Numerical<float>::_GetMin(x, y);
										}

										object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
										ClampScale(object);
									}
									else
									{
										object->SetScale(object->GetScale() + Vector3((x1 - x2) * (modifier * object->GetScale().x), (y1 - y2) * (modifier * object->GetScale().y), 0));
										ClampScale(object);
									}

									UpdateAABB(object);
									break;
								}
										
								case XZ_POINT1:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;

									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;
									
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										if(selectedFace == TOP_FACE)
										{
											float x = (x1 - x2) * (modifier * object->GetScale().x);
											float z = (z2 - z1) * (modifier * object->GetScale().z);
											float scale = 0;

											if(z > 0 || x > 0)
											{
												scale = Numerical<float>::_GetMax(z, x);
											}
											else if(z < 0 || x < 0)
											{
												scale = Numerical<float>::_GetMin(z, x);
											}

											object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
											ClampScale(object);
										}
										else if(selectedFace == BOTTOM_FACE)
										{
											float x = (x2 - x1) * (modifier * object->GetScale().x);
											float z = (z2 - z1) * (modifier * object->GetScale().z);
											float scale = 0;

											if(z > 0 || x > 0)
											{
												scale = Numerical<float>::_GetMax(z, x);
											}
											else if(z < 0 || x < 0)
											{
												scale = Numerical<float>::_GetMin(z, x);
											}

											object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
											ClampScale(object);
										}
									}
									else
									{
										if(selectedFace == TOP_FACE)
										{
											object->SetScale(object->GetScale() + Vector3((x1 - x2) * (modifier * object->GetScale().x), 0, (z2 - z1) * (modifier * object->GetScale().z)));
											ClampScale(object);
										}
										else if(selectedFace == BOTTOM_FACE)
										{
											object->SetScale(object->GetScale() + Vector3((x2 - x1) * (modifier * object->GetScale().x), 0, (z2 - z1) * (modifier * object->GetScale().z)));
											ClampScale(object);
										}
									}

									UpdateAABB(object);
									break;
								}

								case XZ_POINT2:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;

									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										if(selectedFace == TOP_FACE)
										{
											float x = (x2 - x1) * (modifier * object->GetScale().x);
											float z = (z2 - z1) * (modifier * object->GetScale().z);
											float scale = 0;

											if(z > 0 || x > 0)
											{
												scale = Numerical<float>::_GetMax(z, x);
											}
											else if(z < 0 || x < 0)
											{
												scale = Numerical<float>::_GetMin(z, x);
											}

											object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
											ClampScale(object);
										}
										else if(selectedFace == BOTTOM_FACE)
										{
											float x = (x1 - x2) * (modifier * object->GetScale().x);
											float z = (z2 - z1) * (modifier * object->GetScale().z);
											float scale = 0;

											if(z > 0 || x > 0)
											{
												scale = Numerical<float>::_GetMax(z, x);
											}
											else if(z < 0 || x < 0)
											{
												scale = Numerical<float>::_GetMin(z, x);
											}

											object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
											ClampScale(object);
										}
									}
									else
									{
										if(selectedFace == TOP_FACE)
										{
											object->SetScale(object->GetScale() + Vector3((x2 - x1) * (modifier * object->GetScale().x), 0, (z2 - z1) * (modifier * object->GetScale().z)));
											ClampScale(object);
										}
										else if(selectedFace == BOTTOM_FACE)
										{
											object->SetScale(object->GetScale() + Vector3((x1 - x2) * (modifier * object->GetScale().x), 0, (z2 - z1) * (modifier * object->GetScale().z)));
											ClampScale(object);
										}
									}

									UpdateAABB(object);
									break;
								}

								case XZ_POINT3:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;

									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										if(selectedFace == TOP_FACE)
										{
											float x = (x1 - x2) * (modifier * object->GetScale().x);
											float z = (z1 - z2) * (modifier * object->GetScale().z);
											float scale = 0;

											if(z > 0 || x > 0)
											{
												scale = Numerical<float>::_GetMax(z, x);
											}
											else if(z < 0 || x < 0)
											{
												scale = Numerical<float>::_GetMin(z, x);
											}

											object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
											ClampScale(object);
										}
										else if(selectedFace == BOTTOM_FACE)
										{
											float x = (x2 - x1) * (modifier * object->GetScale().x);
											float z = (z1 - z2) * (modifier * object->GetScale().z);
											float scale = 0;

											if(z > 0 || x > 0)
											{
												scale = Numerical<float>::_GetMax(z, x);
											}
											else if(z < 0 || x < 0)
											{
												scale = Numerical<float>::_GetMin(z, x);
											}

											object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
											ClampScale(object);
										}
									}
									else
									{
										if(selectedFace == TOP_FACE)
										{
											object->SetScale(object->GetScale() + Vector3((x1 - x2) * (modifier * object->GetScale().x), 0, (z1 - z2) * (modifier * object->GetScale().z)));
											ClampScale(object);
										}
										else if(selectedFace == BOTTOM_FACE)
										{
											object->SetScale(object->GetScale() + Vector3((x2 - x1) * (modifier * object->GetScale().x), 0, (z1 - z2) * (modifier * object->GetScale().z)));
											ClampScale(object);
										}
									}

									UpdateAABB(object);
									break;
								}

								case XZ_POINT4:
								{
									float x1 = (m * previousCursorPosition).x;
									float x2 = (m * currentCursorPosition).x;

									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										if(selectedFace == TOP_FACE)
										{
											float x = (x2 - x1) * (modifier * object->GetScale().x);
											float z = (z1 - z2) * (modifier * object->GetScale().z);
											float scale = 0;

											if(z > 0 || x > 0)
											{
												scale = Numerical<float>::_GetMax(z, x);
											}
											else if(z < 0 || x < 0)
											{
												scale = Numerical<float>::_GetMin(z, x);
											}

											object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
											ClampScale(object);
										}
										else if(selectedFace == BOTTOM_FACE)
										{
											float x = (x1 - x2) * (modifier * object->GetScale().x);
											float z = (z1 - z2) * (modifier * object->GetScale().z);
											float scale = 0;

											if(z > 0 || x > 0)
											{
												scale = Numerical<float>::_GetMax(z, x);
											}
											else if(z < 0 || x < 0)
											{
												scale = Numerical<float>::_GetMin(z, x);
											}

											object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
											ClampScale(object);
										}
									}
									else
									{
										if(selectedFace == TOP_FACE)
										{
											object->SetScale(object->GetScale() + Vector3((x2 - x1) * (modifier * object->GetScale().x), 0, (z1 - z2) * (modifier * object->GetScale().z)));
											ClampScale(object);
										}
										else if(selectedFace == BOTTOM_FACE)
										{
											object->SetScale(object->GetScale() + Vector3((x1 - x2) * (modifier * object->GetScale().x), 0, (z1 - z2) * (modifier * object->GetScale().z)));
											ClampScale(object);
										}
									}

									UpdateAABB(object);
									break;
								}

								case YZ_POINT1:
								{
									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;

									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										
										float y = (y2 - y1) * (modifier * object->GetScale().y);
										float z = (z2 - z1) * (modifier * object->GetScale().z);
										float scale = 0;

										if(z > 0 || y > 0)
										{
											scale = Numerical<float>::_GetMax(z, y);
										}
										else if(z < 0 || y < 0)
										{
											scale = Numerical<float>::_GetMin(z, y);
										}

										object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
										ClampScale(object);
									}
									else
									{
										object->SetScale(object->GetScale() + Vector3(0, (y2 - y1) * (modifier * object->GetScale().y), (z2 - z1) * (modifier * object->GetScale().z)));
										ClampScale(object);
									}

									UpdateAABB(object);
									break;
								}

								case YZ_POINT2:
								{
									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;

									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										
										float y = (y1 - y2) * (modifier * object->GetScale().y);
										float z = (z2 - z1) * (modifier * object->GetScale().z);
										float scale = 0;

										if(z > 0 || y > 0)
										{
											scale = Numerical<float>::_GetMax(z, y);
										}
										else if(z < 0 || y < 0)
										{
											scale = Numerical<float>::_GetMin(z, y);
										}

										object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
										ClampScale(object);
									}
									else
									{
										object->SetScale(object->GetScale() + Vector3(0, (y1 - y2) * (modifier * object->GetScale().y), (z2 - z1) * (modifier * object->GetScale().z)));
										ClampScale(object);
									}

									UpdateAABB(object);
									break;
								}

								case YZ_POINT3:
								{
									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;

									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										
										float y = (y2 - y1) * (modifier * object->GetScale().y);
										float z = (z1 - z2) * (modifier * object->GetScale().z);
										float scale = 0;

										if(z > 0 || y > 0)
										{
											scale = Numerical<float>::_GetMax(z, y);
										}
										else if(z < 0 || y < 0)
										{
											scale = Numerical<float>::_GetMin(z, y);
										}

										object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
										ClampScale(object);
									}
									else
									{
										object->SetScale(object->GetScale() + Vector3(0, (y2 - y1) * (modifier * object->GetScale().y), (z1 - z2) * (modifier * object->GetScale().z)));
										ClampScale(object);
									}

									UpdateAABB(object);
									break;
								}

								case YZ_POINT4:
								{
									float z1 = (m * previousCursorPosition).z;
									float z2 = (m * currentCursorPosition).z;

									float y1 = (m * previousCursorPosition).y;
									float y2 = (m * currentCursorPosition).y;
										
									if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
									{
										
										float y = (y1 - y2) * (modifier * object->GetScale().y);
										float z = (z1 - z2) * (modifier * object->GetScale().z);
										float scale = 0;

										if(z > 0 || y > 0)
										{
											scale = Numerical<float>::_GetMax(z, y);
										}
										else if(z < 0 || y < 0)
										{
											scale = Numerical<float>::_GetMin(z, y);
										}

										object->SetScale(object->GetScale() + Vector3(scale, scale, scale));
										ClampScale(object);
									}
									else
									{
										object->SetScale(object->GetScale() + Vector3(0, (y1 - y2) * (modifier * object->GetScale().y), (z1 - z2) * (modifier * object->GetScale().z)));
										ClampScale(object);
									}

									UpdateAABB(object);
									break;
								}
							} 
						}
						previousCursorPosition = currentCursorPosition;
					} 
				}
			}  
		}
	}
}




void ScaleGizmo::CheckSelect(void)
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
		
	switch(selectedFace)
	{
		case FRONT_FACE:
		{
			if(yColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = Y_POINT1;
				vX1->SetColor(selectColor); 
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(yColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = Y_POINT2;
				vX2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(xColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = X_POINT1;
				vY1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(xColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = X_POINT2;
				vY2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = XY_POINT1;
				vFace1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = XY_POINT2;
				vFace2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(3,0,0,0) == pixelColor)
			{
				selected = XY_POINT3;
				vFace3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(4,0,0,0) == pixelColor)
			{
				selected = XY_POINT4;
				vFace4->SetColor(selectColor);
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
			break;
		}

		case BACK_FACE:
		{
			if(yColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = Y_POINT1;
				vX3->SetColor(selectColor); 
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(yColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = Y_POINT2;
				vX4->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(xColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = X_POINT1;
				vY3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(xColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = X_POINT2;
				vY4->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = XY_POINT1;
				vBack1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = XY_POINT2;
				vBack2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(3,0,0,0) == pixelColor)
			{
				selected = XY_POINT3;
				vBack3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(4,0,0,0) == pixelColor)
			{
				selected = XY_POINT4;
				vBack4->SetColor(selectColor);
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
			break;
		}

		case TOP_FACE:
		{
			if(zColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = Z_POINT1;
				vX1->SetColor(selectColor); 
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(zColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = Z_POINT2;
				vX3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(xColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = X_POINT1;
				vZ1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(xColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = X_POINT2;
				vZ3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = XZ_POINT1;
				vFace1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = XZ_POINT2;
				vFace2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(3,0,0,0) == pixelColor)
			{
				selected = XZ_POINT3;
				vBack1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(4,0,0,0) == pixelColor)
			{
				selected = XZ_POINT4;
				vBack2->SetColor(selectColor);
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
			break;
		}

		case BOTTOM_FACE:
		{
			if(zColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = Z_POINT1;
				vX2->SetColor(selectColor);  
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(zColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = Z_POINT2;
				vX4->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(xColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = X_POINT1;
				vZ2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(xColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = X_POINT2;
				vZ4->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = XZ_POINT1;
				vFace3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = XZ_POINT2;
				vFace4->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(3,0,0,0) == pixelColor)
			{
				selected = XZ_POINT3;
				vBack3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(4,0,0,0) == pixelColor)
			{
				selected = XZ_POINT4;
				vBack4->SetColor(selectColor);
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
			break;
		}

		case LEFT_FACE: 
		{
			if(zColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = Z_POINT1;
				vY1->SetColor(selectColor); 
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(zColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = Z_POINT2;
				vY3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(yColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = Y_POINT1;
				vZ1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(yColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = Y_POINT2;
				vZ2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = YZ_POINT1;
				vFace1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = YZ_POINT2;
				vFace4->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(3,0,0,0) == pixelColor)
			{
				selected = YZ_POINT3;
				vBack1->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(4,0,0,0) == pixelColor)
			{
				selected = YZ_POINT4;
				vBack4->SetColor(selectColor);
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
			break;
		}
			
		case RIGHT_FACE:  
		{
			if(zColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = Z_POINT1;
				vY2->SetColor(selectColor); 
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(zColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = Z_POINT2;
				vY4->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(yColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = Y_POINT1;
				vZ3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(yColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = Y_POINT2;
				vZ4->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(1,0,0,0) == pixelColor)
			{
				selected = YZ_POINT1;
				vFace2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(2,0,0,0) == pixelColor)
			{
				selected = YZ_POINT2;
				vFace3->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(3,0,0,0) == pixelColor)
			{
				selected = YZ_POINT3;
				vBack2->SetColor(selectColor);
				unclenchedHandCursor->SetShowEnable(true);
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
			}
			else if(cornerPointColor + RGBA(4,0,0,0) == pixelColor)
			{
				selected = YZ_POINT4;
				vBack3->SetColor(selectColor);
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
			break;
		}
	}
}