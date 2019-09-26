
#include "Window.h"
#include <Engine/Output/File/File.h>
#include <Engine/Core/Time/SystemTime/SystemTime.h>
MagicList<Window> Window::windowList;
MagicList<Window::WindowClass> Window::windowClassList;




Window::Window(void): deviceContext(NIL), handle(NIL), keyboardFocus(true), clientWindowProc(NIL), minimized(false), clientWindow(NIL)
{
	hInstance = ::GetModuleHandle(NIL);
	Define defaultState;
	SetState(defaultState); 
}



Window::~Window(void)
{

	Destroy();
}


void Window::Destroy(void)
{

	if(handle)
	{ 
		if(!::DestroyWindow(handle))
		{
			File file(GetLogFile());
		
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(title + ".Destroy - FAILED\r\n");
			}
		}
	}
}



bool Window::RegisterWindowClass(void)
{
	windowClassName = title + "WindowClass" + NumberToString(Window::windowClassList.GetSize() + 1);
	WNDCLASSEXA wndClassDef;
	wndClassDef.cbSize = sizeof(WNDCLASSEX);
	wndClassDef.lpfnWndProc = WindowProc;
	wndClassDef.style = windowClassStyle;
	wndClassDef.cbClsExtra = windowClassExtraStyle;
	wndClassDef.cbWndExtra = windowExtra;
	wndClassDef.hInstance = hInstance;
	wndClassDef.hIcon = hBigIcon;
	wndClassDef.hIconSm = hSmallIcon;
	wndClassDef.hCursor = hCursor;
	wndClassDef.hbrBackground = hBrushBackground;
	wndClassDef.lpszMenuName = menuName.c_str();
	wndClassDef.lpszClassName = windowClassName.c_str();
	if(::RegisterClassExA(&wndClassDef) == 0)
	{
		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			file.WriteFormatString(title + ".RegisterWindowClass - FAILED\r\n");
		}
		return false;
	}

	Window::windowClassList.AddToTail(new WindowClass(windowClassName));
	return true;
}


Vector2 Window::GetWindowSize(void)const
{
	return windowSize;
}



void Window::UpdateWindowSize(Vector2 _clientAreaSize)
{
	RECT clientAreaRect;
	clientAreaRect.left = 0;
	clientAreaRect.right = (int32)_clientAreaSize.x;
	clientAreaRect.top = 0;
	clientAreaRect.bottom = (int32)_clientAreaSize.y;
	::AdjustWindowRectEx(&clientAreaRect,
		                 windowStyle,
					     (hMenu) ? true : false,
					     windowExtraStyle);

	windowSize = Vector2((float)(clientAreaRect.right - clientAreaRect.left), (float)(clientAreaRect.bottom - clientAreaRect.top));
}



bool Window::Create(Window::Define _define)
{
	if(handle)
	{ 
		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			file.WriteFormatString(title + ".Create: window has been created!\r\n");
		}
		return false;
	}
	SetState(_define);
    
	ClearLogFile();
	workingTime = 0;
	lastTime = 0;
	deltaTime = 0;
	if(clientWindow)
	{
		if(clientWindowProc = (WNDPROC)SetWindowLongPtr(clientWindow, GWL_WNDPROC, (LONG)WindowProc))
		{
			handle = clientWindow;
			Window::windowList.AddToTail(this);
			SetWindowLong(handle, GWL_USERDATA, (LONG)this);
			UpdateState();
			return true;
		}
		return false;
	}
	if(!RegisterWindowClass())
	{
		return false;
	}
	UpdateWindowSize(clientAreaSize);
	::CreateWindowExA(windowExtraStyle,
					  windowClassName.c_str(),
					  title.c_str(),
					  windowStyle,
					  (int32)position.x, (int32)position.y,
				      (int32)windowSize.x, (int32)windowSize.y,
					  parentWindow ?  parentWindow : NIL,
				      hMenu,
					  hInstance,
					  this);
	if(handle == NIL)
	{
		File file(GetLogFile());
		
		if(file.Open(File::WRITE))
		{
			file.WriteFormatString(title + ".Window::Create - FAILED\r\n");
		}
		return false;
	}

	CreateDC(handle);
	::ShowWindow(handle, SW_SHOW);

	::UpdateWindow(handle);
	return true;
}




