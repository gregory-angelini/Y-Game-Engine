#include "Memory.h"
 



float Memory::_FromByteToKB(uint32 byte)
{
	return (float)byte / BYTE_IN_KBYTE;
}


float Memory::_FromKBToMB(uint32 _kb)
{
	return (float)_kb / KBYTE_IN_MBYTE;
}
				
			
float Memory::_FromMBToGB(uint32 _mb)
{
	return (float)_mb / MBYTE_IN_GBYTE;
}



float Memory::_FromByteToMB(uint32 _byte)
{
	return (float)_FromKBToMB(_FromByteToKB(_byte));
}
				
			
float Memory::_FromByteToGB(uint32 _byte)
{
	return (float)_FromMBToGB(_FromKBToMB(_FromByteToKB(_byte)));
}
