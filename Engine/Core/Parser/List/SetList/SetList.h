#ifndef SETLIST_H
#define SETLIST_H
#include <Engine/Core/Parser/List/List.h>
#include <Engine/Core/Parser/Set/Set.h>
#include <Engine/Core/Parser/Variable/AbstractVariable.h>
#include <Engine/Core/Parser/List/AbstractObjectList/AbstractObjectList.h>
template<>
class List<Set>: public AbstractList,
	             public MagicList<Set>
{
	public: enum: int64 
	{
		SET_LIST_CLASS = 144115188075855872
	};
	friend class AbstractList;
	public: List(void);
	public: virtual ~List(void);
	public: virtual int32 GetValueType(void)const;
	public: virtual MagicNode::Address* AddItem(AbstractObject* _item, int32 _index);
	public: virtual void CopyTo(AbstractList* _list, int32 _copyMode);
	public: virtual int32 GetItemIndex(AbstractObject* _item);
	public: virtual AbstractObject* GetItem(int32 _index);
	public: virtual int32 GetAmountOfItems(void)const;
	public: virtual bool IsListEmpty(void)const;
	public: virtual void SwapIndex(int32 _index1, int32 _index2);
	public: virtual void SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2);
	public: virtual void SwapList(AbstractList* _list);
	public: virtual void InverseList(void);
	public: virtual void RotateItems(bool _right);
	public: virtual void RandomizeItems(int32 _repetitions);
	public: virtual void RemoveIndex(int32 _index, int32 _removeMode);
	public: virtual void RemoveItem(AbstractObject* _item, int32 _removeMode);
	public: virtual void ClearList(int32 _removeMode);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void SaveDerivedToFile(File& _file);
	protected: void SaveItemToFile(MagicList<Set>::AddressDerived* _address, File& _file);
	protected: void LoadDerivedFromFile(File& _file);
	protected: virtual void LoadItemFromFile(File& _file);
};

#endif