Window::Define Window::GetState(void)const
{
	Define define;
	define.windowClassStyle = windowClassStyle;
	define.windowClassExtraStyle = windowClassExtraStyle;
    define.windowExtra = windowExtra;
	define.hCursor = hCursor;
	define.hBrushBackground = hBrushBackground;
	define.menuName = menuName;
    define.hBigIcon = hBigIcon;
    define.hSmallIcon = hSmallIcon;
	define.title = title;
	define.windowStyle = windowStyle;
	define.windowExtraStyle = windowExtraStyle;
	define.position = position;
	define.clientAreaSize = clientAreaSize;
	define.parentWindow = parentWindow;
	define.hMenu = hMenu;
    define.logFilePath = logFilePath;
    define.mouseEnable = mouseEnable;
	define.keyboardEnable = keyboardEnable;
	return define;
}



void Window::UpdateTime(void)
{
	currentTime = SystemTime::_GetSystemTimeInMs();
	if(lastTime == 0) { lastTime = SystemTime::_GetSystemTimeInMs(); }
	deltaTime = (uint32)(currentTime - lastTime);
	
	workingTime += deltaTime;
	lastTime = currentTime;
}


float Window::GetWorkingTimeInSec(void)const
{ 
	return (float)workingTime / HMS::MS_IN_SECOND;
}


uint32 Window::GetWorkingTimeInMs(void)const
{
	return workingTime;
}


float Window::GetDeltaTimeInSec(void)const
{
	return (float)deltaTime / HMS::MS_IN_SECOND;
}


uint32 Window::GetDeltaTimeInMs(void)const
{
	return deltaTime;
}


HDC Window::GetDeviceContext(void)const
{
	return deviceContext; 
}



