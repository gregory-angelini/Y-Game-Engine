#ifndef ENGINE_H
#define ENGINE_H
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Math/Ratio/Ratio.h>
class Engine: public Renderer
{
	public: class Define
	{
		public: Renderer::Define rendererDef;
		public: Define(void)
		{
			rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_CLASSIC);
			rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
			rendererDef.realTimeWorkerDef.windowDef.title = "Engine";
			rendererDef.backgroundColor = RGBA(RGBA::BLACK_COLOR).GetRGBAf();
		}
	};
    private: RGBA pixelColorUnderCursor;
	private: Matrix4x4 screenProjectionMatrix;
	private: Vector3 pointUnderCursor;
    private: Vector2 cursorPosition;
	public: Engine(void);
	public: virtual ~Engine(void);
	public: virtual bool Create(Define _define);
	public: Vector3 GetPointUnderCursor(void)const;
    public: RGBA GetPixelColorUnderCursor(void)const;
	public: Vector2 GetCursorPosition(void)const;
	public: virtual void Update(float _delta);
	public: virtual void Draw(float _delta);
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event);
	public: virtual void ResizeEventFilter(Vector2 _size);
};



#endif 