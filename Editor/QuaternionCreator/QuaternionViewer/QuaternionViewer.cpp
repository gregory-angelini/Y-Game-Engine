
#include "QuaternionViewer.h"
#include <Editor/QuaternionCreator/QuaternionCreator.h>
#include <Editor/Editor/Editor.h>



QuaternionViewer::QuaternionViewer(QuaternionCreator* _quaternionCreator): quaternionCreator(_quaternionCreator), camera(NIL), camera2d(NIL), font(NIL), scene(NIL), rightPressed(false), sphere(NIL), gizmo(NIL), startSphereRotation(false), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), arrowCursor(NIL), zoom100(0.0f)
{}


			
QuaternionViewer::~QuaternionViewer(void)
{ 
	arrowCursor->SetShowEnable(true);

	delete camera;
	delete camera2d;

	delete font;
	delete gizmo;
	delete sphere;

	delete clenchedHandCursor;
	delete unclenchedHandCursor;
	delete arrowCursor;
		
	delete scene;
	delete scene2d;
} 



StringANSI QuaternionViewer::GetRelativeRotationString(void)
{
	return "relative rotation: [" + relativeRotationString + "]";
}


bool QuaternionViewer::Create(Engine::Define _define)
{
	if(Engine::Create(_define))
	{
		{
			scene = new Scene();
		}
		{
			Font2D::_CheckSupportedFonts(this);

			Font2D::Define def;
			def.fontName = "Arial";
			def.fontSize = 16;
			def.color = RGBA(RGBA::GRAY_COLOR).GetRGBAf();

			font = new Font2D();
			font->Create(def);
		}
		{
			Camera3D::Define def;
			def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;
			def.rotationEnable = false;
			def.zoomNear = 2.3f;
			def.zoomFar = 7.3f;
			def.distanceToViewingPoint = 4.3f;
			def.zoomEnable = true;
			def.projectionMode = Camera3D::PERSPECTIVE_PROJECTION;

			def.pitchClampEnable = false;
				
			camera = new Camera3D();
			camera->Create(def);
			scene->BindCamera(camera);

			zoom100 = camera->GetDistanceToViewingPoint();
		}
		{
			sphere = new QuaternionSphere();
			sphere->SetColor(RGBA(230,230,230,255));
			
			Mesh* mesh = new Mesh();
			mesh->CreateIcosphere(4);
			sphere->BindMesh(mesh);
			
			scene->AddToRenderList(sphere);
			scene->AddToUpdateList(sphere);
		}
		{
			gizmo = new QuaternionGizmo();
			gizmo->SetRotation(Quaternion());
			gizmo->SetShowEnable(false);
			scene->AddToRenderList(gizmo);
		}
		{
			Camera3D::Define def;
			def.projectionMode = Camera3D::SCREEN_PROJECTION;
			def.distanceToViewingPoint = 1.0f;
				
			camera2d = new Camera3D();
			camera2d->Create(def);
		}
		{
			scene2d = new Scene();
			scene2d->BindCamera(camera2d);
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/unclenched.png").toAscii().data());

			unclenchedHandCursor = new TexturingSurface();
			
			Mesh* mesh = new Mesh();
			mesh->CreateQuad(Quad(Vector3(0,0,-0.102), Vector2(24, 29)));
			unclenchedHandCursor->BindMesh(mesh);
			
			unclenchedHandCursor->BindFrontTexture(texture);
			unclenchedHandCursor->SetShowEnable(false);
			scene2d->AddToRenderList(unclenchedHandCursor);
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/clenched.png").toAscii().data());

			clenchedHandCursor = new TexturingSurface();
			
			Mesh* mesh = new Mesh();
			mesh->CreateQuad(Quad(Vector3(0,0,-0.102), Vector2(22, 24)));
			clenchedHandCursor->BindMesh(mesh);

			clenchedHandCursor->BindFrontTexture(texture);

			clenchedHandCursor->SetShowEnable(false);
			scene2d->AddToRenderList(clenchedHandCursor);
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/cursor.png").toAscii().data());

			arrowCursor = new TexturingSurface();
			
			Mesh* mesh = new Mesh();
			mesh->CreateQuad(Quad(Vector3(14/2, 24/2, -0.102), Vector2(14, 24)));
			arrowCursor->BindMesh(mesh);

			arrowCursor->BindFrontTexture(texture);

			arrowCursor->SetShowEnable(true);

			scene2d->AddToRenderList(arrowCursor);
		}
		Mouse::_SetShowCursor(true);

	
		Renderer::_SetLineWidth(6.0f);
		Renderer::_SetPointSize(8.0f);
		SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));
		return true;
	}
	return false;
}