int32 Window::Frame(void)
{
	if(handle == NIL) { return result = Window::CLOSE; }

	UpdateTime();

	::ZeroMemory(&msg, sizeof(MSG));
	if(::PeekMessage(&msg,
			         0,
					 0,
					 0,
					 PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
		result = Window::MESSAGE;
	}
	else
	{
		result = Window::EMPTY;
	}
	
	return result;
}





bool Window::UpdateWindowPosition(void)
{
	RECT rect;
	if(::GetWindowRect(handle, &rect))
	{
		position.x = (float)rect.left;
		position.y = (float)rect.top;
		return true;
	}

	return false;
}



void Window::KeyboardEventHandler(UINT _msg, WPARAM _wParam, LPARAM _lParam) 
{
	if(keyboardEvent = keyboard.EventFilter(this, _msg, _wParam, _lParam))
	{
		if(keyboardEvent = KeyboardEventFilter(keyboardEvent))
		{
			if(keyboardEvent->IsAutoRepeat())
			{
				delete keyboardEvent;
			}
			else
			{
				keyboard.events.AddToTail(keyboardEvent);
				SendPackage(keyboardEvent->GetMessageId(), *keyboardEvent);
			}
		}
	}
}



void Window::MouseEventHandler(UINT _msg, WPARAM _wParam, LPARAM _lParam) 
{
	if(mouseEvent = mouse.EventFilter(this, _msg, _wParam, _lParam))
	{
		if(mouseEvent = MouseEventFilter(mouseEvent))
		{
			 mouse.events.AddToTail(mouseEvent);
			 SendPackage(mouseEvent->GetMessageId(), *mouseEvent);
		}
	}
}



LRESULT Window::Proc(HWND _hWindow, UINT _msg, WPARAM _wParam, LPARAM _lParam) 
{ 

	unknownEvent = true;
	if(keyboardEnable)
	{
		switch(_msg)
		{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				KeyboardEventHandler(_msg, _wParam, _lParam);
				switch(_wParam)
				{
					case VK_MENU:
					case VK_F10:
					{
						return 0;
					}
				}
				unknownEvent = false;
				break;
			}

			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				KeyboardEventHandler(_msg, _wParam, _lParam);
				switch(_wParam)
				{
					case VK_MENU:
					case VK_F10:
					{
						return 0;
					}
				}
				unknownEvent = false;
				break;
			}
		}
	}
	if(mouseEnable && unknownEvent)
	{
		switch(_msg)
		{
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
            {
				MouseEventHandler(_msg, _wParam, _lParam);
				unknownEvent = false;
				break;
			}

			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			{
				MouseEventHandler(_msg, _wParam, _lParam);
				unknownEvent = false;
				break;
			}

			case WM_LBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			{
				MouseEventHandler(_msg, _wParam, _lParam);
				unknownEvent = false;
				break;
			}

			case WM_MOUSEMOVE:
			{
				MouseEventHandler(_msg, _wParam, _lParam);
				unknownEvent = false;
				break;
			}

			case WM_MOUSEWHEEL:
			{
				MouseEventHandler(_msg, _wParam, _lParam);
				unknownEvent = false;
				break;
			}
		}
	}
	if(unknownEvent)
	{
		switch(_msg)
		{
			case WM_SETFOCUS:
			{
				keyboardFocus = true;
				break;  
			}

			case WM_KILLFOCUS:
			{
				keyboardFocus = false;
				break;  
			}

			case WM_ACTIVATE:
			{
				if(LOWORD(_wParam) != WA_INACTIVE)
				{
				}
				else
				{
				}
				if(HIWORD(_wParam)) 
				{
					if(!minimized)
					{
						minimized = true; 
						MinimizedEventFilter();
						if(IsReceiversExist(MINIMIZED_WINDOW_MESSAGE)) { SendMessage(MINIMIZED_WINDOW_MESSAGE); }
					}
				}
				else
				{
					if(minimized)
					{
						minimized = false;
						MaximizedEventFilter();
						if(IsReceiversExist(MAXIMIZED_WINDOW_MESSAGE)) { SendMessage(MAXIMIZED_WINDOW_MESSAGE); }
					}
				}
				break;  
			} 
	       
			case WM_MOVE:
			{
				clientAreaPosition.Set(LOWORD(_lParam),
								       HIWORD(_lParam));
				
				UpdateWindowPosition();
				
				MoveEventFilter(clientAreaPosition);
				if(IsReceiversExist(MOVE_WINDOW_MESSAGE)) { SendPackage(MOVE_WINDOW_MESSAGE, position); }
				break;
			}

			case WM_SIZE:
			{
				clientAreaSize.Set(LOWORD(_lParam),
								   HIWORD(_lParam));
				
				UpdateWindowSize(clientAreaSize);
				
				ResizeEventFilter(clientAreaSize);
				if(IsReceiversExist(RESIZE_WINDOW_MESSAGE)) { SendPackage(RESIZE_WINDOW_MESSAGE, clientAreaSize); }
				break; 
			} 

			case WM_CREATE:
			{
				File file(logFilePath, File::REWRITE);
				file.WriteFormatString(title + ".WM_CREATE\r\n");
				break;
			}
 
			case WM_DESTROY:
			{
				ReleaseDC(handle);
				if(clientWindow)
				{
					::CallWindowProc(clientWindowProc, _hWindow, _msg, _wParam, _lParam);
					(WNDPROC)SetWindowLongPtr(handle, GWL_WNDPROC, (LONG)clientWindowProc);
					clientWindowProc = NIL;
					clientWindow = NIL;
				}
	 
				windowList.Remove(this, MagicContainer::REMOVE_NODE);
				handle = NIL;

				File file(GetLogFile());
		
				if(file.Open(File::WRITE))
				{
					file.WriteFormatString(title + ".WM_DESTROY\r\n");
				}
				break;
			}
		}
	}
	if(clientWindowProc)
	{
		return ::CallWindowProc(clientWindowProc, _hWindow, _msg, _wParam, _lParam);
	}
	return ::DefWindowProc(_hWindow, _msg, _wParam, _lParam);
}


void Window::MinimizedEventFilter(void)
{}


void Window::MaximizedEventFilter(void)
{}


void Window::ResizeEventFilter(Vector2 _size)
{}


void Window::MoveEventFilter(Vector2 _position)
{}



StringANSI Window::GetLogFile(void)const
{
	return logFilePath;
}


bool Window::IsKeyboardFocus(void)const
{
	return keyboardFocus;
}


void Window::SetFocus(void)
{
	if(handle) { ::SetFocus(handle); }
}



bool Window::IsMinimized(void)const
{
	return minimized;
}



Vector2 Window::GetClientAreaPosition(void)const
{
	return clientAreaPosition;
}


