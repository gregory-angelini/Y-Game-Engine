
#include "KeyboardViewer.h"
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Output/Graphics/Shape/Quad/GraphicsQuad/GraphicsQuad.h>



VirtualKey::VirtualKey(void): activated(false), isValid(false)
{
	SetColor(RGBA(RGBA::RED_COLOR));
}



VirtualKey::~VirtualKey(void)
{
	Destroying();
}


bool VirtualKey::IsActivated(void)const
{
	return activated;
}


void VirtualKey::MouseEvent(Mouse::Event _event)
{
	if(_event.GetEventType() == Mouse::KEY_DOWN)
	{
		if(_event.GetKey() == Mouse::LEFT_BUTTON)
		{
			Vector3 intersectedPoint;

			GraphicsContainer* scene = dynamic_cast<GraphicsContainer*>(GetGraphicsContainer());

			if(scene)
			{
				Camera3D* camera = scene->GetCamera();

				if(camera)
				{
					if(IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &intersectedPoint) != -1)
					{
						activated = true;
						SetColor(RGBA(RGBA::GREEN_COLOR));
					}
					else
					{ 
						activated = false; 
						SetColor(RGBA(RGBA::RED_COLOR));
					}
				}
			}
		}
	}
}


void VirtualKey::KeyboardEvent(Keyboard::Event _event)
{
	if(event.IsExactEquals(_event))
	{
		activated = true;
		SetColor(RGBA(RGBA::GREEN_COLOR));
	}
	else 
	{ 
		activated = false; 
		SetColor(RGBA(RGBA::RED_COLOR));
	}
}



Keyboard::Key VirtualKey::GetKey(void)const
{
	return event.GetKey(); 
}
		


bool VirtualKey::IsExist(void)const
{
	return isValid;
}



void VirtualKey::SetQuad(Quad _quad)
{
	quad = _quad;
}



void VirtualKey::Draw(void)
{
	GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(0)), GetTransformation().TransformVector(GetMesh()->GetVertex(1)), GetColorRGBA());
	GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(1)), GetTransformation().TransformVector(GetMesh()->GetVertex(2)), GetColorRGBA());
	GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(2)), GetTransformation().TransformVector(GetMesh()->GetVertex(3)), GetColorRGBA());
	GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(3)), GetTransformation().TransformVector(GetMesh()->GetVertex(0)), GetColorRGBA());
}




VirtualKeyboard::VirtualKeyboard(void)
{
	texture = new Texture2D(); 
	texture->Create(Texture2D::QUAD_TEXTURE_MAP, (Editor::_GetWorkDir() + Editor::_GetResDir() + "PlayerControlCreator/keyboard.png").toAscii().data());   

	BindFrontTexture(texture);
	Mesh* mesh = new Mesh();
	mesh->CreateQuad(Quad(Vector3(), Vector2(1498, 458) / 400.0f));
	BindMesh(mesh);
}



VirtualKeyboard::~VirtualKeyboard(void)
{
	Destroying();
	virtualKeys.Clear(MagicContainer::DELETE_NODE);
	delete texture;
}



VirtualKey* VirtualKeyboard::GetLastVirtualKey(void)
{
	return virtualKeys.GetTail();
}



MagicList<VirtualKey>& VirtualKeyboard::GetVirtualKeys(void)
{
	return virtualKeys;
}



VirtualKey* VirtualKeyboard::GetLastSelectedVirtualKey(void) 
{ 
	MagicList<VirtualKey>::AddressDerived* it = virtualKeys.GetHeadAddress();
	for(MagicList<VirtualKey>::AddressDerived* it = virtualKeys.GetHeadAddress(); it != NIL; it = it->GetRight())
	{
		if(it->GetNode()->IsActivated())
		{
			return it->GetNode();
		}
	}
	return NIL;
}



void VirtualKeyboard::AddVirtualKey(VirtualKey* _virtualKey)
{
	virtualKeys.AddToTail(_virtualKey);
}


bool VirtualKeyboard::LoadFromFile(StringANSI _path, bool _auto)
{ 
	File file(_path, File::READ);

	if(file.IsOpened())
	{
		int32 amountOfVirtualKeys = 0;
		file.Read(amountOfVirtualKeys);
		for(int32 i = 0; i < amountOfVirtualKeys; ++i)
		{
			VirtualKey* virtualKey = new VirtualKey();
			file.Read(virtualKey->quad);

			Mesh* mesh = new Mesh();
			mesh->CreateQuad(virtualKey->quad);
			virtualKey->BindMesh(mesh);
					
			virtualKey->event.LoadFromFile(file);

			virtualKey->isValid = true;
			Renderer::_GetPicked()->Connect(Window::KEYBOARD_KEY_DOWN_MESSAGE, virtualKey, Caller<>(virtualKey, &VirtualKey::KeyboardEvent));
			Renderer::_GetPicked()->Connect(Window::MOUSE_KEY_DOWN_MESSAGE, virtualKey, Caller<>(virtualKey, &VirtualKey::MouseEvent));
				
			virtualKey->activated = false;
			virtualKeys.AddToTail(virtualKey);
		} 
	}
	return true;
}
 



