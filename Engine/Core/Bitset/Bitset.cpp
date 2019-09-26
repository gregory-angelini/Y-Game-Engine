#include "Bitset.h"
#include <Engine/Core/Common/Common.h>


Bitset::Bitset(void): bits(NIL), size(NIL)
{}


Bitset::Bitset(int32 _size): bits(NIL), size(NIL)
{
	Resize(_size);
}


Bitset::Bitset(const Bitset& _bitset)
{
	*this = _bitset;
}


Bitset::~Bitset(void)
{
	Destroy();
}


Bitset& Bitset::operator = (const Bitset& _bitset)
{
	if(this == &_bitset)
	{
		return *this;
	}

	const int32 oldBytes = _BitsToBytes(size);
	const int32 newBytes = _BitsToBytes(_bitset.size);

	if(oldBytes != newBytes)
	{
		Destroy();
		CreateBitset(_bitset.size);
	}
	for(int32 i = 0; i < newBytes; ++i)
	{
		bits[i] = _bitset.bits[i];
	}
	return *this;
}


void Bitset::Destroy(void)
{
	if(bits)
	{
		delete[] bits;
		bits = NIL;
		size = NIL;
	}
}



int32 Bitset::GetSize(void)const
{
	return size;
}



int32 Bitset::_BitsToBytes(int32 _size)
{
	int32 bytes = _size / Memory::BIT_IN_BYTE;
	const int32 partOfByte = _size % Memory::BIT_IN_BYTE;
	bytes += partOfByte ? 1 : 0;
	return bytes;
}


void Bitset::CreateBitset(int32 _size)
{
	if(bits == NIL && _size > 0)
	{
		size = _size;
		bits = new uint8[_BitsToBytes(size)];
	}
}

bool Bitset::IsExist(void)const
{
	return bits ? true : false; 
}


void Bitset::Reset(void)
{
	if(bits)
	{
		const int32 bytes = _BitsToBytes(size);
		for(int32 i = 0; i < bytes; ++i)
		{
			bits[i] = NIL;
		}
	}
}


void Bitset::Resize(int32 _size)
{
	if(_size > 0)
	{
		if(bits)
		{
			if(size != _size)
			{
				Bitset oldBitset = *this;
				int32 minSize = Numerical<int32>::_GetMin(_size, size);

				Destroy();
				CreateBitset(_size);
				Reset();
				for(int32 i = 0; i < minSize; ++i)
				{
					SetBit(i, oldBitset.GetBit(i));
				}
			}
		}
		else
		{
			CreateBitset(_size);
			Reset();
		}
	}
	else if(_size == 0)
	{ 
		Destroy();
	}
}


bool Bitset::IsBitExist(int32 _index)const
{
	if(bits && _index >= 0 && _index < size)
	{
		return true;
	}
	return false;
}


uint8* Bitset::GetBits(void)const
{
	return bits;
}


bool Bitset::GetBit(int32 _index)const
{
	if(bits && _index >= 0 && _index < size)
	{
		int32 arrayIndex = _index / Memory::BIT_IN_BYTE;
        int32 bitIndex = _index % Memory::BIT_IN_BYTE;
		return _GetBit(bits[arrayIndex], bitIndex);
	}
	return false;
}
 

bool Bitset::SetBit(int32 _index, bool _enable)
{
	if(bits && _index >= 0 && _index < size)
	{
		int32 arrayIndex = _index / Memory::BIT_IN_BYTE;
        int32 bitIndex = _index % Memory::BIT_IN_BYTE;
		_SetBit(bits[arrayIndex], bitIndex, _enable);
		return true;
	}
	return false;
}


void Bitset::NOT(int32 _index)
{
	SetBit(_index, !GetBit(_index));
}

 
StringANSI Bitset::ToString(void)const 
{
	StringANSI string;

	if(bits)
	{
		const int32 wholeBytes = size / Memory::BIT_IN_BYTE;
		const int32 partOfByte = size % Memory::BIT_IN_BYTE;
		for(int32 i = 0; i < wholeBytes; i++)
		{
			string += _ToString(bits[i]);
		}
		if(partOfByte)
		{
			for(int32 i = 0; i < partOfByte; i++)
			{
				string += NumberToString(_GetBit(bits[wholeBytes], i));
			}
		}
	}
	return string;
}



void Bitset::NOT(void)
{
	if(bits)
	{
		const int32 bytes = _BitsToBytes(size);
		for(int32 i = 0; i < bytes; ++i)
		{
			_NOT(bits[i]);
		}
	}
}