Vector2 Window::GetClientAreaSize(void)const
{
	return clientAreaSize;
}


HWND Window::GetHandle(void)const
{
	return handle;
}
 

void Window::SetLogFile(StringANSI _path)
{
	logFilePath = _path;
}


void Window::ClearLogFile(void)
{
	File file(logFilePath, File::REWRITE);
}

Mouse::Event* Window::MouseEventFilter(Mouse::Event* _event)
{
	return NIL;
}



Keyboard::Event* Window::KeyboardEventFilter(Keyboard::Event* _event)
{
	return NIL;
}



void Window::SetMouseEnable(bool _value)
{
	mouseEnable = _value;
}



bool Window::IsMouseEnabled(void)const
{
	return mouseEnable;
}



void Window::SetKeyboardEnable(bool _value)
{
	keyboardEnable = _value;
}

bool Window::IsKeyboardEnabled(void)const
{
	return keyboardEnable;
}


bool Window::SetSmallIcon(StringANSI _path)
{
	if(handle)
	{
		HANDLE hIcon = ::LoadImageA((HINSTANCE)handle, _path.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);

		if(hIcon)
		{
			::SendMessageA(handle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		    return true;
		}
	}

	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(title + ".SetSmallIcon - FAILED\r\n");
	}
	return false;
}



Vector2 Window::GetWindowPosition(void)const
{
	return position;
}



Vector2 Window::ClientToScreen(const Vector2& _clientPosition)const
{
	return _clientPosition + clientAreaPosition;
}


Vector2 Window::ScreenToClient(const Vector2& _screenPosition)const
{
	return _screenPosition - clientAreaPosition;
}


Vector2 Window::GetClientAreaCenter(void)const
{
	return ClientToScreen(Vector2(clientAreaSize.x / 2.0f, clientAreaSize.y / 2.0f));
}


void Window::MoveToCenter(void)
{
	Vector2 center(((float)::GetSystemMetrics(SM_CXSCREEN) - windowSize.x) / 2.0f,
		            ((float)::GetSystemMetrics(SM_CYSCREEN) - windowSize.y) / 2.0f);
	SetWindowPosition(center);
}
 


bool Window::SetClientAreaSize(Vector2 _size)
{ 
	if(handle)
	{ 
		if(_size == clientAreaSize) { return true; }

		UINT flags = SWP_NOZORDER | SWP_NOMOVE;
		
		UpdateWindowSize(_size);
		if(SetWindowPos(handle, 0, 0, 0, (int32)windowSize.x, (int32)windowSize.y, flags))                                                                      
		{
			return true;
		}
	}

	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(title + ".SetClientAreaSize - FAILED\r\n");
	}
	return false;
}



bool Window::SetWindowPosition(Vector2 _position)
{
	if(handle)
	{ 
		if(_position == position) { return true; }

		UINT flags = SWP_NOZORDER | SWP_NOSIZE;
		if(::SetWindowPos(handle, 0, (int32)_position.x, (int32)_position.y, 0, 0, flags))                                                                      
		{
			position = _position;
			return true;
		}
	}

	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(title + ".SetWindowPosition - FAILED\r\n");
	}
	return false;
}





bool Window::SetTitle(StringANSI _title)
{
	if(handle)
	{
		if(title == _title)
		{
			return true;
		}
		if(SetWindowTextA(handle, _title.c_str()))
		{
			title = _title;
			return true;
		}
	}
	
	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(title + ".SetTitle - FAILED\r\n");
	}
	return false;
}



StringANSI Window::GetTitle(void)
{
	return title;
}




void Window::SetState(Window::Define _define)
{
	windowClassStyle = _define.windowClassStyle;
	windowClassExtraStyle = _define.windowClassExtraStyle;
    windowExtra = _define.windowExtra;
	hCursor = _define.hCursor;
	hBrushBackground = _define.hBrushBackground;
	menuName = _define.menuName;
    hBigIcon = _define.hBigIcon;
    hSmallIcon = _define.hSmallIcon;
	title = _define.title;
	windowStyle = _define.windowStyle;
	windowExtraStyle = _define.windowExtraStyle;
	position = _define.position;
	clientAreaSize = _define.clientAreaSize;
	parentWindow = _define.parentWindow;
	hMenu = _define.hMenu;
    logFilePath = _define.logFilePath;
    mouseEnable = _define.mouseEnable;
	keyboardEnable = _define.keyboardEnable;
	clientWindow = _define.clientWindow;
}



