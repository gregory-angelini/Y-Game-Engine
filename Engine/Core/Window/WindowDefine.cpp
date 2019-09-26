
#include "Window.h"

Window::Define::Define(void)
{
	windowClassStyle = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	windowClassExtraStyle = 0;
    windowExtra = 0;
	hCursor = LoadCursor(NIL, IDC_ARROW);
	hBrushBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	menuName = "";
	hBigIcon = LoadIcon(NIL, IDI_APPLICATION);
	hSmallIcon = LoadIcon(NIL, IDI_APPLICATION);
	title = "Title";
	windowStyle = WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
	windowExtraStyle = 0;
	clientAreaSize.Set(640, 480);
	MoveToCenter();
	parentWindow = NIL;
	hMenu = NIL;
	logFilePath = "engine.log";
	mouseEnable = true;
	keyboardEnable = true;
	clientWindow = NIL;
}


Vector2 Window::Define::UpdateWindowSize(void)
{
	RECT clientAreaRect;
	clientAreaRect.left = 0;
	clientAreaRect.right = (int32)clientAreaSize.x;
	clientAreaRect.top = 0;
	clientAreaRect.bottom = (int32)clientAreaSize.y;
	::AdjustWindowRectEx(&clientAreaRect,
		                 windowStyle,
					     (hMenu) ? true : false,
					     windowExtraStyle);
	return Vector2((float)(clientAreaRect.right - clientAreaRect.left), (float)(clientAreaRect.bottom - clientAreaRect.top));
}



void Window::Define::MoveToCenter(void)
{
	Vector2 windowSize = UpdateWindowSize();
	position.Set(((float)::GetSystemMetrics(SM_CXSCREEN) -  windowSize.x) / 2.0f,
		         ((float)::GetSystemMetrics(SM_CYSCREEN) -  windowSize.y) / 2.0f);

}



void Window::Define::SetTemplateStyle(Window::Style _style)
{
	switch(_style)
	{ 
		case Window::FULLSCREEN: 
		{
			windowStyle = WS_VISIBLE | WS_POPUP;
		    windowExtraStyle = 0;
			clientAreaSize.Set((float)::GetSystemMetrics(SM_CXSCREEN),
			              	   (float)::GetSystemMetrics(SM_CYSCREEN));
		    position.Set(0, 0);
		    break;
		}

		case Window::WND_VIEWPORT:
	    {
			windowStyle = WS_VISIBLE | WS_BORDER;
		    windowExtraStyle = WS_EX_TOPMOST; 
		    clientAreaSize.Set(640, 480);
		    MoveToCenter();
		    break; 
		}

		case Window::WND_TITTLE:
		{  
			windowStyle = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION;
		    windowExtraStyle = 0;
		    clientAreaSize.Set(640, 480);
		    MoveToCenter();
		    break;
		}
		
		case Window::WND_LIGHT:
		{
			windowStyle = WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
		    windowExtraStyle = 0; 
		    clientAreaSize.Set(640, 480);
		    MoveToCenter();
		    break;
		} 
		case Window::WND_CLASSIC: 
		{
			windowStyle = WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
		    windowExtraStyle = 0; 
		    clientAreaSize.Set(640, 480);
		    MoveToCenter();
		    break;
		}

		case WND_RESIZING:
		{
			windowStyle = WS_VISIBLE | WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
		    windowExtraStyle = 0; 
		    clientAreaSize.Set(640, 480);
		    MoveToCenter();
			break;
		}  
	}
}


