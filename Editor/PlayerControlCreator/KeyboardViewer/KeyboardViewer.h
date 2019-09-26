#ifndef KEYBOARDVIEWER_H
#define KEYBOARDVIEWER_H
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Output/Graphics/Surface/GraphicsSurface/GraphicsSurface.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
class VirtualKey: public GraphicsSurface
{
	friend class VirtualKeyboard;
	private: Quad quad;
	private: bool activated;
	public: Keyboard::Event event;
	public: bool isValid;
	public: VirtualKey(void);
	public: virtual ~VirtualKey(void);
    public: bool IsActivated(void)const;
	public: void MouseEvent(Mouse::Event _event);
	public: void KeyboardEvent(Keyboard::Event _event);
	public: Keyboard::Key GetKey(void)const;
	public: bool IsExist(void)const;
	public: void SetQuad(Quad _quad);
	public: virtual void Draw(void);
};
class VirtualKeyboard: public TexturingSurface
{
	private: MagicList<VirtualKey> virtualKeys;
	private: MagicList<Keyboard::Event> lastKeyboardEvent;
    private: Texture2D* texture;
	public: VirtualKeyboard(void);
	public: virtual ~VirtualKeyboard(void);
	public: VirtualKey* GetLastVirtualKey(void);
	public: MagicList<VirtualKey>& GetVirtualKeys(void);
	public: VirtualKey* GetLastSelectedVirtualKey(void) ;
	public: void AddVirtualKey(VirtualKey* _virtualKey);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
};
class KeyboardViewer: public Engine,
               	      public MessageHandler<int32>::Receiver
{
	private: Camera3D* camera, *camera2d;
    private: Font2D *font;
	private: Scene* scene;
	
	private: VirtualKeyboard* virtualKeyboard;
	private: bool rightPressed;
	
    private: bool selectedKeySensor;
	private: Keyboard::Key selectedKey;
	public: KeyboardViewer(void);
	public: virtual ~KeyboardViewer(void);
	public: bool Create(Engine::Define _define);
	public: virtual void Update(float _delta);
	public: virtual void Draw(float _delta);
	public: void SetSelectedKey(int32 _key);
	public: int32 GetSelectedKey(void);
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event);
};






#endif 
