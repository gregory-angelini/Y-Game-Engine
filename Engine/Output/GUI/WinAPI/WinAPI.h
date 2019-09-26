#ifndef WINAPI_H
#define WINAPI_H
#include <windows.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
class WinAPI
{
	public: enum
	{
		TEXT_BUFFER_SIZE = 4000
	};
	private: static char textBuffer[TEXT_BUFFER_SIZE + 1];
	public: static void _MessageBox(StringANSI _caption, StringANSI _text, ...);
};

#endif 