void QuaternionViewer::Update(float _delta)
{
	scene->Update(_delta);
}



void QuaternionViewer::ShowCursor(void)
{
	if(Mouse::_IsShowCursor())
	{
		Mouse::_SetShowCursor(false);
	}
}


void QuaternionViewer::HideCursor(void)
{
	if(!Mouse::_IsShowCursor())
	{
		Mouse::_SetShowCursor(true);
	}
}


void QuaternionViewer::Draw(float _delta)
{
	ClearScene();

	scene->Draw(); 
	GetPixelColor(Mouse::_GetCursorPosition(this), pixelColorUnderCursor);
	scene2d->Draw();
}



void QuaternionViewer::ShowGlobalSpins(int _state)
{
	gizmo->ShowGlobalSpins(_state);
}


void QuaternionViewer::ShowGlobalAxises(int _state)
{
	gizmo->ShowGlobalAxises(_state);
}



QuaternionSphere* QuaternionViewer::GetSphere(void)const
{
	return sphere;
}


void QuaternionViewer::SetZoom100(void)
{
	camera->SetDistanceToViewingPoint(zoom100);
}


Mouse::Event* QuaternionViewer::MouseEventFilter(Mouse::Event* _event)
{
	camera->MouseEventFilter(_event);

	if(_event->GetEventType() == Mouse::KEY_DOWN)
	{
		if(_event->GetKey() == Mouse::LEFT_BUTTON)
		{
			if(gizmo->GetRotationMode() == QuaternionGizmo::EULER_SCROLLBAR)
			{
				if(!rightPressed)
				{
					if(!startSphereRotation && sphere->GetSelected() != QuaternionSphere::NO_SELECTED)
					{
						clenchedHandCursor->SetShowEnable(true);
						arrowCursor->SetShowEnable(false);
						unclenchedHandCursor->SetShowEnable(false);

						Vector3 intersectedPoint;

						if(sphere->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
						{
							startSphereRotation = true;
							startCursorPosition = intersectedPoint;
							previousCursorPosition = intersectedPoint;
						}
					}
				}
			}
			else
			{
				if(!rightPressed)
				{
					Vector3 intersectedPoint;

					if(sphere->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						startCursorPosition = intersectedPoint;
						previousCursorPosition = intersectedPoint;

						startSpherePosition = sphere->GetRotation() * Vector3::UNIT_Z;
					}
				}
			}
		}
	}
	else if(_event->GetEventType() == Mouse::KEY_UP)
	{
		if(_event->GetKey() == Mouse::LEFT_BUTTON)
		{
			if(gizmo->GetRotationMode() == QuaternionGizmo::EULER_SCROLLBAR)
			{
				if(startSphereRotation) 
				{ 
					startSphereRotation = false;

					clenchedHandCursor->SetShowEnable(false);
					arrowCursor->SetShowEnable(false);
					unclenchedHandCursor->SetShowEnable(true);
				}
			}
		}
		else if(_event->GetKey() == Mouse::RIGHT_BUTTON)
		{
			if(rightPressed)
			{
				camera->SetRotationEnable(false);
				rightPressed = false;

				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(true);
				unclenchedHandCursor->SetShowEnable(false);
				
				Vector2 position(quaternionCreator->GetViewport()->x(), quaternionCreator->GetViewport()->y());
				position += Vector2(quaternionCreator->x(), quaternionCreator->y());
				
				Vector2 size(quaternionCreator->GetViewport()->width(), quaternionCreator->GetViewport()->height());
				size += Vector2(quaternionCreator->GetViewport()->x(), quaternionCreator->GetViewport()->y());
				size += Vector2(0, 16);

				Mouse::_SetCursorPosition(position + size / 2.0f, 0);

				camera->SetZoomEnable(true);
			}
		}
	}	
	else if(_event->GetEventType() == Mouse::MOVE)
	{
		{
			if(GetMouse().IsKeyPressed(Mouse::LEFT_BUTTON))
			{
				if(gizmo->GetRotationMode() == QuaternionGizmo::SPHERICAL_TWO_POLE)
				{
					Vector3 intersectedPoint;

					if(sphere->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						Vector3 v1(Vector3::UNIT_Z);
							
						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), intersectedPoint);
						
						if(Vector3::_GetDot(v1, v2) < 0.0f)
						{
							v1 = -Vector3::UNIT_Z;
							
							sphere->SetRotation(Quaternion(v1, v2));
							sphere->SetRotation(sphere->GetRotation() * Quaternion(Euler(0.0f, Angle::PI, 0.0f)));
						}
						else
						{
							sphere->SetRotation(Quaternion(v1, v2));
						}
						
						quaternionCreator->SetQuaternion(sphere->GetRotation(), false);
						UpdateRelativeRotation(intersectedPoint);
					}
				}
				else if(gizmo->GetRotationMode() == QuaternionGizmo::SPHERICAL_SWING_TWIST)
				{ 
					Vector3 intersectedPoint;

					if(sphere->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						Vector3 v1(Vector3::UNIT_Z);
							
						Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), intersectedPoint);	
							
						sphere->SetRotation(Quaternion(v1, v2));
						
						quaternionCreator->SetQuaternion(sphere->GetRotation(), false);
						UpdateRelativeRotation(intersectedPoint);
					}
				}
				else if(gizmo->GetRotationMode() == QuaternionGizmo::SPHERICAL_FROM_TO)
				{					
					Vector3 intersectedPoint;

					if(sphere->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						sphere->SetRotation(Quaternion(Matrix3x3<Vector3>::_LookAt(sphere->GetPosition(), intersectedPoint, Vector3::UNIT_Y)));
					
						quaternionCreator->SetQuaternion(sphere->GetRotation(), false);
						UpdateRelativeRotation(intersectedPoint);
					}
				}
			}
			else if(GetMouse().IsKeyPressed(Mouse::RIGHT_BUTTON))
			{
				if(!rightPressed)
				{
					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());
					camera->SetRotationEnable(true);
					rightPressed = true;
					clenchedHandCursor->SetShowEnable(false);
					arrowCursor->SetShowEnable(false);
					unclenchedHandCursor->SetShowEnable(false);

					sphere->Unselect();
				}
			}
		}
		Vector2 cursorPosition = Mouse::_GetCursorPosition(Renderer::_GetPicked());
		cursorPosition.y = Renderer::_GetPicked()->GetViewportSize().y - cursorPosition.y;

		arrowCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - (arrowCursor->GetFrontTexture()->GetImageSize().y-1), 0.0f));
		unclenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((unclenchedHandCursor->GetFrontTexture()->GetImageSize().y-1)/2), 0.0f));
		clenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((clenchedHandCursor->GetFrontTexture()->GetImageSize().y-1)/2), 0.0f));


		if(gizmo->GetRotationMode() == QuaternionGizmo::EULER_SCROLLBAR)
		{
			if(!startSphereRotation && !rightPressed)
			{
				if(sphere->GetXColor() == pixelColorUnderCursor)
				{
					sphere->SelectX();
					gizmo->SetShowEnable(true);

					clenchedHandCursor->SetShowEnable(false);
					arrowCursor->SetShowEnable(false);
					unclenchedHandCursor->SetShowEnable(true);
				}
				else if(sphere->GetYColor() == pixelColorUnderCursor)
				{
					sphere->SelectY();
					gizmo->SetShowEnable(true);

					clenchedHandCursor->SetShowEnable(false);
					arrowCursor->SetShowEnable(false);
					unclenchedHandCursor->SetShowEnable(true);
				}
				else if(sphere->GetZColor() == pixelColorUnderCursor)
				{
					sphere->SelectZ();
					gizmo->SetShowEnable(true);

					clenchedHandCursor->SetShowEnable(false);
					arrowCursor->SetShowEnable(false);
					unclenchedHandCursor->SetShowEnable(true);
				}
				else
				{
					if(pixelColorUnderCursor == RGBA(230,230,230,255) || pixelColorUnderCursor == RGBA(RGBA::WHITE_COLOR))
					{
						clenchedHandCursor->SetShowEnable(false);
						arrowCursor->SetShowEnable(true);
						unclenchedHandCursor->SetShowEnable(false);

						sphere->Unselect();
						gizmo->SetShowEnable(false);
					}
				}
			}
			else if(startSphereRotation && !rightPressed)
			{
				Vector3 intersectedPoint;

				if(sphere->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
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

					switch(sphere->GetSelected())
					{
						case QuaternionSphere::CIRCLE_X:
						{
							float x1 = v1.GetAngleX();
							float x2 = v2.GetAngleX();
								
							sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationX(x2 - x1)));
							previousCursorPosition = currentCursorPosition;
								
							float x3 = v3.GetAngleX();
							relativeRotationString = NumberToString((int32)Angle::_RadiansToDegrees(x2 - x3)) + ", 0, 0";
							break;
						}

						case QuaternionSphere::CIRCLE_Y:
						{
							float y1 = v1.GetAngleY();
							float y2 = v2.GetAngleY();

							sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationY(y2 - y1)));
							previousCursorPosition = currentCursorPosition;
								
							float y3 = v3.GetAngleY();
							relativeRotationString = "0, " + NumberToString((int32)Angle::_RadiansToDegrees(y2 - y3)) + ", 0";
							break;
						}

						case QuaternionSphere::CIRCLE_Z:
						{
							float z1 = v1.GetAngleZ();
							float z2 = v2.GetAngleZ();

							sphere->SetRotation(sphere->GetRotation() * Quaternion(Matrix4x4::_RotationZ(z2 - z1)));
							previousCursorPosition = currentCursorPosition;
								
							float z3 = v3.GetAngleZ();
							relativeRotationString = "0, 0, " + NumberToString((int32)Angle::_RadiansToDegrees(z2 - z3));
							break;
						}
					}

					quaternionCreator->SetQuaternion(sphere->GetRotation(), false);
					quaternionCreator->SetRelativeRotation(GetRelativeRotationString().c_str());
				}
			}
		}
	}
	else if(_event->GetEventType() == Mouse::WHEEL)
	{ 
		if(!rightPressed)
		{
			clenchedHandCursor->SetShowEnable(false);
			arrowCursor->SetShowEnable(true);
			unclenchedHandCursor->SetShowEnable(false);

			sphere->Unselect();


			float zoom = 100.0f;

			if(camera->GetDistanceToViewingPoint() > zoom100)
			{
				Ratio::_SetKnown<float>(Ratio::WHOLE, zoom100);
				Ratio::_SetKnown<float>(Ratio::PART, (zoom100 - camera->GetDistanceToViewingPoint()) + zoom100);
				zoom = Ratio::_GetUnknown<float>(Ratio::PART_RATIO);	
			}
			else if(camera->GetDistanceToViewingPoint() < zoom100)
			{
				Ratio::_SetKnown<float>(Ratio::WHOLE, zoom100);
				Ratio::_SetKnown<float>(Ratio::PART, zoom100 - (camera->GetDistanceToViewingPoint() - zoom100));
				zoom = Ratio::_GetUnknown<float>(Ratio::PART_RATIO);	
			}

			quaternionCreator->SetZoomValue((int32)zoom); 
		}
	}

	GetMouse().ClearEventList();
	return _event;
}


