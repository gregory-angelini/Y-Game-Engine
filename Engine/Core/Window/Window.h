#ifndef WINDOW_H
#define WINDOW_H
#include <windows.h>

#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Input/Mouse/Mouse.h>
#include <Engine/Input/Keyboard/Keyboard.h>
#include <Engine/Core/MessageHandler/MessageHandler.h>
#define WindowsOS
#ifdef WindowsOS
   #define WIN32_LEAN_AND_MEAN
#endif
class Window: public MagicNode,
			  public MessageHandler<int32>
{
	public: enum Style
	{
		FULLSCREEN,
		WND_VIEWPORT,
		WND_TITTLE,
		WND_LIGHT,
		WND_CLASSIC,
		WND_RESIZING
	};
	public: enum 
	{
		CLOSE   = 0,
		MESSAGE = 1,
		EMPTY   = 2
	};
	public: enum
	{
		RESIZE_WINDOW_MESSAGE = 50,
		MOVE_WINDOW_MESSAGE = 51,
		MINIMIZED_WINDOW_MESSAGE = 52,
		MAXIMIZED_WINDOW_MESSAGE = 53,
		
		MOUSE_KEY_UP_MESSAGE = 54,
		MOUSE_KEY_DOWN_MESSAGE = 55,
		MOUSE_WHEEL_MESSAGE = 57,
		MOUSE_MOVE_MESSAGE = 58,
        MOUSE_DOUBLE_CLICK_MESSAGE = 59,

		KEYBOARD_KEY_UP_MESSAGE = 60,
		KEYBOARD_KEY_DOWN_MESSAGE = 61
	};
    public: class Define
	{
		public: DWORD windowClassStyle;
		public: int32 windowClassExtraStyle;
        public: int32 windowExtra;
		public: HCURSOR hCursor;
		public: HBRUSH hBrushBackground;
		public: StringANSI menuName;
        public: HICON hBigIcon;
        public: HICON hSmallIcon;
		public: StringANSI title;
		public: DWORD windowStyle;
		public: DWORD windowExtraStyle;
		public: Vector2 position;
		public: Vector2 clientAreaSize;
		public: HWND parentWindow;
		public: HMENU hMenu;
        public: StringANSI logFilePath;
		public: bool mouseEnable;
		public: bool keyboardEnable;
		public: HWND clientWindow;
		public:Define(void);
	    public: void SetTemplateStyle(Window::Style _style);
		private: Vector2 UpdateWindowSize(void);
		public: void MoveToCenter(void);
	};
	private: class WindowClass: public MagicNode
	{
		public: StringANSI className;
		public: WindowClass(StringANSI _className);
		public: ~WindowClass(void);
	};
	friend class RealTimeWorker; 
	friend class Renderer;
	private: DWORD windowClassStyle;
	private: int32 windowClassExtraStyle;
    private: int32 windowExtra;
	private: HCURSOR hCursor;
	private: HBRUSH hBrushBackground;
	private: StringANSI menuName;
	private: StringANSI windowClassName;
    private: HICON hBigIcon;
    private: HICON hSmallIcon;
	private: StringANSI title;
	private: DWORD windowStyle;
	private: DWORD windowExtraStyle;
	private: Vector2 position;
	private: Vector2 clientAreaSize;
	private: HWND parentWindow;
	private: HMENU hMenu;
    private: StringANSI logFilePath;
	private: bool mouseEnable;
	private: bool keyboardEnable;
	private: HWND clientWindow;
		
    private: static MagicList<Window> windowList;
	private: static MagicList<WindowClass> windowClassList;
    private: Keyboard keyboard;
    private: bool keyboardFocus;
	private: Keyboard::Event* keyboardEvent;
	private: Mouse mouse;
    private: Mouse::Event* mouseEvent;
    private: HWND handle;
	private: bool minimized;
	private: Vector2 windowSize;
	private: Vector2 clientAreaPosition;
	private: HINSTANCE hInstance;
	private: HDC deviceContext;
    private: uint32 currentTime;
	private: uint32 deltaTime;
	private: uint32 lastTime;
	private: uint32 workingTime;
    private: LRESULT (CALLBACK* clientWindowProc)(HWND, UINT, WPARAM, LPARAM);
	private: MSG msg;
	private: int32 result;
	private: bool unknownEvent;
    public: Window(void);
	public: virtual ~Window(void);
    public: bool Create(Window::Define _define);
	private: bool RegisterWindowClass(void);
	private: bool CreateDC(HWND _handle);
	private: void AddToList(void);
	public: void Destroy(void);
	private: void ReleaseDC(HWND _handle);
	private: void UpdateTime(void);
	public: float GetWorkingTimeInSec(void)const;
	public: uint32 GetWorkingTimeInMs(void)const;
    public: float GetDeltaTimeInSec(void)const;
    public: uint32 GetDeltaTimeInMs(void)const;
	public: Mouse& GetMouse(void);
	public: void SetMouseEnable(bool _value);
	public: bool IsMouseEnabled(void)const;
    public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event);
	private: void MouseEventHandler(UINT _msg, WPARAM _wParam, LPARAM _lParam);
	public: Keyboard& GetKeyboard(void);
	public: void SetKeyboardEnable(bool _value);
	public: bool IsKeyboardEnabled(void)const;
	public: bool IsKeyboardFocus(void)const;
	public: void SetFocus(void);
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event);
	private: void KeyboardEventHandler(UINT _msg, WPARAM _wParam, LPARAM _lParam);
	public: StringANSI GetLogFile(void)const;
	public: void SetLogFile(StringANSI _path);
	public: void ClearLogFile(void);
	public: bool IsParentWindow(void)const;
	public: Window* GetParentWindow(void)const;
	public: bool IsChildWindow(void)const;
	public: SimpleList<Window*> GetChildWindows(void)const;
	public: static Window* _Find(HWND _hWindow);
	public: static Window* _Find(StringANSI _title);
	public: static MagicList<Window>& _GetWindowList(void);
	public: StringANSI GetTitle(void);
	public: bool SetTitle(StringANSI _title);
	public: bool SetSmallIcon(StringANSI _path);
	public: bool IsAttached(void)const;
	public: Window::Define GetState(void)const;
	private: void SetState(Window::Define _define);
	private: void UpdateState(void);
	private: bool UpdateWindowPosition(void);
	private: bool UpdateClientAreaPosition(void);
	private: void UpdateWindowSize(Vector2 _clientAreaSize);
	private: bool UpdateClientAreaSize(void);
	private: void UpdateDC(void);
	public: HDC GetDeviceContext(void)const;
	public: bool IsMinimized(void)const;
	public: bool IsWindowed(void)const;
	public: HINSTANCE GetInstance(void)const;
	public: HWND GetHandle(void)const;
	public: void MoveToCenter(void);
	public: Vector2 GetClientAreaCenter(void)const;
	public: Vector2 ClientToScreen(const Vector2& _clientPosition)const;
	public: Vector2 ScreenToClient(const Vector2& _screenPosition)const;
	public: Vector2 GetWindowPosition(void)const;
	public: bool SetWindowPosition(Vector2 _position);
	public: Vector2 GetClientAreaPosition(void)const;
	public: Vector2 GetWindowSize(void)const;
	public: Vector2 GetClientAreaSize(void)const;
	public: bool SetClientAreaSize(Vector2 _size);
	public: bool SetClassStyle(DWORD _style, bool _enable);
	public: bool SetWindowStyle(DWORD _style, bool _enable);
	public: bool SetWindowStyle(Window::Style _style);
	public: bool SetWindowExtraStyle(DWORD _style, bool _enable);
	public: virtual LRESULT Proc(HWND _hWindow, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	private: static LRESULT CALLBACK WindowProc(HWND _hWindow,
								                UINT _msg,
								                WPARAM _wParam,
								                LPARAM _lParam);
	public: virtual int32 Frame(void);
	public: virtual void ResizeEventFilter(Vector2 _size);
	public: virtual void MoveEventFilter(Vector2 _position);
	public: virtual void MinimizedEventFilter(void);
	public: virtual void MaximizedEventFilter(void);
};




#endif 