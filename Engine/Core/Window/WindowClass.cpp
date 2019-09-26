
#include "Window.h"
		


Window::WindowClass::WindowClass(StringANSI _className): className(_className)
{}



Window::WindowClass::~WindowClass(void)
{
	::UnregisterClassA(className.c_str(), ::GetModuleHandle(NIL));
}