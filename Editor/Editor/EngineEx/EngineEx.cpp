
#include "EngineEx.h"
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Output/Graphics/Scene/TiledSpace/TiledSpace3D/TiledSpace3D.h>



EngineEx::EngineEx(Editor* _editor): editor(_editor), mouseRightButtonPressed(false), showCursorEnable(true), cursorShowEnable(true), translateGizmoScene3d(NIL), rotateGizmoScene3d(NIL), scaleGizmoScene3d(NIL), translateGizmoScene2d(NIL), rotateGizmoScene2d(NIL), scaleGizmoScene2d(NIL), scene2d(NIL), camera2d(NIL), font(NIL), rotateGizmo(NIL), translateGizmo(NIL), scaleGizmo(NIL), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), arrowCursor(NIL), timeSpeed(1.0f), showCurrentTime(false), currentTimeInSec(0.0f)
{
	cameraMinScale = 0.1f;
	camaraMaxScale = 70.0f;
	ñamaraZoomSpeed = 10.0f;
	camera2dMoveSpeed = 5.0f;
	camera3dMoveSpeed = 0.04f;

	cameraMovementEnable = true;

	cameraSightColor = RGBA(RGBA::YELLOW_COLOR);
	cameraSightSize = 20.0f;
	cameraShowSightEnable = false;
}
  


void EngineEx::SetViewMovementEnable(bool _enable)
{ 
	cameraMovementEnable = _enable;
}


bool EngineEx::GetViewMovementEnable(void)const
{
	return cameraMovementEnable;
}



void EngineEx::ResetTime(void)
{
	currentTimeInSec = 0.0f;
}



void EngineEx::TimeShowEnable(bool _enable)
{
	if(!_enable)
	{
		currentTimeInSec = 0.0f;
	}

	showCurrentTime = _enable;
}


void EngineEx::SetCamera3dMoveSpeed(float _moveSpeed)
{
	if(_moveSpeed >= 0.0f)
	{
		camera3dMoveSpeed = _moveSpeed;
	}
}


void EngineEx::SetCameraMoveUp(Keyboard::Key _key)
{
	if(_key != Keyboard::NO_KEY)
	{
		cameraMoveUpKey = _key;
	}
}


 void EngineEx::SetCameraMoveDown(Keyboard::Key _key)
 {
	if(_key != Keyboard::NO_KEY)
	{
		cameraMoveDownKey = _key;
	}
 }


void EngineEx::SetCameraMoveLeft(Keyboard::Key _key)
{
	if(_key != Keyboard::NO_KEY)
	{
		cameraMoveLeftKey = _key;
	}
}

				
void EngineEx::SetCameraMoveRight(Keyboard::Key _key)
{
	if(_key != Keyboard::NO_KEY)
	{
		cameraMoveRightKey = _key;
	}
}


void EngineEx::SetCameraMoveForward(Keyboard::Key _key)
{
	if(_key != Keyboard::NO_KEY)
	{
		cameraMoveForwardKey = _key;
	}
}

			
void EngineEx::SetCameraMoveBackward(Keyboard::Key _key)
{
	if(_key != Keyboard::NO_KEY)
	{
		cameraMoveBackwardKey = _key;
	}
}



float EngineEx::GetCamera3dMoveSpeed(void)const
{
	return camera3dMoveSpeed;
}


bool EngineEx::Create(Engine::Define _define)
{
	if(Engine::Create(_define))
	{
		{
			Font2D::_CheckSupportedFonts(this);

			Font2D::Define def;
			def.fontName = "Arial";
			def.fontSize = 16;
			def.fontWeight = Font2D::WEIGHT_BOLD;
			def.color = RGBA(RGBA::WHITE_COLOR).GetRGBAf();

			font = new Font2D();
			font->Create(def);
		}


		CursorInit();

		TranslateGizmoInit();

		RotateGizmoInit();
	
		ScaleGizmoInit();
		SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));
		return true;
	}
	return false;
}