void Bitset::EnableBits(bool _enable)
{
	if(bits)
	{
		const int32 bytes = _BitsToBytes(size);
		for(int32 i = 0; i < bytes; ++i)
		{
			_EnableBits(bits[i], _enable);
		}
	}
}


void Bitset::AND(int32 _index, bool _enable)
{
	if(bits && _index >= 0 && _index < size)
	{
		const int32 arrayIndex = _index / Memory::BIT_IN_BYTE;
        const int32 bitIndex = _index % Memory::BIT_IN_BYTE;
		_AND(bits[arrayIndex], bitIndex, _enable);
	}
}


void Bitset::OR(int32 _index, bool _enable)
{
	if(bits && _index >= 0 && _index < size)
	{
		const int32 arrayIndex = _index / Memory::BIT_IN_BYTE;
        const int32 bitIndex = _index % Memory::BIT_IN_BYTE;
		_OR(bits[arrayIndex], bitIndex, _enable);
	}
}


void Bitset::XOR(int32 _index, bool _enable)
{
	if(bits && _index >= 0 && _index < size)
	{
		const int32 arrayIndex = _index / Memory::BIT_IN_BYTE;
        const int32 bitIndex = _index % Memory::BIT_IN_BYTE;
		_XOR(bits[arrayIndex], bitIndex, _enable);
	}
}


void Bitset::EQUAL(int32 _index, bool _enable)
{
	if(bits && _index >= 0 && _index < size)
	{
		const int32 arrayIndex = _index / Memory::BIT_IN_BYTE;
        const int32 bitIndex = _index % Memory::BIT_IN_BYTE;
		_EQUAL(bits[arrayIndex], bitIndex, _enable);
	}
}


bool Bitset::operator[] (int32 _index)const
{
	return GetBit(_index);
}



bool Bitset::operator == (const Bitset &_bitset)const
{ 
	const Bitset* max = this;
	const Bitset* min = &_bitset;
	 
	if(_bitset.size > size)
	{
		max = &_bitset;
		min = this;
	}

	const int32 bytes = _BitsToBytes(min->size);
	for(int32 i = 0; i < bytes; ++i)
	{
		if(bits[i] != _bitset.bits[i])
		{
			return false;
		}
	}
	for(int32 i = bytes * Memory::BIT_IN_BYTE; i < max->size; ++i)
	{
		if(GetBit(i) != _bitset.GetBit(i))
		{
			return false;
		}
	}
	return true;
}



bool Bitset::operator < (const Bitset &_bitset)const
{
	const Bitset* max = this;
	int32 maxSize, minSize; 
	 
	if(_bitset.size > size)
	{
		maxSize = _bitset.size;
		minSize = size;
	}
	else
	{
		maxSize = size;
		minSize = _bitset.size;
	}

	const int32 bytes = _BitsToBytes(minSize);
	for(int32 i = 0; i < bytes; ++i)
	{
		if(bits[i] > _bitset.bits[i])
		{
			max = this;
		}
		else if(bits[i] < _bitset.bits[i])
		{
			max = &_bitset;
		}
	}
	for(int32 i = bytes * Memory::BIT_IN_BYTE; i < maxSize; ++i)
	{
		if(GetBit(i) > _bitset.GetBit(i))
		{
			max = this;
		}
		else if(GetBit(i) < _bitset.GetBit(i))
		{
			max = &_bitset;
		}
	}
	if(max == this) { return false; }
	return true;
}
	
			

bool Bitset::operator > (const Bitset &_bitset)const
{
	const Bitset* max = &_bitset;
	int32 maxSize, minSize;
	 
	if(_bitset.size > size)
	{
		maxSize = _bitset.size;
		minSize = size;
	}
	else
	{
		maxSize = size;
		minSize = _bitset.size;
	}

	const int32 bytes = _BitsToBytes(minSize);
	for(int32 i = 0; i < bytes; ++i)
	{
		if(bits[i] > _bitset.bits[i])
		{
			max = this;
		}
		else if(bits[i] < _bitset.bits[i])
		{
			max = &_bitset;
		}
	}
	for(int32 i = bytes * Memory::BIT_IN_BYTE; i < maxSize; ++i)
	{
		if(GetBit(i) > _bitset.GetBit(i))
		{
			max = this;
		}
		else if(GetBit(i) < _bitset.GetBit(i))
		{
			max = &_bitset;
		}
	}
	if(max == this) { return true; }
	return false;
}