bool Window::UpdateClientAreaPosition(void)
{
	RECT rect;
	if(::GetClientRect(handle, &rect))
	{
		POINT clientLeftTopCorner;
		clientLeftTopCorner.x = (float)rect.left;
		clientLeftTopCorner.y = (float)rect.top;
		if(::ClientToScreen(handle, &clientLeftTopCorner))
		{
			clientAreaPosition.Set((float)clientLeftTopCorner.x, (float)clientLeftTopCorner.y);
			return true;
		}
	}

	return false;
}


bool Window::UpdateClientAreaSize(void)
{
	RECT rect;
	if(::GetClientRect(handle, &rect))
	{
		clientAreaSize.x = (float)rect.right;
		clientAreaSize.y = (float)rect.bottom;
		return true;
	}

	return false;
}


void Window::UpdateState(void)
{
	UpdateClientAreaPosition();
	UpdateWindowPosition();

	UpdateClientAreaSize();
	UpdateWindowSize(clientAreaSize);

	UpdateDC();
}

void Window::UpdateDC(void)
{
	ReleaseDC(handle);
	CreateDC(handle);
}


bool Window::CreateDC(HWND _handle)
{
	if(deviceContext == NIL)
	{
		deviceContext = ::GetDC(_handle);
		return (deviceContext != NIL);
	}
	return false;
}


void Window::ReleaseDC(HWND _handle)
{
	if(deviceContext)
	{
		if(::ReleaseDC(_handle, deviceContext))
		{
			deviceContext = NIL;
		}
		else
		{ 
			File file(GetLogFile());
		
			if(file.Open(File::WRITE))
			{
				file.WriteFormatString(title + ".ReleaseDC - FAILED\r\n");
			}
		}
	}
}






bool Window::SetClassStyle(DWORD _style, bool _enable)
{
	if(handle)
	{
		DWORD currentWindowStyle = ::GetClassLong(handle, GCL_STYLE);
		
		if(windowClassStyle == _style) { return true; }

		if(_enable)
		{
			currentWindowStyle |= _style;
		}
		else
		{
			currentWindowStyle &= ~_style;
		}
		::SetClassLong(handle, GCL_STYLE, currentWindowStyle);
		
		windowClassStyle = currentWindowStyle;
		if(::SetWindowPos(handle, 0, 0, 0, 0, 0, 
					    SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED))
		{ return true; }
	}

	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(title + ".SetClassStyle - FAILED\r\n");
	}
	return false;
}




bool Window::SetWindowStyle(DWORD _style, bool _enable)
{
	if(handle)
	{
		DWORD currentStyle = ::GetWindowLong(handle, GWL_STYLE);
		
		if(windowStyle == _style) { return true; }

		if(_enable)
		{
			currentStyle |= _style;
		}
		else
		{
			currentStyle &= ~_style;
		}
		SetWindowLong(handle, GWL_STYLE, currentStyle);
		
		windowStyle = currentStyle;
		if(SetWindowPos(handle, 0, 0, 0, 0, 0, 
					    SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED))
		{ return true; }
	}

	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(title + ".SetWindowStyle - FAILED\r\n");
	}
	return false;
}


  
bool Window::SetWindowExtraStyle(DWORD _style, bool _enable)
{  
	if(handle)
	{
		DWORD currentExtraStyle = ::GetWindowLong(handle, GWL_EXSTYLE);
		
		if(windowExtraStyle == _style) { return true; }

		if(_enable)
		{
			currentExtraStyle |= _style;
		}
		else
		{
			currentExtraStyle &= ~_style;
		}
		SetWindowLong(handle, GWL_EXSTYLE, currentExtraStyle);
			
		windowExtraStyle = currentExtraStyle;
		if(SetWindowPos(handle, 0, 0, 0, 0, 0, 
					    SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED))
		{ return true; }
	}

	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(title + ".SetWindowExtraStyle - FAILED\r\n");
	}
	return false;
} 
 



