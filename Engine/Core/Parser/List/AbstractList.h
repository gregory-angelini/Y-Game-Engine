#ifndef ABSTRACTLIST_H
#define ABSTRACTLIST_H
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Random/Random.h>  
#include <Engine/Math/Vector/Vector3/Vector3.h>
#include <Engine/Math/Vector/Vector2/Vector2.h>
#include <Engine/Math/Quaternion/Quaternion.h>
#include <Engine/Core/Object/TranslatableObject/TranslatableObject.h>
#include <Engine/Core/Object/ScalableObject/ScalableObject.h>
#include <Engine/Core/Object/RotatableObject/RotatableObject.h>
#include <Engine/Output/Graphics/Object/ColorableObject/ColorableObject.h> 
#include <Engine/Core/Parser/Set/Set.h>
class AbstractList: public virtual AbstractObject
{
	public: enum: int64
	{
		ABSTRACT_LIST_CLASS = 2147483648
	};
	public: enum
	{
		INTEGER_TYPE = 1,
		FLOAT_TYPE = 2,
		BOOLEAN_TYPE = 3,
		STRING_TYPE = 4,
		VECTOR_3_TYPE = 5,
		QUATERNION_TYPE = 6,
		VECTOR_2_TYPE = 7,

		OBJECT_TYPE = 8,
		TRANSLATABLE_OBJECT_TYPE = 9,
		SCALABLE_OBJECT_TYPE = 10,
		ROTATABLE_OBJECT_TYPE = 11,
		COLORABLE_OBJECT_TYPE = 12,
		SET_TYPE = 13
	};
	public: enum
	{
		CREATE_LIST_MESSAGE = 64,
		ADD_TO_LIST_MESSAGE = 65,
		GET_DATA_FROM_ITEM_MESSAGE = 66,
		RANDOMIZE_LIST_MESSAGE = 67,
		REMOVE_INDEX_FROM_LIST_MESSAGE = 102,
		GET_AMOUNT_OF_ITEMS_MESSAGE = 103,
		COPY_LIST_MESSAGE = 109,
		CLEAR_LIST_MESSAGE = 110,
			CHANGE_LIST_BOOLEAN_MESSAGE = 147,
			CHANGE_LIST_COLORABLE_MESSAGE = 96,
			CHANGE_LIST_FLOAT_MESSAGE = 125,
			CHANGE_LIST_INTEGER_MESSAGE = 149,
			CHANGE_LIST_QUATERNION_MESSAGE = 150,
			CHANGE_LIST_ROTATABLE_MESSAGE = 152,
			CHANGE_LIST_SCALABLE_MESSAGE = 153,
			CHANGE_LIST_STRING_MESSAGE = 154,
			CHANGE_LIST_TRANSLATABLE_MESSAGE = 155,
			CHANGE_LIST_VECTOR_2_MESSAGE = 156,
			CHANGE_LIST_VECTOR_3_MESSAGE = 157,
			CHANGE_ITEM_INTEGER_MESSAGE = 134,
			CHANGE_ITEM_FLOAT_MESSAGE = 135,
			CHANGE_ITEM_BOOLEAN_MESSAGE = 136,
			CHANGE_ITEM_STRING_MESSAGE = 137,
			CHANGE_ITEM_VECTOR_2_MESSAGE = 138,
			CHANGE_ITEM_VECTOR_3_MESSAGE = 139,
			CHANGE_ITEM_QUATERNION_MESSAGE = 140,
			CHANGE_ITEM_TRANSLATABLE_MESSAGE = 143,
			CHANGE_ITEM_COLORABLE_MESSAGE = 144,
			CHANGE_ITEM_SCALABLE_MESSAGE = 145,
			CHANGE_ITEM_ROTATABLE_MESSAGE = 146,
		GET_LIST_LENGTH_MESSAGE = 248,
		CHECK_LIST_EMPTY_MESSAGE = 313,
		ROTATE_LIST_MESSAGE = 70,
		REMOVE_ITEM_FROM_LIST_MESSAGE = 330,
		INVERSE_LIST_MESSAGE = 331,
		GET_ITEM_INDEX_MESSAGE = 332,
			SWAP_INDEX_MESSAGE = 336,
			SWAP_ITEM_MESSAGE = 337,
			SWAP_LIST_MESSAGE = 338
	};
	public: enum
	{
		HEAD = -1,
		TAIL = -2,
		RANDOM = -3
	};
	public: enum
	{
		SHARE_ITEM,
		COPY_IN_VARIABLE,
		COPY_ONLY_DATA
	};
	public: enum
	{
		DESTROY_ITEM = MagicContainer::DELETE_NODE,
		REMOVE_ITEM = MagicContainer::DELETE_UNIQUE_NODE
	};
	template<typename Type> friend class List;
	private: static StringANSI listsSuffix;
    private: static StringANSI listsDir;
	public: AbstractList(void);
	public: virtual ~AbstractList(void);
	public: virtual float GetLength(void)const;
	public: virtual int32 GetItemIndex(AbstractObject* _item) = 0;
	public: virtual AbstractObject* GetItem(int32 _index) = 0;
	public: virtual int32 GetAmountOfItems(void)const = 0;
	public: virtual bool IsListEmpty(void)const = 0;
	public: virtual MagicNode::Address* AddItem(AbstractObject* _item, int32 _index) = 0; 
	public: virtual void CopyTo(AbstractList* _list, int32 _copyMode) = 0;
	public: virtual void SwapIndex(int32 _index1, int32 _index2) = 0;
	public: virtual void SwapItem(AbstractObject* _item1, AbstractList* _list, AbstractObject* _item2) = 0;
	public: virtual void SwapList(AbstractList* _list) = 0;
	public: virtual void InverseList(void) = 0;
	public: virtual void RotateItems(bool _right) = 0;
	public: virtual void RandomizeItems(int32 _repetitions) = 0;
	public: virtual void SetValue(int32 _value);
	public: virtual void SetValue(float _value);
	public: virtual void SetValue(bool _value);
	public: virtual void SetValue(StringANSI _value);
	public: virtual void SetValue(Vector2 _value);
	public: virtual void SetValue(Vector3 _value);
	public: virtual void SetValue(Vector4 _value);
	public: virtual void SetValue(Quaternion _value);
	public: virtual void SetItemValue(int32 _index, int32 _value);
	public: virtual void SetItemValue(int32 _index, float _value);
	public: virtual void SetItemValue(int32 _index, bool _value);
	public: virtual void SetItemValue(int32 _index, StringANSI _value);
	public: virtual void SetItemValue(int32 _index, Vector2 _value);
	public: virtual void SetItemValue(int32 _index, Vector3 _value);
	public: virtual void SetItemValue(int32 _index, Quaternion _value);
	public: virtual void RemoveIndex(int32 _index, int32 _removeMode) = 0;
	public: virtual void RemoveItem(AbstractObject* _item, int32 _removeMode) = 0;
	public: virtual void ClearList(int32 _removeMode) = 0;
	public: virtual int32 GetValueType(void)const = 0;
	public: template<typename Type>
	static int32 _GetValueType(void)
	{
		return 0;
	}
	public: template<>
	static int32 _GetValueType<int32>(void)
	{
		return INTEGER_TYPE;
	}
	public: template<>
	static int32 _GetValueType<float>(void)
	{
		return FLOAT_TYPE;
	}
	public: template<>
	static int32 _GetValueType<bool>(void)
	{
		return BOOLEAN_TYPE;
	}
	public: template<>
	static int32 _GetValueType<StringANSI>(void)
	{
		return STRING_TYPE;
	}
	public: template<>
	static int32 _GetValueType<Vector3>(void)
	{
		return VECTOR_3_TYPE;
	}
	public: template<>
	static int32 _GetValueType<Quaternion>(void)
	{
		return QUATERNION_TYPE;
	}
	public: template<>
	static int32 _GetValueType<Vector2>(void)
	{
		return VECTOR_2_TYPE;
	}
	public: template<> 
	static int32 _GetValueType<AbstractObject>(void)
	{
		return OBJECT_TYPE;
	}
	public: template<> 
	static int32 _GetValueType<TranslatableObject>(void)
	{
		return TRANSLATABLE_OBJECT_TYPE;
	}
	public: template<> 
	static int32 _GetValueType<ScalableObject>(void)
	{
		return SCALABLE_OBJECT_TYPE;
	}
	public: template<> 
	static int32 _GetValueType<RotatableObject>(void)
	{
		return ROTATABLE_OBJECT_TYPE;
	}
	public: template<> 
	static int32 _GetValueType<ColorableObject>(void)
	{
		return COLORABLE_OBJECT_TYPE;
	}
	public: template<> 
	static int32 _GetValueType<Set>(void)
	{
		return SET_TYPE;
	}
	public: static void _SetFileSuffix(StringANSI _suffix);
	public: static StringANSI _GetFileSuffix(void);
	public: static StringANSI _GetDir(void);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	public: virtual bool LoadFromFile(File& _file);
    public: static AbstractList* _LoadFromFile(StringANSI _path, bool _auto = true);
	public: static AbstractList* _LoadFromFile(File& _file);
};


#endif