void EngineEx::CursorInit(void)
{
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
		texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/unclenched.png").toStdString());

		unclenchedHandCursor = new TexturingSurface();
		
		Mesh* mesh = new Mesh();
		mesh->CreateQuad(Quad(Vector3(0, 0, -0.101), Vector2(24, 29)));
		unclenchedHandCursor->BindMesh(mesh);
		
		unclenchedHandCursor->BindFrontTexture(texture);
		
		unclenchedHandCursor->SetShowEnable(false);
		scene2d->AddToRenderList(unclenchedHandCursor);
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/clenched.png").toStdString());


		clenchedHandCursor = new TexturingSurface();
		
		Mesh* mesh = new Mesh();
		mesh->CreateQuad(Quad(Vector3(0, 0, -0.101), Vector2(22, 24)));
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
		mesh->CreateQuad(Quad(Vector3(14/2, 24/2, -0.101), Vector2(14, 24)));
		arrowCursor->BindMesh(mesh);

		arrowCursor->BindFrontTexture(texture);

		arrowCursor->SetShowEnable(true);
		scene2d->AddToRenderList(arrowCursor);
	}
}


void EngineEx::ScaleGizmoInit(void)
{
	scaleGizmo = new ScaleGizmo();
	scaleGizmoScene3d = new Scene();
	scaleGizmoScene3d->SetColor(RGBA(RGBA::INVISIBLE_COLOR));
	
	scaleGizmoScene3d->BindTexture(scaleGizmo->GetGizmoSprite()->GetFrontTexture());
	scaleGizmoScene3d->AddToRenderList(scaleGizmo);
	scaleGizmoScene2d = new Scene();
	scaleGizmoScene2d->AddToRenderList(scaleGizmo->GetGizmoSprite()); 
	scaleGizmoScene2d->BindCamera(camera2d);

	scaleGizmo->Init(arrowCursor, unclenchedHandCursor, clenchedHandCursor);
}



void EngineEx::RotateGizmoInit(void)
{
	rotateGizmo = new RotateGizmo();
	rotateGizmoScene3d = new Scene();
	rotateGizmoScene3d->SetColor(RGBA(RGBA::INVISIBLE_COLOR));
		
	rotateGizmoScene3d->BindTexture(rotateGizmo->GetGizmoSprite()->GetFrontTexture());
	rotateGizmoScene3d->AddToRenderList(rotateGizmo);
	rotateGizmoScene2d = new Scene();
	rotateGizmoScene2d->AddToRenderList(rotateGizmo->GetGizmoSprite()); 
	rotateGizmoScene2d->BindCamera(camera2d);

	rotateGizmo->Init(arrowCursor, unclenchedHandCursor, clenchedHandCursor);
}



void EngineEx::TranslateGizmoInit(void)
{
	translateGizmo = new TranslateGizmo();
	translateGizmoScene3d = new Scene();
	translateGizmoScene3d->SetColor(RGBA(RGBA::INVISIBLE_COLOR));
		
	translateGizmoScene3d->BindTexture(translateGizmo->GetGizmoSprite()->GetFrontTexture());
	translateGizmoScene3d->AddToRenderList(translateGizmo);
	translateGizmoScene2d = new Scene();
	translateGizmoScene2d->AddToRenderList(translateGizmo->GetGizmoSprite()); 
	translateGizmoScene2d->BindCamera(camera2d);

	translateGizmo->Init(arrowCursor, unclenchedHandCursor, clenchedHandCursor);
}


EngineEx::~EngineEx(void)
{
	delete clenchedHandCursor;
	delete unclenchedHandCursor;
	delete arrowCursor;

	delete font;

	delete rotateGizmo;
	delete translateGizmo;
	delete scaleGizmo;
		
	delete translateGizmoScene3d;
	delete translateGizmoScene2d;

	delete rotateGizmoScene3d;
	delete rotateGizmoScene2d;

	delete scaleGizmoScene3d;
	delete scaleGizmoScene2d;
	
	delete camera2d;
	delete scene2d;
}




void EngineEx::ResetDesignTools(void)
{
	scaleGizmo->Reset();
	translateGizmo->Reset();
	rotateGizmo->Reset();
}



void EngineEx::ResetTool(int32 _toolId)
{
	switch(_toolId)
	{
		case DesignToolBar::SCALE:
		{
			scaleGizmo->Reset();
			break;
		}

		case DesignToolBar::TRANSLATE:
		{
			translateGizmo->Reset();
			break;
		}

		case DesignToolBar::ROTATE:
		{
			rotateGizmo->Reset();
			break;
		}
	}

}

