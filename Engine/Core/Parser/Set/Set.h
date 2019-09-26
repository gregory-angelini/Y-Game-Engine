#ifndef SET_H
#define SET_H
#include <Engine/Core/Parser/Variable/AbstractVariable.h>
class Set: public AbstractObject
{
	public: enum
	{
		CREATE_SET_MESSAGE = 325,
		RESIZE_SET_MESSAGE = 326,
		GET_AMOUNT_OF_SLOTS_MESSAGE = 327,
		SET_DATA_TO_SLOT_MESSAGE = 328,
		GET_DATA_FROM_SLOT_MESSAGE = 329
	};
	public: enum: int64 
	{
		SET_CLASS = 576460752303423488
	};
	private: AbstractVariable** array;
	private: int32 size;
	public: Set(const Set& _set);
    public: Set(void);
	public: Set(int32 _size);
	public: virtual ~Set(void);
	public: void Destroy(void);
	public: void Resize(int32 _size);
    public: Set& operator = (const Set& _set);
	public: AbstractVariable* GetSlot(int32 _index)const;
	public: void SetSlot(int32 _index, AbstractVariable* _variable);
	public: int32 GetSize(void)const;
	public: using AbstractObject::SaveToFile;
	public: virtual bool SaveToFile(File& _file);
	protected: void SaveDerivedToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	public: using AbstractObject::LoadFromFile;
	protected: void LoadDerivedFromFile(File& _file);
	protected: virtual bool LoadFromFile(File& _file);
};





#endif 