void QuaternionViewer::UpdateRelativeRotation(Vector3 _currentCursorPosition)
{
	Vector3 v2 = Vector3::_FromTo(sphere->GetPosition(), _currentCursorPosition);
	v2.Normalize();
	v2 = Quaternion::_GetInverse(sphere->GetRotation()) * v2;

					
	Vector3 v3 = Vector3::_FromTo(sphere->GetPosition(), startSpherePosition);
	v3.Normalize();
	v3 = Quaternion::_GetInverse(sphere->GetRotation()) * v3;

	float x2 = v2.GetAngleX();					
	float x3 = v3.GetAngleX();
					
	float y2 = v2.GetAngleY();						
	float y3 = v3.GetAngleY();
				
	relativeRotationString = NumberToString((int32)Angle::_RadiansToDegrees(x2 - x3)) + ", " + NumberToString((int32)Angle::_RadiansToDegrees(y2 - y3)) + ", 0]";

	quaternionCreator->SetRelativeRotation(GetRelativeRotationString().c_str());
}


void QuaternionViewer::SetRightView(void)
{
	camera->SetRotation(Quaternion(Matrix4x4::_RotationY(Angle::HALF_PI)));
	gizmo->SetShowEnable(true);
	camera->SetReverseScrollYEnable(false);
	camera->SetViewingPointYAxis(Vector3::UNIT_Y);
}