void EngineEx::SetRotateHome(void)
{
	rotateGizmo->SetHome();
}


void EngineEx::SetScaleHome(void)
{
	scaleGizmo->SetHome();
}


void EngineEx::SetMoveHome(void)
{
	translateGizmo->SetHome();
}



void EngineEx::SetViewZoomEnable(void)
{
	if(Camera3D::_GetPicked())
	{
		if(Camera3D::_GetPicked()->IsZoomEnabled())
		{
			Camera3D::_GetPicked()->SetZoomEnable(false);
		}
		else
		{
			Camera3D::_GetPicked()->SetZoomEnable(true);
		}
	} 
}



void EngineEx::SetViewRotationEnable(void)
{
	if(Camera3D::_GetPicked())
	{
		if(Camera3D::_GetPicked()->IsRotationEnabled())
		{
			Camera3D::_GetPicked()->SetRotationEnable(false);
		}
		else
		{
			Camera3D::_GetPicked()->SetRotationEnable(true);
		}
	} 
}


void EngineEx::SetViewHome(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetPosition(Vector3(0.0f, 0.0, 1.0f));
		Camera3D::_GetPicked()->SetRotation(Quaternion());
		Camera3D::_GetPicked()->SetDistanceToViewingPoint(1.0f);
		Camera3D::_GetPicked()->SetFov((float)Angle::HALF_PI / 2.0f);
	}
}



void EngineEx::SetViewShowSightEnable(bool _enable)
{
	cameraShowSightEnable = _enable;
}


bool EngineEx::GetViewShowSightEnable(void)const
{
	return cameraShowSightEnable;
}
 

void EngineEx::SetViewPerspective(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetProjectionMode(Camera3D::PERSPECTIVE_PROJECTION);
	}
}


void EngineEx::SetViewOrthographic(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetProjectionMode(Camera3D::ORTHOGRAPHIC_PROJECTION);
	}
}


void EngineEx::SetViewScreen(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetProjectionMode(Camera3D::SCREEN_PROJECTION);
	}
}



void EngineEx::SetViewSpectator(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetMovementMode(Camera3D::SPECTATOR_MOVEMENT);
	}
}


void EngineEx::SetViewOverland(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetMovementMode(Camera3D::OVERLAND_MOVEMENT);
	}
}


void EngineEx::SetViewFlight(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetMovementMode(Camera3D::FLIGHT_MOVEMENT);
	}
}


void EngineEx::SetViewFirstPerson(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetRotationMode(Camera3D::FIRST_PERSON_ROTATION);
	}
}



void EngineEx::SetViewObjectViewer(void)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->SetRotationMode(Camera3D::OBJECT_VIEWER_ROTATION);

		if(selectedList.IsNotEmpty())
		{ 
			if(selectedList.GetHead()->GetMesh())
			{
				float maxSize = Numerical<float>::_GetMax(selectedList.GetHead()->GetMesh()->GetBoundingBox().GetWidth() * selectedList.GetHead()->GetScale().x, selectedList.GetHead()->GetMesh()->GetBoundingBox().GetHeight() * selectedList.GetHead()->GetScale().y);
				maxSize = Numerical<float>::_GetMax(maxSize, selectedList.GetHead()->GetMesh()->GetBoundingBox().GetLength() * selectedList.GetHead()->GetScale().z);

				Camera3D::_GetPicked()->SetZoomFar(maxSize * camaraMaxScale);
				Camera3D::_GetPicked()->SetZoomNear(maxSize * cameraMinScale);
				Camera3D::_GetPicked()->SetZoomSpeed(maxSize / ñamaraZoomSpeed);

				float dist = Numerical<float>::_GetMax(selectedList.GetHead()->GetMesh()->GetBoundingQuad().GetSize().x, selectedList.GetHead()->GetMesh()->GetBoundingQuad().GetSize().y) + Angle::_RadiansToDegrees(Camera3D::_GetPicked()->GetFov() * 2.0f);
				Camera3D::_GetPicked()->SetDistanceToViewingPoint(dist);
				Camera3D::_GetPicked()->SetViewingPoint(selectedList.GetHead()->GetPosition());
			}
		}
	}
}


void EngineEx::SetTimeSpeed(float _scale)
{
	if(_scale != 0.0f)
	{
		timeSpeed = _scale;
	}
}