bool Window::SetWindowStyle(Window::Style _style)
{
	if(handle)
	{
		Define define;
		define.SetTemplateStyle(_style);
		int32 equal = 0;
		if(windowStyle != define.windowStyle)
		{
			SetWindowLong(handle, GWL_STYLE, define.windowStyle);
			windowStyle = define.windowStyle;
		} 
		else { ++equal; }
		if( windowExtraStyle != define.windowExtraStyle)
		{
			SetWindowLong(handle, GWL_EXSTYLE, define.windowExtraStyle);
			windowExtraStyle = define.windowExtraStyle;
		}
		else { ++equal; }

		if(equal == 2) { return true; }
		HWND zOrder;
		if(define.windowExtraStyle & WS_EX_TOPMOST)
		{
			zOrder = HWND_TOPMOST;
		}
		else { zOrder = HWND_NOTOPMOST; }
		int32 xPos;
		int32 yPos;
		int32 width;
		int32 height;

		DWORD flags;
		if(_style == Window::FULLSCREEN)
		{   
			xPos = (int32)define.position.x;
			yPos = (int32)define.position.y;
			width = (int32)define.clientAreaSize.x;
			height = (int32)define.clientAreaSize.y;
			flags = SWP_FRAMECHANGED;
		}
		else
		{
			xPos = 0;
			yPos = 0;
			width = 0;
			height = 0; 
			flags = SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED;
		} 
		if(SetWindowPos(handle, zOrder, xPos, yPos, width, height, flags)) 
		{ 
			return true;
		}
	} 
  
	File file(GetLogFile());
		
	if(file.Open(File::WRITE))
	{
		file.WriteFormatString(title + ".SetWindowStyle - FAILED\r\n");
	}
	return false;
}




bool Window::IsWindowed(void)const
{
	if(windowStyle & WS_POPUP)
	{
		return false;
	}
	return true;
}





bool Window::IsAttached(void)const
{
	return (handle && clientWindow);
}



bool Window::IsParentWindow(void)const
{
	if(handle)
	{
		if(!GetChildWindows().IsEmpty())
		{ return true; }
	}
	return false;
}


bool Window::IsChildWindow(void)const
{
	if(handle)
	{
		if(GetParentWindow())
		{ return true; }
	}
	return false;
}




SimpleList<Window*> Window::GetChildWindows(void)const
{

	SimpleList<Window*> list;

	if(handle)
	{
		MagicList<Window>::AddressDerived* it = Window::windowList.GetHeadAddress();
		Window* curWindow;

		while(it)
		{
			curWindow = it->GetNode();

			if(curWindow->GetParentWindow() == this)
			{
				list.AddToTail(curWindow);
			}
			
			it = it->GetRight();
		}
	}

	return list;
}


Window* Window::GetParentWindow(void)const
{
	if(handle)
	{
		if(parentWindow)
		{
			return _Find(parentWindow);
		}
	}
	return NIL;
}


Window* Window::_Find(HWND _hWindow)
{
	return Window::windowList.Find(_hWindow, &Window::GetHandle);
}


Window* Window::_Find(StringANSI _title)
{
	return Window::windowList.Find(_title, &Window::GetTitle);
}


MagicList<Window>& Window::_GetWindowList(void)
{
	return Window::windowList;
}



HINSTANCE Window::GetInstance(void)const
{
	return hInstance;
}


Mouse& Window::GetMouse(void)
{
	return mouse;
}


Keyboard& Window::GetKeyboard(void)
{
	return keyboard;
}


LRESULT CALLBACK Window::WindowProc(HWND _hWindow, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	Window* window = NIL;
	if(_msg == WM_NCCREATE)
	{
		window = (Window*)((LPCREATESTRUCT)_lParam)->lpCreateParams;
		
		if(window)
		{ 
			window->handle = _hWindow; 
		
			Window::windowList.AddToTail(window);
			
			SetWindowLong(_hWindow, GWL_USERDATA, LONG(((LPCREATESTRUCT)_lParam)->lpCreateParams));
		}
	}
	window = (Window*)::GetWindowLong(_hWindow, GWL_USERDATA);
	
	if(window) 
	{ 
		return window->Proc(_hWindow, _msg, _wParam, _lParam);
	} 
	else { return ::DefWindowProc(_hWindow, _msg, _wParam, _lParam); }
}

