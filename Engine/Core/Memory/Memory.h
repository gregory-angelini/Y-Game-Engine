#ifndef MEMORY_H
#define MEMORY_H
#include <Engine/Core/Numerical/Numerical.h>
class Memory
{
	public: enum
	{
		BIT_IN_BYTE = 8,
		BYTE_IN_KBYTE = 1024,
		KBYTE_IN_MBYTE = 1024,
		MBYTE_IN_GBYTE = 1024
	};
	public: static float _FromByteToKB(uint32 _byte);
	public: static float _FromByteToMB(uint32 _byte);
	public: static float _FromByteToGB(uint32 _byte);
	public: static float _FromKBToMB(uint32 _kb);
	public: static float _FromMBToGB(uint32 _mb);
};


#endif 