#include "Engine.h"




Engine::Engine(void)
{}


Engine::~Engine(void)
{
}


bool Engine::Create(Define _define)
{
	if(Renderer::Create(_define.rendererDef))
	{
		screenProjectionMatrix.OrthographicProjection(viewportPosition.x, viewportSize.x, viewportPosition.y, viewportSize.y, 0.1f, 10.0f);

		Mouse::_MoveCursorToCenter(this);
		return true;
	}
	return false;
}



Vector3 Engine::GetPointUnderCursor(void)const
{
	return pointUnderCursor;
}



RGBA Engine::GetPixelColorUnderCursor(void)const
{
	return pixelColorUnderCursor;
}



Vector2 Engine::GetCursorPosition(void)const
{
	return cursorPosition;
}



void Engine::Update(float _delta)
{
	Trigger::_ClearDeletionList();

	if(Scene::_GetPicked())
	{
		Scene::_GetPicked()->Update(_delta);
	}
}



void Engine::Draw(float _delta)
{
	cursorPosition = Mouse::_GetCursorPosition(this);

	ClearScene();

	if(Scene::_GetPicked())
	{
		Scene::_GetPicked()->Draw();
	} 
	GetPixelColor(cursorPosition, pixelColorUnderCursor);

	if(Camera3D::_GetPicked())
	{ 
		pointUnderCursor = Camera3D::_GetPicked()->GetPointUnderPixel(cursorPosition);
	}
}


Mouse::Event* Engine::MouseEventFilter(Mouse::Event* _event)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->MouseEventFilter(_event);
	}

	GetMouse().ClearEventList();
	return _event;
}



Keyboard::Event* Engine::KeyboardEventFilter(Keyboard::Event* _event)
{
	if(Camera3D::_GetPicked())
	{
		Camera3D::_GetPicked()->KeyboardEventFilter(_event);
	}

	GetKeyboard().ClearEventList();
	return _event;
}



void Engine::ResizeEventFilter(Vector2 _size)
{
	Renderer::ResizeEventFilter(_size);
	screenProjectionMatrix.OrthographicProjection(viewportPosition.x, viewportSize.x, viewportPosition.y, viewportSize.y, 0.1f, 10.0f);
}