void EngineEx::Update(float _delta)
{

	Engine::Update(_delta * timeSpeed);
	
	if(Camera3D::_GetPicked())
	{
		editor->UpdateCameraPos(Camera3D::_GetPicked()->GetPosition());
	}

	if(editor->GetControlToolBar()->IsRecordEnabled() && editor->GetAssetManager()->IsEventLogRealTimeShowModeEnabled())
	{
		editor->GetAssetManager()->GetEventTreeWidget()->UpdateEventTree();
	}

	if(showCurrentTime)
	{
		currentTimeInSec += _delta * timeSpeed;
	}
} 
		

void EngineEx::DrawSight(void)
{
	camera2d->BeginTransform(Renderer::PROJECTION);
	camera2d->BeginTransform(Renderer::MODELVIEW);
  

	Vector2 center = GetViewportSize() / 2.0f;
	GraphicsLine::_Draw(Vector3(center.x - cameraSightSize / 2.0f, center.y, -0.1f), Vector3(center.x + cameraSightSize / 2.0f, center.y, -0.1f), cameraSightColor);
	GraphicsLine::_Draw(Vector3(center.x, center.y - cameraSightSize / 2.0f, -0.1f), Vector3(center.x, center.y + cameraSightSize / 2.0f, -0.1f), cameraSightColor);
	camera2d->EndTransform(Renderer::MODELVIEW);
	camera2d->EndTransform(Renderer::PROJECTION);
}


void EngineEx::DrawDesignTool(void)
{
	if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::TRANSLATE)
	{
		if(Camera3D::_GetPicked())
		{
			translateGizmoScene3d->Draw(); 
		} 

		translateGizmoScene2d->Draw();
	}
	else if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::ROTATE)
	{
		if(Camera3D::_GetPicked())
		{
			rotateGizmoScene3d->Draw();
		}

		rotateGizmoScene2d->Draw();
	}
	else if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::SCALE)
	{
		if(Camera3D::_GetPicked())
		{
			scaleGizmoScene3d->Draw();
		}

		scaleGizmoScene2d->Draw();
	}


	if(showCursorEnable)
	{
		if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::TRANSLATE ||
		   editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::ROTATE ||
		   editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::SCALE ||
		   editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::ERASE ||
		   editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::SELECT)
		{
			scene2d->Draw(); 
		}
	}
}


void EngineEx::Draw(float _delta)
{

	Engine::Draw(_delta * timeSpeed);

	DrawDesignTool();
	
	if(cameraShowSightEnable)
	{ 
		DrawSight(); 
	}

	if(showCurrentTime)
	{
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);

		font->DrawScreenText(Vector3(10, 20, -1), "current time: %.3f", currentTimeInSec);
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
	}
}



void EngineEx::UnselectObjects(void)
{
	while(selectedList.IsNotEmpty())
	{
		GraphicsSurface* object = selectedList.GetHead();
		editor->GetAssetManager()->UnselectObject(object);
		UnselectObject(object);
	}
}


MagicList<GraphicsSurface> EngineEx::CheckObjectsUnderCursor(void)
{
	Scene* scene = Scene::_GetPicked();

	MagicList<GraphicsSurface> list;

	if(scene)
	{
		CheckObjectsUnderCursor(scene->GetRenderList().GetHeadAddress(), list);
	}
	return list;
}


void EngineEx::CheckObjectsUnderCursor(MagicList<GraphicsObject>::AddressDerived* _address, MagicList<GraphicsSurface>& _list)const
{ 
	if(_address)
	{ 
		Vector3 intersectedPoint;

		CheckObjectsUnderCursor(_address->GetRight(), _list);

		if(_address->GetNode()->IsClassPropertyExist(GraphicsScene::GRAPHICS_SCENE_CLASS))
		{
			GraphicsScene* scene = dynamic_cast<GraphicsScene*>(_address->GetNode());
			
			if(scene)
			{
				CheckObjectsUnderCursor(scene->GetRenderList().GetHeadAddress(), _list);
			} 
		}
		if(_address->GetNode()->IsClassPropertyExist(TiledSpace3D::TILED_SPACE_3D_CLASS))
		{
			TiledSpace3D* tiledSpace = dynamic_cast<TiledSpace3D*>(_address->GetNode());
			
			if(tiledSpace)
			{
				CheckObjectsUnderCursor(tiledSpace, _list);
			} 
		}
		else if(_address->GetNode()->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS))
		{
			GraphicsSurface* object = dynamic_cast<GraphicsSurface*>(_address->GetNode());

			if(object && Camera3D::_GetPicked())
			{
				if(object->IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
				{
					_list.AddToTail(object);
				}
			}
		}
	}
}


