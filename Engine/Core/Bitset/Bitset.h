#ifndef BITSET_H
#define BITSET_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Memory/Memory.h>
class Bitset
{ 
	
	private: uint8* bits;
	private: int32 size;
	public: Bitset(void);
	public: Bitset(const Bitset& _bitset);
	public: Bitset(int32 _size);
	public: virtual ~Bitset(void);
    public: Bitset& operator = (const Bitset& _bitset);
	public: bool operator [] (int32 _index)const;
	public: bool operator == (const Bitset &_bitset)const;
	public: bool operator < (const Bitset &_bitset)const;
	public: bool operator > (const Bitset &_bitset)const;
	public: int32 GetSize(void)const;
	public: template<typename T> static int32 _GetSize(const T& _bitset)
	{
		return sizeof(T) * Memory::BIT_IN_BYTE;
	}
	public: bool IsExist(void)const;
	public: bool IsBitExist(int32 _index)const;
	public: bool GetBit(int32 _index)const;
	public: uint8* GetBits(void)const;
	public: template<typename T> static bool _GetBit(const T& _bitset, int32 _index)
	{
		if(_index >= 0 && _index < sizeof(T) * Memory::BIT_IN_BYTE)
		{
			return (_bitset & (T(1) << _index) ? true : false);
		}

		return false;
	}
	public: StringANSI ToString(void)const;
	public: template<typename T> static StringANSI _ToString(const T& _bitset)
	{ 
		StringANSI bits;

		const int32 size = sizeof(T) * Memory::BIT_IN_BYTE;
		for(int32 i = 0; i < size; ++i) 
		{
			bits += NumberToString((_bitset & (T(1) << i)) ? 1 : 0);
		}

		return bits;
	}
	public: void Reset(void);
	private: void Destroy(void);
	public: void Resize(int32 _size);
	private: void CreateBitset(int32 _size);
	private: static int32 _BitsToBytes(int32 _size);
	public: bool SetBit(int32 _index, bool _enable);
	public: template<typename T> static bool _SetBit(T& _bitset, int32 _index, bool _enable)
	{
		if(_index >= 0 && _index < (sizeof(T) * Memory::BIT_IN_BYTE))
		{
			if(_enable)
			{
				_bitset |= (T(1) << _index);
			}
			else
			{
				_bitset &= ~(T(1) << _index);
			}
			return true;
		}
		return false;
	}
	public: void EnableBits(bool _enable);
	public: template<typename T> static void _EnableBits(T& _bitset, bool _enable)
	{
		if(_enable)
		{
			_bitset = ~(_bitset & 0);
		}
		else
		{
			_bitset &= 0;
		}
	}
	public: void NOT(int32 _index);
	public: template<typename T> static bool _NOT(T& _bitset,  int32 _index)
	{ 
		return _SetBit(_bitset, _index, !_GetBit(_bitset, _index));
	}
	public: void NOT(void);
	public: template<typename T> static void _NOT(T& _bitset)
	{
		_bitset = ~_bitset;
	}
	public: void AND(int32 _index, bool _enable);
	public: template<typename T> static bool _AND(T& _bitset, int32 _index, bool _enable) 
	{
		if(_index >= 0 && _index < (sizeof(T) * Memory::BIT_IN_BYTE))
		{
			if(_enable)
			{
			}
			else
			{
				_bitset &=  ~(T(1) << _index);
			}
			return true;
		}
		return false;
	}
	public: void OR(int32 _index, bool _enable);
	public: template<typename T> static bool _OR(T& _bitset, int32 _index, bool _enable) 
	{
		if(_index >= 0 && _index < (sizeof(T) * Memory::BIT_IN_BYTE))
		{
			if(_enable)
			{
				_bitset |= (T(1) << _index);
			}
			else
			{
			}
			return true;
		}
		return false;
	}
	public: void XOR(int32 _index, bool _enable);
	public: template<typename T> static bool _XOR(T& _bitset, int32 _index, bool _enable) 
	{
		if(_index >= 0 && _index < (sizeof(T) * Memory::BIT_IN_BYTE))
		{
			if(_enable)
			{
				if(_GetBit(_bitset, _index))
				{
					_SetBit(_bitset, _index, false);
				}
				else
				{
					_SetBit(_bitset, _index, true);
				}
			}
			else
			{
			}
			return true;
		}
		return false;
	}
	public: void EQUAL(int32 _index, bool _enable);
	public: template<typename T> static bool _EQUAL(T& _bitset, int32 _index, bool _enable) 
	{
		if(_index >= 0 && _index < (sizeof(T) * Memory::BIT_IN_BYTE))
		{
			if(_enable)
			{
			}
			else
			{
				if(_GetBit(_bitset, _index))
				{
					_SetBit(_bitset, _index, false);
				}
				else
				{
					_SetBit(_bitset, _index, true);
				}
			}
			return true;
		}
		return false;
	}
};




#endif 