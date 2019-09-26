#include "WinAPI.h"



char WinAPI::textBuffer[TEXT_BUFFER_SIZE + 1];



void WinAPI::_MessageBox(StringANSI _caption, StringANSI _text, ...)
{
	if(WinAPI::textBuffer)
	{
		va_list argList;
		va_start(argList, _text);
		int32 result = _vsnprintf(WinAPI::textBuffer, TEXT_BUFFER_SIZE, _text.c_str(), argList);
		va_end(argList);
		StringANSI text = StringANSI(WinAPI::textBuffer);
		::MessageBoxA(NIL, text.c_str(), _caption.c_str(), MB_OK);
	}
}