void EngineEx::CheckObjectsUnderCursor(TiledSpace3D* _tiledSpace, MagicList<GraphicsSurface>& _list)const
{ 
	if(_tiledSpace)
	{ 
		Vector3 intersectedPoint;
		for(int32 i = 0; i < _tiledSpace->GetAmountOfTiles(); i++)
		{
			TexturingSurface* object = _tiledSpace->GetTile(i)->GetObject();

			if(object)
			{
				if(Camera3D::_GetPicked())
				{
					if(object->IsIntersected(Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						_list.AddToTail(object);
					}
				}
			}
		}
	}
}


void EngineEx::SelectObject(GraphicsSurface* _object)
{
	if(_object)
	{
		selectedList.AddToTail(_object);

		_object->SetBoundingVolumeShowEnable(true);
		_object->SetBoundingVolumeShowMode(GraphicsSurface::SHOW_SHORTEST_BOUNDING_VOLUME | GraphicsSurface::SHOW_TRANSFORMED_BOUNDING_VOLUME | GraphicsSurface::SHOW_BOUNDING_BOX);
		
		translateGizmo->GetSelectedList().AddToTail(_object);
		rotateGizmo->GetSelectedList().AddToTail(_object);
		scaleGizmo->GetSelectedList().AddToTail(_object);
	}
}



void EngineEx::UnselectObject(GraphicsSurface* _object)
{
	if(_object)
	{
		selectedList.Remove(_object, MagicContainer::REMOVE_NODE);
		_object->SetBoundingVolumeShowEnable(false);
		if(translateGizmo->GetTargetObject() == _object)
		{ 
			translateGizmo->Reset();
		}
		if(rotateGizmo->GetTargetObject() == _object)
		{ 
			rotateGizmo->Reset();
		}
		if(scaleGizmo->GetTargetObject() == _object)
		{
			scaleGizmo->Reset();
		}

		translateGizmo->GetSelectedList().Remove(_object, MagicContainer::REMOVE_NODE);
		rotateGizmo->GetSelectedList().Remove(_object, MagicContainer::REMOVE_NODE);
		scaleGizmo->GetSelectedList().Remove(_object, MagicContainer::REMOVE_NODE);
	}
}


void EngineEx::SelectObject(void)
{
	Scene* scene = Scene::_GetPicked();

	if(scene)
	{  
		MagicList<GraphicsSurface> list = CheckObjectsUnderCursor();

		if(list.IsNotEmpty())
		{
			if(!Keyboard::_IsKeyPressed(Keyboard::SHIFT))
			{ 
				UnselectObjects();
			} 
			GraphicsSurface* object = list.Get(0);
			GraphicsContainer* scene = dynamic_cast<GraphicsContainer*>(object->GetGraphicsContainer());
			GraphicsSurface* selectObject = NIL;

			if(scene) 
			{ 
				Camera3D* camera = scene->GetCamera(); 

				if(camera) 
				{ 
					float minDist = Line::_GetLength(object->GetPosition(), camera->GetPosition());
					
					selectObject = object;
					for(int32 i = 1; i < list.GetSize(); ++i)
					{
						object = list.Get(i);

						scene = dynamic_cast<GraphicsContainer*>(object->GetGraphicsContainer());
						
						if(scene) 
						{ 
							camera = scene->GetCamera(); 

							if(camera) 
							{ 
								float length = Line::_GetLength(object->GetPosition(), camera->GetPosition());

								if(minDist > length)
								{
									minDist = length;
									selectObject = object;
								}
							} 
						}
					}

					if(selectedList.AddToTail(selectObject))
					{
						SelectObject(selectObject);
						editor->GetAssetManager()->SelectObject(selectObject);
					}
					else
					{
						if(Keyboard::_IsKeyPressed(Keyboard::SHIFT))
						{ 
							UnselectObject(selectObject);
							editor->GetAssetManager()->UnselectObject(selectObject);
						}
					}
				}
			}
		}
		else
		{
			if(!translateGizmo->IsSelected())
			{ 
				UnselectObjects();
			}
		}
	}
}


void EngineEx::DestroySelectedObjects(void)
{
	while(selectedList.IsNotEmpty())
	{
		delete selectedList.GetHead();
	}
}



void EngineEx::RotateCameraBegin(void)
{
	if(!mouseRightButtonPressed)
	{
		if(Camera3D::_GetPicked()) 
		{ 
			Mouse::_MoveCursorToCenter(Renderer::_GetPicked());
		
			Camera3D::_GetPicked()->SetRotationEnable(true);
		
			mouseRightButtonPressed = true;
			showCursorEnable = false;
			cursorShowEnable = Mouse::_IsShowCursor();
			Mouse::_SetShowCursor(false);
		}
	}
}

void EngineEx::RotateCameraEnd(void)
{
	if(mouseRightButtonPressed)
	{
		if(Camera3D::_GetPicked()) 
		{ 
			Camera3D::_GetPicked()->SetRotationEnable(false);
		
			mouseRightButtonPressed = false;
			showCursorEnable = true;
			Mouse::_SetShowCursor(cursorShowEnable);
		}
	}
}


Mouse::Event* EngineEx::MouseEventFilter(Mouse::Event* _event)
{
	if(editor->GetControlToolBar()->GetCurrentTool() == ControlToolBar::PAUSE)
	{
		delete _event;
		return NIL;
	}
	else if(editor->IsViewportFocus() && Camera3D::_GetPicked())
	{
		if(_event->GetEventType() == Mouse::KEY_DOWN)
		{
			if(_event->GetKey() == Mouse::LEFT_BUTTON)
			{
				if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::SELECT)
				{ 
					SelectObject();
				}
				else if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::ERASE)
				{ 
					DestroySelectedObjects();	
				} 
			}
			else if(cameraMovementEnable && _event->GetKey() == Mouse::RIGHT_BUTTON)
			{
				RotateCameraBegin();
			}
		}
		else if(_event->GetEventType() == Mouse::KEY_UP)
		{
			if(cameraMovementEnable && _event->GetKey() == Mouse::RIGHT_BUTTON)
			{
				RotateCameraEnd();
			}
		}
		else if(_event->GetEventType() == Mouse::WHEEL)
		{
			if(Camera3D::_GetPicked()->GetRotationMode() == Camera3D::FIRST_PERSON_ROTATION)
			{
				Camera3D::_GetPicked()->Walk(camera3dMoveSpeed * _event->GetWheel());
			}
		}
		else if(_event->GetEventType() == Mouse::MOVE)
		{
			editor->UpdateCursorPos(_event->GetPosition());
		}
		if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::TRANSLATE)
		{
			translateGizmo->MouseEventFilter(_event);
		}
		else if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::ROTATE)
		{
			rotateGizmo->MouseEventFilter(_event);
		}
		else if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::SCALE)
		{
			scaleGizmo->MouseEventFilter(_event); 
		}
		else if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::SELECT)
		{
			if(_event->GetEventType() == Mouse::MOVE)
			{
				Vector2 cursorPosition = Mouse::_GetCursorPosition(Renderer::_GetPicked());
				arrowCursor->SetPosition(Vector3(cursorPosition.x, GetViewportSize().y - cursorPosition.y - (arrowCursor->GetFrontTexture()->GetImageSize().y - 1), -0.1f));
			}
		}
		else if(editor->GetDesignToolBar()->GetCurrentTool() == DesignToolBar::ERASE)
		{
			if(_event->GetEventType() == Mouse::MOVE)
			{
				Vector2 cursorPosition = Mouse::_GetCursorPosition(Renderer::_GetPicked());  
				arrowCursor->SetPosition(Vector3(cursorPosition.x, GetViewportSize().y - cursorPosition.y - (arrowCursor->GetFrontTexture()->GetImageSize().y - 1), -0.1f));
			}
		}
	}

	Engine::MouseEventFilter(_event);
	return _event;
}

	