void QuaternionViewer::SetTopDownView(void)
{
	camera->SetRotation(Quaternion(Euler(-Angle::HALF_PI, Angle::HALF_PI, 0)));
	gizmo->SetShowEnable(true);
	camera->SetReverseScrollYEnable(true);
	camera->SetViewingPointYAxis(Vector3::UNIT_X);
}


void QuaternionViewer::SetFrontView(void)			
{
	camera->SetRotation(Quaternion(Euler(0.0f, 0.0f, 0.0f)));
	gizmo->SetShowEnable(true);
	camera->SetViewingPointYAxis(Vector3::UNIT_Y);
}


void QuaternionViewer::SetEulerScrollbar(void)
{
	gizmo->SetRotationMode(QuaternionGizmo::EULER_SCROLLBAR);
}


void QuaternionViewer::SetSphericalSwingTwist(void)
{
	gizmo->SetRotationMode(QuaternionGizmo::SPHERICAL_SWING_TWIST);
	gizmo->SetShowEnable(true);

	clenchedHandCursor->SetShowEnable(false);
	arrowCursor->SetShowEnable(true);
	unclenchedHandCursor->SetShowEnable(false);
}


void QuaternionViewer::SetSphericalTwoPole(void)
{
	gizmo->SetRotationMode(QuaternionGizmo::SPHERICAL_TWO_POLE);
	gizmo->SetShowEnable(true);

	clenchedHandCursor->SetShowEnable(false);
	arrowCursor->SetShowEnable(true);
	unclenchedHandCursor->SetShowEnable(false);
}


void QuaternionViewer::SetSphericalFromTo(void)
{
	gizmo->SetRotationMode(QuaternionGizmo::SPHERICAL_FROM_TO);
	gizmo->SetShowEnable(true);

	clenchedHandCursor->SetShowEnable(false);
	arrowCursor->SetShowEnable(true);
	unclenchedHandCursor->SetShowEnable(false);
}


Keyboard::Event* QuaternionViewer::KeyboardEventFilter(Keyboard::Event* _event)
{
	GetKeyboard().ClearEventList();

	return _event;
}