KeyboardViewer::KeyboardViewer(void): camera(NIL), font(NIL), scene(NIL), virtualKeyboard(NIL), rightPressed(false), selectedKeySensor(false), selectedKey(Keyboard::NO_KEY)
{}



KeyboardViewer::~KeyboardViewer(void)
{
	delete virtualKeyboard;

	delete camera;
	delete camera2d;

	delete font;

	delete scene;
} 




bool KeyboardViewer::Create(Engine::Define _define)
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
			def.color = RGBA(RGBA::WHITE_COLOR).GetRGBAf();

			font = new Font2D();
			font->Create(def);
		}
		{
			virtualKeyboard = new VirtualKeyboard();
			scene->AddToRenderList(virtualKeyboard);
			virtualKeyboard->LoadFromFile((Editor::_GetWorkDir() + Editor::_GetResDir() + "PlayerControlCreator/keyboard.script").toAscii().data());
			for(int32 i = 0; i < virtualKeyboard->GetVirtualKeys().GetSize(); ++i)
			{
				VirtualKey* it = virtualKeyboard->GetVirtualKeys()[i];
								
				scene->AddToRenderList(it);
			}
		}
		{
			Camera3D::Define def;
			
			def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;
			def.viewingPoint = virtualKeyboard->GetPosition();
			def.rotationEnable = false;
			def.yawClampEnable = true;
			def.pitchClampEnable = true;
			def.zoomEnable = true;

			def.pitchMax = Angle::_DegreesToRadians(40);
			def.pitchMin = Angle::_DegreesToRadians(-40);
			def.yawMax = Angle::_DegreesToRadians(40);
			def.yawMin = Angle::_DegreesToRadians(-40);

			camera = new Camera3D();
			camera->Create(def);
			scene->BindCamera(camera);
			camera->SetZoomNear(camera->GetZoomNear() * 10);
			camera->SetZoomFar(camera->GetZoomFar() * 2);
		}
		{
			Camera3D::Define def;
			def.projectionMode = Camera3D::SCREEN_PROJECTION;
			def.distanceToViewingPoint = 1.0f;
			def.pitchClampEnable = false;
				
			camera2d = new Camera3D();
			camera2d->Create(def);
		}
		SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
		return true;
	}
	return false;
}






void KeyboardViewer::Update(float _delta)
{
	scene->Update(_delta);


	if(selectedKeySensor)
	{
		SetFocus();
		Keyboard::_CreateEvent(selectedKey, Keyboard::KEY_DOWN);
					

		Keyboard::_CreateEvent(selectedKey ,Keyboard::KEY_UP);

		selectedKeySensor = false;
	}
}



void KeyboardViewer::Draw(float _delta)
{
	ClearScene();
	scene->Draw();
	camera2d->BeginTransform(Renderer::PROJECTION);
	camera2d->BeginTransform(Renderer::MODELVIEW);
	int32 i = 0;
	

	if(virtualKeyboard->GetLastSelectedVirtualKey())
	{
		font->DrawScreenText(Vector3(10, i+=20, -1), "last key: %s", Keyboard::_GetKeyName(virtualKeyboard->GetLastSelectedVirtualKey()->GetKey()).c_str()); 
	}
	else 
	{ 
		font->DrawScreenText(Vector3(10, i+=20, -1), "last key: ");
	}
	camera2d->EndTransform(Renderer::PROJECTION);
	camera2d->EndTransform(Renderer::MODELVIEW);
}

			

Mouse::Event* KeyboardViewer::MouseEventFilter(Mouse::Event* _event)
{
	camera->MouseEventFilter(_event);

	if(_event->GetEventType() == Mouse::KEY_DOWN)
	{
		if(_event->GetKey() == Mouse::RIGHT_BUTTON)
		{

			if(!rightPressed)
			{
				Mouse::_MoveCursorToCenter(Renderer::_GetPicked());
				camera->SetRotationEnable(true);
				rightPressed = true;
				Mouse::_SetShowCursor(false);
			}
		}
	}
	else if(_event->GetEventType() == Mouse::KEY_UP)
	{
		if(_event->GetKey() == Mouse::RIGHT_BUTTON)
		{
			if(rightPressed)
			{
				camera->SetRotationEnable(false);
				rightPressed = false;
				Mouse::_SetShowCursor(true);
			}
		}
	}

	GetMouse().ClearEventList();
	return _event;
}



Keyboard::Event* KeyboardViewer::KeyboardEventFilter(Keyboard::Event* _event)
{
	GetKeyboard().ClearEventList();

	return _event;
}


void KeyboardViewer::SetSelectedKey(int32 _key)
{
	selectedKeySensor = true;
	selectedKey = (Keyboard::Key)_key;
}


int32 KeyboardViewer::GetSelectedKey(void)
{
	VirtualKey* virtualKey = virtualKeyboard->GetLastSelectedVirtualKey();

	if(virtualKey)
	{
		return virtualKey->event.GetKey();
	}
	return Keyboard::NO_KEY;
}