Keyboard::Event* EngineEx::KeyboardEventFilter(Keyboard::Event* _event)
{ 
	if(editor->GetControlToolBar()->GetCurrentTool() == ControlToolBar::PAUSE)
	{
		delete _event;
		return NIL;
	}
	else if(editor->IsViewportFocus() && Camera3D::_GetPicked())
	{
		if(_event->GetEventType() == Keyboard::KEY_DOWN)
		{
			if(_event->GetKey() == cameraMoveUpKey)
			{
				if(Camera3D::_GetPicked()->GetProjectionMode() == Camera3D::SCREEN_PROJECTION)
				{
					Camera3D::_GetPicked()->Fly(-camera2dMoveSpeed);
				}
				else if(Camera3D::_GetPicked()->GetRotationMode() == Camera3D::FIRST_PERSON_ROTATION)
				{
					Camera3D::_GetPicked()->Fly(camera3dMoveSpeed);
				}
			}
			else if(_event->GetKey() == cameraMoveDownKey)
			{
				if(Camera3D::_GetPicked()->GetProjectionMode() == Camera3D::SCREEN_PROJECTION)
				{
					Camera3D::_GetPicked()->Fly(camera2dMoveSpeed);
				}
				else if(Camera3D::_GetPicked()->GetRotationMode() == Camera3D::FIRST_PERSON_ROTATION)
				{
					Camera3D::_GetPicked()->Fly(-camera3dMoveSpeed);
				}
			}
			else if(_event->GetKey() == cameraMoveRightKey)
			{
				if(Camera3D::_GetPicked()->GetProjectionMode() == Camera3D::SCREEN_PROJECTION)
				{
					Camera3D::_GetPicked()->Strafe(camera2dMoveSpeed);
				}
				else if(Camera3D::_GetPicked()->GetRotationMode() == Camera3D::FIRST_PERSON_ROTATION)
				{
					Camera3D::_GetPicked()->Strafe(camera3dMoveSpeed);
				}
			}
			else if(_event->GetKey() == cameraMoveLeftKey)
			{
				if(Camera3D::_GetPicked()->GetProjectionMode() == Camera3D::SCREEN_PROJECTION)
				{
					Camera3D::_GetPicked()->Strafe(-camera2dMoveSpeed);
				}
				else if(Camera3D::_GetPicked()->GetRotationMode() == Camera3D::FIRST_PERSON_ROTATION)
				{
					Camera3D::_GetPicked()->Strafe(-camera3dMoveSpeed);
				}
			}
		
			if(!_event->IsAutoRepeat())
			{
				if(_event->GetKey() == Keyboard::R)
				{
					if(Keyboard::_IsKeyPressed(Keyboard::CTRL))
					{
						editor->GetDesignToolBar()->SetRotateTool();
					}
				}
				else if(_event->GetKey() == Keyboard::M)
				{
					if(Keyboard::_IsKeyPressed(Keyboard::CTRL))
					{
						editor->GetDesignToolBar()->SetMoveTool();
					}
				}
				else if(_event->GetKey() == Keyboard::S)
				{
					if(Keyboard::_IsKeyPressed(Keyboard::CTRL))
					{
						editor->GetDesignToolBar()->SetScaleTool();
					}
				}
				else if(_event->GetKey() == Keyboard::P)
				{
					if(Keyboard::_IsKeyPressed(Keyboard::CTRL))
					{
						editor->GetDesignToolBar()->SetSelectTool();
					}
				}
				else if(_event->GetKey() == Keyboard::D)
				{
					if(Keyboard::_IsKeyPressed(Keyboard::CTRL))
					{
						editor->GetDesignToolBar()->SetEraseTool();
					}
				}
			}
		}
	}

	Engine::KeyboardEventFilter(_event);
	return _event;
}



LRESULT EngineEx::Proc(HWND _hWindow, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch(_message)
	{
		case WM_ERASEBKGND:
		case WM_PAINT:
		case WM_KEYDOWN:
		case WM_KEYUP:

		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MOUSEWHEEL:

		case WM_MOVE:
		case WM_SIZE:
		case WM_KILLFOCUS:
		case WM_SETFOCUS:
		case WM_ACTIVATE:
		{  
			if(editor->GetControlToolBar()->GetCurrentTool() == ControlToolBar::PAUSE)
			{
				Draw(GetDeltaTimeInSec());
				glFlush();
				SwapBuffers(GetDeviceContext());
			}
			break;
		}
	}
	return Renderer::Proc(_hWindow, _message, _wParam, _lParam);
} 
